#include "sphere.hpp"
#include <iostream>

//implement these two functions
Hit Sphere::trace(ray & inRay) {
    vec4 origin = (this->worldToModel).transform(inRay.origin);
    vec4 dir = (this->worldToModel).transform(inRay.direction);

    //ray ray(origin, dir);

    vec4 center(0,0,0,1);

    Hit hit;

    // ray from center of sphere to eye pt
    vec4 r2s = center - origin;
    // L2: distance from center of sphere to eye 
    float L2 = r2s.dot(r2s);

    //========== MISS: ray originates inside sphere ===================
    if (L2 <= 0) {
        return hit;
    }

    // tca: distance from eye pt to midpoint of 2 hit points on the sphere
    // to be safe, assume we don't know if dir is a unit vector, so:
    // tcaS: scaled tca     =>      tcaS = tca * len(dir)   =>      tca = tcaS / len(dir)
    float tcaS = dir.dot(r2s);

    //========== MISS: chord midpoint is behind the camera ============
    if (tcaS < 0.0) {  
        return hit;       
    } 

    // tca2: [distance from eye to midpoint]^2 

    // DL2: [len(dir)]^2
    float DL2 = dir.dot(dir);
    // [tca]^2 = [tcaS]^2 / [len(dir)]^2
    float tca2 = tcaS*tcaS / DL2;

    // LM2: [distance from center of sphere to midpoint]^2
    
    // dist(center -> eye) = dist(center -> midpoint) + dist(midpoint -> eye)
    // [L]^2 = [LM]^2 + [tca]^2     =>      [LM]^2 = [L]^2 - [tca]^2;
    float LM2 = L2 - tca2;  
 
    //========== MISS: chord midpoint is outside of the sphere ========
    if (LM2 > 1.0) {  
        return hit;
    }

    //--------- find hit points ---------
    // L2hc: length of half chord, distance from midpoint to hit point
    // chord midpoint forms 90 deg angle: one leg from center to midpoint, hypotenuse = radius
    // [LM]^2 + [Lhc]^2 = [1]^2    =>      [Lhc]^2 = 1 - [LM]^2
    float L2hc = (1.0 - LM2);

    // length of the ray from eye to hit point = dist(eye -> midpoint) += Lhc
    // ray-length = tca +/- Lhc
    // ray-length = t * (len(dir))

    // t = ray-length / sqrt([DL]^2) = (tca +/- sqrt([Lhc]^2)) / sqrt([DL]^2)
    // t = ( tca / sqrt([DL]^2) ) +/- ( sqrt([Lhc]^2) / sqrt([DL]^2 )
    // t = ( tca / DL ) +/- ( Lhc / DL )
    // t = ( tcaS / DL*DL ) +/- ( Lhc / DL )
    float t0hit = tcaS / DL2 - sqrt(L2hc / DL2); 

    hit.t = t0hit;
    hit.modelSpacePos = origin + dir * t0hit;
    hit.pos = inRay.origin + inRay.direction * t0hit;
    hit.normal = this->getNormal(hit.modelSpacePos, inRay);
    hit.inRay = inRay;

    hit.color.r = 0.0;
    hit.color.g = 0.0;
    hit.color.b = 0.0;

    return hit;
}

vec4 Sphere::getNormal(vec4 & pos, ray & inRay) {
    
    vec4 normVec = pos;
    normVec.w = 0.0;

    normVec = (this->normalToWorld).transform(normVec);

    return normVec;
}