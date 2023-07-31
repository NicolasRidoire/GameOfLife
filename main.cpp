#include <SFML/Graphics.hpp>
#include <vector>

#include "draw.h"

using namespace sf;

int WinMain() {
	// Initialisation
	RenderWindow window(VideoMode(800, 600), "OK");
	
	Draw* draw = new Draw(&window);

	float deltaTime = 0.f;
	Clock deltaClock;

	while (window.isOpen()) {
		// Game loop
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		Vector2<float> velocity(10.f, 0.f);
		draw->circleMove(velocity * deltaTime);

		draw->drawLoop();
		
		deltaTime = deltaClock.restart().asSeconds();
	}

	delete draw;

	return 0;
}