#import "camera.h"
#include "material.h"

#include <iostream>

using namespace std;

inline Color background(const Ray& ray) {
    const auto unitDir = ray.dir().unit();
    const auto a = 0.5 * (unitDir.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

inline Color color(const Ray& ray, int depth, const shared_ptr<const Intersectable>& intersectable) {
    if (depth <= 0) return Color(0.0, 0.0, 0.0);
    Hit hit;
    if (intersectable->intersects(ray, Interval(0.001), hit)) {
        Ray scattered;
        Color attenuation;
        if (hit.material()->scatter(ray, hit, attenuation, scattered))
            return attenuation * color(scattered, depth - 1, intersectable);
        return Color(0,0,0);
    }
    
    return background(ray);
}

Camera::Camera(double aspect, int samplesPerPixel,
       int maxDepth, int imageWidth, double vfov, Vector lookFrom, Vector lookAt, Vector vup, double defocusAngle, double focusDist): _aspect(aspect), _samplesPerPixel(samplesPerPixel), _maxDepth(maxDepth), _lookFrom(lookFrom),
           _lookAt(lookAt), _vup(vup), _defocusAngle(defocusAngle), _focusDist(focusDist)
{
    auto imageHeight = int(imageWidth / aspect);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;
    _frameBuffer = make_shared<FrameBuffer>(imageWidth, imageHeight);
    
    const auto theta = vfov * M_PI / 180.0;
    const auto h = tan(theta / 2.0);
    const auto viewportHeight = 2.0 * h * _focusDist;
    const auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
    _viewport = { viewportWidth, viewportHeight };
    
    _w = (_lookFrom - _lookAt).unit();
    _u = Vector::cross(_vup, _w).unit();
    _v = Vector::cross(_w, _u);

    const auto viewportU = _viewport.width * _u;
    const auto viewportV = _viewport.height * -_v;
    _viewportDeltaU = viewportU / imageWidth;
    _viewportDeltaV = viewportV / imageHeight;
    const auto viewportUpperLeft = _lookFrom - (_focusDist * _w) - (viewportU / 2.0) - (viewportV / 2.0);
    _pixel00 = viewportUpperLeft + 0.5 * (_viewportDeltaU + _viewportDeltaV);
    
    const auto defocusRadius = _focusDist * tan((_defocusAngle / 2.0) * M_PI / 180.0);
    _defocusDiskU = _u * defocusRadius;
    _defocusDiskV = _v * defocusRadius;
}

void Camera::render(const shared_ptr<const Intersectable>& intersectable, const string& filename) {
    const auto sampleScaling = 1.0 / _samplesPerPixel;
    for (int y = 0; y < _frameBuffer->height(); ++y) {
        clog << "\rRendering... " << int((y + 1) * 100.0 / _frameBuffer->height()) << "%" << flush;
        for (int x = 0; x < _frameBuffer->width(); ++x) {
            auto pixelColor = Color(0.0, 0.0, 0.0);
            for(int sample = 0; sample < _samplesPerPixel; ++sample) {
                auto pixelSample = _pixel00 + x * _viewportDeltaU + y * _viewportDeltaV;
                if (_samplesPerPixel > 1) {
                    const auto pixelSampleSquare =
                        (-0.5 + randomDouble()) * _viewportDeltaU +
                        (-0.5 + randomDouble()) * _viewportDeltaV;
                    pixelSample += pixelSampleSquare;
                }
                const auto rayOrigin = (_defocusAngle <= 0.0) ? _lookFrom : defocusDiskSample();
                const auto rayDirection = pixelSample - rayOrigin;
                const auto ray = Ray(rayOrigin, rayDirection);
                pixelColor += color(ray, _maxDepth, intersectable);
            }
            _frameBuffer->draw(x, y, (pixelColor * sampleScaling).gammaCorrected());
        }
    }
    clog << endl;
    clog << "Saving image " << filename;
    _frameBuffer->save(filename);
    clog << endl;
}
