#ifndef PFBLOCK_PLANE
#define PFBLOCK_PLANE

#include <string>

#include "pfblock.h"
#include "plane.h"
#include "camera.h"

class PFBlockPlane : public PFBlock {
public:
    PFBlockPlane(std::string block);

    bool getValidity();
    Plane getPlane(Camera& cam);

    BlockType getType() const;
private:
};

#endif