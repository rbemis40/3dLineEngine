#include "pfblock.h"

#include "pfblock_plane.h"
#include "pfblock_point3d.h"
#include "pfblock_cube.h"

PFBlock::PFBlock(std::string block) {
    blockStr = cleanseInput(block);
    //printf("Cleansed input: %s\n", blockStr.c_str());
}

// Deallocate all of the newly created children block pointers
PFBlock::~PFBlock() {
    for(int i = 0; i < children.size(); i++) {
        delete children[i];
    }
}

std::string PFBlock::cleanseInput(std::string input) {
    std::string newStr = "";

    bool inComment = false; // Anything between # marks is commented out

    for (char c : input) {
        if(c == '#') {
            inComment = (inComment ? false : true);
        }
        else if (c != ' ' && c != '\n' && c != '\0' && c != '\t' && !inComment) {
            newStr += c;
        }
    }

    return newStr;
}

// Will go through all of the blocks defined at the top level, and create children blocks for them with proper input
// Will then parse each of the children blocks
void PFBlock::parseBlock() {
    std::string workingStr = blockStr;

    while (workingStr != "") {
        BlockType typeOfBlock; // Holds the data for the type of block that is read here
        int pos = findBlockName(workingStr, typeOfBlock); // Get the type of block, and the pos after the bracket {
        
        if (pos < 0) { // Check if there is an error and exit if there is
            printf("Error reading block!\n");
            return;
        }

        workingStr = workingStr.substr(pos); // Trim the string to start after the bracket found by findBlockName

        std::string dataStr; // Holds the string that stores the data within the brackets of the cur data block {}
        pos = findBlockData(workingStr, dataStr); // Read the data between the brackets and get the pos after the } bracket

        if (pos < 0) { // Check if there is an error and exit if there is
            printf("Error finding block data!\n");
            return;
        }

        workingStr = workingStr.substr(pos); // Trim the string to after the } bracket

        // Create the child block depending on the block type
        switch (typeOfBlock) {
            case BlockType::INVALID:
                printf("Invalid Block Type!\n");
                break;
            case BlockType::TOP:
                printf("Top Block Type!\n");
                
                break;
            case BlockType::PLANE:
            {
                PFBlockPlane* newPlaneBlock = new PFBlockPlane(dataStr); // Create he new block based off the data found in the {}
                children.push_back(newPlaneBlock); // Add it to the list of children that needs to be parsed after this

                break;
            }
            case BlockType::POINT:
            {
                PFBlockPoint3d* newPointBlock = new PFBlockPoint3d(dataStr); // Creat the new point block based off the data found in the {}
                children.push_back(newPointBlock);

                break;
            }
            case BlockType::CUBE:
            {
                PFBlockCube* newCubeBlock = new PFBlockCube(dataStr); // Create the new cube block based off the data found
                children.push_back(newCubeBlock);
            }
        }
    }

    // No errors have been found and all the blocks have been parsed
    // Now parse the children blocks found

    for (int i = 0; i < children.size(); i++) {
        children[i]->parseBlock();
    }
}

int PFBlock::findBlockName(std::string subBlock, BlockType& type) {
    // Find the name
    std::string potName = "";
    for (int i = 0; i < subBlock.length(); i++) {
        char curChar = subBlock[i];
        if (curChar == '{') {
            // The potential name has been found, now determine the type and return it
            if (potName == "Plane") {
                type = BlockType::PLANE;
            }
            else if (potName == "Point3d") {
                type = BlockType::POINT;
            }
            else if (potName == "Cube") {
                type = BlockType::CUBE;
            }
            else {
                type = BlockType::INVALID;
                return BE_INV_BLOCK;
            }

            return i + 1;
        }
        else if (curChar == '}') {
            //TODO: This is an invalid character and needs to be handled
            return BE_INV_CHAR;
        }
        else {
            potName += curChar;
        }
    }

    // If no end bracket was found, return an invalid type 
    type = BlockType::INVALID;
    return BE_EOF;
}

// Takes the position where findBlockName left off, and gets the data within the { } brackets, returns the position after the }
int PFBlock::findBlockData(std::string subBlock, std::string& data) {
    std::string blockData = ""; // Stores the data that is found between the brackets

    // TODO: Needs to track indentation level as a block nested in this block will break it
    int indentLevel = 0;

    for (int i = 0; i < subBlock.length(); i++) {
        char curChar = subBlock[i];
        if (curChar == '{') { // A new data block has been found so add to the indentation level
            indentLevel++;
        }
        if (curChar == '}') { // The end of a data block has been found
            if (indentLevel == 0) { // It is the end of the main data block, and so return the data and position
                data = blockData;
                return i + 1;
            }
            else { // The data block wasn't the main block, so subtract from the indent level
                indentLevel -= 1;
            }
        }
        
        blockData += curChar; // The end of the section hasn't been found so keep adding the data
    }

    // The end of the section was not found, which is an error
    return BE_EOF;
}