#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "cube.h"
#include <chrono>

using namespace std;

// Function prototype for calculate_vertical_extent
inline double calculate_vertical_extent(double angleX, double angleY, double angleZ, double size);

int main() {

    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.671, 0.671, 0.831));

    auto material_1  = make_shared<metal>(color(0.7, 0.7, 0.7), 0.3);
    auto material_2 = make_shared<metal>(color(1,0.57,0.57), 0);
    auto material_3 = make_shared<metal>(color(0.9,0.9,0.9), 0);
    
    
    world.add(make_shared<cube>(point3(0, -500, 0), 1000, 0, 0, 0, material_ground));
    world.add(make_shared<cube>(point3(-2, 0.61, -0.2), 1.2, 0, 0, 0, material_1));
    world.add(make_shared<cube>(point3(2, 0.81, 0), 1.6, 0, 0, 0, material_3));
    world.add(make_shared<cube>(point3(6, 1.01, 0.2), 2, 0, 0, 0, material_2));
    
    int n = 8;
    for (int a = -n; a < n; a++) {
        for (int b = -n; b < n; b++) {
            auto choose_mat = random_double();
            auto x = a - 0.25 + 0.5*random_double();
            auto z = b - 0.25 + 0.5*random_double();
            
            double d = 1;
            if ((fabs(x - -2) > d || fabs(z - -0.2) > d) &&
                (fabs(x - 2) > d || fabs(z) > d) &&
                (fabs(x - 6) > d || fabs(z - 0.2) > d)) {
                shared_ptr<material> material;
                if (choose_mat < 0.8) {
                    // Diffuse
                    auto albedo = color::random() * color::random();
                    material = make_shared<lambertian>(albedo);
                } else {
                    // Metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    material = make_shared<metal>(albedo, fuzz);
                }

                // Randomize the cube's rotation angles
                auto angleX = random_double(0, 2 * pi);
                auto angleY = random_double(0, 2 * pi);
                auto angleZ = random_double(0, 2 * pi);
                
                auto size = random_double(0.1, 0.3);

                auto y = 0.1 + size/2 + random_double(0, 0.1);
                world.add(make_shared<cube>(point3(x, y, z), size, angleX, angleY, angleZ, material));

            }
        }
    }


    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 1000;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;

    cam.vfov = 20.0;

    cam.lookfrom = point3(-14,1.5,4);
    cam.lookat   = point3(0,0.5,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;


    auto start_time = std::chrono::high_resolution_clock::now();
    cam.render(world);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::clog << "Elapsed time: " << elapsed.count() << " seconds\n";
}