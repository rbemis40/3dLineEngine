#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <SDL.h>
#include <vector>
#include "cube.h"
#include "plane.h"

class Object3d {
public:
    virtual void update(Uint32 frameTime) = 0;
    virtual void render() = 0;
protected:
    std::vector<Cube> _cubes;
    std::vector<Plane> _planes;
private:
};

#endif