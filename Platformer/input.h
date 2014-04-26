#pragma once

#include "SDL.h"
#include <map>

class Input
{
public:
	void BeginNewFrame();

	void KeyDownEvent(SDL_Event &event);
	void KeyUpEvent(SDL_Event &event);

	bool WasKeyPressed(SDL_Keycode key);
	bool WasKeyReleased(SDL_Keycode key);
	bool isKeyHeld(SDL_Keycode key);
private:
	std::map<SDL_Keycode, bool> heldKeys;
	std::map<SDL_Keycode, bool> pressedKeys;
	std::map<SDL_Keycode, bool> releasedKeys;
};

