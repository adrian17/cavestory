#pragma once

#include "graphics.h"
#include <string>

class Backdrop
{
public:
	virtual void draw(Graphics &graphics) const = 0;
};

class FixedBackdrop : public Backdrop{
public:
	FixedBackdrop(const std::string &path, Graphics &graphics);
	void draw(Graphics &graphics) const;
private:
	Graphics::TextureID textureID;
};