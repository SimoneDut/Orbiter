#include "sound.h"

SSound::SSound() : mSound(NULL), mVolume(0) {
}

SSound::~SSound() {
  free();
}

bool SSound::loadSound(std::string file) {
  free();
  mSound = Mix_LoadWAV(file.c_str());
  Mix_Volume(-1, 0);
  mVolume = Mix_Volume(-1, -1);
  return true;
}

void SSound::free() {
  if(mSound != NULL) {
	Mix_FreeChunk(mSound);
	mSound = NULL;
  }
  mVolume = 0;
}

void SSound::play() { Mix_PlayChannel(-1, mSound, -1); }

void SSound::stop() { Mix_HaltChannel(-1); }

void SSound::setVolume(int newVolume) {
  Mix_Volume(-1, newVolume);
  mVolume = Mix_Volume(-1, -1);
}
 
int SSound::getVolume() const { return mVolume; }
