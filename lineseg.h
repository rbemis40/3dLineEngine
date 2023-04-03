#ifndef LINESEG_H
#define LINESEG_H

#include <SDL.h>

#include "vector2d.h"
#include "points.h"

struct IntersectPoint {
    Point2d point;
    bool didIntersect = false;
    bool sameLine = false;
    bool onLine = false;
};

class LineSegment {
public:
    LineSegment();
    LineSegment(Vector2d<float> vec, Point2d point);

    void setPoint(Point2d point);
    Point2d getIPoint() const;
    Point2d getFPoint() const;

    Vector2d<float> getVec() const;

    Point2d getPoint(float t) const;

    float getTFromX(float x) const;
    float getTFromY(float y) const;

    bool isPointOnLine(const Point2d& point) const;
    bool isPointOnSegment(const Point2d& point) const;

    void render(SDL_Renderer *ren) const;

    static IntersectPoint calcIntersect(const LineSegment& ls1, const LineSegment& ls2);
    static IntersectPoint calcHorzIntersect(const LineSegment& ls1, float yVal);
private:
    Vector2d<float> _vec;
    Point2d _point;
    float _maxT;
};

#endif