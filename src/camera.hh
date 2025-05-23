#ifndef CAMERA_HH
#define CAMERA_HH

#include "hittable.hh"
#include "color.hh"

class Camera {

public:

private:
    color compute_ray_color(const ray& r, const Hittable& scene) const;
};

#endif // CAMERA_HH