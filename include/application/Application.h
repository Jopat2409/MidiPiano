#ifndef _APPLICATION_INTERNAL_H
#define _APPLICATION_INTERNAL_H

#include "..\io\midi\Midi.h"
#include "AudioHandler.h"
#include "Renderer.h"

class Application {
public:
	Application(int width = 640, int height = 480);
	~Application();
	
	void Run();
	int Init();

private:
	
	Midi* m_currentMidiFile{nullptr};
	int m_width = 0, m_height = 0;
	bool m_isInit{ false };
	bool m_isRunning{ false };

	Renderer m_graphicsRenderer;
	AudioHandler m_audioHandler;

	int InitUI();

	void HandleInput();
	void DrawFrame();
	void Update();
	
	void Close();

	void PlayMidi(const char* path);
};

#endif