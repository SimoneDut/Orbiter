#include <cstring>
#include "assert.h"
#include "window.h"

//Set window icon

SWindow::SWindow() : mWindow(NULL), mRenderer(NULL), mWindowID(-1), mWindowDisplayID(-1), mWidth(0), mHeight(0),
                     mX(0), mY(0) {
}

SWindow::~SWindow() {
	free();
}

bool SWindow::init(const char* title, int x, int y, int w, int h, Uint32 windowFlags,
            int renderIndex, Uint32 rendererFlags, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendMode) {
  if ((mWindow = SDL_CreateWindow(title, x, y, w, h, windowFlags)) == NULL ) {
	return false;
  }
  if ((mWindowID = SDL_GetWindowID(mWindow)) == 0) {
	free();
	return false;
  }
  if ((mWindowDisplayID = SDL_GetWindowDisplayIndex(mWindow)) < 0)  {
	free();
	return false;
  }
  SDL_GetWindowSize(mWindow, &mWidth, &mHeight);
  if ((w != mWidth)||(h != mHeight)) {
	free();
	return false;
  }
  SDL_GetWindowPosition(mWindow, &mX, &mY);
  if ((mRenderer = SDL_CreateRenderer(mWindow, renderIndex, rendererFlags)) == NULL ) {
	free();
	return false;
  }
  if (SDL_SetRenderDrawColor(mRenderer, r, g, b, a) != 0) {
	free();
	return false;
  }
  if (SDL_SetRenderDrawBlendMode(mRenderer, blendMode) != 0) {
	free();
	return false;
  }
  return true;
}

void SWindow::free() {

  if(mRenderer != NULL) {
	SDL_DestroyRenderer(mRenderer);
	mRenderer = NULL;
  }

  if(mWindow != NULL) {
	SDL_DestroyWindow(mWindow);
	mWindow = NULL;
  }
  mWindowID = -1;
  mWindowDisplayID = -1;
  mWidth = 0;
  mHeight = 0;
  mX = 0;
  mY = 0;
}

bool SWindow::render() {
  // Clear screen
  if (SDL_RenderClear(mRenderer) < 0) {
	return false;
  }
  return true;
}

void SWindow::present() {
  SDL_RenderPresent(mRenderer);
}

bool SWindow::updateInternals() {
  SDL_GetWindowSize(mWindow, &mWidth, &mHeight);
  SDL_GetWindowPosition(mWindow, &mX, &mY);
  if ((mWindowID = SDL_GetWindowID(mWindow)) == 0) {
	return false;
  }
  if ((mWindowDisplayID = SDL_GetWindowDisplayIndex(mWindow)) < 0)  {
	return false;
  }
  return true;
}


void SWindow::setIcon(SSurface& iconSurface) { SDL_SetWindowIcon(mWindow, iconSurface.getSurface()); }

bool SWindow::setTitle(const char* title) {
  if (mWindow == NULL) { return false; }
  SDL_SetWindowTitle(mWindow, title);
  const char* currentTitle = SDL_GetWindowTitle(mWindow);
  if (strcmp(title, currentTitle) != 0) { return false; }
  return true;  
}

bool SWindow::setPosition(int x, int y) {
  if (mWindow == NULL) { return false; }
  SDL_SetWindowPosition(mWindow, x, y);
  SDL_GetWindowPosition(mWindow, &mX, &mY);
  if (x != mX) { return false; }
  if (y != mY) { return false; }
  updateInternals();
  return true;
}

bool SWindow::setSize(int w, int h) {
  if (mWindow == NULL) { return false; }
  SDL_SetWindowSize(mWindow, w, h);
  SDL_GetWindowSize(mWindow, &mWidth, &mHeight);
  if (w != mWidth) { return false; }
  if (h != mHeight) { return false; }
  updateInternals();
  return true;
}

bool SWindow::setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  if (mRenderer == NULL) { return false; }
  if (SDL_SetRenderDrawColor(mRenderer, r, g, b, a) != 0) { return false; }
  return true;
}

bool SWindow::setBlendMode(SDL_BlendMode blendMode) {
  if (mRenderer == NULL) { return false; }
  if (SDL_SetRenderDrawBlendMode(mRenderer, blendMode) != 0) { return false; }
  return true;
}

SDL_Renderer* SWindow::getRenderer() const {
  return mRenderer;
}

Uint32 SWindow::getWindowID() const {
  return mWindowID;
}

int SWindow::getWindowDisplayID() const {
  return mWindowDisplayID;
}

int SWindow::getWidth() const {
  return mWidth;
}

int SWindow::getHeight() const {
  return mHeight;
}

int SWindow::getX() const {
  return mX;
}

int SWindow::getY() const {
  return mY;
}
