#ifndef PFBLOCK_CUBE_H
#define PFBLOCK_CUBE_H

#include "pfblock.h"
#include "cube.h"

class PFBlockCube : public PFBlock {
public:
    PFBlockCube(std::string block);
    BlockType getType() const;

    Cube getCube(Camera& cam);
    bool getValidity();
private:
};

#endif