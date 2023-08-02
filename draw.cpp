#include "draw.h"

Draw::Draw(RenderWindow* window) {
	this->window = window;
	this->circle = CircleShape(100.f);
}

void Draw::drawLoop()
{
	this->window->clear();

	this->window->draw(this->circle);

	this->window->display();
}

Draw::~Draw() {
	
}