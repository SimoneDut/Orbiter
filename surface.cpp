#include <cmath>
#include "surface.h"

SSurface::SSurface() : mSurface(NULL), mLocked(false), mPitch(0), mWidth(0), mHeight(0) {
}

SSurface::~SSurface() {
  free();
}

bool SSurface::createFromFile(std::string path) {
  free();
  mSurface = IMG_Load(path.c_str());
  if(mSurface == NULL) { return false; }
  mLocked = false;
  mPitch = mSurface->pitch;
  mWidth = mSurface->w;
  mHeight = mSurface->h;
  return true;
}

bool SSurface::createFromText(std::string textureText, SFont& desiredFont, eFontRenderType renderType, Uint8 rf, Uint8 gf, Uint8 bf, Uint8 af,
                                                                                                      Uint8 rb, Uint8 gb, Uint8 bb, Uint8 ab) {
  free();
  SDL_Color textColor = { rf, gf, bf, af };
  SDL_Color backgroundColor = { rb, gb, bb, ab };
  switch (renderType) {
	case SOLID:
	mSurface = TTF_RenderText_Solid( desiredFont.getFont(), textureText.c_str(), textColor );
	break;
	case SHADED:
	mSurface = TTF_RenderText_Shaded( desiredFont.getFont(), textureText.c_str(), textColor, backgroundColor );
	break;
	case BLENDED:
	mSurface = TTF_RenderText_Blended( desiredFont.getFont(), textureText.c_str(), textColor );
	break;
	default:
	break;
  }
  if(mSurface == NULL) { return false; }
  mLocked = false;
  mPitch = mSurface->pitch;
  mWidth = mSurface->w;
  mHeight = mSurface->h;
  return true;
}

bool SSurface::createFromShapes(int width, int height, SDL_Color backgroundColor, std::list<std::pair<SShape, SDL_Color> > shapes) {

  free();
  if (!createBlank(width, height)) { return false; }
  if (!lockSurface()) { return false; }

  drawRectangle(0, 0, width-1, height-1, true, backgroundColor);

  for (std::list<std::pair<SShape, SDL_Color> >::iterator it = shapes.begin(); it != shapes.end(); ++it) {
    switch (it->first.type) {
	  case POINT:
	  drawPoint(it->first.i1, it->first.i2, it->second);
	  break;
	  case LINE:
	  drawLine(it->first.i1, it->first.i2, it->first.i3, it->first.i4, it->second);
	  break;
	  case RECTANGLE:
	  drawRectangle(it->first.i1, it->first.i2, it->first.i3, it->first.i4, it->first.solid, it->second);
	  break;
	  case TRIANGLE:
	  drawTriangle(it->first.i1, it->first.i2, it->first.i3, it->first.i4, it->first.solid, it->second);
	  break;
	  case CIRCLE:
	  drawCircle(it->first.i1, it->first.i2, it->first.i3, it->first.solid, it->second);
	  break;
	  default:
	  break;
	}
  }
  unlockSurface();
  return true;
}

bool SSurface::createBlank(int width, int height) {
  free();
  if ((width <= 0)||(height <= 0)) { return false; }
  mSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
  //SDL_SetSurfaceBlendMode(mSurface, SDL_BLENDMODE_NONE);
  if(mSurface == NULL) { return false; }
  mLocked = false;
  mPitch = mSurface->pitch;
  mWidth = mSurface->w;
  mHeight = mSurface->h;
  return true;
}

void SSurface::free() {
  if(mSurface != NULL) {
	SDL_FreeSurface(mSurface);
	mSurface = NULL;
  }
  mLocked = false;
  mPitch = 0;
  mWidth = 0;
  mHeight = 0;
}

void SSurface::drawPoint(int x, int y, SDL_Color color) {
  if ((x >= 0)&&(x < mWidth)&&(y >= 0)&&(y < mHeight)) {
	char* targetMemory = (char*)mSurface->pixels;
	Uint8* targetPixel8 = NULL;
	Uint8* targetPixel8temp = NULL;
	Uint16* targetPixel16 = NULL;
	Uint32* targetPixel32 = NULL;
	Uint32 targetColor = SDL_MapRGBA(mSurface->format, color.r, color.g, color.b, color.a);
	switch ((int)mSurface->format->BytesPerPixel) {
	  case 1:
	  targetPixel8 = &((Uint8*)targetMemory)[y * mPitch + x];
	  *targetPixel8 = (Uint8)targetColor;
	  break;
	  case 2:
	  targetPixel16 = &((Uint16*)targetMemory)[y * mPitch/2 + x];
	  *targetPixel16 = (Uint16)targetColor;		
	  break;
	  case 3:
	  targetPixel8 = &((Uint8*)targetMemory)[y * mPitch/3 + x];
	  targetPixel8temp = &targetPixel8[2];
	  *targetPixel8temp = (Uint8)targetColor;
	  targetColor = targetColor >> 8;
	  targetPixel8temp = &targetPixel8[1];
	  *targetPixel8temp = (Uint8)targetColor;
	  targetColor = targetColor >> 8;
	  targetPixel8temp = targetPixel8;
	  *targetPixel8temp = (Uint8)targetColor;
	  break;
	  case 4:
	  targetPixel32 = &((Uint32*)targetMemory)[y * mPitch/4 + x];
	  *targetPixel32 = targetColor;
	  break;
	  default:
	  break;
	}
  }	
}

void SSurface::drawLine(int x1, int y1, int x2, int y2, SDL_Color color) {
  if ((x1 == x2)&&(y1==y2)) { drawPoint(x1, y2, color); }
  else {
	int offsetX = x1;
	int offsetY = y1;
	x1 -= offsetX;
	y1 -= offsetY;
	x2 -= offsetX;
	y2 -= offsetY;
	int xFactor = 1;
	int yFactor = 1;
	if (x2 < 0) {
	  xFactor = -1;
	  x2 = -x2;
	}
	if (y2 < 0) {
	  yFactor = -1;
	  y2 = -y2;
	}
	bool swap = false;
	if (y2 > x2) {
	  swap = true;
	  int temp = x2;
	  x2 = y2;
	  y2 = temp;
	}	  
	int realX = 0;
	int realY = 0;
	for (int i = 0; i <= x2; i++) {
	  realX = i;
	  realY = (int)(((y2*i)/(double)x2)+.5);
	  if (swap) {
		int temp = realX;
		realX = realY;
		realY = temp;
	  }
	  drawPoint(xFactor*realX+offsetX, yFactor*realY+offsetY, color);
	}
  }
}

void SSurface::drawRectangle(int x, int y, int w, int h, bool solid, SDL_Color color) {
  int x1 = x;
  int y1 = y;
  int x2 = x + w;
  int y2 = y + h;
  if (x1 > x2) {
	int temp = x1;
	x1 = x2;
	x2 = temp;
  }
  if (y1 > y2) {
	int temp = y1;
	y1 = y2;
	y2 = temp;
  }
  if ((x1 == x2)||(y1 == y2)||(!solid)) {
	drawLine(x1, y1, x2, y1, color);
	drawLine(x1, y2, x2, y2, color);
	drawLine(x1, y1, x1, y2, color);
	drawLine(x2, y1, x2, y2, color);
  }
  else {
	for (int i = x1; i <= x2; i++) {
	  drawLine(i, y1, i, y2, color);
	}
  }
}

void SSurface::drawTriangle(int x, int y, int b, int h, bool solid, SDL_Color color) {
  int x1 = x;
  int y1 = y;
  int x2 = x + b;
  int y2 = y;
  if (x1 > x2) {
	int temp = x1;
	x1 = x2;
	x2 = temp;
	b = -b;
  }
  int x3 = x1 + b/2;
  int y3 = y1 + h;
  if (x1 == x2) { drawLine(x1, y1, x3, y3, color); }
  else if (y1 == y3) { drawLine(x1, y1, x2, y2, color); }
  else if (!solid) {
	drawLine(x1, y1, x2, y2, color);
	drawLine(x1, y1, x3, y3, color);
	if (b % 2 == 0) { drawLine(x3, y3, x2, y2, color); }
    else { drawLine(x3+1, y3, x2, y2, color); }	  
  }
  else {
	int yFactor = 1;
	if (y1 > y3) {
	  yFactor = -1;
	}
	int limitY = 0;
	for (int i = x1; i <= x3; i++) {
	  limitY = (int)(((yFactor*(y3-y1)*(i-x1))/(double)(x3-x1)) + y1 + .5);
	  drawLine(i, y1, i, yFactor*(limitY-y1)+y1, color);
	  drawLine(x2-(i-x1), y1, x2-(i-x1), yFactor*(limitY-y1)+y1, color);
	}
  }	
}

void SSurface::drawCircle(int x, int y, int r, bool solid, SDL_Color color) {
  if (r < 0) { r = -r; }
  for (int j = 0; 2*j*j <= r*r; j++) {
	int limitI = (int)(sqrt((double)(r*r - j*j)) + .5);
	for (int i = j; i <= limitI; i++) {
	  if ((solid)||(i == limitI)) {
		drawPoint(x + i, y + j, color);
		drawPoint(x + j, y + i, color);
		drawPoint(x - i, y + j, color);
		drawPoint(x - j, y + i, color);
		drawPoint(x + i, y - j, color);
		drawPoint(x + j, y - i, color);
		drawPoint(x - i, y - j, color);
		drawPoint(x - j, y - i, color);
	  }
	}
  }
}

bool SSurface::setBlendMode(SDL_BlendMode blendMode) {
  if ( SDL_SetSurfaceBlendMode(mSurface, blendMode) != 0 ) { return false; }
  return true;
}

bool SSurface::setAlphaMod(Uint8 alpha) {
  if ( SDL_SetSurfaceAlphaMod(mSurface, alpha) != 0 ) { return false; }
  return true;
}

bool SSurface::lockSurface() {
  if (SDL_MUSTLOCK(mSurface) == SDL_TRUE) {
    if (SDL_LockSurface(mSurface) != 0) {
	  return false;
	}
	mLocked = true;	
  }
  return true;
}

void SSurface::unlockSurface() {
  if (mLocked) {
    SDL_UnlockSurface(mSurface);
    mLocked = false;
  }
}

SDL_Surface* SSurface::getSurface() { return mSurface; }

int SSurface::getPitch() const { return mPitch; }

int SSurface::getWidth() const { return mWidth; }

int SSurface::getHeight() const { return mHeight; }

bool SSurface::isLocked() const { return mLocked; }
