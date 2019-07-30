#ifndef ORBITER_H
#define ORBITER_H

#include <cstdlib>
#include <string>
#include <cmath>
#include <SDL.h>
#include "texture.h"
#include "sound.h"

enum eMode
{
  MANUAL,
  GOTOREST,
  ORBITMOUSE,
  MODE_TOTAL
};

class SOrbiter {

public:

  SOrbiter();
  ~SOrbiter();

void initialize(int initialx, int initialy, int initialomega,
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
				STexture* orbiterTextureP, STexture* rocketTextureP, STexture* flameTextureP, STexture* logoTextureP, SSound* engineSoundP); //Initialize the orbiter
  void free(); //Free the orbiter
  
  void updateMode(eMode newMode); //Update the operating mode of the orbiter
  void updateMousePosition(int newMouseX, int newMouseY, bool newActive); //Update the mouse position
  void updateManualControls(bool* requestActivateEngine, bool* requestBasicMovement); //Update the control requests of the orbiter
  void updateMotion(Uint32 currentt); //Update the dynamic variables of the orbiter
  void adjustSound(); //Set the volume of the sound according to the power of the engines
  
  void calculateAction(double* uxp, double* uyp, double* uomegap, double referencex, double referencey, double referenceomega, Uint32 currentt); //Calculate the control action
  void calculateEnginePower(double ux, double uy, double uomega); //Calculate the power of all the engines based on the control action
  
  bool render(); //Render the orbiter
  
  int getX(); //Return the x position of the orbiter
  int getY(); //Return the y position of the orbiter
  double getOmega(); //Return the omega angle of the orbiter (in degrees)
  int getTargetX(); //Return the target x position
  int getTargetY(); //Return the target y position
  double getTargetOmega();  //Return the target omega angle (in degrees)
  int getEnginePower(int engineNumber); //Return the power (as a percentage) of the selected engine
  eMode getMode(); //Return the current operating mode of the orbiter
  
private:
  
  double x, xp, xpp;
  double y, yp, ypp;
  double omega, omegap, omegapp;
  double Kpx, Tdx, Kpy, Tdy, Kpomega, Tdomega;
  double targetx, targety, targetomega;
  double restx, resty, restomega;
  int mousex, mousey;
  bool active;
  bool changedActive;
  double previousex, previousey, previouseomega;
  Uint32 previoust;
  double orbitradius;
  double orbitomegap;
  Uint32 tzero;
  double omegapthreshold;
  bool slowdown;
  double enginePower[4];
  bool activateEngine[4];
  bool basicMovement[6];
  double maxPowerAccelerationTrans;
  double maxPowerAccelerationRot;
  eMode mode;
  bool changedMode;
  
  double scaleOrbiter;
  double scaleRocket;
  double scaleFlame;
  int stdOffsetRocketX;
  int stdOffsetRocketY;
  int stdOffsetFlame1X;
  int stdOffsetFlame1Y;
  int stdOffsetFlame2X;
  int stdOffsetFlame2Y;
  int signRocketX[4];
  int signRocketY[4];
  int angleRocket[4];
  int angleFlame[4];
  int signFlameX[4];
  int signFlameY[4];

  STexture* orbiterTexturePointer;
  STexture* rocketTexturePointer;
  STexture* flameTexturePointer;
  STexture* logoTexturePointer;
  SSound* engineSoundPointer;

};

#endif