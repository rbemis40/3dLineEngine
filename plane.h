#ifndef PLANE_H
#define PLANE_H

#include <vector>
#include <initializer_list>
#include <SDL.h>

#include "points.h"
#include "camera.h"
#include "lineseg.h"

class Plane {
public:
    Plane(std::vector<Point3d> points, Camera& cam);
    ~Plane();

    void printPoints();
    void update(Uint32 frameTime);
    void render();
private:

    void pointsTo2d();

    std::vector<Point3d> _points3d;
    Point3d _rotPoints3d[4];
    std::vector<Point2d> _points2d;
    std::vector<LineSegment> _segs;
    Camera& _cam;

    Point3d _rotPoint{0, 0, 0};
    float _rot;
};

#endif