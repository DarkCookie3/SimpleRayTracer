#pragma once

#include "../Objects/Hittable.h"
#include "Material.h"

class Metal : public Material
{
public:
    Metal(const Vector3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1)
    {
    }

    bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override
    {
        Vector3 reflected = Vector3::Reflect(r_in.GetDirection().GetNormalized(), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * Vector3::RandomInUnitSphere());
        attenuation = albedo;

        return Vector3::Dot(scattered.GetDirection(), rec.normal) > 0;
    }

    Vector3 albedo;
    double fuzz;
};