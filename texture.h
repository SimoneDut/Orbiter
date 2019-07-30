#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdlib>
#include <SDL.h>
#include "window.h"
#include "surface.h"

class STexture {

public:

  STexture();
  ~STexture();

  void associateWindow(SWindow& targetWindow); //Associate a window to the texture for rendering purposes
  bool createFromSurface(SSurface& originalSurface); //Create the texture from a surface
  void free(); //Free the texture

  bool render(int x, int y, SDL_Rect* clip, double scale, double angle, SDL_Point* center, SDL_RendererFlip flip); //Render the texture

  bool setBlendMode(SDL_BlendMode blendMode); //Set the blend mode of the texture
  bool setAlphaMod(Uint8 alpha); //Set the alpha mod of the texture

  int getWidth() const; //Get the width of the texture
  int getHeight() const; //Get the height of the texture

private:
  
  SDL_Texture* mTexture;
  SDL_Renderer* targetRenderer;

  int mWidth;
  int mHeight;

};

#endif