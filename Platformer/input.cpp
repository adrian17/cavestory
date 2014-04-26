#include "input.h"

void Input::BeginNewFrame(){
	pressedKeys.clear();
	releasedKeys.clear();
}

void Input::KeyDownEvent(SDL_Event &event){
	pressedKeys[event.key.keysym.sym] = true;
	heldKeys[event.key.keysym.sym] = true;
}

void Input::KeyUpEvent(SDL_Event &event){
	releasedKeys[event.key.keysym.sym] = true;
	heldKeys[event.key.keysym.sym] = false;
}

bool Input::WasKeyPressed(SDL_Keycode key){
	return pressedKeys[key];
}

bool Input::WasKeyReleased(SDL_Keycode key){
	return releasedKeys[key];
}

bool Input::isKeyHeld(SDL_Keycode key){
	return heldKeys[key];
}