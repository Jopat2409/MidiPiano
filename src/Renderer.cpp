#include "..\include\application\Renderer.h"

#include "..\include\base\Logging.h"

Renderer::Renderer() {

}

Renderer::~Renderer() {

}

void Renderer::Close() {
	SDL_FreeSurface(m_surface);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
}

int Renderer::Init(int width, int height) {
	int returnCode = 0;
	if (SDL_Init(SDL_INIT_VIDEO)) {
		ERROR("Could not initialise SDL video system");
		returnCode |= ERRNO_RENDERER_SDL_NO_INIT;
	}
	if (SDL_CreateWindowAndRenderer(width, height, 0, &m_window, &m_renderer)) {
		ERROR("Could not initialise SDL window and/or renderer");
		returnCode |= ERRNO_RENDERER_WINDOW_NOT_CREATED;
	}
	if (!(m_surface = SDL_GetWindowSurface(m_window))) {
		ERROR("Could not initialise window surface");
		returnCode |= ERRNO_RENDERER_SURFACE_NOT_CREATED;
	}
	return returnCode;
}
