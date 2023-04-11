#include "safe_tex.h"

#include <SDL_image.h>
#include <cstdio>

int SafeTexture::_numTextures = 0;

SafeTexture::SafeTexture(std::string texFile, ProgInstance& inst) :
    _inst(inst),
    _dstRect{},
    _numRefs(0)
{
    // Check for image init

    const int INIT_FLAGS = IMG_INIT_JPG | IMG_INIT_PNG;

    int success = IMG_Init(INIT_FLAGS);
    if (success != INIT_FLAGS) {
        printf("Failed to initialize SDL Image: %s\n", IMG_GetError());
        _isValid = false;
        return;
    }

    _numTextures++;

    _tex = IMG_LoadTexture(inst.getRen(), texFile.c_str());
    if (_tex == NULL) {
        printf("Failed to load img: %s\nSDL: %s\n", texFile.c_str(), IMG_GetError());
        _isValid = false;
        return;
    }

    printf("Num Textures: %d\n", _numTextures);
    _isValid = true;
}

SafeTexture::SafeTexture(SafeTexture&& tex) :
    _isValid(tex._isValid),
    _inst(tex._inst),
    _tex(tex._tex),
    _dstRect(tex._dstRect)
{
    tex._numRefs++;
    _numRefs = tex._numRefs;
    _numTextures++;
}

SafeTexture::~SafeTexture() {
    if (_numRefs == 0) {
        SDL_DestroyTexture(_tex);
    }

    _numTextures--;
    if (_numTextures == 0) {
        IMG_Quit();
    }

    _numRefs--;
}

void SafeTexture::renderAt(int x, int y, int w, int h) {
    if (!_isValid) { return; }

    _dstRect = {x, y, w, h};

    SDL_RenderCopy(_inst.getRen(), _tex, NULL, &_dstRect);
}

SDL_Texture* SafeTexture::getRawTex() {
    return _tex;
}