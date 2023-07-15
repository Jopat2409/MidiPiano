#define SDL_MAIN_HANDLED
#include "..\include\application\Application.h"
#include <iostream>

int main(int argc, char* argv[]) {
	Application a;
	a.Init();
	a.Run();
	return 0;
}