#include "vector2d.h"

#include <cmath>

template <typename T>
Vector2d<T>::Vector2d(T x, T y) : _x(x), _y(y) {}

template <typename T>
T Vector2d<T>::getX() const {
    return _x;
}

template <typename T>
T Vector2d<T>::getY() const {
    return _y;
}

template <typename T>
void Vector2d<T>::setX(T x) {
    _x = x;
}

template <typename T>
void Vector2d<T>::setY(T y) {
    _y = y;
}

template <typename T>
void Vector2d<T>::addVec(const Vector2d& vec2) {
    setX(getX() + vec2.getX());
    setY(getY() + vec2.getY());
}

template <typename T>
T Vector2d<T>::dotProd(const Vector2d& vec1, const Vector2d& vec2) {
    return vec1.getX() * vec2.getX() - vec1.getY() * vec2.getY();
}

template <typename T>
void Vector2d<T>::multScalar(T scalar) {
    _x *= scalar;
    _y *= scalar;
}

template <typename T>
Vector2d<T> Vector2d<T>::getMultScalarVector(T scalar) const {
    return Vector2d<T>(_x * scalar, _y * scalar);
}

template <typename T>
float Vector2d<T>::getMagnitude() const {
    return sqrt(pow(_x, 2) + pow(_y, 2));
}

template <typename T>
Vector2d<T> Vector2d<T>::getScaledVector(T scaleFactor) {
    return Vector2d<T>((_x / getMagnitude()) * scaleFactor, (_y / getMagnitude()) * scaleFactor);
}

template class Vector2d<int>;
template class Vector2d<float>;
template class Vector2d<double>;
