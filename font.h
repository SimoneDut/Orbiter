#ifndef FONT_H
#define FONT_H

#include <cstdlib>
#include <SDL.h>
#include <SDL_ttf.h>

class SFont {

public:

  SFont();
  ~SFont();

  bool loadFromFont(const char* file, int ptsize); //Load the font from a file at a given point size
  void free(); //Free the font

  TTF_Font* getFont();
  
private:
  
  TTF_Font* mFont;

};

#endif