#include "pfblock_parser.h"

PFBlockParser::PFBlockParser(std::string rawStr) :
    indentLevel(0),
    curReadPos(0)
{
    inputStr = cleanseInput(rawStr);
    printf("inputStr: %s\n", inputStr.c_str());
}

std::string PFBlockParser::cleanseInput(std::string dirtyString) {
    std::string newStr = "";
    for (char c: dirtyString) {
        if (c != ' ' && c != '\n') {
            newStr += c;
        }
    }

    return newStr;
}

bool PFBlockParser::parse() {
    while (curReadPos < inputStr.length()) {
        // Step 1: Find the next block name

        // Step 2: Repeat 
    }
}

std::string PFBlockParser::findBlockName(std::string searchStr) {
    std::string potName = "";
    for (char curChar : searchStr) {
        if (curChar == '{') { // The name was found so return it (new block)
            return potName;
        }
        else if (curChar == '}') { // This is an invalid character, so return an error
            std::string errMsg = "Unexpected '}' while searching '" + searchStr + "'!";
            throw PFSyntaxError(errMsg);
        }
        else { // No error and the end of the name hasn't been found yet
            potName += curChar;
        }
    }

    // A new block was never found, so the syntax is incorrect
    std::string errMsg = "Unexpected end of search string!";
    throw PFSyntaxError(errMsg);
}