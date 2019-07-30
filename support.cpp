#include <iostream>
#include <cmath>
#include "support.h"

bool initSDL() {

  //Initialization flag
  bool success = true;

  //Initialize SDL
  int SDLFlags = SDL_INIT_EVERYTHING;
  if (SDL_Init(SDLFlags) < 0 )	{
	printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
	success = false;
  }
  
  //Set texture filtering to linear
  if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
	printf( "Warning: Linear texture filtering not enabled!" );
  }

  int imgFlags = IMG_INIT_PNG;
  if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
	printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    success = false;
  }

  //Initialize SDL_ttf
  if( TTF_Init() == -1 ) {
    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
    success = false;
  }
	
  //Initialize SDL_mixer
  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
	printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	success = false;
  }
  
  return success;

}

void closeSDL() {
	
  //Quit SDL subsystems
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

}

bool initObjects(SWindow* myWindowP, STexture* backgroundTextureP, STexture* orbiterTextureP, STexture* rocketTextureP, STexture* flameTextureP, STexture* logoTextureP,
                 STexture* myTargetCircleTextureP, STexture* myTargetLineTextureP, STexture* engineTextureP, STexture** numbersTextureP,
				 SOrbiter* myOrbiterP, SSound* mySoundP) {

  //Window creation
  myWindowP->init("Orbiter simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE,
                  -1, SDL_RENDERER_ACCELERATED, 0x00, 0x00, 0x00, 0xFF, SDL_BLENDMODE_BLEND);
  SSurface iconSurface;
  iconSurface.createFromFile(std::string("icon.png"));
  myWindowP->setIcon(iconSurface);
  iconSurface.free();

  //Sound creation
  if (!mySoundP->loadSound(std::string("enginesound.wav"))) {
	std::cout << "Failed to create sound\n";
	return false;
  }
    
  //File based textures creation
  SSurface backgroundSurface;
  backgroundSurface.createFromFile(std::string("background.png"));
  backgroundTextureP->associateWindow(*myWindowP);
  if (!backgroundTextureP->createFromSurface(backgroundSurface)) {
    std::cout << "Failed to create background texture from background surface\n";
	return false;
  }
  backgroundSurface.free();

  SSurface orbiterSurface = drawOrbiter(0xFF, 0xFF, 0xFF, 0xFF, 0x5F, 0x5F, 0x5F, 0xFF);
  orbiterTextureP->associateWindow(*myWindowP);
  if (!orbiterTextureP->createFromSurface(orbiterSurface)) {
    std::cout << "Failed to create orbiter texture from orbiter surface\n";
	return false;
  }
  orbiterSurface.free();
  
  SSurface rocketSurface;
  rocketSurface.createFromFile(std::string("rocket.png"));
  rocketTextureP->associateWindow(*myWindowP);
  if (!rocketTextureP->createFromSurface(rocketSurface)) {
    std::cout << "Failed to create rocket texture from rocket surface\n";
	return false;
  }
  rocketSurface.free();

  SSurface flameSurface;
  flameSurface.createFromFile(std::string("flame.png"));
  flameTextureP->associateWindow(*myWindowP);
  if (!flameTextureP->createFromSurface(flameSurface)) {
	std::cout << "Failed to create flame texture from flame surface\n";
	return false;
  }
  flameSurface.free();
 
  //Font based textures creation
  SFont myFont;
  SSurface logoSurface;
  myFont.loadFromFont("TYPEWR__.ttf",24);
  std::string textToType1("Orbiter");
  logoSurface.createFromText(textToType1, myFont, BLENDED, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00);
  logoTextureP->associateWindow(*myWindowP);
  if (!logoTextureP->createFromSurface(logoSurface)) {
	std::cout << "Failed to create logo texture from logo surface\n";
	return false;
  }
  logoSurface.free();
  
  SSurface engineSurface;
  myFont.loadFromFont("TYPEWR__.ttf",18);
  std::string textToType2("Engine   :       %");
  engineSurface.createFromText(textToType2, myFont, BLENDED, 0x00, 0x7F, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00);
  engineTextureP->associateWindow(*myWindowP);
  if (!engineTextureP->createFromSurface(engineSurface)) {
	std::cout << "Failed to create engine texture from engine surface\n";
	return false;
  }
  engineSurface.free();
  
  char buffer[2];
  SSurface numbersSurface;
  for (int i = 0; i < 10; i++) {
	itoa(i, buffer, 10);
	numbersSurface.createFromText(std::string(buffer), myFont, BLENDED, 0x00, 0x7F, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00);
	(*numbersTextureP)[i].associateWindow(*myWindowP);
    (*numbersTextureP)[i].createFromSurface(numbersSurface);
  }
  numbersSurface.free();
  
  myFont.free();

  //Orbiter creation
  myOrbiterP->initialize(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0,
                       0.2, 0.4, 0.2, 0.4, 20, 0.5, // the controls
					   SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0, // rest setting)
					   SDL_GetTicks(),
					   100, 2*PI/10, PI/2, //orbit parameters
					   200, 2*PI/5, //power setting
					   MANUAL,
					   0.5, 0.1, 0.021,
					   35, 41,
					   15, 15,
					   7, 7,
					   orbiterTextureP, rocketTextureP, flameTextureP, logoTextureP, mySoundP); //Initialize the orbiter

  //Target creation
  SSurface myTargetCircleSurface = drawTargetCircle(0xFF, 0x7F, 0x7F, 0xFF, 5);
  myTargetCircleTextureP->associateWindow(*myWindowP);
  if (!myTargetCircleTextureP->createFromSurface(myTargetCircleSurface)) {
	std::cout << "Failed to create target circle texture from target circle surface\n";
	return false;
  }
  myTargetCircleSurface.free();
  
  SSurface myTargetLineSurface = drawDirectionLine(0xFF, 0x7F, 0x7F, 0xFF, 45, 3);
  myTargetLineTextureP->associateWindow(*myWindowP);
  if (!myTargetLineTextureP->createFromSurface(myTargetLineSurface)) {
	std::cout << "Failed to create target line texture from target line surface\n";
	return false;
  }
  myTargetLineSurface.free();
  
  return true;

}

void freeObjects(SWindow* myWindowP, STexture* backgroundTextureP, STexture* orbiterTextureP, STexture* rocketTextureP, STexture* flameTextureP, STexture* logoTextureP,
                 STexture* myTargetCircleTextureP, STexture* myTargetLineTextureP,  STexture* engineTextureP, STexture** numbersTextureP,
				 SOrbiter* myOrbiterP, SSound* mySoundP) {

  myTargetLineTextureP->free();
  myTargetCircleTextureP->free();
  myOrbiterP->free();
  delete[] (*numbersTextureP);
  engineTextureP->free();
  logoTextureP->free();
  flameTextureP->free();
  rocketTextureP->free();
  orbiterTextureP->free();
  backgroundTextureP->free();
  mySoundP->free();
  myWindowP->free();

}

void renderData(SOrbiter& myOrbiter, STexture& myTargetCircleTexture, STexture& myTargetLineTexture, STexture& engineTexture, STexture*& numbersTexture) {

  if (myOrbiter.getMode() != MANUAL) {
	myTargetCircleTexture.render(myOrbiter.getTargetX(), myOrbiter.getTargetY(), NULL, 1, 0, NULL, SDL_FLIP_NONE);
    myTargetLineTexture.render((int)round(myOrbiter.getTargetX() - myTargetLineTexture.getHeight()/2*sin((myOrbiter.getTargetOmega())/180.0*PI)),
                               (int)round(myOrbiter.getTargetY() - myTargetLineTexture.getHeight()/2*cos((myOrbiter.getTargetOmega())/180.0*PI)),
						       NULL, 1, -myOrbiter.getTargetOmega(), NULL, SDL_FLIP_NONE);
  }					
	
  int spacing = 40;
  int offsetTextFromBottomY = 50;
  int offsetEngineNumberFromTextY = -2;
  int offsetEngineNumberFromTextX = 0;
  int offsetPowerFromEngineNumberX = 25;
  int offsetPowerNumbersX = 10;
  int textStartingX = SCREEN_WIDTH/2 - spacing*(1.5) - engineTexture.getWidth()*(2);
	
  for (int i = 0; i < 4; i++) {
	engineTexture.render(textStartingX + spacing*(i)+engineTexture.getWidth()/2*(i*2+1),
	                     SCREEN_HEIGHT-offsetTextFromBottomY, NULL, 1, 0, NULL, SDL_FLIP_NONE);					
	numbersTexture[i+1].render(textStartingX + spacing*(i)+engineTexture.getWidth()/2*(i*2+1)+offsetEngineNumberFromTextX,
	                           SCREEN_HEIGHT-offsetTextFromBottomY+offsetEngineNumberFromTextY, NULL, 1, 0, NULL, SDL_FLIP_NONE);
	int power = myOrbiter.getEnginePower(i+1);
	int u = power % 10;
	power = power / 10;
	int d = power % 10;
	int c = power / 10;
	if (c != 0) {
	  numbersTexture[c].render(textStartingX + spacing*(i)+engineTexture.getWidth()/2*(i*2+1)+offsetEngineNumberFromTextX+offsetPowerFromEngineNumberX,
	                           SCREEN_HEIGHT-offsetTextFromBottomY+offsetEngineNumberFromTextY, NULL, 1, 0, NULL, SDL_FLIP_NONE);
	}
	if ((c != 0)||(d != 0)) {
	  numbersTexture[d].render(textStartingX + spacing*(i)+engineTexture.getWidth()/2*(i*2+1)+offsetEngineNumberFromTextX+offsetPowerFromEngineNumberX+offsetPowerNumbersX,
	                           SCREEN_HEIGHT-offsetTextFromBottomY+offsetEngineNumberFromTextY, NULL, 1, 0, NULL, SDL_FLIP_NONE);
	}
	numbersTexture[u].render(textStartingX + spacing*(i)+engineTexture.getWidth()/2*(i*2+1)+offsetEngineNumberFromTextX+offsetPowerFromEngineNumberX+2*offsetPowerNumbersX,
	                         SCREEN_HEIGHT-offsetTextFromBottomY+offsetEngineNumberFromTextY, NULL, 1, 0, NULL, SDL_FLIP_NONE);
  }
}

SSurface drawTargetCircle(Uint8 r, Uint8 g, Uint8 b, Uint8 a, int radiusCircle) {

  SDL_Color color;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = a;

  SDL_Color backgroundColor;
  backgroundColor.r = 0xFF;
  backgroundColor.g = 0xFF;
  backgroundColor.b = 0xFF;
  backgroundColor.a = 0x00;

  std::list<std::pair<SShape, SDL_Color> > shapes;

  shapes.push_back(std::make_pair(SShape(CIRCLE, true, radiusCircle, radiusCircle, radiusCircle, 0), color));
  SSurface ans;
  ans.createFromShapes(2*radiusCircle+1, 2*radiusCircle+1, backgroundColor, shapes);
  ans.setBlendMode(SDL_BLENDMODE_BLEND);
  
  return ans;

}

SSurface drawDirectionLine(Uint8 r, Uint8 g, Uint8 b, Uint8 a, int lengthLine, int thicknessLine) {

  SDL_Color color;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = a;

  SDL_Color backgroundColor;
  backgroundColor.r = 0xFF;
  backgroundColor.g = 0xFF;
  backgroundColor.b = 0xFF;
  backgroundColor.a = 0x00;

  std::list<std::pair<SShape, SDL_Color> > shapes;

  shapes.push_back(std::make_pair(SShape(RECTANGLE, true, 0, 0, thicknessLine-1, lengthLine-1), color));
  SSurface ans;
  ans.createFromShapes(thicknessLine, lengthLine, backgroundColor, shapes);
  ans.setBlendMode(SDL_BLENDMODE_BLEND);
  
  return ans;

}

SSurface drawOrbiter(Uint8 ri, Uint8 gi, Uint8 bi, Uint8 ai, Uint8 rb, Uint8 gb, Uint8 bb, Uint8 ab) {

  SDL_Color interiorColor;
  interiorColor.r = ri;
  interiorColor.g = gi;
  interiorColor.b = bi;
  interiorColor.a = ai;

  SDL_Color borderColor;
  borderColor.r = rb;
  borderColor.g = gb;
  borderColor.b = bb;
  borderColor.a = ab;

  SDL_Color backgroundColor;
  backgroundColor.r = 0xFF;
  backgroundColor.g = 0xFF;
  backgroundColor.b = 0xFF;
  backgroundColor.a = 0x00;

  std::list<std::pair<SShape, SDL_Color> > shapes;

  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 24, 24, 24, 0), borderColor));
  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 24, 24, 22, 0), interiorColor));

  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 175, 24, 24, 0), borderColor));
  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 175, 24, 22, 0), interiorColor));

  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 24, 175, 24, 0), borderColor));
  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 24, 175, 22, 0), interiorColor));

  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 175, 175, 24, 0), borderColor));
  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 175, 175, 22, 0), interiorColor));

  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 99, 99, 54, 0), borderColor));
  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 100, 99, 54, 0), borderColor));
  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 99, 100, 54, 0), borderColor));
  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 100, 100, 54, 0), borderColor));

  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 99, 99, 52, 0), interiorColor));
  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 100, 99, 52, 0), interiorColor));
  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 99, 100, 52, 0), interiorColor));
  shapes.push_back(std::make_pair(SShape(CIRCLE, true, 100, 100, 52, 0), interiorColor));

  int x1 = 24+17;
  int deltax1 = +1;
  int y1 = 24+17;
  int deltay1 = +1;
  int x2 = 99-38;
  int deltax2 = -1;
  int y2 = 99-38;
  int deltay2 = -1;
  for (int i = -10; i <= 10; i++) {
	int sgn;
	if (i>=0) { sgn = 1; }
	else { sgn = -1; }
    if (abs(i)<= 7) { shapes.push_back(std::make_pair(SShape(LINE, true, x1-abs(i)/2*sgn-4*deltax1, y1+(abs(i)+1)/2*sgn-4*deltay1,
	                                                                     x2-abs(i)/2*sgn-4*deltax2, y2+(abs(i)+1)/2*sgn-4*deltay2), interiorColor)); }
	else  { shapes.push_back(std::make_pair(SShape(LINE, true, x1-abs(i)/2*sgn,y1+(abs(i)+1)/2*sgn, x2-abs(i)/2*sgn, y2+(abs(i)+1)/2*sgn), borderColor)); }
  }

  x1 = 175-17;
  deltax1 = -1;
  y1 = 24+17;
  deltay1 = +1;
  x2 = 100+38;
  deltax2 = +1;
  y2 = 99-38;
  deltay2 = -1;
  for (int i = -10; i <= 10; i++) {
	int sgn;
	if (i>=0) { sgn = 1; }
	else { sgn = -1; }
    if (abs(i)<= 7) { shapes.push_back(std::make_pair(SShape(LINE, true, x1-abs(i)/2*sgn-4*deltax1, y1-(abs(i)+1)/2*sgn-4*deltay1,
	                                                                     x2-abs(i)/2*sgn-4*deltax2, y2-(abs(i)+1)/2*sgn-4*deltay2), interiorColor)); }
	else  { shapes.push_back(std::make_pair(SShape(LINE, true, x1-abs(i)/2*sgn,y1-(abs(i)+1)/2*sgn, x2-abs(i)/2*sgn, y2-(abs(i)+1)/2*sgn), borderColor)); }
  }

  x1 = 24+17;
  deltax1 = +1;
  y1 = 175-17;
  deltay1 = -1;
  x2 = 99-38;
  deltax2 = -1;
  y2 = 100+38;
  deltay2 = +1;
  for (int i = -10; i <= 10; i++) {
	int sgn;
	if (i>=0) { sgn = 1; }
	else { sgn = -1; }
    if (abs(i)<= 7) { shapes.push_back(std::make_pair(SShape(LINE, true, x1-abs(i)/2*sgn-4*deltax1, y1-(abs(i)+1)/2*sgn-4*deltay1,
	                                                                     x2-abs(i)/2*sgn-4*deltax2, y2-(abs(i)+1)/2*sgn-4*deltay2), interiorColor)); }
	else  { shapes.push_back(std::make_pair(SShape(LINE, true, x1-abs(i)/2*sgn,y1-(abs(i)+1)/2*sgn, x2-abs(i)/2*sgn, y2-(abs(i)+1)/2*sgn), borderColor)); }
  }

  x1 = 175-17;
  deltax1 = -1;
  y1 = 175-17;
  deltay1 = -1;
  x2 = 100+38;
  deltax2 = +1;
  y2 = 100+38;
  deltay2 = +1;
  for (int i = -10; i <= 10; i++) {
	int sgn;
	if (i>=0) { sgn = 1; }
	else { sgn = -1; }
    if (abs(i)<= 7) { shapes.push_back(std::make_pair(SShape(LINE, true, x1-abs(i)/2*sgn-4*deltax1, y1+(abs(i)+1)/2*sgn-4*deltay1,
	                                                                     x2-abs(i)/2*sgn-4*deltax2, y2+(abs(i)+1)/2*sgn-4*deltay2), interiorColor)); }
	else  { shapes.push_back(std::make_pair(SShape(LINE, true, x1-abs(i)/2*sgn,y1+(abs(i)+1)/2*sgn, x2-abs(i)/2*sgn, y2+(abs(i)+1)/2*sgn), borderColor)); }
  }

  SSurface ans;
  ans.createFromShapes(200, 200, backgroundColor, shapes);
  ans.setBlendMode(SDL_BLENDMODE_BLEND);
  
  return ans;

}