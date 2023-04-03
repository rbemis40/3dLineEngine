#ifndef POINT_FILE_H
#define POINT_FILE_H

#include <string>
#include <fstream>

class PointFile {
public:
    PointFile(std::string file);

    void close();

private:
    readFile();

    std::ifstream inStream;
};

#endif