#pragma once

#include "scene.h"

#include <algorithm>

class Bvh : public Intersectable {

private:
    
    std::shared_ptr<Intersectable> _left;
    std::shared_ptr<Intersectable> _right;
    Aabb _bbox;
    
public:
    Bvh(const Scene* scene): Bvh(*scene) {}
    Bvh(const Scene& scene): Bvh(scene.intersectables(), 0, scene.intersectables().size()) {}
    Bvh(const std::vector<std::shared_ptr<Intersectable> >& intersectables, size_t start, size_t end) {
        auto objects = intersectables;

        int axis = randomInt(0, 2);
        auto comparator = (axis == 0) ? Bvh::boxXCompare
                          : (axis == 1) ? Bvh::boxYCompare
                          : Bvh::boxZCompare;

        size_t object_span = end - start;

        if (object_span == 1) {
            _left = _right = objects[start];
        } else if (object_span == 2) {
            if (comparator(objects[start], objects[start + 1])) {
                _left = objects[start];
                _right = objects[start + 1];
            } else {
                _left = objects[start + 1];
                _right = objects[start];
            }
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span / 2;
            _left = std::make_shared<Bvh>(objects, start, mid);
            _right = std::make_shared<Bvh>(objects, mid, end);
        }

        _bbox = Aabb(_left->boundingBox(), _right->boundingBox());

    }
    
    bool intersects(const Ray &ray, const Interval &interval, Hit &hit) const override {
        if (!_bbox.intersects(ray, interval)) return false;

        bool hit_left = _left->intersects(ray, interval, hit);
        bool hit_right = _right->intersects(ray, Interval(interval.min(), hit_left ? hit.t() : interval.max()), hit);

        return hit_left || hit_right;
    }
    
    Aabb boundingBox() const override { return _bbox; }
    
    static bool boxCompare(const std::shared_ptr<Intersectable>& a, const std::shared_ptr<Intersectable> b, int axis_index) {
        return a->boundingBox().axis(axis_index).min() < b->boundingBox().axis(axis_index).min();
    }

    static bool boxXCompare(const std::shared_ptr<Intersectable>& a, const std::shared_ptr<Intersectable>& b) {
        return boxCompare(a, b, 0);
    }

    static bool boxYCompare(const std::shared_ptr<Intersectable>& a, const std::shared_ptr<Intersectable>& b) {
        return boxCompare(a, b, 1);
    }

    static bool boxZCompare(const std::shared_ptr<Intersectable>& a, const std::shared_ptr<Intersectable>& b) {
        return boxCompare(a, b, 2);
    }
    
};
