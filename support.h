#ifndef SUPPORT_H
#define SUPPORT_H

#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "window.h"
#include "surface.h"
#include "texture.h"
#include "font.h"
#include "sound.h"
#include "orbiter.h"
#include "constants.h"

bool initSDL(); //Initialize the SDL systems
void closeSDL(); //Close the SDL systems

bool initObjects(SWindow* myWindowP, STexture* backgroundTextureP, STexture* orbiterTextureP, STexture* rocketTextureP, STexture* flameTextureP, STexture* logoTextureP,
                 STexture* myTargetCircleTextureP, STexture* myTargetLineTextureP, STexture* engineTextureP, STexture** numbersTextureP,
				 SOrbiter* myOrbiterP, SSound* mySoundP); //Initialize all the objects

void freeObjects(SWindow* myWindowP, STexture* backgroundTextureP, STexture* orbiterTextureP, STexture* rocketTextureP, STexture* flameTextureP, STexture* logoTextureP,
                 STexture* myTargetCircleTextureP, STexture* myTargetLineTextureP,  STexture* engineTextureP, STexture** numbersTextureP,
				 SOrbiter* myOrbiterP, SSound* mySoundP); //Frees all the objects

void renderData(SOrbiter& myOrbiter, STexture& myTargetCircleTexture, STexture& myTargetLineTexture, STexture& engineTexture, STexture*& numbersTexture); //Draw the data

SSurface drawTargetCircle(Uint8 r, Uint8 g, Uint8 b, Uint8 a, int radiusCircle); //Draw the target cirle
SSurface drawDirectionLine(Uint8 r, Uint8 g, Uint8 b, Uint8 a, int lengthLine, int thicknessLine); //Draw the direction line
SSurface drawOrbiter(Uint8 ri, Uint8 gi, Uint8 bi, Uint8 ai, Uint8 rb, Uint8 gb, Uint8 bb, Uint8 ab); //Draw the orbiter

// Add function to print the data, with a subfunction to convert number to string

#endif