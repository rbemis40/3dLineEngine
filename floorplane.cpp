#include "floorplane.h"

FloorPlane::FloorPlane(Camera& cam) :
    _numPlanes(100),
    _pool(8)
{
    for (int zI = 0; zI < _numPlanes; zI++) {
        for (int xI = 0; xI < _numPlanes; xI++) {
            float x = (float)xI;
            float z = (float)zI;
            _planes.push_back(Plane({
                Point3d{x, 0, z},
                Point3d{x+1, 0, z},
                Point3d{x+1, 0, z+1},
                Point3d{x, 0, z+1}
            }, cam));
        }
    }
}

void FloorPlane::update(Uint32 frameTime) {
    for(int i=0; i < _planes.size() / (_numPlanes * 2); i++) {
        _pool.addJob(RenderJob{&_planes, i * _numPlanes * 2, (i * _numPlanes * 2) + (_numPlanes * 2), frameTime});
    }

    _pool.waitForEmptyQueue();

    // for(auto pIt = _planes.begin(); pIt != _planes.end(); pIt++) {
    //      pIt->update(frameTime);
    // }
}

void FloorPlane::render() {
    for(auto pIt = _planes.begin(); pIt != _planes.end(); pIt++) {
        pIt->render();
    }
}