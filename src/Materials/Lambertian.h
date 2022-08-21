#pragma once

#include "Material.h"
#include "../Objects/Hittable.h"

class Lambertian : public Material
{
public:
    Lambertian(const Vector3& a) : albedo(a)
    {
    }

    bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override
    {
        const Vector3 scatter_direction = rec.normal + Vector3::RandomUnitVector();
        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;

        return true;
    }

    Vector3 albedo;
};