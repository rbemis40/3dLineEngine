#ifndef FLOOR_PLANE_H
#define FLOOR_PLANE_H

#include "object3d.h"
#include "threadpool.h"

class FloorPlane : public Object3d {
public:
    FloorPlane(Camera& cam);

    void update(Uint32 frameTime);
    void render();
private:
    RenThreadPool _pool;
    const int _numPlanes;
};

#endif