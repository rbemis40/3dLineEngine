#ifndef DRAWTOOLS_H
#define DRAWTOOLS_H

#include <SDL.h>
#include <vector>

#include "vector2d.h"
#include "points.h"
#include "lineseg.h"

using PolyVec = const Vector2d<float>&;

class DrawTools {

public:
    static void fillQPolygon(SDL_Renderer* ren, std::vector<Point2d>& points, SDL_Texture* tex, int u, int v);
    static void drawUVMap(SDL_Renderer* ren, SDL_Texture* tex, const std::vector<Point2d>& points);
private:
    static Point2d calcUVPoint(
        PolyVec vecTop, LineSegment segTop, 
        PolyVec vecBot, LineSegment segBot, 
        PolyVec vecLeft, LineSegment segLeft, 
        PolyVec vecRight, LineSegment segRight,
        int u, int v, int numSegs, SDL_Renderer* ren);
};

#endif