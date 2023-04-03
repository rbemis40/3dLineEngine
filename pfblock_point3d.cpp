#include "pfblock_point3d.h"

PFBlockPoint3d::PFBlockPoint3d(std::string blockStr) : 
    PFBlock(blockStr), 
    isValid(false) {}

// TODO: Break this into smaller function?
void PFBlockPoint3d::parseBlock() {
    std::size_t lastPos = 0; // Stores the start of the substr for the float values
    std::size_t curPos = 0; // Stores the current position of the comma to substring the rest of the string

    // The first x value should be the data up to the first ','
    curPos = blockStr.find_first_of(',', curPos);
    if (curPos == std::string::npos) {
        printf("Invalid point data!!!\n");
        return;
    }

    std::string xVal = blockStr.substr(lastPos, curPos - lastPos);
    printf("XVal: %s\n", xVal.c_str());

    // True to convert x to a float
    try {
        point.x = std::stof(xVal);
    }
    catch (std::invalid_argument ia) {
        printf("Unable to convert x value: %s\n", ia.what());
        return;
    }

    lastPos = curPos + 1;

    // The second value up to the next ',' should be the y value
    curPos = blockStr.find_first_of(',', lastPos);
    if (curPos == std::string::npos) {
        printf("Invalid point data!!!\n");
        return;
    }


    std::string yVal = blockStr.substr(lastPos, curPos - lastPos);
    printf("YVal: %s\n", yVal.c_str());

    try {
        point.y = std::stof(yVal);
    }
    catch (std::invalid_argument ia) {
        printf("Unable to convert y value: %s\n", ia.what());
        return;
    }

    lastPos = curPos + 1;

    // The final value up to the end of the string should be the z value
    std::string zVal = blockStr.substr(lastPos);
    printf("ZVal: %s\n", zVal.c_str());

    try {
        point.z = std::stof(zVal);
    }
    catch (std::invalid_argument ia) {
        printf("Unable to convert z value: %s\n", ia.what());
        return;
    }

    isValid = true; // Update he validity, since all of the data has been correctly read
}

BlockType PFBlockPoint3d::getType() const {
    return BlockType::POINT;
}

bool PFBlockPoint3d::getValidity() {
    return isValid;
}

Point3d PFBlockPoint3d::getPoint() {
    return point;
}
