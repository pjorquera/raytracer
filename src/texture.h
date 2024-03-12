#pragma once

#include "vector.h"

#include <memory>

class Texture {
  
public:
    
    virtual ~Texture() = default;
    virtual Color color(double u, double v, const Point& point) = 0;
    
};

class SolidColor : public Texture {
    
private:
    
    Color _color;
    
public:
    
    SolidColor(const Color& color):_color(color) {}
    
    Color color(double u, double v, const Point &point) override {
        return _color;
    }
    
};

class CheckerTexture : public Texture {
  
private:
    
    double _invScale;
    std::shared_ptr<Texture> _even;
    std::shared_ptr<Texture> _odd;
    
public:
    
    CheckerTexture(double scale,
                   const std::shared_ptr<Texture>& even,
                   const std::shared_ptr<Texture> odd):
    _invScale(1.0 / scale), _even(even), _odd(odd) {}
    
    CheckerTexture(double scale,
                   const Color& color1,
                   const Color& color2):
    CheckerTexture(scale,
                   std::make_shared<SolidColor>(color1),
                   std::make_shared<SolidColor>(color2)) {}
    
    Color color(double u, double v, const Point &point) override {
        const auto xInteger = int(std::floor(_invScale * point.x()));
        const auto yInteger = int(std::floor(_invScale * point.y()));
        const auto zInteger = int(std::floor(_invScale * point.z()));

        const bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? _even->color(u, v, point) : _odd->color(u, v, point);
    }
    
};