#include <SFML/Graphics.hpp>
#include <vector>

#include "draw.h"
#include "Pixel.h"
using namespace sf;

const int column = 800;
const int row = 600;
const float coeff = column / row;
const float pixSize = 10.f;
Pixel* screen[(column / 10) * (row / 10)];
Pixel* nextScreen[(column / 10) * (row / 10)];

int WinMain() {
	// Initialisation
	RenderWindow window(VideoMode(column, row), "OK");
	Font font;
	font.loadFromFile("C:/Windows/Fonts/ariblk.ttf");
	Text number;
	number.setFont(font);
	number.setFillColor(Color::Red);

	for (int i = 0; i < column / 10; i++) {
		for (int j = 0; j < row / 10; j++) {
			screen[i * (row / 10) + j] = new Pixel(coeff, i, j, pixSize);
		}
	}
	for (int i = 0; i < column / 10; i++) {
		for (int j = 0; j < row / 10; j++) {
			if (j != 0) {
				screen[i * (row / 10) + j]->close[0] = screen[(i - 1) * (row / 10) + j - 1];
				screen[i * (row / 10) + j]->close[1] = screen[i * (row / 10) + j - 1];
				screen[i * (row / 10) + j]->close[2] = screen[(i + 1) * (row / 10) + j - 1];
			}
			else {
				screen[i * (row / 10) + j]->close[0] = nullptr;
				screen[i * (row / 10) + j]->close[1] = nullptr;
				screen[i * (row / 10) + j]->close[2] = nullptr;
			}
			if (j != (row / 10) - 1) {
				screen[i * (row / 10) + j]->close[5] = screen[(i - 1) * (row / 10) + j + 1];
				screen[i * (row / 10) + j]->close[6] = screen[i * (row / 10) + j + 1];
				screen[i * (row / 10) + j]->close[7] = screen[(i + 1) * (row / 10) + j + 1];
			}
			else {
				screen[i * (row / 10) + j]->close[5] = nullptr;
				screen[i * (row / 10) + j]->close[6] = nullptr;
				screen[i * (row / 10) + j]->close[7] = nullptr;
			}
			if (i != 0)
				screen[i * (row / 10) + j]->close[3] = screen[(i - 1) * (row / 10) + j];
			else {
				screen[i * (row / 10) + j]->close[0] = nullptr;
				screen[i * (row / 10) + j]->close[3] = nullptr;
				screen[i * (row / 10) + j]->close[5] = nullptr;
			}
			if (i != (column / 10) - 1)
				screen[i * (row / 10) + j]->close[4] = screen[(i + 1) * (row / 10) + j];
			else {
				screen[i * (row / 10) + j]->close[2] = nullptr;
				screen[i * (row / 10) + j]->close[4] = nullptr;
				screen[i * (row / 10) + j]->close[7] = nullptr;
			}
		}
	}
	screen[2000]->Born();
	screen[2001]->Born();
	screen[2002]->Born();
	//screen[10 * row / 10]->Born();
	//screen[10 * row / 10 + 20]->Born();

	int iteration = 0;
	window.clear();
	for (int i = 0; i < column / 10; i++) {
		for (int j = 0; j < row / 10; j++) {
			//number.setString(std::to_string(i * (column / 10) + j));
			window.draw(screen[i * (row / 10) + j]->square);
		}
	}
	number.setString(std::to_string(iteration));
	window.draw(number);
	window.display();

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
		
		sleep(milliseconds(1000));
		iteration++;

		for (int i = 0; i < column / 10; i++) {
			for (int j = 0; j < row / 10; j++) {
				int closeAlive = 0;
				for (int k = 0; k < 8; k++) {
					if (screen[i * (row / 10) + j]->close[k] != nullptr && screen[i * (row / 10) + j]->close[k]->isAlive == true)
						closeAlive++;
				}
				if (screen[i * (row / 10) + j]->isAlive == false && closeAlive == 3) {
					screen[i * (row / 10) + j]->nextStatus = true;
				}
				else if (screen[i * (row / 10) + j]->isAlive == true && (closeAlive < 2 || closeAlive > 3)) {
					screen[i * (row / 10) + j]->nextStatus = false;
				}
				else if (screen[i * (row / 10) + j]->isAlive == true && (closeAlive > 2 || closeAlive < 3)) {
					screen[i * (row / 10) + j]->nextStatus = true;
				}
			}
		}

		window.clear();
		for (int i = 0; i < column / 10; i++) {
			for (int j = 0; j < row / 10; j++) {
				if (screen[i * (row / 10) + j]->nextStatus != screen[i * (row / 10) + j]->isAlive) {
					if (screen[i * (row / 10) + j]->nextStatus == false)
						screen[i * (row / 10) + j]->Dies();
					else
						screen[i * (row / 10) + j]->Born();
				}
				window.draw(screen[i * (row / 10) + j]->square);
			}
		}
		number.setString(std::to_string(iteration));
		window.draw(number);
		window.display();
		deltaTime = deltaClock.restart().asSeconds();
	}

	return 0;
}