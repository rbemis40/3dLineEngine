#ifndef PFBLOCK_POINT3D_H
#define PFBLOCK_POINT3D_H

#include <string>
#include "pfblock.h"
#include "points.h"

class PFBlockPoint3d : public PFBlock {
public:
    PFBlockPoint3d(std::string blocKStr);

    void parseBlock(); // Needs to override this method because the point takes 3 comma separated values

    bool getValidity(); // Returns whether the data has been read and is currently valid
    Point3d getPoint();

    BlockType getType() const;
private:
    Point3d point;

    bool isValid;
};

#endif