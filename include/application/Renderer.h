#ifndef _INTERNAL_RENDERER_H
#define _INTERNAL_RENDERER_H

#include "..\dependencies\sdl\include\SDL.h"

class Renderer {
public:
	Renderer();
	~Renderer();
	int Init(int width, int height);

	void Close();
private:

	SDL_Window* m_window{nullptr};
	SDL_Surface* m_surface{nullptr};
	SDL_Renderer* m_renderer{nullptr};
};

typedef enum {
	ERRNO_RENDERER_SDL_NO_INIT = (1 << 1),
	ERRNO_RENDERER_WINDOW_NOT_CREATED = (1 << 2),
	ERRNO_RENDERER_SURFACE_NOT_CREATED = (1 << 3)
};

#endif