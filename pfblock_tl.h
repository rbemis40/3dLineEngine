#ifndef PF_BLOCK_TL_H
#define PF_BLOCK_TL_H

#include <string>
#include <vector>

#include "pfblock.h"
#include "plane.h"
#include "camera.h"
#include "cube.h"

class PFBlockTopLevel : public PFBlock {
public:
    PFBlockTopLevel(std::string block);
    
    bool getValidity();
    std::vector<Plane> getPlanes(Camera& cam);
    std::vector<Cube> getCubes(Camera& cam);

    BlockType getType() const;
private:

};

#endif