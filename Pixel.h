#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Pixel
{
public:
	RectangleShape square;
	bool isAlive;
	bool nextStatus = false;
	Pixel* close[8];

	Pixel(float coeff, int col, int row, float pixSize);
	void Born();
	void Dies();
};

