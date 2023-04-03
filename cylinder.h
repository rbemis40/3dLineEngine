#ifndef CYLINDER_H
#define CYLINDER_H

#include <vector>
#include <SDL.h>
#include "points.h"
#include "plane.h"
#include "object3d.h"
#include "camera.h"

class Cylinder : public Object3d {
public:
    Cylinder(Point3d pos, float radius, float h, Camera& cam);

    void update(Uint32 frameTime);
    void render();
private:
    std::vector<Plane> _planes;
};

#endif