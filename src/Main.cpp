#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include "Vector3Float.h"
#include "Camera.h"
#include "Utils.h"
#include "Materials/Dielectric.h"
#include "Objects/HittableList.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Objects/Sphere.h"
#include "ThreadPool/ThreadPool.h"


const int image_width = 1000;
const int image_height = 1000;
const int samples_per_pixel = 20;
const int max_depth = 30;
const auto aspect_ratio = static_cast<float>(image_width) / image_height;

const Vector3 lookfrom(13, 2, 3);
const Vector3 lookat(0, 0, 0);
const Vector3 vup(0, 1, 0);
const auto dist_to_focus = 7.0;
const auto aperture = 0.1;

const char* imageName = "image.ppm";

const Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
const HittableList* world = nullptr;

float* image_buffer = nullptr;

Vector3 Ray_color(const Ray& r, const HittableList* world, int depth)
{
    HitRecord rec;

    if (depth <= 0)
    {
        return Vector3(0, 0, 0);
    }

    if (world->Hit(r, 0.001, infinity, rec))
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

void Pixel_color(int i, int j)
{
    Vector3 color(0, 0, 0);

    for (int s = 0; s < samples_per_pixel; ++s)
    {
        const auto u = (i + random_float()) / image_width;
        const auto v = (j + random_float()) / image_height;
        Ray r = cam.GetRay(u, v);
        color += Ray_color(r, world, max_depth);
    }
    int index = (j * image_width + i) * 3;
    image_buffer[index++] = color.r;
    image_buffer[index++] = color.g;
    image_buffer[index] = color.b;
}

HittableList* random_scene()
{
    HittableList* world = new HittableList();

    world->Add(std::make_shared<Sphere>(Vector3(0, -1000, 0), 1000, std::make_shared<Lambertian>(Vector3(0.5, 0.5, 0.5))));

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
                    world->Add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Lambertian>(albedo)));
                }
                else if (choose_mat < 0.95)
                {
                    auto albedo = Vector3::Random(0.5, 1);
                    auto fuzz = random_float(0, 0.5);
                    world->Add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Metal>(albedo, fuzz)));
                }
                else
                {
                    world->Add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Dielectric>(1.5)));
                }
            }
        }
    }

    world->Add(std::make_shared<Sphere>(Vector3(0, 1, 0), 1.0, std::make_shared<Dielectric>(1.5)));

    world->Add(std::make_shared<Sphere>(Vector3(-4, 1, -2), 1.0, std::make_shared<Lambertian>(Vector3(0.4, 0.2, 0.1))));

    world->Add(std::make_shared<Sphere>(Vector3(4, 1, 0), 1.0, std::make_shared<Metal>(Vector3(0.7, 0.6, 0.5), 0.0)));

    return world;
}

int main()
{
    image_buffer = new float[image_height * image_width * 3];
    world = random_scene();

    ThreadPool* threads = new ThreadPool();
    threads->Start();
    
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int j = 0; j < image_height; j++)
    {
        for (int i = 0; i < image_width; i++)
        {
            threads->QueueJob([i, j] {Pixel_color(i, j); });
        }
    }

    while (auto n = threads->GetJobsCount())
    {
        std::cerr << "\r" << "Pixels left: " << n << "   " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cerr << "\r" << "Pixels left: " << 0 << "   " << std::flush;
    threads->Stop();

    std::ofstream img_file;
    img_file.open(imageName);

    img_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            int index = (j * image_width + i) * 3;
            Vector3::NormalizeAndOutput(image_buffer[index], image_buffer[index+1], image_buffer[index+2], img_file, samples_per_pixel);
        }
    }
    img_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cerr << "\x1b[2K";
    std::cerr << "\rElapsed time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
    std::cerr << "\nDone.\n";

    return 0;
}
