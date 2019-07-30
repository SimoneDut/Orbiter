#ifndef WINDOW_H
#define WINDOW_H

#include <cstdlib>
#include <SDL.h>
#include "surface.h"

class SWindow {

public:

  SWindow();
  ~SWindow();

  bool init(const char* title, int x, int y, int w, int h, Uint32 windowFlags,
            int renderIndex, Uint32 rendererFlags, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendMode); //Initialize the window
  void free(); //Free the window
  
  bool render(); //Clear the background (clear the current rendering target with the drawing color)
  void present(); //Present the rendered
  bool updateInternals(); //Update the internal variables of the window

  void setIcon(SSurface& iconSurface); //Set the icon of the window
  bool setTitle(const char* title); //Set the title of the window
  bool setPosition(int x, int y); //Set the position of the window
  bool setSize(int w, int h); //Set the size of the window
  bool setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a); //Set the background color of the window
  bool setBlendMode(SDL_BlendMode blendMode); //Set the blend mode of the renderer

  SDL_Renderer* getRenderer() const; //Get the rendered associated to the window
  Uint32 getWindowID() const; //Get the window ID of the window
  int getWindowDisplayID() const; //Get the window display ID of the window
  int getWidth() const; //Get the width of the window
  int getHeight() const; //Get the height of the window
  int getX() const; //Get the x position of the window
  int getY() const; //Get the y position of the window

private:
  
  SDL_Window* mWindow;
  SDL_Renderer* mRenderer;
  Uint32 mWindowID;
  int mWindowDisplayID;
  int mWidth;
  int mHeight;
  int mX;
  int mY;

};

#endif