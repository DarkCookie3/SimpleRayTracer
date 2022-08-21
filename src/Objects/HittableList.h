#pragma once

#include "Hittable.h"

#include <memory>
#include <utility>
#include <vector>

class HittableList : public Hittable
{
public:
    HittableList() = default;

    HittableList(std::shared_ptr<Hittable> object)
    {
        Add(std::move(object));
    }

    void Clear()
    {
        objects.clear();
    }

    void Add(std::shared_ptr<Hittable>&& object)
    {
        objects.push_back(object);
    }

    bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;

    std::vector<std::shared_ptr<Hittable>> objects;
};

inline bool HittableList::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects)
    {
        if (object->Hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}