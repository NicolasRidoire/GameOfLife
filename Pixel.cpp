#include "Pixel.h"

Pixel::Pixel(float coeff, int col, int row, float pixSize) {
	this->isAlive = false;
	this->square = RectangleShape({ pixSize, pixSize * coeff });
	this->square.move({ pixSize * col, pixSize * row});
}

void Pixel::Born() {
	this->isAlive = true;
	this->square.setFillColor(Color::Black);
}

void Pixel::Dies() {
	this->isAlive = false;
	this->square.setFillColor(Color::White);
}
