#pragma once

class Rectangle
{
public:
	Rectangle(int x, int y, int w, int h) :
		x(x), y(y), w(w), h(h) {}

	int left() const{ return x; }
	int right() const{ return x + w; }
	int top() const{ return y; }
	int bottom() const{ return y + h; }
	int width() const{ return w; }
	int height() const{ return h; }
private:
	const int x, y, w, h;
};

