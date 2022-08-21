#pragma once
#include "Hittable.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;

    Sphere(Point3 cen, float r, std::shared_ptr<Material> m)
        : center(cen), radius(r), mat_ptr(m)
    {
    }

    bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;

private:
    Point3 center;
    float radius = 0.0f;
    std::shared_ptr<Material> mat_ptr;
};

bool Sphere::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
    const Vector3 oc = r.GetOrigin() - center;
    const auto a = r.GetDirection().GetSquaredLength();
    const auto half_b = Vector3::Dot(oc, r.GetDirection());
    const auto c = oc.GetSquaredLength() - radius * radius;
    const auto discriminant = half_b * half_b - a * c;

    if (discriminant > 0)
    {
        const auto root = sqrt(discriminant);
        auto temp = (-half_b - root) / a;

        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.GetCoordinateAt(rec.t);

            const Vector3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;

            return true;
        }

        temp = (-half_b + root) / a;

        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.GetCoordinateAt(rec.t);

            const Vector3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;

            return true;
        }
    }

    return false;
}