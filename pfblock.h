#ifndef PF_BLOCK_H
#define PF_BLOCK_H

#include <vector>
#include <string>

enum class BlockType {
    INVALID,
    TOP,
    POINT,
    PLANE,
    CUBE
};

enum BlockErrors {
    BE_EOF = -1,
    BE_INV_CHAR = -2,
    BE_INV_BLOCK = -3
};

class PFBlock {

public:
    PFBlock(std::string block);
    virtual ~PFBlock();

    virtual void parseBlock();
    virtual BlockType getType() const = 0;
protected:
    std::vector<PFBlock*> children;
    std::string blockStr;
private:
    // Takes the input string and removews any white space to make parsing easier
    std::string cleanseInput(std::string input);

    // Find the name of the block and returns the position after the bracket
    int findBlockName(std::string subBlock, BlockType& type);

    // Find the inner data of the block and return the position after the bracket
    int findBlockData(std::string subBlock, std::string& data);
};

#endif