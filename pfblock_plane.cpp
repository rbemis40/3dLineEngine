#include "pfblock_plane.h"
#include "pfblock_point3d.h"

PFBlockPlane::PFBlockPlane(std::string block) : PFBlock(block) {}

BlockType PFBlockPlane::getType() const {
    return BlockType::PLANE;
}

bool PFBlockPlane::getValidity() {
    if (children.size() != 4) {
        return false;
    }

    for (const auto child : children) {
        if (child->getType() != BlockType::POINT || !((PFBlockPoint3d*)child)->getValidity()) {
            return false;
        }
    }

    return true;
}

Plane PFBlockPlane::getPlane(Camera& cam) {
    std::vector<Point3d> points(4);

    for(int i = 0; i < children.size(); i++) {
        points[i] = ((PFBlockPoint3d*)children[i])->getPoint();
    }

    return Plane(points, cam);
}