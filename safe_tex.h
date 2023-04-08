#ifndef SAFE_TEX_H
#define SAFE_TEX_H

#include <SDL.h>
#include <string>

#include "prog_inst.h"

class SafeTexture {
public:
    SafeTexture(std::string texFile, ProgInstance& inst);
    ~SafeTexture();

    void renderAt(int x, int y, int w, int h);

    SDL_Texture* getRawTex();
private:
    bool _isValid;

    ProgInstance& _inst;
    SDL_Texture* _tex;

    SDL_Rect _dstRect;

    static int _numTextures;
};

#endif