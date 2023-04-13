#ifndef RUFONT_H
#define RUFONT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "prog_inst.h"

class RUFont {
public:
    RUFont(std::string fontFile, int ptSize, ProgInstance& inst);
    ~RUFont();

    void setPosition(int x, int y);

    // Changes the text for the RUFont, if colorBg is null, then the background is transparent (blended)
    void setText(std::string newText, SDL_Color colorFg, const SDL_Color* colorBg);
    void render();

    int getPxWidth();
    int getPxHeight();

    std::string getText();

private:
    void clean();

    TTF_Font* _font;

    int _size;

    SDL_Surface* _fSurf;
    SDL_Texture* _fTex;

    std::string _text;

    SDL_Rect _pos;

    ProgInstance& _inst;
};

#endif