#include "rayTracer.hpp"
#include <iostream>

Image RayTracer::takePicture(Scene &scene, int camIndex)
{
    Image output;
    Camera cam = (*scene.cameras[camIndex]);
    output.width = cam.width;
    output.height = cam.height;
    for (int i = 0; i < output.width; i++)
    {
        for (int j = 0; j < output.height; j++)
        {
            Color c;
            output.pixels.push_back(c);
        }
    }

    //Start at the top right corner. Go left to right, top to bottom
    for (int j = (cam.height - 1); j >= 0; j--)
    {
        for (int i = 0; i < (cam.width); i++)
        {
            float x = 1.0 * i / cam.width;
            float y = 1.0 - (cam.height - 1.0 - j) / cam.height;
            //TODO: rewrite this to get a gradient
            Color c(x, y, 0);

            output.setPixel(i, j, c);
        }
    }

    return output;
}