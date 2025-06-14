#include "bvh.hh"

BVHNode::BVHNode(HittableList list) 
    : BVHNode{list.m_objects, 0, list.m_objects.size()}
{}

BVHNode::BVHNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end) {
    
    m_bbox = AABB::empty;
    /// NOTE: Concatinate the bounding boxes to find the span
    for (size_t object_index = start; object_index < end; object_index++) {
        m_bbox = AABB(m_bbox, objects[object_index]->bounding_box());
    }
    
    i32 axis = m_bbox.longest_axis(); 
    /// NOTE: We split along the longest axis
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;
    
    size_t object_span = end - start;

    if (object_span == 1) {
        // If we are left with one object we duplicate it.
        m_left = m_right = objects[start];
    } else if (object_span == 2) {
        // If we are left with two object we split them
        m_left = objects[start];
        m_right = objects[start+1];
    } else {
        std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);
        
        // If we have more objects we split them into two groups
        size_t mid = start + object_span/2;
        m_left = make_shared<BVHNode>(objects, start, mid);
        m_right = make_shared<BVHNode>(objects, mid, end);
    }

    // m_bbox = AABB(m_left->bounding_box(), m_right->bounding_box());
}

bool BVHNode::hit(const ray& r, math::Interval ray_t, HitRecord& record) const {
    if (!m_bbox.hit(r, ray_t)) {
        return false;
    }

    bool hit_left = m_left->hit(r, ray_t, record);
    /// EXAMPLE: we are hitting the boxes for the sides 
    if (hit_left) {
        ray_t = math::Interval(ray_t.m_min, record.t);
    }
    bool hit_right = m_right->hit(r, ray_t, record);

    return hit_left || hit_right;
}

AABB BVHNode::bounding_box() const {
    return m_bbox;
}
