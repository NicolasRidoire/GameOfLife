#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <functional>

#include "draw.h"
#include "Pixel.h"
#include "thread.h"
using namespace sf;

const int column = 800;
const int row = 600;
const float coeff = column / row;
const float pixSize = 10.f;
Pixel* screen[(column / 10) * (row / 10)];
Mutex mutex;

int WinMain() {
	// Initialisation
	RenderWindow window(VideoMode(column, row), "OK");
	Font font;
	font.loadFromFile("C:/Windows/Fonts/ariblk.ttf");
	Text number;
	number.setFont(font);
	number.setFillColor(Color::Red);
	bool oneTwo = false;
	bool changeLine = false;
	for (int i = 0; i < column / 10; i++) {
		if (changeLine == true) {
			oneTwo = true;
			changeLine = false;
		}
		else {
			changeLine = true;
			oneTwo = false;
		}
		for (int j = 0; j < row / 10; j++) {
			screen[i * (row / 10) + j] = new Pixel(coeff, i, j, pixSize);
			if (oneTwo == true) {
				screen[i * (row / 10) + j]->Born();
				oneTwo = false;
			}
			else
				oneTwo = true;
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

	int iteration = 0;
	window.clear();
	for (int i = 0; i < column / 10; i++) {
		for (int j = 0; j < row / 10; j++) {
			window.draw(screen[i * (row / 10) + j]->square);
		}
	}
	number.setString(std::to_string(iteration));
	window.draw(number);
	window.display();

	bool pause = false;

	while (window.isOpen()) {
		// Game loop
		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape) {
					if (pause == true)
						pause = false;
					else
						pause = true;
				}
				break;
			}
		}
		sleep(milliseconds(250));
		if (!pause) {
			iteration++;
			int closeAlive = 0;
			for (int i = 0; i < column / 10; i++) {
				for (int j = 0; j < row / 10; j++) {
					closeAlive = 0;
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
			for (int i = 0; i < column / 10; i++) {
				for (int j = 0; j < row / 10; j++) {
					if (screen[i * (row / 10) + j]->nextStatus != screen[i * (row / 10) + j]->isAlive) {
						if (screen[i * (row / 10) + j]->nextStatus == false)
							screen[i * (row / 10) + j]->Dies();
						else
							screen[i * (row / 10) + j]->Born();
					}
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
		if (pause) {
			RectangleShape lRect({ 20.f, 60.f });
			RectangleShape rRect({ 20.f, 60.f });
			lRect.setFillColor(Color::Blue);
			rRect.setFillColor(Color::Blue);
			lRect.move({ column / 2 - 35, row / 2 - 40 });
			rRect.move({ column / 2 - 5, row / 2 - 40 });
			window.draw(lRect);
			window.draw(rRect);
		}
		number.setString(std::to_string(iteration));
		window.draw(number);
		window.display();
	}

	return 0;
}