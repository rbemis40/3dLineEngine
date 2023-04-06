#include "pfblock_tl.h"

#include "pfblock_plane.h"
#include "pfblock_cube.h"

PFBlockTopLevel::PFBlockTopLevel(std::string block) : PFBlock(block) {}

BlockType PFBlockTopLevel::getType() const {
    return BlockType::TOP;
}

bool PFBlockTopLevel::getValidity() {
    if (children.size() == 0) {
        return false;
    }

    for(const auto child : children) {
        if (child->getType() == BlockType::PLANE) {
            if(!((PFBlockPlane*)child)->getValidity()) { return false; }
        }
        else if (child->getType() == BlockType::CUBE) {
            if(!((PFBlockCube*)child)->getValidity()) { return false; }
        }
        else {
            return false;
        }
    }

    return true;
}

// TODO: Should integrate the validity check into this to ensure we arent returning invalid planes
std::vector<Plane> PFBlockTopLevel::getPlanes(Camera& cam) {
    std::vector<Plane> planes;

    for(const auto child : children) {
        if(child->getType() == BlockType::PLANE) {
            planes.push_back(((PFBlockPlane*)child)->getPlane(cam));
        }
    }

    return planes;
}

// TODO: Should integrate the validity check into this to ensure we arent returning invalid cubes
std::vector<Cube> PFBlockTopLevel::getCubes(Camera& cam) {
    std::vector<Cube> cubes;

    for(const auto child : children) {
        if (child->getType() == BlockType::CUBE) {
            cubes.push_back(((PFBlockCube*)child)->getCube(cam));
        }
    }

    return cubes;
}