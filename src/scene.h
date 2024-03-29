#pragma once

#include "intersectable.h"
#include "aabb.h"

#include <vector>
#include <memory>

class Scene : public Intersectable {

private:
    
    std::vector<std::shared_ptr<Intersectable> > _intersectables;
    Aabb _boundingBox;
    
public:
    bool intersects(const Ray& ray, const Interval& interval, Hit& hit) const override;
    void add(std::shared_ptr<Intersectable> intersectable);
    
    Aabb boundingBox() const override { return _boundingBox; }
    
    const std::vector<std::shared_ptr<Intersectable> >& intersectables() const { return _intersectables; }
};
