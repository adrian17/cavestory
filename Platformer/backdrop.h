#pragma once

class Graphics;
struct SDL_Texture;

class Backdrop
{
public:
	virtual void draw(Graphics &graphics) const = 0;
};

class FixedBackdrop : public Backdrop{
public:
	FixedBackdrop(const char* path, Graphics &graphics);
	void draw(Graphics &graphics) const;
private:
	SDL_Texture *textureID;
};