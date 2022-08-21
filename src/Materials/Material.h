#pragma once
#include "../Ray.h"

struct HitRecord;

class Material
{
public:
    virtual ~Material() = default;
    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const = 0;
};

float Schlick(float cosine, float ref_idx)
{
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;

    return r0 + (1 - r0) * pow((1 - cosine), 5);
}