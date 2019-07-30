#include "texture.h"

STexture::STexture() : mTexture(NULL), targetRenderer(NULL), mWidth(0), mHeight(0) {
}

STexture::~STexture() {
  free();
}

void STexture::associateWindow(SWindow& targetWindow) {
  targetRenderer = targetWindow.getRenderer();
}

bool STexture::createFromSurface(SSurface& originalSurface) {
  if ( targetRenderer == NULL ) { return false; }
  mTexture = SDL_CreateTextureFromSurface(targetRenderer, originalSurface.getSurface());
  if ( mTexture == NULL ) { return false; }
  Uint32 format;
  int access;
  SDL_QueryTexture(mTexture, &format, &access, &mWidth, &mHeight);
  return true;
}

void STexture::free() {
  if(mTexture != NULL) {
	SDL_DestroyTexture(mTexture);
	mTexture = NULL;
  }
  targetRenderer = NULL;
  mWidth = 0;
  mHeight = 0;
}

bool STexture::render(int x, int y, SDL_Rect* clip, double scale, double angle, SDL_Point* center, SDL_RendererFlip flip) {
  if ( targetRenderer == NULL ) { return false; }
  //Set rendering space and render to screen
  SDL_Rect renderQuad;

  //Set clip rendering dimensions
  if( clip != NULL ) {
    renderQuad.x = x-(int)(((clip->w)*scale)/2);
    renderQuad.y = y-(int)(((clip->h)*scale)/2);
    renderQuad.w = (int)((clip->w)*scale);
	renderQuad.h = (int)((clip->h)*scale);
  }
  else {
    renderQuad.x = x-(int)((mWidth*scale)/2);
	renderQuad.y = y-(int)((mHeight*scale)/2);
	renderQuad.w = (int)(mWidth*scale);
	renderQuad.h = (int)(mHeight*scale);
  }

  SDL_RenderCopyEx(targetRenderer, mTexture, clip, &renderQuad, angle, center, flip ); 
  return true;
}

bool STexture::setBlendMode(SDL_BlendMode blendMode) {
  if ( SDL_SetTextureBlendMode(mTexture, blendMode) != 0 ) { return false; }
  return true;
}

bool STexture::setAlphaMod(Uint8 alpha) {
  if ( SDL_SetTextureAlphaMod(mTexture, alpha) != 0 ) { return false; }
  return true;
}

int STexture::getWidth() const { return mWidth; }

int STexture::getHeight() const { return mHeight; }
