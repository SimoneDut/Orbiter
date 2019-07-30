#include <cstdlib>
#include <iostream>
#include "window.h"
#include "surface.h"
#include "texture.h"
#include "font.h"
#include "sound.h"
#include "orbiter.h"
#include "support.h"
#include "constants.h"

int main(int argc, char* argv[]) {
  
  system("cls");

  initSDL();

  SWindow myWindow;
  STexture backgroundTexture;
  STexture orbiterTexture;
  STexture rocketTexture;
  STexture flameTexture;
  STexture logoTexture;
  STexture myTargetCircleTexture;
  STexture myTargetLineTexture;
  STexture engineTexture;
  STexture* numbersTexture = new STexture[10];
  SOrbiter myOrbiter;
  SSound mySound;

  if (!initObjects(&myWindow, &backgroundTexture, &orbiterTexture, &rocketTexture, &flameTexture, &logoTexture,
                  &myTargetCircleTexture, &myTargetLineTexture, &engineTexture, &numbersTexture, &myOrbiter, &mySound)) {
    std::cout << "Failed to initialize objects\n";
	freeObjects(&myWindow, &backgroundTexture, &orbiterTexture, &rocketTexture, &flameTexture, &logoTexture,
                &myTargetCircleTexture, &myTargetLineTexture, &engineTexture, &numbersTexture, &myOrbiter, &mySound);
	return EXIT_FAILURE;
  }

  SDL_Event e;
  Uint32 currentt;
  bool requestActivateEngine[4] = { false, false, false, false };
  bool requestBasicMovement[6] = { false, false, false, false, false, false };
  bool keyboardfocus = true;
  bool mousefocus = false;
  bool sound = true;
  bool data = false;
  bool quit = false;
    
  while (!quit) {

	while (SDL_PollEvent(&e) != 0) {
	  if (e.type == SDL_QUIT) { quit = true; }
	  else if (e.type == SDL_WINDOWEVENT) {
		switch (e.window.event) {
		  case SDL_WINDOWEVENT_ENTER:
          mousefocus = true;
		  break;
          case SDL_WINDOWEVENT_LEAVE:
          mousefocus = false;
          break;
          case SDL_WINDOWEVENT_FOCUS_GAINED:
          keyboardfocus = true;
          break;
          case SDL_WINDOWEVENT_FOCUS_LOST:
          keyboardfocus = false;
          break;          
		  default:
		  break;
		}
	  }				
	  else if (e.type == SDL_KEYDOWN) {
	    switch (e.key.keysym.sym) {
		  case SDLK_z:
		    myOrbiter.updateMode(MANUAL);
		  break;
		  case SDLK_x:
		    myOrbiter.updateMode(GOTOREST);
		  break;
		  case SDLK_c:
		    myOrbiter.updateMode(ORBITMOUSE);
		  break;
		  case SDLK_s:
		    sound = !sound;
			if (sound) { mySound.play(); }
			else { mySound.stop(); }
		  break;
		  case SDLK_d:
		    data = !data;
		  break;
		  default:
		  break;
		}
	  }
	}
	
	int newMouseX, newMouseY;
	SDL_GetMouseState(&newMouseX, &newMouseY);
	myOrbiter.updateMousePosition(newMouseX, newMouseY, keyboardfocus&&mousefocus);
	
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    requestActivateEngine[0] = currentKeyStates[SDL_SCANCODE_1];
    requestActivateEngine[1] = currentKeyStates[SDL_SCANCODE_2];
    requestActivateEngine[2] = currentKeyStates[SDL_SCANCODE_3];
    requestActivateEngine[3] = currentKeyStates[SDL_SCANCODE_4];
	requestBasicMovement[0] = currentKeyStates[SDL_SCANCODE_UP];
	requestBasicMovement[1] = currentKeyStates[SDL_SCANCODE_DOWN];
	requestBasicMovement[2] = currentKeyStates[SDL_SCANCODE_LEFT];
	requestBasicMovement[3] = currentKeyStates[SDL_SCANCODE_RIGHT];
	requestBasicMovement[4] = currentKeyStates[SDL_SCANCODE_PERIOD];
	requestBasicMovement[5] = currentKeyStates[SDL_SCANCODE_COMMA];
	myOrbiter.updateManualControls(requestActivateEngine, requestBasicMovement);
	
	while ((SDL_GetTicks() - currentt) <= 10) {}
	
	currentt = SDL_GetTicks();
	myOrbiter.updateMotion(currentt);
	myOrbiter.adjustSound();
	
	myWindow.render();

	backgroundTexture.render(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, NULL, 1, 0, NULL, SDL_FLIP_NONE);
    myOrbiter.render();

    if (data) { renderData(myOrbiter, myTargetCircleTexture, myTargetLineTexture, engineTexture, numbersTexture); }

	myWindow.present();
	
  }

  freeObjects(&myWindow, &backgroundTexture, &orbiterTexture, &rocketTexture, &flameTexture, &logoTexture,
              &myTargetCircleTexture, &myTargetLineTexture, &engineTexture, &numbersTexture, &myOrbiter, &mySound);

  closeSDL();
  
  return EXIT_SUCCESS;

}
