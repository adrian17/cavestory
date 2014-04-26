#pragma once

class Game{
public:
	Game();
	~Game();
private:
	void eventLoop();

	void update();
	void draw();
};