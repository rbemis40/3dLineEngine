#include "loop_timer.h"

LoopTimer::LoopTimer(ProgInstance& prgInst) :
    _prgInst(prgInst),
    _statFont("fonts/Geneva.ttf", 12, prgInst),
    _lastUpdateTime(0),
    _startUpdateTime(0),
    _lastRenderTime(0),
    _startRenderTime(0),
    _fgColor{255, 255, 255, 255},
    _bgColor{0, 0, 0, 255}
{}

// Update methods

void LoopTimer::updateBegin() {
    _startUpdateTime = SDL_GetTicks();
}

void LoopTimer::updateEnd() {
    _lastUpdateTime = SDL_GetTicks() - _startUpdateTime;
}

// Render methods

void LoopTimer::renderBegin() {
    _startRenderTime = SDL_GetTicks();
}

void LoopTimer::renderEnd() {
    _lastRenderTime = SDL_GetTicks() - _startRenderTime;
}

void LoopTimer::renderStats(int x, int y) {

    const int FONT_SIZE = 12;
    int curFontY = y;

    _statFont.setPosition(x, curFontY);
    _statFont.setText("Update: " + std::to_string(_lastUpdateTime) + " ms", _fgColor, &_bgColor);

    _statFont.render();


    curFontY += FONT_SIZE;

    _statFont.setPosition(x, curFontY);
    _statFont.setText("Render: " + std::to_string(_lastRenderTime) + " ms", _fgColor, &_bgColor);

    _statFont.render();
}