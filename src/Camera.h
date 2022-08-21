#pragma once
#include "Utils.h"
#include "Vector3Float.h"
#include "Ray.h"

class Camera
{
public:
    Camera(Vector3 lookfrom, Vector3 lookat, Vector3 worldUp, float vfov, float aspect, float aperture, float focus_dist)
        : origin(lookfrom)
    {
        lens_radius = aperture / 2;

        auto theta = degrees_to_radians(vfov);
        auto half_height = tan(theta / 2) * focus_dist;
        auto half_width = aspect * half_height;

        lookDirectionNormalized = (lookfrom - lookat).GetNormalized();
        cameraRightNormalized = Vector3::Cross(lookDirectionNormalized, worldUp).GetNormalized();
        cameraUpNormalized = Vector3::Cross(cameraRightNormalized, lookDirectionNormalized);
        lower_left_corner = origin - half_width * cameraRightNormalized - half_height * cameraUpNormalized - focus_dist * lookDirectionNormalized;

        horizontal = 2 * half_width * cameraRightNormalized;
        vertical = 2 * half_height * cameraUpNormalized;
    }

    Ray GetRay(float s, float t) const
    {
        Vector3 rd = lens_radius * Vector3::RandomInUnitDisk();
        Vector3 offset = cameraRightNormalized * rd.x + cameraUpNormalized * rd.y;

        return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

private:
    Vector3 origin;
    Vector3 lower_left_corner;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 lookDirectionNormalized;
    Vector3 cameraUpNormalized;
    Vector3 cameraRightNormalized;
    float lens_radius;
};