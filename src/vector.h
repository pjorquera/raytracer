#pragma once

#include <cmath>
#include <random>

inline double randomDouble(double min = 0.0, double max = 1.0) {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static auto generator = std::mt19937{std::random_device()()};
    return min + (max - min) * distribution(generator);
}

inline int randomInt(int min, int max) {
    return int(randomDouble(min, max + 1));
}

class Vector {

private:

    double _c[3];
    
public:
    
    Vector() : _c{0.0, 0.0, 0.0} {}
    Vector(double x, double y, double z) : _c{x, y, z} {}
    
    inline double x() const { return _c[0]; }
    inline double y() const { return _c[1]; }
    inline double z() const { return _c[2]; }
    
    inline double r() const { return _c[0]; }
    inline double g() const { return _c[1]; }
    inline double b() const { return _c[2]; }
    
    inline double operator[](int index) const { return _c[index]; }
    
    Vector operator-() const { return Vector(-x(), -y(), -z()); }
    
    Vector& operator+=(const Vector& v) {
        _c[0] += v.x();
        _c[1] += v.y();
        _c[2] += v.z();
        return *this;
    }
    
    Vector& operator*=(double t) {
        _c[0] *= t;
        _c[1] *= t;
        _c[2] *= t;
        return *this;
    }
    
    Vector& operator/=(double t) {
        return *this *= 1.0 / t;
    }
    
    inline friend Vector operator*(double t, const Vector& v) {
        return Vector(t * v.x(), t * v.y(), t * v.z());
    }

    inline friend Vector operator*(const Vector& v, double t) {
        return t * v;
    }
    
    inline friend Vector operator+(const Vector& u, const Vector& v) {
        return Vector(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
    }

    inline friend Vector operator-(const Vector& u, const Vector& v) {
        return Vector(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
    }

    inline friend Vector operator*(const Vector& u, const Vector& v) {
        return Vector(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
    }

    inline friend Vector operator/(const Vector& v, double t) {
        return (1.0 / t) * v;
    }
    
    double length() const {
        return std::sqrt(lengthSquared());
    }
    
    double lengthSquared() const {
        return x() * x() + y() * y() + z() * z();
    }
    
    Vector unit() const {
        const auto l = length();
        return Vector(x() / l, y() / l, z() / l);
    }
    
    Vector& gammaCorrected() {
        _c[0] = std::sqrt(_c[0]);
        _c[1] = std::sqrt(_c[1]);
        _c[2] = std::sqrt(_c[2]);
        return *this;
    }
    
    bool isNearZero() const {
        const auto s = 1e-8;
        return (fabs(_c[0]) < s) && (fabs(_c[1]) < s) && (fabs(_c[2]) < s);
    }
    
    static Vector reflect(const Vector& vector, const Vector& normal) {
        return vector - 2.0 * Vector::dot(vector, normal) * normal;
    }
    
    static double dot(const Vector& u, const Vector& v) {
        return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
    }

    static Vector cross(const Vector& u, const Vector& v) {
        return Vector(u.y() * v.z() - u.z() * v.y(),
                    u.z() * v.x() - u.x() * v.z(),
                    u.x() * v.y() - u.y() * v.x());
    }
    
    static Vector random(double min, double max) {
        return Vector(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
    }
    
    static Vector randomInUnitSphere() {
        while (true) {
            auto randomVector = Vector::random(-1.0, 1.0);
            if (randomVector.lengthSquared() < 1.0) {
                return randomVector;
            }
        }
    }
    
    static Vector randomUnitInUnitSphere() {
        return randomInUnitSphere().unit();
    }
    
    static Vector randomOnHemisphere(const Vector& normal) {
        const auto onUnitSphere = randomUnitInUnitSphere();
        if (Vector::dot(onUnitSphere, normal) > 0.0) return onUnitSphere;
        else return -onUnitSphere;
    }
    
    static Vector refract(const Vector& uv, const Vector& n, double etaiOverEtat) {
        const auto cosTheta = std::fmin(Vector::dot(-uv, n), 1.0);
        const Vector rOutPerp =  etaiOverEtat * (uv + cosTheta * n);
        const Vector rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.lengthSquared())) * n;
        return rOutPerp + rOutParallel;
    }
    
    static Vector randomInUnitDisc() {
        while (true) {
            const auto vector = Vector(randomDouble(-1.0, 1.0), randomDouble(-1.0, 1.0), 0.0);
            if (vector.lengthSquared() < 1.0) return vector;
        }
    }

};

using Point = Vector;
using Color = Vector;
