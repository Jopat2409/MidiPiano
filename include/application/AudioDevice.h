#ifndef _INTERNAL_AUDIODEVICE_H
#define _INTERNAL_AUDIODEVICE_H

#include "..\dependencies\sdl\include\SDL_audio.h"

class AudioDevice {
public:
	~AudioDevice();

	void Open();
	void Close();

private:

	void OpenDefault();
	SDL_AudioSpec* m_desiredAudioSpecification{nullptr};
	SDL_AudioSpec* m_audioSpecification{nullptr};
	
};



#endif