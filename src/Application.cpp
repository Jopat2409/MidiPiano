#define SDL_MAIN_HANDLED // MUST be at the top of the file

#include "..\include\application\Application.h"

Application::Application(int width, int height) : m_width(width), m_height(height) {
	SDL_SetMainReady();
}

Application::~Application() {
	Close();
}
int Application::Init() {
	m_isInit = true;
	return m_graphicsRenderer.Init(m_width, m_height);
}

void Application::Run() {
	m_isRunning = true;
	PlayMidi("D:\\test.mid");
	while (m_isRunning) {
		HandleInput();
		Update();
		DrawFrame();
	}
}

void Application::HandleInput() {
	SDL_Event currentEvent;
	while (SDL_PollEvent(&currentEvent)) {
		if (currentEvent.type == SDL_QUIT) {
			m_isRunning = false;
			return;
		}
	}
}
void Application::Update() {
	// Update current song timer - 
	// Workout time in ms
	// 
	// if the next note has the current song timer, send to audio controller

	// send all notes that end before screen to midi renderer
}

void Application::PlayMidi(const char* path) {
	m_currentMidiFile = Midi::FromFile(path);
	if (Midi::GetLastError()) {
		std::cout << "Could not load midi: " << Midi::GetLastError() << "\n";
	}
}

void Application::DrawFrame() {
	// call render scene
	// call render UI
}

void Application::Close() {
	if (m_isInit) {
		m_isInit = false;
		m_graphicsRenderer.Close();
		m_audioHandler.Close();
		SDL_Quit();
	}
}