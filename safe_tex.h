#ifndef SAFE_TEX_H
#define SAFE_TEX_H

#include <SDL.h>
#include <string>

#include "prog_inst.h"

class SafeTexture {
public:
    SafeTexture(std::string texFile, ProgInstance& inst);
    SafeTexture(SafeTexture&& tex);
    ~SafeTexture();

    void renderAt(int x, int y, int w, int h);

    SDL_Texture* getRawTex();
private:
    bool _isValid;

    ProgInstance& _inst;
    SDL_Texture* _tex;

    SDL_Rect _dstRect;

    int _numRefs; //Need to keep track of the number of references to make sure the SDL_Texture isn't destoryed earlier

    static int _numTextures;
};

#endif