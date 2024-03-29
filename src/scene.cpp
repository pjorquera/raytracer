#include "scene.h"

using namespace std;

void Scene::add(shared_ptr<Intersectable> intersectable) {
    _intersectables.push_back(intersectable);
    _boundingBox = Aabb(_boundingBox, intersectable->boundingBox());
}

bool Scene::intersects(const Ray& ray, const Interval& interval, Hit& hit) const {
    Hit currentHit;
    auto intersected = false;
    auto closesSoFar = interval.max();
    for(auto intersectable : _intersectables) {
        if (intersectable->intersects(ray, Interval(interval.min(), closesSoFar), currentHit)) {
            intersected = true;
            closesSoFar = currentHit.t();
            hit = currentHit;
        }
    }
    return intersected;
}
