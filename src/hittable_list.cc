#include "hittable_list.hh"

HittableList::HittableList() {}
HittableList::HittableList(shared_ptr<Hittable> object) {
    add(object);
}

void HittableList::clear() {
    m_objects.clear();
}
void HittableList::add(shared_ptr<Hittable> object) {
    m_objects.push_back(object);
}
/// NOTE: Here it is! here's where all the spacial optimizations occur.
/// Linearly searching through list is quite expensive. BVHs come in later.
bool HittableList::hit(const ray& r, math::Interval ray_t, HitRecord& record) const {
    using namespace math;

    HitRecord temp_record;
    bool hit_anything = false;
    f64 closest_t = ray_t.m_max;

    for (const auto& object : m_objects) {
        if (object->hit(r, Interval(ray_t.m_min, closest_t), temp_record)) {
            hit_anything = true;
            closest_t = temp_record.t;
            record = temp_record;
        }
    }
    return hit_anything;
}