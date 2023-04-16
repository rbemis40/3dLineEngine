#include "safe_tex.h"

#include <SDL_image.h>
#include <cstdio>

int SafeTexture::_numTextures = 0;

SafeTexture::SafeTexture(std::string texFile, ProgInstance& inst) :
    _inst(inst),
    _dstRect{}
{
    _numRefs = new int(1);


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

    //printf("Num Textures: %d\n", _numTextures);
    _isValid = true;
}

SafeTexture::SafeTexture(SafeTexture&& tex) :
    _isValid(tex._isValid),
    _inst(tex._inst),
    _tex(tex._tex),
    _dstRect(tex._dstRect)
{
    (*tex._numRefs)++;
    _numRefs = tex._numRefs;
    _numTextures++;
}

SafeTexture::~SafeTexture() {
    (*_numRefs)--;
    if ((*_numRefs) == 0) {
        SDL_DestroyTexture(_tex);
        delete _numRefs;
        //printf("Texture destroyed refs: %d\n", *_numRefs);
    }

    _numTextures--;
    if (_numTextures == 0) {
        IMG_Quit();
        //printf("IMG Quit\n");
    }

    //printf("Deconstructor called: Num refs: %d\n", *_numRefs);
}

void SafeTexture::renderAt(int x, int y, int w, int h) {
    if (!_isValid) { return; }

    _dstRect = {x, y, w, h};
    SDL_RenderCopy(_inst.getRen(), _tex, NULL, &_dstRect);
}

void SafeTexture::renderAt(int x, int y, int w, int h, float clipW, float clipH) {
    if (!_isValid) { return; }

    _dstRect = {x, y, w, h};
    SDL_Rect clipRect = {0, 0, int(getWidth() * clipW), int(getHeight() * clipH)};

    SDL_RenderCopy(_inst.getRen(), _tex, &clipRect, &_dstRect);
}

// TODO: This should all be done via a surface then converted to a texture so it isn't performed every frame
void SafeTexture::renderCont(int x, int y, int w, int h, float texScale) {
    if (!_isValid) { return; }

    SDL_Rect dstRect = {x, y, int(getWidth() * texScale), int(getHeight() * texScale)};

    auto ren = _inst.getRen();

    // Keep track of how much of the target width and height has actually been filled
    int wFilled = 0;
    int hFilled = 0;

    const int numXCopies = w / dstRect.w;
    const int numYCopies = h / dstRect.h;

    for (int curY = 0; curY < numYCopies; curY++) {
        for (int curX = 0; curX < numXCopies; curX++) {
            SDL_RenderCopy(ren, _tex, NULL, &dstRect);

            dstRect.x += dstRect.w;
            wFilled += dstRect.w;
        }

        // Fill in whatever is left on the edge (not a full texture amount)
        if (wFilled < w) {
            SDL_Rect partialSrc = {0, 0, int((w - wFilled) / texScale), getHeight()};
            SDL_Rect partialDst = {dstRect.x, dstRect.y, w - wFilled, dstRect.h};
            SDL_RenderCopy(ren, _tex, &partialSrc, &partialDst);
        }

        dstRect.y += dstRect.h;
        hFilled += dstRect.h;

        dstRect.x = x;
        wFilled = 0;
    }

    // Now fill the last partial bottom row if need be
    if (hFilled < h) {
        SDL_Rect partialYSrc = {0, 0, getWidth(), int((h - hFilled) / texScale)};
        SDL_Rect partialYDst = {x, dstRect.y, dstRect.w, h - hFilled};

        // Fill all the full x's possible
        for (int curX = 0; curX < numXCopies; curX++) {
            SDL_RenderCopy(ren, _tex, &partialYSrc, &partialYDst);

            partialYDst.x += partialYDst.w;
            wFilled += partialYDst.w;
        }

        // Fill in any remaining x that couldn't be fully filled
        if (wFilled < w) {
            SDL_Rect partialXSrc = {0, 0, int((w - wFilled) / texScale), partialYSrc.h};
            SDL_Rect partialXDst = {partialYDst.x, partialYDst.y, w - wFilled, partialYDst.h};

            SDL_RenderCopy(ren, _tex, &partialXSrc, &partialXDst);
        }
    }
}

SDL_Texture* SafeTexture::getRawTex() {
    return _tex;
}

int SafeTexture::getWidth() const {
    int w;
    SDL_QueryTexture(_tex, NULL, NULL, &w, NULL);
    return w;
}

int SafeTexture::getHeight() const {
    int h;
    SDL_QueryTexture(_tex, NULL, NULL, NULL, &h);
    return h;
}