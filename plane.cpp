#include "plane.h"

#include <cstdio>

#include "drawtools.h"
#include "vector2d.h"

Plane::Plane(std::vector<Point3d> points, Camera& cam) : 
    _segs(4),
    _points2d(4), 
    _cam(cam),
    _rot(0.05)
{
    if (points.size() != 4) {
        throw (points.size());
    }

    _points3d = points;

    for (int i = 0; i < 4; i++) {
        _points2d[i] = Point2d{0, 0};
    }
}

Plane::~Plane() {}

void Plane::printPoints() {
    for (int i = 0; i < 4; i++) {
        std::printf("_points[%d] = (%f, %f, %f)\n", i, _points3d[i].x, _points3d[i].y, _points3d[i].z);
    }
}

void Plane::update(Uint32 frameTime) {
    int i = 0;
    for (auto it = _points3d.begin(); it != _points3d.end(); it++, i++) {
        _rotPoints3d[i] = *it;
        _cam.applyPosition(_rotPoints3d[i]);
        _cam.applyRotation(_rotPoints3d[i]);
    }
    pointsTo2d();
}

void Plane::pointsTo2d() {
    for (int i = 0; i < 4; i++) {
        _points2d[i] = _cam.pointProjection(_rotPoints3d[i]);
    }

    auto lastPoint = _points2d[3];
    for (int i = 0; i < 4; i++) {
        _segs[i] = LineSegment(Vector2d<float>(lastPoint.x - _points2d[i].x, lastPoint.y - _points2d[i].y), _points2d[i]);
        lastPoint = _points2d[i];
    }
}

void Plane::render(SDL_Renderer *ren) {
    // TODO: Need to implement a fill polygon function, as this only works for planes perpendicular to the camera view
    Point3d camPos = _cam.getPos();
    Point3d lastPoint = _rotPoints3d[3];

    ProgInstance& curInst = _cam.getInstance();

    for (int i = 0; i < 4; i++) {
        bool shouldDraw = true;

        // Check if the segment is off the screen to the left
        if (_segs[i].getIPoint().x < 0 && _segs[i].getFPoint().x < 0) {
            shouldDraw = false;
        }
        //Check if the segment is off the screen to the right
        else if(_segs[i].getIPoint().x > curInst.getRenWidth() && _segs[i].getFPoint().x > curInst.getRenWidth()) {
            shouldDraw = false;
        }
        // Check if the segment is off the screen to the top
        else if(_segs[i].getIPoint().y < 0 && _segs[i].getFPoint().y < 0) {
            shouldDraw = false;
        }
        // Check if the segment is off the screen to the bottom
        else if(_segs[i].getIPoint().y > curInst.getRenHeight() && _segs[i].getFPoint().y > curInst.getRenHeight()) {
            shouldDraw = false;
        }

        Point3d curPoint = _rotPoints3d[i];
        // Check if the segment is on the screen in the z-direction
        if(lastPoint.z < 0 && curPoint.z < 0) {
            shouldDraw = false;
        }

        // Increment the lastPoint to check the next segment's 3d coordinates
        lastPoint = curPoint;

        if (shouldDraw) {
            _segs[i].render(ren);
        }
    }

    //if (shouldDraw) DrawTools::fillQPolygon(ren, _points2d, SDL_Color{255, 255, 255, 255});

    //SDL_RenderCopy(ren, _colorTex, NULL, NULL);

    // SDL_Rect drawRect;
    // drawRect.x = _points2d[0].x;
    // drawRect.y = _points2d[0].y;

    // drawRect.w = _points2d[1].x - _points2d[0].x;
    // drawRect.h = _points2d[3].y - _points2d[0].y;

    // SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    // SDL_RenderFillRect(ren, &drawRect);
}
