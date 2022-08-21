#pragma once
#include "Vector3Float.h"

class Ray
{
public:
    Ray() = default; 

    Ray(const Vector3& _origin, const Vector3& _direction) : origin(_origin), direction(_direction)
    {
    }

    Vector3 GetOrigin() const
    {
        return origin;
    }

    Vector3 GetDirection() const
    {
        return direction;
    }

    Vector3 GetCoordinateAt(float t) const
    {
        return origin + t * direction;
    }

private:
    Point3 origin;
    Vector3 direction;
};