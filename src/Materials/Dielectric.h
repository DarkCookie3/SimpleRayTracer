#pragma once

#include "material.h"
#include "../Objects/Hittable.h"

class Dielectric : public Material
{
public:
    Dielectric(float ri) : ref_idx(ri)
    {       
    }

    bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override
    {
        attenuation = Vector3(1.0f, 1.0f, 1.0f);
        const float etai_over_etat = rec.front_face ? (1.0 / ref_idx) : ref_idx;

        const Vector3 unit_direction = r_in.GetDirection().GetNormalized();
        const float cos_theta = ffmin(Vector3::Dot(-unit_direction, rec.normal), 1.0);
        const float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0)
        {
            const Vector3 reflected = Vector3::Reflect(unit_direction, rec.normal);
            scattered = Ray(rec.p, reflected);

            return true;
        }

        const float reflect_prob = Schlick(cos_theta, etai_over_etat);
        if (random_float() < reflect_prob)
        {
            const Vector3 reflected = Vector3::Reflect(unit_direction, rec.normal);
            scattered = Ray(rec.p, reflected);

            return true;
        }

        const Vector3 refracted = Vector3::Refract(unit_direction, rec.normal, etai_over_etat);
        scattered = Ray(rec.p, refracted);

        return true;
    }

    float ref_idx;
};