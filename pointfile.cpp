#include "pointfile.h"

PointFile::PointFile(std::string file) {
    try {
        inStream.open(file);
    }
    catch (std::ios_base::failure err) {
        printf("Error: %s\n", err.what());
    }

    close();
}

void PointFile::close() {
    if (inStream.is_open()) {
        inStream.close();
    }
}