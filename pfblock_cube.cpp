#include "pfblock_cube.h"
#include "pfblock_point3d.h"

PFBlockCube::PFBlockCube(std::string block) : PFBlock(block){}

BlockType PFBlockCube::getType() const {
    return BlockType::CUBE;
}

bool PFBlockCube::getValidity() {
    if (children.size() != 2) {
        return false;
    }

    // Ensure all children are Points and they are valid
    for (const PFBlock* block : children) {
        if(block->getType() != BlockType::POINT || !((PFBlockPoint3d*)(block))->getValidity()) {
            return false;
        }
    }

    return true;
}

// TODO: Should integrate validity check into this method
Cube PFBlockCube::getCube(Camera& cam) {
    Point3d cubePos = ((PFBlockPoint3d*)(children[0]))->getPoint();
    Point3d cubeDim = ((PFBlockPoint3d*)(children[1]))->getPoint();

    Cube newCube(cubePos, cubeDim.x, cubeDim.y, cubeDim.z, cam);
    return newCube;
}