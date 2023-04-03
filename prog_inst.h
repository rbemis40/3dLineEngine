#ifndef PROG_INST_H
#define PROG_INST_H

#include <string>
#include <SDL.h>

class ProgInstance {

public:
    ProgInstance(std::string title, int width, int height, bool reqVsync);
    ~ProgInstance();

    bool isValid();

    int getWinWidth();
    int getWinHeight();

    SDL_Renderer* getRen();

private:
    SDL_Window* _win;
    SDL_Renderer* _ren;

    bool _initSuccess;

    int _winWidth;
    int _winHeight;
};

#endif