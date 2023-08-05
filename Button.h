#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Button : Event
{
private:
	Sprite bSprite;
	RectangleShape hitbox;

public:
	Button(Vector2<float> size, Vector2<float> hPosition, Vector2<float> sPosition, Sprite* buttonSprite);
	bool pressed(Event event);
	bool releaed(Event event);
};

