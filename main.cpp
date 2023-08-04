#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <functional>

#include "Pixel.h"
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
	Vector2 pauseSize = { 10.f, 30.f };
	RectangleShape pauseBar1(pauseSize);
	RectangleShape pauseBar2(pauseSize);
	RectangleShape pauseOutline({ 40.f, 40.f });
	pauseBar1.setPosition({column - 40.f, 10.f});
	pauseBar2.setPosition({ column - 20.f, 10.f });
	pauseOutline.setPosition({ column - 45.f, 5.f });
	pauseBar1.setFillColor(Color::Blue);
	pauseBar2.setFillColor(Color::Blue);
	pauseOutline.setFillColor(Color::White);
	pauseOutline.setOutlineColor(Color::Blue);
	pauseOutline.setOutlineThickness(3.f);
	RectangleShape lRect({ 20.f, 60.f });
	RectangleShape rRect({ 20.f, 60.f });
	RectangleShape outRect({ 70.f, 80.f });
	lRect.setFillColor(Color::Blue);
	rRect.setFillColor(Color::Blue);
	outRect.setFillColor(Color::White);
	outRect.setOutlineColor(Color::Blue);
	outRect.setOutlineThickness(5.f);
	lRect.setPosition({ column / 2 - 35, row / 2 - 40 });
	rRect.setPosition({ column / 2 - 5, row / 2 - 40 });
	outRect.setPosition({ column / 2 - 45, row / 2 - 50 });
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
	window.draw(pauseBar1);
	window.draw(pauseBar2);
	window.draw(pauseOutline);
	number.setString(std::to_string(iteration));
	window.draw(number);
	window.display();

	bool pause = false;
	Clock currentTime;
	currentTime.restart();

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
			case Event::MouseMoved:
				if (event.mouseMove.x > column - 48.f && event.mouseMove.x < column - 2.f && event.mouseMove.y > 2.f && event.mouseMove.y < 48.f) {
					pauseBar1.setFillColor(Color::Red);
					pauseBar2.setFillColor(Color::Red);
					pauseOutline.setOutlineColor(Color::Red);
				}
				else {
					pauseBar1.setFillColor(Color::Blue);
					pauseBar2.setFillColor(Color::Blue);
					pauseOutline.setOutlineColor(Color::Blue);
				}
				if (pause) {
					if (event.mouseMove.x > column / 2 - 50 && event.mouseMove.x < column / 2 + 30 && event.mouseMove.y > row / 2 - 55 && event.mouseMove.y < row / 2 + 35) {
						lRect.setFillColor(Color::Red);
						rRect.setFillColor(Color::Red);
						outRect.setOutlineColor(Color::Red);
					}
					else {
						lRect.setFillColor(Color::Blue);
						rRect.setFillColor(Color::Blue);
						outRect.setOutlineColor(Color::Blue);
					}
				}
				break;
			case Event::MouseButtonReleased:
				if (event.mouseButton.x > column - 48.f && event.mouseButton.x < column - 2.f && event.mouseButton.y > 2.f && event.mouseButton.y < 48.f) {
					pause = true;
				}
				else if (event.mouseButton.x > column / 2 - 50 && event.mouseButton.x < column / 2 + 30 && event.mouseButton.y > row / 2 - 55 && event.mouseButton.y < row / 2 + 35) {
					pause = false;
				}
			}
		}
		if (currentTime.getElapsedTime().asMilliseconds() > 500) {
			currentTime.restart();
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
		}
		window.clear();
		for (int i = 0; i < column / 10; i++) {
			for (int j = 0; j < row / 10; j++) {
				window.draw(screen[i * (row / 10) + j]->square);
			}
		}
		if (pause) {
			window.draw(outRect);
			window.draw(lRect);
			window.draw(rRect);
		}
		else {
			window.draw(pauseOutline);
			window.draw(pauseBar1);
			window.draw(pauseBar2);
		}
		number.setString(std::to_string(iteration));
		window.draw(number);
		window.display();
	}

	return 0;
}