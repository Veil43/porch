#pragma once

#include "aabb.hh"
#include "hittable.hh"
#include "hittable_list.hh"

class BVHNode : public Hittable {
public:
    BVHNode(HittableList list);
    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end);

    bool hit(const ray& r, math::Interval ray_t, HitRecord& record) const override;
    AABB bounding_box() const override;

private:
    shared_ptr<Hittable> m_left;
    shared_ptr<Hittable> m_right;
    AABB m_bbox;

    /*
        sorts the objects in ascending order with respect to their x position
    */
    static bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        math::Interval a_x_interval = a->bounding_box().m_x_bounds;
        math::Interval b_x_interval = b->bounding_box().m_x_bounds;
        return a_x_interval.m_min < b_x_interval.m_min; // a comes before b
    }
    
    static bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        math::Interval a_y_interval = a->bounding_box().m_y_bounds;
        math::Interval b_y_interval = b->bounding_box().m_y_bounds;
        return a_y_interval.m_min < b_y_interval.m_min; // a comes before b
    }
    
    static bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        math::Interval a_z_interval = a->bounding_box().m_z_bounds;
        math::Interval b_z_interval = b->bounding_box().m_z_bounds;
        return a_z_interval.m_min < b_z_interval.m_min; // a comes before b
    }
};