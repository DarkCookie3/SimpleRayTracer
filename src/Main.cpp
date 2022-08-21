#include <iostream>
#include "Vector3Float.h"
#include "Camera.h"
#include "Utils.h"
#include "Materials/Dielectric.h"
#include "Objects/HittableList.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Objects/Sphere.h"

Vector3 Ray_color(const Ray& r, const Hittable& world, int depth)
{
    HitRecord rec;

    if (depth <= 0)
    {
        return Vector3(0, 0, 0);
    }

    if (world.Hit(r, 0.001, infinity, rec))
    {
        Ray scattered;
        Vector3 attenuation;

        if (rec.mat_ptr->Scatter(r, rec, attenuation, scattered))
        {
            return attenuation * Ray_color(scattered, world, depth - 1);
        }

        return Vector3(0, 0, 0);
    }

    const Vector3 unit_direction = r.GetDirection().GetNormalized();
    const auto t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * Vector3(1.0, 1.0, 1.0) + t * Vector3(0.5, 0.7, 1.0);
}

HittableList random_scene()
{
    HittableList world;

    world.Add(std::make_shared<Sphere>(Vector3(0, -1000, 0), 1000, std::make_shared<Lambertian>(Vector3(0.5, 0.5, 0.5))));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            const auto choose_mat = random_float();
            Vector3 center(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());
            if ((center - Vector3(4, 0.2, 0)).GetLength() > 0.9)
            {
                if (choose_mat < 0.8)
                {
                    auto albedo = Vector3::Random() * Vector3::Random();
                    world.Add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Lambertian>(albedo)));
                }
                else if (choose_mat < 0.95)
                {
                    auto albedo = Vector3::Random(0.5, 1);
                    auto fuzz = random_float(0, 0.5);
                    world.Add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Metal>(albedo, fuzz)));
                }
                else
                {
                    world.Add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Dielectric>(1.5)));
                }
            }
        }
    }

    world.Add(std::make_shared<Sphere>(Vector3(0, 1, 0), 1.0, std::make_shared<Dielectric>(1.5)));

    world.Add(std::make_shared<Sphere>(Vector3(-4, 1, -2), 1.0, std::make_shared<Lambertian>(Vector3(0.4, 0.2, 0.1))));

    world.Add(std::make_shared<Sphere>(Vector3(4, 1, 0), 1.0, std::make_shared<Metal>(Vector3(0.7, 0.6, 0.5), 0.0)));

    return world;
}

int main()
{
    const int image_width = 1920;
    const int image_height = 1080;
    const int samples_per_pixel = 100;
    const int max_depth = 30;
    const auto aspect_ratio = static_cast<float>(image_width) / image_height;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    const auto world = random_scene();

    const Vector3 lookfrom(13, 2, 3);
    const Vector3 lookat(0, 0, 0);
    const Vector3 vup(0, 1, 0);
    const auto dist_to_focus = 7.0;
    const auto aperture = 0.1;

    const Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            Vector3 color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                const auto u = (i + random_float()) / image_width;
                const auto v = (j + random_float()) / image_height;
                Ray r = cam.GetRay(u, v);
                color += Ray_color(r, world, max_depth);
            }

            color.OutputValues(std::cout, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}
