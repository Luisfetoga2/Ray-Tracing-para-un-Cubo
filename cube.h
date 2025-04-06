#ifndef CUBE_H
#define CUBE_H

#include "hittable.h"
#include "rtweekend.h"

vec3 rotate_vector(const vec3& v, double angleX_, double angleY_, double angleZ_) {
    // Rotate around the X-axis
    double cosX = cos(angleX_);
    double sinX = sin(angleX_);
    vec3 rotatedX(
        v.x(),
        v.y() * cosX - v.z() * sinX,
        v.y() * sinX + v.z() * cosX
    );

    // Rotate around the Y-axis
    double cosY = cos(angleY_);
    double sinY = sin(angleY_);
    vec3 rotatedY(
        rotatedX.x() * cosY + rotatedX.z() * sinY,
        rotatedX.y(),
        -rotatedX.x() * sinY + rotatedX.z() * cosY
    );

    // Rotate around the Z-axis
    double cosZ = cos(angleZ_);
    double sinZ = sin(angleZ_);
    vec3 rotatedZ(
        rotatedY.x() * cosZ - rotatedY.y() * sinZ,
        rotatedY.x() * sinZ + rotatedY.y() * cosZ,
        rotatedY.z()
    );

    return rotatedZ;
}

class cube : public hittable {
  public:
    cube(const point3& center_, double side_length_, double angleX_, double angleY_, double angleZ_, shared_ptr<material> mat_):
    center(center_),
    side_length(std::fmax(0,side_length_)),
    angleX(angleX_),
    angleY(angleY_),
    angleZ(angleZ_),
    mat(mat_)
    {}
    

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 local_origin = r.origin() - center;
    
        // Apply inverse rotations to the ray direction and origin
        vec3 rotated_origin = rotate_vector(local_origin, -angleX, -angleY, -angleZ);
        vec3 rotated_direction = rotate_vector(r.direction(), -angleX, -angleY, -angleZ);
    
        // Define the cube's bounds in local space
        double half_side = side_length / 2.0;
        vec3 min_bound(-half_side, -half_side, -half_side);
        vec3 max_bound(half_side, half_side, half_side);
    
        // Check for intersection with the cube's bounding planes
        double t_min = (min_bound.x() - rotated_origin.x()) / rotated_direction.x();
        double t_max = (max_bound.x() - rotated_origin.x()) / rotated_direction.x();
        if (t_min > t_max) std::swap(t_min, t_max);
    
        double ty_min = (min_bound.y() - rotated_origin.y()) / rotated_direction.y();
        double ty_max = (max_bound.y() - rotated_origin.y()) / rotated_direction.y();
        if (ty_min > ty_max) std::swap(ty_min, ty_max);
    
        if ((t_min > ty_max) || (ty_min > t_max)) return false;
        t_min = std::max(t_min, ty_min);
        t_max = std::min(t_max, ty_max);
    
        double tz_min = (min_bound.z() - rotated_origin.z()) / rotated_direction.z();
        double tz_max = (max_bound.z() - rotated_origin.z()) / rotated_direction.z();
        if (tz_min > tz_max) std::swap(tz_min, tz_max);
    
        if ((t_min > tz_max) || (tz_min > t_max)) return false;
        t_min = std::max(t_min, tz_min);
        t_max = std::min(t_max, tz_max);
    
        // Check if the ray intersects the cube within the valid range
        if (!ray_t.surrounds(t_min)) return false; // t_min must be within the ray_t interval
    
        rec.t = t_min;
        rec.p = r.at(t_min);
    
        // Determine which face of the cube was hit
        vec3 hit_point = rotated_origin + t_min * rotated_direction;
        vec3 normal;
    
        if (fabs(hit_point.x() - min_bound.x()) < 1e-6) normal = vec3(-1, 0, 0);
        else if (fabs(hit_point.x() - max_bound.x()) < 1e-6) normal = vec3(1, 0, 0);
        else if (fabs(hit_point.y() - min_bound.y()) < 1e-6) normal = vec3(0, -1, 0);
        else if (fabs(hit_point.y() - max_bound.y()) < 1e-6) normal = vec3(0, 1, 0);
        else if (fabs(hit_point.z() - min_bound.z()) < 1e-6) normal = vec3(0, 0, -1);
        else if (fabs(hit_point.z() - max_bound.z()) < 1e-6) normal = vec3(0, 0, 1);
    
        // Rotate the normal back to world space
        vec3 world_normal = rotate_vector(normal, angleX, angleY, angleZ);
        rec.set_face_normal(r, world_normal);
        rec.mat = mat;
        return true;
    }

  private:
    point3 center;
    double side_length;
    double angleX;
    double angleY;
    double angleZ;
    shared_ptr<material> mat;
};

#endif