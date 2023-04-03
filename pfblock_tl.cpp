#include "pfblock_tl.h"

#include "pfblock_plane.h"

PFBlockTopLevel::PFBlockTopLevel(std::string block) : PFBlock(block) {}

BlockType PFBlockTopLevel::getType() const {
    return BlockType::TOP;
}

bool PFBlockTopLevel::getValidity() {
    if (children.size() == 0) {
        return false;
    }

    for(const auto child : children) {
        if (child->getType() != BlockType::PLANE || !((PFBlockPlane*)child)->getValidity()) {
            return false;
        }
    }

    return true;
}

// TODO: Should integrate the validity check into this to ensure we arent returning invalid planes
std::vector<Plane> PFBlockTopLevel::getPlanes(Camera& cam) {
    std::vector<Plane> planes;

    for(const auto child : children) {
        planes.push_back(((PFBlockPlane*)child)->getPlane(cam));
    }

    return planes;
}