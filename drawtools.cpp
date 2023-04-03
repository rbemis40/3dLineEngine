#include "drawtools.h"
#include <cmath>

void DrawTools::drawUVMap(SDL_Renderer* ren, SDL_Texture* tex, const std::vector<Point2d>& points) {
    if (points.size() != 4) {
        throw points.size();
    }

    auto vecTop = Vector2d<float>(points[1].x - points[0].x, points[1].y - points[0].y);
    auto vecRight = Vector2d<float>(points[2].x - points[1].x, points[2].y - points[1].y);
    auto vecBottom = Vector2d<float>(points[3].x - points[2].x, points[3].y - points[2].y);
    auto vecLeft = Vector2d<float>(points[0].x - points[3].x, points[0].y - points[3].y);

    // Generate Line Segments
    LineSegment segTop(vecTop, points[0]);
    LineSegment segRight(vecRight, points[1]);
    LineSegment segBottom(vecBottom, points[2]);
    LineSegment segLeft(vecLeft, points[3]);

    //int texWidth;
    //SDL_QueryTexture(tex, NULL, NULL, &texWidth, NULL);

    const int numSegs = 32;

    //segTop.render(ren);
    //segBottom.render(ren);
    //segRight.render(ren);
    //segLeft.render(ren);

    std::vector<Point2d> dPoints(4);

    for (int v = 0; v < numSegs; v++) {
        for (int u = 0; u < numSegs; u++) {
            
            

            dPoints[0] = calcUVPoint(vecTop, segTop, vecBottom, segBottom, vecLeft, segLeft, vecRight, segRight, u, v, numSegs, ren);
            dPoints[1] = calcUVPoint(vecTop, segTop, vecBottom, segBottom, vecLeft, segLeft, vecRight, segRight, u+1, v, numSegs, ren);
            dPoints[2] = calcUVPoint(vecTop, segTop, vecBottom, segBottom, vecLeft, segLeft, vecRight, segRight, u+1, v+1, numSegs, ren);
            dPoints[3] = calcUVPoint(vecTop, segTop, vecBottom, segBottom, vecLeft, segLeft, vecRight, segRight, u, v+1, numSegs, ren);
        
            fillQPolygon(ren, dPoints, tex, u, v);
        }
    }
}

Point2d DrawTools::calcUVPoint(PolyVec vecTop, LineSegment segTop, 
        PolyVec vecBot, LineSegment segBot, 
        PolyVec vecLeft, LineSegment segLeft, 
        PolyVec vecRight, LineSegment segRight, int u, int v, int numSegs, SDL_Renderer* ren
) {

    auto scldLeftSeg = LineSegment(vecLeft.getMultScalarVector((float)v / (float)numSegs), segLeft.getIPoint());
    auto scldRightSeg = LineSegment(vecRight.getMultScalarVector(-(float)v / (float)numSegs), segRight.getFPoint());

    //printf("Full: %f, Partial: %f\n", vecRight.getY(), scldRightSeg.getVec().getY());


    auto vVec = Vector2d<float>(scldLeftSeg.getFPoint().x - scldRightSeg.getFPoint().x, scldLeftSeg.getFPoint().y - scldRightSeg.getFPoint().y);
    auto vSeg = LineSegment(vVec, scldRightSeg.getFPoint());


    //vSeg.render(ren);

    auto scldTopSeg = LineSegment(vecTop.getMultScalarVector((float)u / numSegs), segTop.getIPoint());
    auto scldBotSeg = LineSegment(vecBot.getMultScalarVector(-(float)u / numSegs), segBot.getFPoint());

    auto uVec = Vector2d<float>(scldTopSeg.getFPoint().x - scldBotSeg.getFPoint().x, scldTopSeg.getFPoint().y - scldBotSeg.getFPoint().y);
    auto uSeg = LineSegment(uVec, scldBotSeg.getFPoint());

    //uSeg.render(ren);

    return LineSegment::calcIntersect(uSeg, vSeg).point;
}



void DrawTools::fillQPolygon(SDL_Renderer* ren, std::vector<Point2d>& points, SDL_Texture* tex, int u, int v) {
    if (points.size() != 4) {
        throw points.size();
    }

    for (auto it = points.begin(); it != points.end(); it++) {
        it->x = round(it->x);
        it->y = round(it->y);
    }

    // Generate Line Segments
    LineSegment seg1(Vector2d<float>(points[1].x - points[0].x, points[1].y - points[0].y), points[0]);
    LineSegment seg2(Vector2d<float>(points[2].x - points[1].x, points[2].y - points[1].y), points[1]);
    LineSegment seg3(Vector2d<float>(points[3].x - points[2].x, points[3].y - points[2].y), points[2]);
    LineSegment seg4(Vector2d<float>(points[0].x - points[3].x, points[0].y - points[3].y), points[3]);

    std::vector<LineSegment> segs{seg1, seg2, seg3, seg4};

    /*
    LineSegment horz(Vector2d<float>(1000, 0), Point2d{0, 175});
    IntersectPoint p1 = LineSegment::calcIntersect(horz, seg2);

    
    printf("Did intersect? : %d\n", p1.didIntersect);
    if(p1.didIntersect) {
        printf("Same Line? : %d\n", p1.sameLine);
        printf("On Line? : (%d)\n", p1.onLine);
        printf("At point: (%f, %f)\n", p1.point.x, p1.point.y);
    }
    */

    
    std::initializer_list<float> xVals{points[0].x, points[1].x, points[2].x, points[3].x};
    std::initializer_list<float> yVals{points[0].y, points[1].y, points[2].y, points[3].y};

    int xMin = int(std::min(xVals));
    int yMin = int(std::min(yVals));

    int xMax = int(std::max(xVals));
    int yMax = int(std::max(yVals));

    SDL_Texture* drawTex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, xMax - xMin, yMax - yMin);
    SDL_SetRenderTarget(ren, drawTex);

    LineSegment horz(Vector2d<float>(30000, 0), Point2d{-10000,0}); // THIS IS VERY UGLY FIX IT AT SOME POINT
    IntersectPoint testPoint = LineSegment::calcIntersect(horz, segs[3]);

    //printf("Seg3: %f\n", seg3.getIPoint().x);

    for (int y = 0; y < yMax - yMin; y++) {
        // Create the horizontal line that will check for intersects
        horz.setPoint(Point2d{float(xMin), float(y + yMin)});

        Point2d p1;
        Point2d p2;

        int numIntersections = 0;

        for (auto it = segs.begin(); it != segs.end(); it++) {
            IntersectPoint intPoint = LineSegment::calcHorzIntersect(*it, y + yMin);

            if (intPoint.didIntersect && intPoint.sameLine) {
                p1 = it->getIPoint();
                p2 = it->getFPoint();

                numIntersections = 2;
                break;
            }

            else if (intPoint.didIntersect && intPoint.onLine) {
                if (numIntersections == 0) {
                    numIntersections++;
                    p1 = intPoint.point;
                }
                else if (numIntersections == 1 && !(intPoint.point.x == p1.x && intPoint.point.y == p1.y)) {
                    numIntersections++;
                    p2 = intPoint.point;
                    break;
                }
            }
        }

        //printf("Num Intersections: %d, Y: %d\n", numIntersections, y);

        if (numIntersections == 2) {
            SDL_Rect copyRect{u, v, 1, 1};
            int xInit = std::min((int)(p1.x - xMin), (int)(p2.x - xMin));
            SDL_Rect dstRect{xInit, y, (int)round(abs(p2.x - p1.x)), 1};
            //printf("%d, %d\n", (int)(p1.x - xMin), (int)(p1.x - p2.x));
            SDL_RenderCopy(ren, tex, &copyRect, &dstRect);
            //SDL_RenderDrawLine(ren, p1.x - xMin, y, p2.x - xMin, y);
        }
    }

    SDL_SetRenderTarget(ren, NULL);

    SDL_SetTextureBlendMode(drawTex, SDL_BLENDMODE_BLEND);

    float xOffset = seg1.getIPoint().x - (seg1.getIPoint().x - xMin);
    float yOffset = seg1.getIPoint().y - (seg1.getIPoint().y - yMin);

    SDL_Rect drawRect{int(xOffset), int(yOffset), xMax - xMin, yMax - yMin};
    SDL_RenderCopy(ren, drawTex, NULL, &drawRect);

    SDL_DestroyTexture(drawTex);
}