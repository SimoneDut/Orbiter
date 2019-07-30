#include "font.h"

SFont::SFont() : mFont(NULL) {
}

SFont::~SFont() {
  free();
}

bool SFont::loadFromFont(const char* file, int ptsize) {
  free();
  mFont = TTF_OpenFont(file, ptsize);
  if(mFont == NULL) { return false; }
  return true;
}

void SFont::free() {
  TTF_CloseFont(mFont);
  mFont = NULL;
}

TTF_Font* SFont::getFont() { return mFont; }

