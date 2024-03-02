#pragma once

#include "intersectable.h"

class Material {
  
public:
    
    virtual ~Material() = default;
    virtual bool scatter(const Ray& ray, const Hit& hit, Color& attenuation, Ray& scattered) const = 0;
    
};

class Lambertian : public Material {
  
private:
    
    Color _albedo;
    
public:
    
    Lambertian(const Color& color):_albedo(color) {}
    bool scatter(const Ray& ray, const Hit& hit, Color& attenuation, Ray& scattered) const override {
        auto direction = hit.normal() + Vector::randomUnitInUnitSphere();
        if (direction.isNearZero()) direction = hit.normal();
        scattered = Ray(hit.point(), direction);
        attenuation = _albedo;
        return true;
    }
    
};

class Metal : public Material {
  
private:
    
    Color _albedo;
    double _fuzz;
    
public:
    
    Metal(const Color& color, double fuzz):_albedo(color), _fuzz(fuzz < 1.0 ? fuzz : 1.0) {}
    bool scatter(const Ray& ray, const Hit& hit, Color& attenuation, Ray& scattered) const override {
        Vector reflected = Vector::reflect(ray.dir().unit(), hit.normal());
        scattered = Ray(hit.point(), reflected + _fuzz * Vector::randomUnitInUnitSphere());
        attenuation = _albedo;
        return true;
    }
    
};

class Dielectric : public Material {
  
private:
    
    double _ir;
    
    static double reflectance(double cosine, double index) {
        auto r0 = (1.0 - index) / (1.0 + index);
        r0 = r0 * r0;
        return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5.0);
    }
    
public:
    
    Dielectric(double indexOfRefraction):_ir(indexOfRefraction) {}
    bool scatter(const Ray& ray, const Hit& hit, Color& attenuation, Ray& scattered) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        const auto refractionRatio = hit.frontFace() ? (1.0 / _ir) : _ir;
        const auto unitDirection = ray.dir().unit();

        const auto cos_theta = std::fmin(Vector::dot(-unitDirection, hit.normal()), 1.0);
        const auto sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        const auto cannotRefract = refractionRatio * sin_theta > 1.0;
        Vector direction;

        if (cannotRefract || Dielectric::reflectance(cos_theta, refractionRatio) > randomDouble())
            direction = Vector::reflect(unitDirection, hit.normal());
        else
            direction = Vector::refract(unitDirection, hit.normal(), refractionRatio);

        scattered = Ray(hit.point(), direction);
        
        return true;
    }
    
};
