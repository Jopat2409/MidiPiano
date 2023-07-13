#ifndef _APPLICATION_INTERNAL_H
#define _APPLICATION_INTERNAL_H

#include "..\io\midi\Midi.h"

class Application {
public:
	Application(int width = 640, int height = 480);
	~Application();

private:
	Midi* m_currentMidiFile{nullptr};
};

#endif