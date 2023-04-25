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

    int samplesPerPixel = 4;

    //Start at the top right corner. Go left to right, top to bottom
    for (int j = (cam.height - 1); j >= 0; j--)
    {
        for (int i = 0; i < (cam.width); i++)
        {
            Color c = output.getPixel(i, j);

            for (int i2 = 0; i2 < samplesPerPixel; i2++) {
            // looping thru rows of sampled section
                for (int j2 = 0; j2 < samplesPerPixel; j2++) {

                    // location of ray in subtile i2, j2
                    float i2loc = (i - 0.5 + 1/(2*samplesPerPixel) + i2/samplesPerPixel);
                    float j2loc = (j - 0.5 + 1/(2*samplesPerPixel) + j2/samplesPerPixel);

                    ray eyeRay = cam.getEyeRay(i2loc + .5, j2loc + .5);

                    Hit hit;
                    hit = this->traceRay(scene, eyeRay, hit, 0);

                    c.r += hit.color.r;
                    c.g += hit.color.g;
                    c.b += hit.color.b;
                    
                }
            }

            c.r /= samplesPerPixel * samplesPerPixel;
            c.g /= samplesPerPixel * samplesPerPixel;
            c.b /= samplesPerPixel * samplesPerPixel;

            output.setPixel(i, j, c);

        }
    }

    return output;
}

Hit RayTracer::traceRay(Scene &scene, ray &eyeRay, Hit &hit, int depth)
{
    Hit closest = hit;
    for (Geometry *item : scene.items)
    {
        Hit current = item->trace(eyeRay);
        if (current.t < closest.t)
        {
            closest = current;
        }
    }
    this->findShade(scene, closest, depth);
    return closest;
}

void RayTracer::findShade(Scene &scene, Hit &hit, int depth)
{
    // missed
    if (hit.t > 1e10)
    {
        hit.color = scene.backgroundColor;
        hit.brightness = scene.ambientLight;
    }
    // hit
    else
    {
           
            ray outRay;
            outRay = hit.material->scatter(hit.inRay, hit.pos, hit.normal);

            Hit newHit;

            if (depth < 1)
            {
                newHit = this->traceRay(scene, outRay, newHit, depth + 1);
            }

            Color c1 = hit.material->getColor(hit.modelSpacePos);
            Color c2 = newHit.color;
            
            hit.color.r += c1.r * c2.r * newHit.brightness;
            hit.color.g += c1.g * c2.g * newHit.brightness;
            hit.color.b += c1.b * c2.b * newHit.brightness;

            // need to ask about this: only works if i multiply by .2 but why?
            hit.color.r += c1.r * 0.20;
            hit.color.g += c1.g * 0.20;
            hit.color.b += c1.b * 0.20;
            
            
        
        
    }
}