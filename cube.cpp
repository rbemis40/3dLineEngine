#include "cube.h"

Cube::Cube(Point3d pos, float w, float h, float d, Camera& cam) :
    _pos(pos),
    _planes{
        Plane({
            Point3d{pos.x, pos.y, pos.z},
            Point3d{pos.x + w, pos.y, pos.z},
            Point3d{pos.x + w, pos.y + h, pos.z},
            Point3d{pos.x, pos.y + h, pos.z}
        }, cam),
        Plane({
            Point3d{pos.x, pos.y, pos.z},
            Point3d{pos.x, pos.y, pos.z + d},
            Point3d{pos.x, pos.y + h, pos.z + d},
            Point3d{pos.x, pos.y + h, pos.z}
        }, cam),
        Plane({
            Point3d{pos.x, pos.y, pos.z + d},
            Point3d{pos.x + w, pos.y, pos.z + d},
            Point3d{pos.x + w, pos.y + h, pos.z + d},
            Point3d{pos.x, pos.y + h, pos.z + d}
        }, cam),
        Plane({
            Point3d{pos.x + w, pos.y, pos.z},
            Point3d{pos.x + w, pos.y, pos.z + d},
            Point3d{pos.x + w, pos.y + h, pos.z + d},
            Point3d{pos.x + w, pos.y + h, pos.z}
        }, cam)
    }
{
    _dim = Point3d{w, h, d};
}

void Cube::setPos(Point3d newPos) {

}

Point3d Cube::getPos() {
    return _pos;
}

Point3d Cube::getDim() {
    return _dim;
}

void Cube::update(Uint32 frameTime) {
    for (int i = 0; i < 4; i++) {
        _planes[i].update(frameTime);
    }
}

void Cube::render() {
    for (int i = 0; i < 4; i++) {
        _planes[i].render();
    }
}