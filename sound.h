#ifndef SOUND_H
#define SOUND_H

#include <cstdlib>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

class SSound {

public:

  SSound();
  ~SSound();

  bool loadSound(std::string file); //Load the sound from the specified file
  void free(); //Free the sound
  
  void play(); //Start playing the sound
  void stop(); //Stop playing the sound
  void setVolume(int newVolume); //Set the volume of the sound
  
  int getVolume() const;  //Get the volume of the sound
  
private:
  
  Mix_Chunk* mSound;
  int mVolume;

};

#endif