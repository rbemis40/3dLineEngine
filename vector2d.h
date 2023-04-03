#ifndef POINTS_H
#define POINTS_H

#include "points.h"

template <typename T>
class Vector2d {
public:
    Vector2d(T x, T y);

    T getX() const;
    T getY() const;

    void setX(T x);
    void setY(T y);

    float getMagnitude() const;

    void multScalar(T scalar);
    Vector2d getMultScalarVector(T scalar) const;

    Vector2d getScaledVector(T scaleFactor);

    void addVec(const Vector2d& vec2);
    static T dotProd(const Vector2d& vec1, const Vector2d& vec2);

private:
    float _x;
    float _y;
};

#endif