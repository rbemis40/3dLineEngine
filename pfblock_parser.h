#ifndef PFBLOCK_PARSER_H
#define PFBLOCK_PARSER_H

#include <vector>
#include <string>

#include "pfblock.h"

class PFSyntaxError : public std::exception {
public:
    PFSyntaxError(std::string errMsg) : msg(errMsg) {}

    const char* what() {
        return msg.c_str();
    }
private:
    std::string msg;
};

class PFBlockParser {
public:
    PFBlockParser(std::string rawStr);
    bool parse();
private:
    std::string cleanseInput(std::string dirtyStr);
    std::string findBlockName(std::string searchStr);

    char safeRead();


    std::string inputStr;
    std::vector<PFBlock*> tlBlocks; // Top-level blocks

    int indentLevel;
    int curReadPos;
};

#endif