#include "lineseg.h"

#include <cmath>
#include <algorithm>

LineSegment::LineSegment() : 
    _vec(0, 0)
{
    setPoint(Point2d{0, 0});
}

LineSegment::LineSegment(Vector2d<float> vec, Point2d point) :
    _vec(vec)
{
    setPoint(point);
}

void LineSegment::setPoint(Point2d point) {
    _point = point;
    if (_vec.getX() != 0) {
        _maxT = getTFromX(point.x + _vec.getX());
    }
    else {
        _maxT = getTFromY(point.y + _vec.getY());
    }
}

Point2d LineSegment::getIPoint() const {
    return _point;
}

Point2d LineSegment::getFPoint() const {
    return Point2d{_point.x + _vec.getX(), _point.y + _vec.getY()};
}

Vector2d<float> LineSegment::getVec() const {
    return _vec;
}

Point2d LineSegment::getPoint(float t) const {
    float xVal = _vec.getX() * t + _point.x;
    float yVal = _vec.getY() * t + _point.y;

    return Point2d{xVal, yVal};
}

void LineSegment::render(SDL_Renderer *ren) const {
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderDrawLine(ren, _point.x, _point.y, _point.x + _vec.getX(), _point.y + _vec.getY());
}

float LineSegment::getTFromX(float x) const {
    return (x - _point.x) / _vec.getX();
}

float LineSegment::getTFromY(float y) const {
    return (y - _point.y) / _vec.getY();
}

bool LineSegment::isPointOnLine(const Point2d& point) const {
    float xT = getTFromX(point.x);
    float yT = getTFromY(point.y);

    // Handle special casees
    if (_vec.getX() == 0) {
        return point.x == _point.x;
    }
    else if (_vec.getY() == 0) {
        return point.y == _point.y;
    }
    else {
        if (abs(xT - yT) <= 0.001) {
            return true;
        }
        return false;
    }
}

// For the point to be on the line, the x and y values have to be at the same t value
bool LineSegment::isPointOnSegment(const Point2d& point) const {
    bool onLine = isPointOnLine(point);

    // Handle special cases

    Point2d iPoint = getIPoint();
    Point2d fPoint = getFPoint();

    if (_vec.getX() == 0) {
        float maxY = std::max(iPoint.y, fPoint.y);
        float minY = std::min(iPoint.y, fPoint.y);

        return (point.y <= maxY && point.y >= minY);
    }
    else if (_vec.getY() == 0) {
        float maxX = std::max(iPoint.x, fPoint.x);
        float minX = std::min(iPoint.x, fPoint.x);

        return (point.x <= maxX && point.x >= minX);
    }
    else {
        float t = getTFromX(point.x);
        if (t <= _maxT && t >= 0) {
            return true;
        }

        return false;
    }
}

IntersectPoint LineSegment::calcHorzIntersect(const LineSegment& ls1, float yVal) {
    float t = ls1.getTFromY(yVal);
    if (t >= 0 && t <= ls1._maxT && ls1._vec.getY() != 0) {
        IntersectPoint point;
        point.didIntersect = true;
        point.onLine = true;
        point.sameLine = false;
        // NEED to actually determine if its the same line
        point.point = ls1.getPoint(t);

        return point;
    }
    else {
        IntersectPoint point;
        point.didIntersect = false;
        return point;
    }
}

IntersectPoint LineSegment::calcIntersect(const LineSegment& ls1, const LineSegment& ls2) {
    float tNum = ls2._vec.getY() * (ls1._point.x - ls2._point.x) - ls2._vec.getX() * (ls1._point.y - ls2._point.y);
    float tDenom = ls2._vec.getX() * ls1._vec.getY() - ls2._vec.getY() * ls1._vec.getX();

    IntersectPoint intPoint;

    if (abs(tDenom) <= 0.00001) {
        // The lines do not intersect, or are the same line

        // Check if the lines are the same (contain the same points)
        bool sameLine = ls2.isPointOnSegment(ls1._point);

        if (sameLine) {
            intPoint.didIntersect = true;
            intPoint.sameLine = true;
        }
        else {
            intPoint.didIntersect = false;
        }
    }
    else {
        // At this point, the LINES do intersect, now we need to see if the SEGMENTS intersect
        float t = tNum / tDenom;

        intPoint.didIntersect = true;
        intPoint.point = ls1.getPoint(t);

        intPoint.sameLine = false;

        if (ls1.isPointOnSegment(intPoint.point) && ls2.isPointOnSegment(intPoint.point)) {
            intPoint.onLine = true;
        }
        else {
            intPoint.onLine = false;
        }
    }
    return intPoint;
}