#include "common.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "cylinder.h"
#include "interval.h"
#include "mpi_camera.h"
#include "material.h"

#include <mpi.h>
#include <iostream>

int main()
{
    MPI_Init(NULL, NULL);

    clock_t start, end;

    hittable_list world;

    auto R = cos(pi / 4);

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    mpi_camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth = 50;

    cam.vfov = 90;
    cam.lookfrom = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 10.0;
    cam.focus_dist = 3.4;

    cam.vfov = 20;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        start = clock();
    }
    cam.render(world);
    if (rank == 0)
    {
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        std::clog << "Time taken by program: " << time_taken << " sec " << std::endl
                  << std::flush;
    }

    MPI_Finalize();
}