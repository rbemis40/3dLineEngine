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

    _win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    if (_win == NULL) {
        _initSuccess = false;
        return;
    }
    _ren = SDL_CreateRenderer(_win, -1, SDL_RENDERER_ACCELERATED | (reqVsync ? SDL_RENDERER_PRESENTVSYNC : 0));
    if (_ren == NULL) {
        _initSuccess = false;
        return;
    }

    // Update the dpi scale for supporting high dpi screens
    int winWidthPts, winHeightPts;
    SDL_GetWindowSize(_win, &winWidthPts, &winHeightPts);

    int renWidthPx, renHeightPx;
    SDL_GetRendererOutputSize(_ren, &renWidthPx, &renHeightPx);

    _dpiScale = (float)(renWidthPx) / winWidthPts;

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

void ProgInstance::beginRender(SDL_Color bgColor) {
    SDL_SetRenderDrawColor(_ren, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(_ren);
}

void ProgInstance::finishRender() {
    SDL_RenderPresent(_ren);
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

float ProgInstance::getDPIScale() {
    return _dpiScale;
}

int ProgInstance::getRenWidth() {
    return (int)(_winWidth * _dpiScale);
}

int ProgInstance::getRenHeight() {
    return (int)(_winHeight * _dpiScale);
}