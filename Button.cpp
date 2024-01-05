#include "Button.h"

Button::Button(Vector2<float> size, Vector2<float> hPosition, Vector2<float> sPosition, Sprite* buttonSprite) {
	this->bSprite = *buttonSprite;
	this->bSprite.setPosition(sPosition);
	this->hitbox = RectangleShape(size);
	this->hitbox.setPosition(hPosition);
}

bool Button::pressed(Event event) {
	return false;
}
