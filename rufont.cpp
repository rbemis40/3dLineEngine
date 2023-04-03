#include "rufont.h"

RUFont::RUFont(std::string fontFile, int ptSize, ProgInstance& inst) :
    _fSurf(nullptr),
    _fTex(nullptr),
    _font(nullptr),
    _size(ptSize),
    _inst(inst)
{
    int ttfSuccess = TTF_Init();
    if (ttfSuccess != 0) {
        printf("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        return;
    }

    // Adjust point size for dpi
    int adjPtSize = ptSize * _inst.getDPIScale();

    _font = TTF_OpenFont(fontFile.c_str(), adjPtSize);
    if (_font == NULL) {
        printf("Failed to open font file: %s\n", TTF_GetError());
        return;
    }

    setPosition(0, 0);
}

RUFont::~RUFont() {
    clean();
    TTF_CloseFont(_font);
}

void RUFont::setPosition(int x, int y) {
    // Adjust x and y for dpi

    float dpiScale = _inst.getDPIScale();

    _pos.x = (int)(x * dpiScale);
    _pos.y = (int)(y * dpiScale);
}

void RUFont::setText(std::string newText, SDL_Color color) {
    _text = newText;
    clean();

    SDL_Renderer* ren = _inst.getRen();

    if (ren == NULL) {
        printf("Cannot set text with invalid renderer!\n");
        return;
    }

    if (_font == NULL) {
        printf("Cannot set text with invalid font!\n");
        return;
    }

    _fSurf = TTF_RenderUTF8_Blended(_font, _text.c_str(), color);
    if (_fSurf == NULL) {
        printf("Failed to create font surface with text: %s\nSDL Error: %s\n", _text.c_str(), TTF_GetError());
        return;
    }

    _fTex = SDL_CreateTextureFromSurface(ren, _fSurf);
    if (_fTex == NULL) {
        printf("Failed to create font texture with text: %s\nSDL Error: %s\n", _text.c_str(), TTF_GetError());
        return;
    }

    _pos.w = _fSurf->w;
    _pos.h = _fSurf->h;

}

std::string RUFont::getText() {
    return _text;
}

void RUFont::render() {
    SDL_Renderer* ren = _inst.getRen();
    if (_fTex == nullptr || ren == nullptr) {
        return;
    }

    SDL_RenderCopy(ren, _fTex, NULL, &_pos);
}

void RUFont::clean() {
    SDL_DestroyTexture(_fTex);
    SDL_FreeSurface(_fSurf);

    _fTex = nullptr;
    _fSurf = nullptr;
}