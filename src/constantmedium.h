#pragma once

#include "intersectable.h"
#include "material.h"

#include <memory>

class ConstantMedium : public Intersectable {
    
private:
    
    std::shared_ptr<Intersectable> _boundary;
    double _negInvDensity;
    std::shared_ptr<Material> _phaseFunction;
    
public:
    
    ConstantMedium(std::shared_ptr<Intersectable> b, double d, std::shared_ptr<Texture> a):
        _boundary(b), _negInvDensity(-1.0 / d), _phaseFunction(std::make_shared<Isotropic>(a)) {}

    ConstantMedium(std::shared_ptr<Intersectable> b, double d, Color c):
        _boundary(b), _negInvDensity(-1.0 / d), _phaseFunction(std::make_shared<Isotropic>(c)) {}
    
    bool intersects(const Ray &ray, const Interval &interval, Hit &hit) const override;
    Aabb boundingBox() const override { return _boundary->boundingBox(); }

};
