#pragma once

#include "vector.h"
#include "intersectable.h"
#include "framebuffer.h"

#include <string>
#include <memory>

typedef struct {
    double width;
    double height;
} Viewport;

class Camera {
  
private:
    
    double _aspect;
    int _samplesPerPixel;
    int _maxDepth;
    std::shared_ptr<FrameBuffer> _frameBuffer;
    Viewport _viewport;
    Point _pixel00;
    Vector _viewportDeltaU;
    Vector _viewportDeltaV;
    
    Point _lookFrom;
    Point _lookAt;
    Vector _vup;
    Vector _u, _v, _w;
    
    double _defocusAngle;
    double _focusDist;
    Vector _defocusDiskU;
    Vector _defocusDiskV;
    
    Point defocusDiskSample() {
        const auto p = Vector::randomInUnitDisc();
        return _lookFrom + (p.x() * _defocusDiskU) + (p.y() * _defocusDiskV);
    }
    
public:
    
    Camera(double aspect = 16.0 / 9.0,
           int samplesPerPixel = 500,
           int maxDepth = 50,
           int imageWidth = 1800,
           double vfov = 20.0,
           Vector lookFrom = Vector(13.0, 2.0, 3.0),
           Vector lookAt = Vector(0.0, 0.0, 0.0),
           Vector vup = Vector(0.0, 1.0, 0.0),
           double defocusAngle = 0.6,
           double focusDist = 10.0);

    double aspect() const { return _aspect; }
    const Viewport& viewport() const { return _viewport; }

    void render(const std::shared_ptr<const Intersectable>& intersectable, const std::string& filename);
    
};
