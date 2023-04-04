#include <cstdio>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

#include "prog_inst.h"
#include "vector2d.h"
#include "camera.h"
#include "points.h"
#include "plane.h"
#include "lineseg.h"
#include "drawtools.h"
#include "cube.h"
#include "floorplane.h"
#include "cylinder.h"
#include "pfblock_tl.h"
#include "rufont.h"
#include "loop_timer.h"

// Forward declarations
struct RectDirs {
    bool right = false;
    bool left = false;
    bool up = false;
    bool down = false;
    bool forward = false;
    bool backward = false;
};

float msToFps(Uint32 ms, int numFrames);
void testRender(SDL_Renderer* ren, SDL_Texture* tex);
std::string readFile(std::string fileName);
void handleKeyPressed(const SDL_Event& event, RectDirs& dirs);


int main(int argc, char* argv[]) {
    ProgInstance mainInst("Hello World!", 1280, 720, true);

    if (!mainInst.isValid()) {
        printf("Program instance creation failed: SDL Error: %s\n", SDL_GetError());
        return -1;
    }

    std::string test = readFile("pointfiles/test.pf");

    bool hasQuit = false;

    SDL_Renderer *progRen = mainInst.getRen();

    RUFont fpsFont("fonts/Geneva.ttf", 12, mainInst);

    LoopTimer loopTimer(mainInst);

    Camera mainCam((float)M_PI/2, mainInst);

    FloorPlane fPlane(mainCam);

    Cube blLeg(Point3d{-0.5, 0, 0.5}, 0.25, 1, 0.25, mainCam);
    Cube brLeg(Point3d{0.5, 0, 0.5}, 0.25, 1, 0.25, mainCam);
    Cube flLeg(Point3d{-0.5, 0, -0.5}, 0.25, 1, 0.25, mainCam);
    Cube frLeg(Point3d{0.5, 0, -0.5}, 0.25, 1, 0.25, mainCam);
    Cube seat(Point3d{-0.75, -0.1, -0.75}, 1.75, 0.1, 1.75, mainCam);

    Cube meterCube(Point3d{3, -5, 0}, 5, 5, 5, mainCam);

    PFBlockTopLevel tlFile(test);
    tlFile.parseBlock();

    if (!tlFile.getValidity()) {
        printf("Invalid PFFIle!\n");
        return -1;
    }

    std::vector<Plane> tlPFPlanes = tlFile.getPlanes(mainCam);

    RectDirs moveDirs;

    Cylinder testCylinder(Point3d{-10, 0, 5}, 2, 3, mainCam);

    Uint32 curTime;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 elapsedTime;

    Uint32 accTime;
    int numFrames = 0;

    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_Color winBgColor{69, 105, 72, 255};

    SDL_Event event;
    while (!hasQuit) {

        curTime = SDL_GetTicks();
        elapsedTime = curTime - lastTime;
        lastTime = curTime;

        accTime += elapsedTime;
        numFrames++;

        SDL_Color bgColor{0, 0, 0, 255};
        fpsFont.setText(std::to_string(msToFps(elapsedTime, 1)), SDL_Color{255, 255, 255, 255}, &bgColor);

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) hasQuit = true;
        else if(event.type == SDL_MOUSEMOTION) {
            const float CAM_SPEED_X = event.motion.xrel / 100.0;
            const float CAM_SPEED_Y = event.motion.yrel / 100.0;

            mainCam.changeXRot(CAM_SPEED_X);
            mainCam.changeYRot(-CAM_SPEED_Y);
        }

        handleKeyPressed(event, moveDirs);

        // ------- UPDATES --------

        loopTimer.updateBegin();

        mainCam.update(elapsedTime);
        float camSpeed = mainCam.getSpeed();

        if (moveDirs.right) mainCam.moveAdjX(camSpeed);
        if (moveDirs.left) mainCam.moveAdjX(-camSpeed);
        if (moveDirs.up) mainCam.changeY(-camSpeed);
        if (moveDirs.down) mainCam.changeY(camSpeed);
        if (moveDirs.forward) mainCam.moveAdjZ(camSpeed);
        if (moveDirs.backward) mainCam.moveAdjZ(-camSpeed);

        // Update points here
        blLeg.update(elapsedTime);
        brLeg.update(elapsedTime);
        flLeg.update(elapsedTime);
        frLeg.update(elapsedTime);
        seat.update(elapsedTime);

        meterCube.update(elapsedTime);
        fPlane.update(elapsedTime);

        testCylinder.update(elapsedTime);

        for (auto& pfPlane : tlPFPlanes) {
            pfPlane.update(elapsedTime);
        }

        loopTimer.updateEnd();

        // -------- RENDER -------

        loopTimer.renderBegin();

        mainInst.beginRender(winBgColor);

        // Draw here
        blLeg.render();
        brLeg.render();
        flLeg.render();
        frLeg.render();
        seat.render();
        meterCube.render();
        fPlane.render();

        testCylinder.render();

        for (auto& pfPlane : tlPFPlanes) {
            pfPlane.render();
        }

        fpsFont.render();
        mainCam.renderDebug(0, 24);

        loopTimer.renderStats(0, 108);

        mainInst.finishRender();

        loopTimer.renderEnd();
    }

    return 0;
}

// Definitions
float msToFps(Uint32 ms, int numFrames) {
    return 1000.0f * numFrames / ms;
}

void testRender(SDL_Renderer* ren, SDL_Texture* tex) {
    SDL_SetRenderTarget(ren, tex);

    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    for (int y = 0; y < 720; y++) {
        SDL_RenderDrawLine(ren, 0, y, 1280, y);
    }

    SDL_SetRenderTarget(ren, NULL);
    SDL_RenderCopy(ren, tex, NULL, NULL);

    SDL_DestroyTexture(tex);
}

std::string readFile(std::string fileName) {
    std::ifstream inFile;
    std::stringbuf fileBuf;
    inFile.open(fileName);

    inFile.get(fileBuf, '\0');

    inFile.close();

    return fileBuf.str();
}

void handleKeyPressed(const SDL_Event& event, RectDirs& dirs) {
    switch(event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_d:
                    dirs.right = true;
                    break;
                case SDLK_a:
                    dirs.left = true;
                    break;
                case SDLK_w:
                    dirs.forward = true;
                    break;
                case SDLK_s:
                    dirs.backward = true;
                    break;
                case SDLK_SPACE:
                    dirs.up = true;
                    break;
                case SDLK_LSHIFT:
                    dirs.down = true;
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym) {
                case SDLK_d:
                    dirs.right = false;
                    break;
                case SDLK_a:
                    dirs.left = false;
                    break;
                case SDLK_w:
                    dirs.forward = false;
                    break;
                case SDLK_s:
                    dirs.backward = false;
                    break;
                case SDLK_SPACE:
                    dirs.up = false;
                    break;
                case SDLK_LSHIFT:
                    dirs.down = false;
                    break;
            }
        break;
    }
}