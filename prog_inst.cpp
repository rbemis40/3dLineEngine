#include "prog_inst.h"

ProgInstance::ProgInstance(std::string title, int width, int height, bool reqVsync) : 
    _win(nullptr),
    _ren(nullptr),
    _winWidth(width),
    _winHeight(height)
{
    int err = SDL_Init(SDL_INIT_EVERYTHING);
    if (err != 0) {
        _initSuccess = false;
        return;
    }

    _win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (_win == NULL) {
        _initSuccess = false;
        return;
    }
    _ren = SDL_CreateRenderer(_win, -1, SDL_RENDERER_ACCELERATED | (reqVsync ? SDL_RENDERER_PRESENTVSYNC : 0));
    if (_ren == NULL) {
        _initSuccess = false;
        return;
    }

    _initSuccess = true;
}

ProgInstance::~ProgInstance() {
    if (_ren != nullptr) {
        SDL_DestroyRenderer(_ren);
    }

    if (_win != nullptr) {
        SDL_DestroyWindow(_win);
    }

    SDL_Quit();
}

bool ProgInstance::isValid() {
    return _initSuccess;
}

SDL_Renderer* ProgInstance::getRen() {
    return _ren;
}

int ProgInstance::getWinWidth() {
    return _winWidth;
}

int ProgInstance::getWinHeight() {
    return _winHeight;
}