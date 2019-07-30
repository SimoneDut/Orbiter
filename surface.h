#ifndef SURFACE_H
#define SURFACE_H

#include <cstdlib>
#include <string>
#include <list>
#include <SDL.h>
#include <SDL_image.h>
#include "font.h"

enum eShapeType
{
  POINT,
  LINE,
  RECTANGLE,
  TRIANGLE,
  CIRCLE,
  SHAPETYPE_TOTAL
};

enum eFontRenderType
{
  SOLID,
  SHADED,
  BLENDED,
  FONTRENDERTYPE_TOTAL
};


class SShape {
  public:
  SShape() : type(SHAPETYPE_TOTAL), solid(true), i1(0), i2(0), i3(0), i4(0) {}
  SShape(eShapeType desiredType, bool desiredSolid, int desiredI1, int desiredI2, int desiredI3, int desiredI4) : type(desiredType),
																												  solid(desiredSolid),
																												  i1(desiredI1), i2(desiredI2),
																												  i3(desiredI3), i4(desiredI4) {}
  eShapeType type;
  bool solid;
  int i1;
  int i2;
  int i3;
  int i4;
};


class SSurface {

public:

  SSurface();
  ~SSurface();

  bool createFromFile(std::string path); //Create the surface from a file
  bool createFromText(std::string textureText, SFont& desiredFont, eFontRenderType renderType, Uint8 rf, Uint8 gf, Uint8 bf, Uint8 af,
                                                                                              Uint8 rb, Uint8 gb, Uint8 bb, Uint8 ab); //Create the surface from a string
  bool createFromShapes(int width, int height, SDL_Color backgroundColor, std::list<std::pair<SShape, SDL_Color> > shapes); //Create the surface from an array of shapes
  bool createBlank(int width, int height); //Create a blank surface
  void free(); //Free the surface

  void drawPoint(int x, int y, SDL_Color color); //Draw a point
  void drawLine(int x1, int y1, int x2, int y2, SDL_Color color); //Draw a line
  void drawRectangle(int x, int y, int w, int h, bool solid, SDL_Color color); //Draw a rectangle
  void drawTriangle(int x, int y, int b, int h, bool solid, SDL_Color color); //Draw a triangle
  void drawCircle(int x, int y, int r, bool solid, SDL_Color color); //Draw a circle

  bool setBlendMode(SDL_BlendMode blendMode); //Set the blend mode of the surface
  bool setAlphaMod(Uint8 alpha); //Set the alpha mod of the surface
						   
  bool lockSurface(); //Lock the surface from editing
  void unlockSurface(); //Unlock the surface for editing
  
  SDL_Surface* getSurface(); //Get the surface
  int getPitch() const; //Get the pitch of the surface
  int getWidth() const; //Get the width of the surface
  int getHeight() const; //Get the height of the surface
  bool isLocked() const; //Check if the surface is locked
  
private:
  
  SDL_Surface* mSurface;
  bool mLocked;
  int mPitch;
  int mWidth;
  int mHeight;

};

#endif