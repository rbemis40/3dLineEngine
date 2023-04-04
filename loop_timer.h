#ifndef LOOP_TIMER_H
#define LOOP_TIMER_H

#include <SDL.h>

#include "prog_inst.h"
#include "rufont.h"

class LoopTimer {
public:
    LoopTimer(ProgInstance& prgInst);

    void updateBegin();
    void updateEnd();

    void renderBegin();
    void renderEnd();

    void renderStats(int x, int y);
private:
    RUFont _statFont;
    ProgInstance& _prgInst;

    Uint32 _startUpdateTime;
    Uint32 _startRenderTime;

    Uint32 _lastUpdateTime;
    Uint32 _lastRenderTime;

    SDL_Color _fgColor;
    SDL_Color _bgColor;
};

#endif