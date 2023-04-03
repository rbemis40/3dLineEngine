#ifndef CAMERA_H
#define CAMERA_H

#include "prog_inst.h"
#include "points.h"
#include "rufont.h"

class Camera {
public:
    Camera(float fov, ProgInstance& progInst);

    Point2d pointProjection(Point3d pt);

    void applyPosition(Point3d& pt);
    void applyRotation(Point3d& pt);

    void changeX(float dX);
    void changeY(float dY);
    void changeZ(float dz);

    void moveAdjZ(float dZ);
    void moveAdjX(float dX);
    void moveAdjY(float dY);

    void setX(float x);
    void setY(float y);
    void setZ(float z);

    void changeXRot(float dXRot);
    void changeYRot(float dYRot);

    Point3d getPos() const;

    void update(Uint32 frameTime);
    void renderDebug(int x, int y);

    float getSpeed() const;

    ProgInstance& getInstance();

private:
    float _fov;
    float _fovTan;
    const float _defSpeed;
    float _lastFrameTime;
    ProgInstance& _progInst;

    Point3d _pos;

    float _rotX;
    float _rotY;

    RUFont _posFont;
};

#endif