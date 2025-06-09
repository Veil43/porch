#ifndef HITTABLE_LIST_HH
#define HITTABLE_LIST_HH

#include "hittable.hh"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
public:
    std::vector<shared_ptr<Hittable>> m_objects;

    HittableList();
    HittableList(shared_ptr<Hittable> object);

    void clear();
    void add(shared_ptr<Hittable> object);
    bool hit(const ray& r, math::Interval ray_t, HitRecord& record) const override;
    AABB bounding_box() const override;
private:
    AABB m_bbox;
};

#endif // HITTABLE_LIST_HH