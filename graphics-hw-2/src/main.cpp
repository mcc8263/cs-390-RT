#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

#include "camera.hpp"
#include "renderers/rayTracer.hpp"

int main() {
    int nx = 400;
    int ny = 400;

    Scene scene;

    Plane plane;
    scene.items.push_back(&plane);

    Sphere sphere;
    scene.items.push_back(&sphere);



    vec4 sca(0.75, 0.75, 0.75, 1.0);
    sphere.scale(sca);

    vec4 tra(0, 0, 1.75, 0);
    sphere.translate(tra);

    //vec4 tra(0, 0, 2, 0);
    //sphere.translate(tra);

    Camera cam(vec4(-2, 0, 1, 1), 0, 0, nx, ny, .01, 90, 1);
    scene.cameras.push_back(&cam);

    RayTracer rayTracer;
    Image image;

    image = rayTracer.takePicture(scene, 0);

    std::string path = "../data/image.ppm";
    std::ofstream output(path);
    output << image.dump_ppm();
}