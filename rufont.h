#ifndef RUFONT_H
#define RUFONT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class RUFont {
public:
    RUFont(std::string fontFile, int ptSize);
    ~RUFont();

    void setPosition(int x, int y);
    void setText(std::string newText, SDL_Color color, SDL_Renderer* ren);
    void render(SDL_Renderer* ren);

    std::string getText();

private:
    void clean();

    TTF_Font* _font;

    int _size;

    SDL_Surface* _fSurf;
    SDL_Texture* _fTex;

    std::string _text;

    SDL_Rect _pos;
};

#endif