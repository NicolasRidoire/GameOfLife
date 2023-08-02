#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Draw {
private:
	RenderWindow* window;

public:
	CircleShape circle;
	Draw(RenderWindow* window);
	void drawLoop();
	~Draw();
};

