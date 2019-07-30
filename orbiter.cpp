#include "orbiter.h"
#include "constants.h"

SOrbiter::SOrbiter() : x(0.0), xp(0.0), xpp(0.0), y(0.0), yp(0.0), ypp(0.0), omega(0.0), omegap(0.0), omegapp(0.0),
                       Kpx(0.0), Tdx(0.0), Kpy(0.0), Tdy(0.0), Kpomega(0.0), Tdomega(0.0),
					   targetx(0.0), targety(0.0), targetomega(0.0), restx(0.0), resty(0.0), restomega(0.0),
					   mousex(0), mousey(0), active(false), changedActive(false), previousex(0.0), previousey(0.0), previouseomega(0.0), previoust(0),
					   orbitradius(0.0), orbitomegap(0.0), tzero(0), omegapthreshold(0.0), slowdown(false),
					   enginePower{ 0.0, 0.0, 0.0, 0.0},
					   activateEngine{ false, false, false, false },
					   basicMovement{ false, false, false, false, false, false },
					   maxPowerAccelerationTrans(0.0), maxPowerAccelerationRot(0.0),
					   mode(MODE_TOTAL), changedMode(true), scaleOrbiter(0.0), scaleRocket(0.0), scaleFlame(0.0),
					   stdOffsetRocketX(0), stdOffsetRocketY(0), stdOffsetFlame1X(0), stdOffsetFlame1Y(0), stdOffsetFlame2X(0), stdOffsetFlame2Y(0),
					   signRocketX{0, 0, 0, 0}, signRocketY{0, 0, 0, 0},
					   angleRocket{0, 0, 0, 0}, angleFlame{0, 0, 0, 0},
					   signFlameX{0, 0, 0, 0}, signFlameY{0, 0, 0, 0},
					   orbiterTexturePointer(NULL), rocketTexturePointer(NULL), flameTexturePointer(NULL), logoTexturePointer(NULL), engineSoundPointer(NULL) {
}

SOrbiter::~SOrbiter() {
  free();
}

void SOrbiter::initialize(int initialx, int initialy, int initialomega,
                          double KpxSetting, double TdxSetting, double KpySetting, double TdySetting, double KpomegaSetting, double TDomegaSetting,
						  int restxSetting, int restySetting, int restomegaSetting,
						  Uint32 initialt,
						  double orbitradiusSetting, double orbitomegapSetting, double omegapthresholdSetting,
						  double maxPowerAccelerationTransSetting, double maxPowerAccelerationRotSetting,
						  eMode initialmode,
						  double scaleOrbiterSetting, double scaleRocketSetting, double scaleFlameSetting,
						  int stdOffsetRocketXSetting, int stdOffsetRocketYSetting,
						  int stdOffsetFlame1XSetting, int stdOffsetFlame1YSetting,
						  int stdOffsetFlame2XSetting, int stdOffsetFlame2YSetting,
						  STexture* orbiterTextureP, STexture* rocketTextureP, STexture* flameTextureP, STexture* logoTextureP, SSound* engineSoundP) {
  x = initialx;
  y = initialy;
  omega = ((double)initialomega)/180*PI;
  Kpx = KpxSetting;
  Tdx = TdxSetting;
  Kpy = KpySetting;
  Tdy = TdySetting;
  Kpomega = KpomegaSetting;
  Tdomega = TDomegaSetting;
  restx = restxSetting;
  resty = restySetting;
  restomega = ((double)(restomegaSetting % 360))/180*PI;
  previoust = initialt;
  orbitradius = orbitradiusSetting;
  orbitomegap =  orbitomegapSetting;
  omegapthreshold = omegapthresholdSetting;

  maxPowerAccelerationTrans = maxPowerAccelerationTransSetting;
  maxPowerAccelerationRot = maxPowerAccelerationRotSetting;
  mode = initialmode;
  changedMode = true;
 
  scaleOrbiter = scaleOrbiterSetting;
  scaleRocket = scaleRocketSetting;
  scaleFlame = scaleFlameSetting;
  stdOffsetRocketX = stdOffsetRocketXSetting;
  stdOffsetRocketY = stdOffsetRocketYSetting;
  stdOffsetFlame1X = stdOffsetFlame1XSetting;
  stdOffsetFlame1Y = stdOffsetFlame1YSetting;
  stdOffsetFlame2X = stdOffsetFlame2XSetting;
  stdOffsetFlame2Y = stdOffsetFlame2YSetting;
  
  signRocketX[0] = -1;
  signRocketX[1] = +1;
  signRocketX[2] = +1;
  signRocketX[3] = -1;
  signRocketY[0] = -1;
  signRocketY[1] = -1;
  signRocketY[2] = +1;
  signRocketY[3] = +1;
  angleRocket[0] = +45;
  angleRocket[1] = -45;
  angleRocket[2] = -135;
  angleRocket[3] = +135;
  angleFlame[0] = +45+180;
  angleFlame[1] = -45+180;
  angleFlame[2] = -135+180;
  angleFlame[3] = +135+180;
  signFlameX[0] = -1;
  signFlameX[1] = +1;
  signFlameX[2] = +1;
  signFlameX[3] = -1;
  signFlameY[0] = +1;
  signFlameY[1] = +1;
  signFlameY[2] = -1;
  signFlameY[3] = -1;

  orbiterTexturePointer = orbiterTextureP;
  rocketTexturePointer = rocketTextureP; 
  flameTexturePointer = flameTextureP;
  logoTexturePointer = logoTextureP;
  engineSoundPointer = engineSoundP;
  engineSoundPointer->play();
  
}

void SOrbiter::free() {

  x = 0.0;
  xp = 0.0;
  xpp = 0.0;
  y = 0.0;
  yp = 0.0;
  ypp = 0.0;
  omega = 0.0;
  omegap = 0.0;
  omegapp = 0.0;
  Kpx = 0.0;
  Tdx = 0.0;
  Kpy = 0.0;
  Tdy = 0.0;
  Kpomega = 0.0;
  Tdomega = 0.0;
  targetx = 0.0;
  targety = 0.0;
  targetomega = 0.0;
  restx = 0.0;
  resty = 0.0;
  restomega = 0.0;
  mousex = 0;
  mousey = 0;
  active = false;
  changedActive = false;
  previousex = 0.0;
  previousey = 0.0;
  previouseomega = 0.0;
  previoust = 0;
  orbitradius = 0.0;
  orbitomegap = 0.0;
  tzero = 0;
  omegapthreshold = 0.0;
  slowdown = false;

  enginePower[0] = 0.0;
  enginePower[1] = 0.0;
  enginePower[2] = 0.0;
  enginePower[3] = 0.0;
  activateEngine[0] = false;
  activateEngine[1] = false;
  activateEngine[2] = false;
  activateEngine[3] = false;
  basicMovement[0] = false;
  basicMovement[1] = false;
  basicMovement[2] = false;
  basicMovement[3] = false;
  basicMovement[4] = false;
  basicMovement[5] = false;
  maxPowerAccelerationTrans = 0;
  maxPowerAccelerationRot = 0;
  mode = MODE_TOTAL;
  changedMode = true;

  scaleOrbiter = 0.0;
  scaleRocket = 0.0;
  scaleFlame = 0.0;
  stdOffsetRocketX = 0;
  stdOffsetRocketY = 0;
  stdOffsetFlame1X = 0;
  stdOffsetFlame1Y = 0;
  stdOffsetFlame2X = 0;
  stdOffsetFlame2Y = 0;
  signRocketX[0] = 0;
  signRocketX[1] = 0;
  signRocketX[2] = 0;
  signRocketX[3] = 0;
  signRocketY[0] = 0;
  signRocketY[1] = 0;
  signRocketY[2] = 0;
  signRocketY[3] = 0;
  angleRocket[0] = 0;
  angleRocket[1] = 0;
  angleRocket[2] = 0;
  angleRocket[3] = 0;
  angleFlame[0] = 0;
  angleFlame[1] = 0;
  angleFlame[2] = 0;
  angleFlame[3] = 0;
  signFlameX[0] = 0;
  signFlameX[1] = 0;
  signFlameX[2] = 0;
  signFlameX[3] = 0;
  signFlameY[0] = 0;
  signFlameY[1] = 0;
  signFlameY[2] = 0;
  signFlameY[3] = 0;

  orbiterTexturePointer = NULL;
  rocketTexturePointer = NULL;
  flameTexturePointer = NULL;
  logoTexturePointer = NULL;
  engineSoundPointer->stop();
  engineSoundPointer = NULL;

}
 
void SOrbiter::updateMode(eMode newMode) {
  if (newMode != mode) {
	changedMode = true;
	if (abs(omegap) > omegapthreshold) { slowdown = true; }
	else { slowdown = false; }
	tzero = previoust;
	omega -= ((double)(((int)(omega*180/PI))/360))*2*PI;
  }
  else { changedMode = false; }
  mode = newMode;
}

void SOrbiter::updateMousePosition(int newMouseX, int newMouseY, bool newActive) {
  mousex = newMouseX;
  mousey = newMouseY;
  if (active != newActive) { changedActive = true; }
  else { changedActive = false; }
  active = newActive;
}

void SOrbiter::updateManualControls(bool* requestActivateEngine, bool* requestBasicMovement) {
  
  activateEngine[0] = requestActivateEngine[0];
  activateEngine[1] = requestActivateEngine[1];
  activateEngine[2] = requestActivateEngine[2];
  activateEngine[3] = requestActivateEngine[3];
  basicMovement[0] = requestBasicMovement[0];
  basicMovement[1] = requestBasicMovement[1];
  basicMovement[2] = requestBasicMovement[2];
  basicMovement[3] = requestBasicMovement[3];
  basicMovement[4] = requestBasicMovement[4];
  basicMovement[5] = requestBasicMovement[5];

}

void SOrbiter::updateMotion(Uint32 currentt) {

  //Calculate the new coordinates of the orbiter
  
  double deltaxp = xpp * ((double)(currentt - previoust))/1000;
  double deltayp = ypp * ((double)(currentt - previoust))/1000;
  double deltaomegap = omegapp * ((double)(currentt - previoust))/1000;

  xp = xp + deltaxp;
  yp = yp + deltayp;
  omegap = omegap + deltaomegap; 

  x = x + (xp - deltaxp/2) * ((double)(currentt - previoust))/1000;
  y = y + (yp - deltayp/2) * ((double)(currentt - previoust))/1000;
  omega = omega + (omegap - deltaomegap/2) * ((double)(currentt - previoust))/1000;

  //Determine action based on mode and inputs
  if (mode == MANUAL) {
	targetx = 0.0;
	targety = 0.0;
	targetomega = 0.0;
    for (int i = 0; i < 4; i++) { enginePower[i] = 0; }
    if (activateEngine[0]||activateEngine[1]||activateEngine[2]||activateEngine[3]) {
      for (int i = 0; i < 4; i++) {
	    if (activateEngine[i]) { enginePower[i] = 0.5; }
	  }
    }
	else {
	  if (basicMovement[0]) {
		enginePower[0] += 0.5;
		enginePower[1] += 0.5;
	  }
	  if (basicMovement[1]) {
		enginePower[2] += 0.5;
		enginePower[3] += 0.5;
	  }
	  if (basicMovement[2]) {
		enginePower[1] += 0.5;
		enginePower[2] += 0.5;
	  }
	  if (basicMovement[3]) {
		enginePower[0] += 0.5;
		enginePower[3] += 0.5;
	  }
	  if (basicMovement[4]) {
		enginePower[0] += 0.5;
		enginePower[2] += 0.5;
	  }
	  if (basicMovement[5]) {
		enginePower[1] += 0.5;
		enginePower[3] += 0.5;
	  }
	}
  }
  else if (mode == GOTOREST) {
    
	double ux, uy, uomega;
	if ((slowdown)&&(abs(omegap) > omegapthreshold)) {
	  targetx = restx;
	  targety = resty;
	  targetomega = omega-omegap/abs(omegap)*(2/Kpomega);
	}
    else {
	  if (slowdown) {
		slowdown = false;
	    omega -= ((double)(((int)((omega-restomega)*180/PI))/360))*2*PI;
	    changedMode = true;
	  }
	  targetx = restx;
	  targety = resty;
	  targetomega = restomega;
	}
	
	calculateAction(&ux, &uy, &uomega, targetx, targety, targetomega, currentt);
	calculateEnginePower(ux, uy, uomega);	

  }
  else if (mode == ORBITMOUSE) {
    
	double ux, uy, uomega;
	if ((slowdown)&&(abs(omegap) > omegapthreshold)) {
		targetx = restx;
		targety = resty;
		targetomega = omega-omegap/abs(omegap)*(2/Kpomega);
	}
    else {
	  if (slowdown) {
		slowdown = false;
	    omega = ((double)(((int)(omega*180/PI)) % 360))/180*PI;
	    tzero = currentt;
	    changedMode = true;
	  }
	  if (changedActive) {
		if (active) { tzero = previoust; }
		else  { omega -= ((double)(((int)((omega-restomega)*180/PI))/360))*2*PI; }
	  }
	  if (active) {
	  targetx = mousex + orbitradius * cos(orbitomegap * (((double)(currentt - tzero))/1000));
	  targety = mousey - orbitradius * sin(orbitomegap * (((double)(currentt - tzero))/1000));
	  targetomega = orbitomegap * (((double)(currentt - tzero))/1000) + PI/2;
      }
	  else {
	  targetx = restx;
	  targety = resty;
	  targetomega = restomega;
	  }
	}
	
	calculateAction(&ux, &uy, &uomega, targetx, targety, targetomega, currentt);
	calculateEnginePower(ux, uy, uomega);	

  }

  //Determine the new accelerations
  xpp = (+(enginePower[0] - enginePower[2]) * cos(omega + 45.0/180*PI) +
         +(enginePower[3] - enginePower[1]) * sin(omega + 45.0/180*PI)) * maxPowerAccelerationTrans;

  ypp = (-(enginePower[0] - enginePower[2]) * sin(omega + 45.0/180*PI) +
         +(enginePower[3] - enginePower[1]) * cos(omega + 45.0/180*PI)) * maxPowerAccelerationTrans;
  
  omegapp = (enginePower[1] + enginePower[3] - enginePower[0] - enginePower[2]) * maxPowerAccelerationRot;

  //Update some variables
  previoust = currentt;
  changedMode = false;

}

void SOrbiter::adjustSound() {
	
    engineSoundPointer->setVolume((int)(MIX_MAX_VOLUME*(enginePower[0]+enginePower[1]+enginePower[2]+enginePower[3])/4));

}

void SOrbiter::calculateAction(double* uxp, double* uyp, double* uomegap, double referencex, double referencey, double referenceomega, Uint32 currentt) {

  double ex = referencex - x;
  double ey = - (referencey - y); // special treatment due to screen coordinates reference system
  double eomega = referenceomega - omega;
  
  if (changedMode) {
	previousex = ex;
	previousey = ey;
	previouseomega = eomega;
  }
  
  *uxp = Kpx * ( ex + Tdx * ( ex - previousex ) / (((double)( currentt - previoust ))/1000) );
  *uyp = Kpy * ( ey + Tdy * ( ey - previousey ) / (((double)( currentt - previoust ))/1000) );
  *uomegap = Kpomega * ( eomega + Tdomega * ( eomega - previouseomega ) / (((double)( currentt - previoust ))/1000) );

  previousex = ex;
  previousey = ey;
  previouseomega = eomega;

}

void SOrbiter::calculateEnginePower(double ux, double uy, double uomega) {
	
  double u1 = + ux * cos(omega) + uy * sin(omega);
  double u2 = - ux * sin(omega) + uy * cos(omega);
	
  double u1positive, u1negative, u2positive, u2negative, uomegapositive, uomeganegative;
  if (u1 >= 0) {
	u1positive = u1;
	u1negative = 0;
  }
  else {
    u1positive = 0;
    u1negative = -u1;
  }
  if (u2 >= 0) {
    u2positive = u2;
    u2negative = 0;
  }
  else {
    u2positive = 0;
    u2negative = -u2;
  }
  if (uomega >= 0) {
    uomegapositive = uomega;
    uomeganegative = 0;
  }
  else {
    uomegapositive = 0;
    uomeganegative = -uomega;
  }
	
  enginePower[0] = u1positive / sqrt(2) + u2positive / sqrt(2) + uomeganegative / 2;
  enginePower[1] = u1negative / sqrt(2) + u2positive / sqrt(2) + uomegapositive / 2;
  enginePower[2] = u1negative / sqrt(2) + u2negative / sqrt(2) + uomeganegative / 2;
  enginePower[3] = u1positive / sqrt(2) + u2negative / sqrt(2) + uomegapositive / 2;
    
  double min = enginePower[0];	
  for (int i = 1; i <4; i++) {
    if (enginePower[i] < min) { min = enginePower[i]; }
  }
  for (int i = 0; i <4; i++) {
	enginePower[i] -= min;
  }

  double max = enginePower[0];	
  for (int i = 1; i <4; i++) {	
    if (enginePower[i] > max) { max = enginePower[i]; }
  }
  if (max > 1) {
	for (int i = 0; i <4; i++) {
	  enginePower[i] /= max;
    }
  }
}
 
bool SOrbiter::render() {
  orbiterTexturePointer->render((int)round(x),(int)round(y), NULL, scaleOrbiter, -omega/PI*180, NULL, SDL_FLIP_NONE);
  for (int i = 0; i <4; i++) {
	int offsetRocketX = stdOffsetRocketX * signRocketX[i];
	int offsetRocketY = stdOffsetRocketY * signRocketY[i];
	int offsetFlameX = offsetRocketX + (stdOffsetFlame1X + (double)stdOffsetFlame2X * enginePower[i]) * signFlameX[i];
	int offsetFlameY = offsetRocketY + (stdOffsetFlame1Y + (double)stdOffsetFlame2Y * enginePower[i]) * signFlameY[i];  
	  
	double temp1 = offsetRocketX * cos(omega) + offsetRocketY * sin(omega);
	double temp2 = -offsetRocketX * sin(omega) + offsetRocketY * cos(omega);
	offsetRocketX = round(temp1);
	offsetRocketY = round(temp2);
	  
	temp1 = offsetFlameX * cos(omega) + offsetFlameY * sin(omega);
	temp2 = -offsetFlameX * sin(omega) + offsetFlameY * cos(omega);
	offsetFlameX = round(temp1);
	offsetFlameY = round(temp2);
	  
	SDL_Rect flameClip;
	flameClip.x = 0;
	flameClip .y = 0;
	flameClip.w = flameTexturePointer->getWidth();
	flameClip.h = flameTexturePointer->getHeight() * enginePower[i];
	flameTexturePointer->render((int)round(x+offsetFlameX), (int)round(y+offsetFlameY), &flameClip , scaleFlame, angleFlame[i]-omega/PI*180, NULL, SDL_FLIP_NONE);
	rocketTexturePointer->render((int)round(x+offsetRocketX), (int)round(y+offsetRocketY), NULL, scaleRocket, angleRocket[i]-omega/PI*180, NULL, SDL_FLIP_NONE);
    }
  logoTexturePointer->render((int)round(x),(int)round(y), NULL, scaleOrbiter, (int)round(-omega/PI*180), NULL, SDL_FLIP_NONE);

  return true;
}
 
int SOrbiter::getX() { return (int)round(x); }

int SOrbiter::getY() { return (int)round(y); }

double SOrbiter::getOmega() { return omega*180/PI; }

int SOrbiter::getTargetX() { return (int)round(targetx); }

int SOrbiter::getTargetY() { return (int)round(targety); }

double SOrbiter::getTargetOmega() { return targetomega*180/PI; }

int SOrbiter::getEnginePower(int engineNumber) { return (int)round(enginePower[engineNumber-1]*100); }

eMode SOrbiter::getMode() { return mode; }
