#include "camera.h"
#include "material.h"

#include <iostream>

using namespace std;

Color Camera::background(const Ray& ray) {
    /*
    auto unitDir = ray.dir().unit();
    auto a = 0.5 * (unitDir.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    */
    return _background;
}

Color Camera::color(const Ray& ray, int depth, const shared_ptr<const Intersectable>& intersectable) {
    if (depth <= 0) return Color(0.0, 0.0, 0.0);
    Hit hit;
    if (!intersectable->intersects(ray, Interval(0.001), hit)) return background(ray);

    Ray scattered;
    Color attenuation;
    Color color_from_emission = hit.material()->emitted(hit.u(), hit.v(), hit.point());

    if (!hit.material()->scatter(ray, hit, attenuation, scattered)) return color_from_emission;

    Color color_from_scatter = attenuation * color(scattered, depth - 1, intersectable);

    return color_from_emission + color_from_scatter;
}

Camera::Camera(double aspect, int samplesPerPixel,
       int maxDepth, int imageWidth, double vfov, Vector lookFrom, Vector lookAt, Vector vup, Color background, double defocusAngle, double focusDist): _aspect(aspect), _samplesPerPixel(samplesPerPixel), _maxDepth(maxDepth), _lookFrom(lookFrom),
           _lookAt(lookAt), _vup(vup), _background(background), _defocusAngle(defocusAngle), _focusDist(focusDist)
{
    auto imageHeight = int(imageWidth / aspect);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;
    _frameBuffer = make_shared<FrameBuffer>(imageWidth, imageHeight);
    
    auto theta = vfov * M_PI / 180.0;
    auto h = tan(theta / 2.0);
    auto viewportHeight = 2.0 * h * _focusDist;
    auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
    _viewport = { viewportWidth, viewportHeight };
    
    _w = (_lookFrom - _lookAt).unit();
    _u = Vector::cross(_vup, _w).unit();
    _v = Vector::cross(_w, _u);

    auto viewportU = _viewport.width * _u;
    auto viewportV = _viewport.height * -_v;
    _viewportDeltaU = viewportU / imageWidth;
    _viewportDeltaV = viewportV / imageHeight;
    auto viewportUpperLeft = _lookFrom - (_focusDist * _w) - (viewportU / 2.0) - (viewportV / 2.0);
    _pixel00 = viewportUpperLeft + 0.5 * (_viewportDeltaU + _viewportDeltaV);
    
    auto defocusRadius = _focusDist * tan((_defocusAngle / 2.0) * M_PI / 180.0);
    _defocusDiskU = _u * defocusRadius;
    _defocusDiskV = _v * defocusRadius;
}

void Camera::render(const shared_ptr<const Intersectable>& intersectable, const string& filename) {
    auto sampleScaling = 1.0 / _samplesPerPixel;
    for (int y = 0; y < _frameBuffer->height(); ++y) {
        clog << "\rRendering... " << int((y + 1) * 100.0 / _frameBuffer->height()) << "%" << flush;
        for (int x = 0; x < _frameBuffer->width(); ++x) {
            auto pixelColor = Color(0.0, 0.0, 0.0);
            for(int sample = 0; sample < _samplesPerPixel; ++sample) {
                auto pixelSample = _pixel00 + x * _viewportDeltaU + y * _viewportDeltaV;
                if (_samplesPerPixel > 1) {
                    auto pixelSampleSquare =
                        (-0.5 + randomDouble()) * _viewportDeltaU +
                        (-0.5 + randomDouble()) * _viewportDeltaV;
                    pixelSample += pixelSampleSquare;
                }
                auto rayOrigin = (_defocusAngle <= 0.0) ? _lookFrom : defocusDiskSample();
                auto rayDirection = pixelSample - rayOrigin;
                auto rayTime = randomDouble();
                auto ray = Ray(rayOrigin, rayDirection, rayTime);
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
