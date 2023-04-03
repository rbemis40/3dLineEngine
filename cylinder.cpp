#include "cylinder.h"

#include <cmath>

Cylinder::Cylinder(Point3d pos, float radius, float h, Camera& cam) {
    const int numPlanes = 50;
    const int numPoints = numPlanes * 2;

    const float radPerPlane = (M_PI * 2) / numPlanes;

    float curAngle = 0;

    for (int i = 0; i < numPlanes; i++) {
        Point3d p1;
        Point3d p2;

        p1.y = pos.y;

        p1.x = pos.x + radius * cos(curAngle);
        p1.z = pos.z + radius * sin(curAngle);

        curAngle += radPerPlane;

        p2.y = pos.y;

        p2.x = pos.x + radius * cos(curAngle);
        p2.z = pos.z + radius * sin(curAngle);
        

        Point3d p3{p2.x, p2.y + h, p2.z};
        Point3d p4{p1.x, p1.y + h, p1.z};

        // printf("Point 1: %f, %f, %f\n", p1.x, p1.y, p1.z);
        // printf("Point 2: %f, %f, %f\n", p2.x, p2.y, p2.z);
        // printf("Point 3: %f, %f, %f\n", p3.x, p3.y, p3.z);
        // printf("Point 4: %f, %f, %f\n", p4.x, p4.y, p4.z);

        std::vector<Point3d> newPoints{
            p1,
            p2,
            p3,
            p4
        };

        _planes.push_back(Plane(newPoints, cam));
    }
}

void Cylinder::update(Uint32 frameTime) {
    for (auto& plane : _planes) {
        plane.update(frameTime);
    }
}

void Cylinder::render() {
    for(auto& plane : _planes) {
        plane.render();
    }
}