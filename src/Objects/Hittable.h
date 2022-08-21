#pragma once

#include "../Ray.h"

class Material;

struct HitRecord
{
    Point3 p;
    Vector3 normal;
    std::shared_ptr<Material> mat_ptr;
    float t;
    bool front_face = false;

    void set_face_normal(const Ray& r, const Vector3& outward_normal)
    {
        front_face = Vector3::Dot(r.GetDirection(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual ~Hittable() = default;
    virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};