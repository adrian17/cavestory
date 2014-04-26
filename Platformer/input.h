#pragma once

#include "SDL.h"
#include <map>

class Input
{
public:
	void beginNewFrame();

	void keyDownEvent(SDL_Event &event);
	void keyUpEvent(SDL_Event &event);

	bool wasKeyPressed(SDL_Keycode key);
	bool wasKeyReleased(SDL_Keycode key);
	bool isKeyHeld(SDL_Keycode key);
private:
	std::map<SDL_Keycode, bool> heldKeys;
	std::map<SDL_Keycode, bool> pressedKeys;
	std::map<SDL_Keycode, bool> releasedKeys;
};

