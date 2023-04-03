#ifndef CUBE_H
#define CUBE_H

#include <initializer_list>
#include <SDL.h>
#include <vector>

#include "points.h"
#include "plane.h"
#include "camera.h"

class Cube {
public:
    Cube(Point3d pos, float w, float h, float d, Camera& cam);

    void setPos(Point3d newPos);

    void update(Uint32 frameTime);
    void render(SDL_Renderer* ren);

    Point3d getPos();
    Point3d getDim();

private:
    Point3d _pos;
    Point3d _dim;

    Plane _planes[4];
};

#endif