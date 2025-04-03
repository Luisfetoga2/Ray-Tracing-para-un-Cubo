#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "cube.h"

using namespace std;

int main() {

    hittable_list world;

    world.add(make_shared<cube>(point3(0, 0, -1), 0.5, 0.5, -0.5, 0));
    world.add(make_shared<cube>(point3(0, -6, -1), 10, 0.5, 0, 0));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.render(world);
}