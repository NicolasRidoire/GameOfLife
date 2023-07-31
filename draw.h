#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Draw {
private:
	RenderWindow* window;
	CircleShape circle;

public:
	Draw(RenderWindow* window);
	void drawLoop();
	void circleMove(Vector2<float> velocity);
	~Draw();
};

