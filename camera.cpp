#include "camera.h"

#include <cmath>

Camera::Camera(float fov, ProgInstance& progInst) : 
    _fov(fov),
    _fovTan(tan(_fov/2)),
    _progInst(progInst),
    _pos(Point3d{0, 0, 0}),
    _defSpeed(0.05),
    _lastFrameTime(0),
    _rotX(0),
    _rotY(0),
    _posFont("fonts/Geneva.ttf", 12, progInst)
{}


Point2d Camera::pointProjection(Point3d pt) {
    float clippedZ = pt.z;

    // Prevent the view from flipping inverse or expanding to infinity
    float zCutoff = _defSpeed * _lastFrameTime;
    if (clippedZ < 0.2) {
        clippedZ = 0.2;
    }

    float zPlaneScaleM = _fovTan * (clippedZ) * 2.0f;

    // _progInst.getRenWidth() can be any value, but scales z distances which can get weird for other values
    float pxPerM = _progInst.getRenWidth() / zPlaneScaleM;

    float xPos = (_progInst.getRenWidth() / 2.0) + ((pt.x) * pxPerM);
    float yPos = (_progInst.getRenHeight() / 2.0) + ((pt.y) * pxPerM);

    Point2d projPoint = {xPos, yPos};
    return projPoint;
}

void Camera::applyPosition(Point3d& pt) {
    pt.x -= _pos.x;
    pt.y -= _pos.y;
    pt.z -= _pos.z;
}

void Camera::applyRotation(Point3d& pt) {
    float ptX = pt.x;
    float ptZ = pt.z;
    float ptY = pt.y;

    float newX = ptX * cos(_rotX) - ptZ * sin(_rotX);
    ptZ = ptX * sin(_rotX) + ptZ * cos(_rotX);

    float newZ = ptZ * cos(_rotY) - ptY * sin(_rotY);
    float newY = ptZ * sin(_rotY) + ptY * cos(_rotY);

    pt.x = newX;
    pt.y = newY;
    pt.z = newZ;
}

void Camera::renderDebug(int x, int y) {
    SDL_Color debugFg = {255, 255, 255, 255};
    SDL_Color debugBg = {0, 0, 0, 255};

    const int FONT_SIZE = 12;
    int curYPos = y;

    // Draw x pos text
    _posFont.setPosition(x, curYPos);
    _posFont.setText("X: " + std::to_string(_pos.x), debugFg, &debugBg);

    _posFont.render();

    curYPos += FONT_SIZE;

    // Draw y pos text
    _posFont.setPosition(x, curYPos);
    _posFont.setText("Y: " + std::to_string(_pos.y), debugFg, &debugBg);

    _posFont.render();

    curYPos += FONT_SIZE;

    // Draw z pos text
    _posFont.setPosition(x, curYPos);
    _posFont.setText("Z: " + std::to_string(_pos.z), debugFg, &debugBg);

    _posFont.render();

    // Draw xRot text

    curYPos += FONT_SIZE * 2;

    _posFont.setPosition(x, curYPos);
    _posFont.setText("X Rot: " + std::to_string(_rotX), debugFg, &debugBg);

    _posFont.render();

    // Draw yRot text
    curYPos += FONT_SIZE;

    _posFont.setPosition(x, curYPos);
    _posFont.setText("Y Rot: " + std::to_string(_rotY), debugFg, &debugBg);

    _posFont.render();
}

void Camera::changeX(float dX) {
    _pos.x += dX;
}

void Camera::changeY(float dY) {
    _pos.y += dY;
}

void Camera::changeZ(float dZ) {
    _pos.z += dZ;
}

void Camera::moveAdjZ(float dZ) {
    _pos.z += dZ * cos(_rotX);
    _pos.x += dZ * sin(_rotX);
}

void Camera::moveAdjX(float dX) {
    _pos.x += dX * cos(_rotX);
    _pos.z -= dX * sin(_rotX);
}

void Camera::setX(float x) {
    _pos.x = x;
}

void Camera::setY(float y) {
    _pos.y = y;
}

void Camera::setZ(float z) {
    _pos.z = z;
}

void Camera::changeXRot(float dXRot) {
    _rotX += dXRot;
}

void Camera::changeYRot(float dYRot) {
    _rotY += dYRot;
}

float Camera::getSpeed() const {
    return _defSpeed * _lastFrameTime;
}

void Camera::update(Uint32 frameTime) {
    _lastFrameTime = frameTime;
}

Point3d Camera::getPos() const {
    return _pos;
}

ProgInstance& Camera::getInstance() {
    return _progInst;
}