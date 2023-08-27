#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <functional>

#include "Pixel.h"
#include "Button.h"
using namespace sf;

const int column = 1280;
const int row = 800;
const float pixSize = 10.f;
const float coeff = column / row;
Pixel* screen[(column / 10) * (row / 10)];
Mutex mutex;
int xTime = 1;

int WinMain() {
	// Initialisation
	bool menu = true;
	RenderWindow window(VideoMode(column, row), "OK", Style::Titlebar | Style::Close);
	Font font;
	font.loadFromFile("C:/Windows/Fonts/ariblk.ttf");
	Text number;
	number.setFont(font);
	number.setFillColor(Color::Red);
	Text speed;
	speed.setFillColor(Color::Magenta);
	speed.setFont(font);
	speed.setPosition({ 10.f, row - 70 });
	Vector2 pauseSize = { 10.f, 30.f };
	RectangleShape pauseBar1(pauseSize);
	RectangleShape pauseBar2(pauseSize);
	RectangleShape pauseOutline({ 40.f, 40.f });
	RectangleShape forwardOutline({ 27.5f, 22.5f});
	forwardOutline.setFillColor(Color::White);
	forwardOutline.setOutlineColor(Color::Magenta);
	forwardOutline.setOutlineThickness(3.f);
	forwardOutline.setPosition({ 42.5f, row - 32.5f });
	RectangleShape backwardOutline = forwardOutline;
	backwardOutline.setPosition({ 8.f, row - 32.5f });
	CircleShape for1Triangle(10, 3);
	for1Triangle.setPosition({ 60.f, row - 30 });
	for1Triangle.setRotation(90);
	for1Triangle.setFillColor(Color::Magenta);
	CircleShape for2Triangle = for1Triangle;
	for2Triangle.setPosition({ 67.5f, row - 30 });
	CircleShape back1Triangle = for1Triangle;
	back1Triangle.rotate(180);
	back1Triangle.setPosition({ 10.f, row - 10 });
	CircleShape back2Triangle = back1Triangle;
	back2Triangle.setPosition({ 17.5f, row - 10 });

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
	/*
	window.clear();
	for (int i = 0; i < column / 10; i++) {
		for (int j = 0; j < row / 10; j++) {
			window.draw(screen[i * (row / 10) + j]->square);
		}
	}
	window.draw(pauseBar1);
	window.draw(pauseBar2);
	window.draw(pauseOutline);
	window.draw(forwardOutline);
	window.draw(backwardOutline);
	window.draw(for1Triangle);
	window.draw(for2Triangle);
	window.draw(back1Triangle);
	window.draw(back2Triangle);
	number.setString(std::to_string(iteration));
	speed.setString("x" + std::to_string(xTime));
	window.draw(speed);
	window.draw(number);
	window.display();*/

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
				if (!menu) {
					if (event.key.code == Keyboard::Escape) {
						if (pause == true)
							pause = false;
						else
							pause = true;
					}
					else if (event.key.code == Keyboard::Left) {
						if (xTime > 1)
							xTime /= 2;
					}
					else if (event.key.code == Keyboard::Right) {
						if (xTime < 512)
							xTime *= 2;
					}
				}
				break;
			case Event::MouseButtonPressed:
				if (!menu) {
					if (pause) {
						if (event.mouseButton.x > column / 2 - 50 && event.mouseButton.x < column / 2 + 30 && event.mouseButton.y > row / 2 - 55 && event.mouseButton.y < row / 2 + 35) {
							lRect.setFillColor(Color::Red);
							rRect.setFillColor(Color::Red);
							outRect.setOutlineColor(Color::Red);
						}
					}
					else if (event.mouseButton.x > column - 48.f && event.mouseButton.x < column - 2.f && event.mouseButton.y > 2.f && event.mouseButton.y < 48.f) {
						pauseBar1.setFillColor(Color::Red);
						pauseBar2.setFillColor(Color::Red);
						pauseOutline.setOutlineColor(Color::Red);
					}
					else if (event.mouseButton.x > 4.f && event.mouseButton.x < 35.5f && event.mouseButton.y > row - 34.5f && event.mouseButton.y < row - 7.f) {
						backwardOutline.setOutlineColor(Color::Red);
						back1Triangle.setFillColor(Color::Red);
						back2Triangle.setFillColor(Color::Red);
					}
					else if (event.mouseButton.x > 38.5f && event.mouseButton.x < 70.f && event.mouseButton.y > row - 34.5f && event.mouseButton.y < row - 7.f) {
						forwardOutline.setOutlineColor(Color::Red);
						for1Triangle.setFillColor(Color::Red);
						for2Triangle.setFillColor(Color::Red);
					}
				}
				break;
			case Event::MouseButtonReleased:
				if (!menu) {
					if (event.mouseButton.x > column - 48.f && event.mouseButton.x < column - 2.f && event.mouseButton.y > 2.f && event.mouseButton.y < 48.f) {
						pause = true;
						pauseBar1.setFillColor(Color::Blue);
						pauseBar2.setFillColor(Color::Blue);
						pauseOutline.setOutlineColor(Color::Blue);
					}
					else if (event.mouseButton.x > column / 2 - 50 && event.mouseButton.x < column / 2 + 30 && event.mouseButton.y > row / 2 - 55 && event.mouseButton.y < row / 2 + 35) {
						pause = false;
						lRect.setFillColor(Color::Blue);
						rRect.setFillColor(Color::Blue);
						outRect.setOutlineColor(Color::Blue);
					}
					else if (event.mouseButton.x > 38.5f && event.mouseButton.x < 70.f && event.mouseButton.y > row - 34.5f && event.mouseButton.y < row - 7.f) {
						if (xTime < 512) {
							xTime *= 2;
						}
						forwardOutline.setOutlineColor(Color::Magenta);
						for1Triangle.setFillColor(Color::Magenta);
						for2Triangle.setFillColor(Color::Magenta);
					}

					else if (event.mouseButton.x > 4.f && event.mouseButton.x < 35.5f && event.mouseButton.y > row - 34.5f && event.mouseButton.y < row - 7.f) {
						if (xTime > 1) {
							xTime /= 2;
						}
						backwardOutline.setOutlineColor(Color::Magenta);
						back1Triangle.setFillColor(Color::Magenta);
						back2Triangle.setFillColor(Color::Magenta);
					}
					else {
						forwardOutline.setOutlineColor(Color::Magenta);
						for1Triangle.setFillColor(Color::Magenta);
						for2Triangle.setFillColor(Color::Magenta);
						backwardOutline.setOutlineColor(Color::Magenta);
						back1Triangle.setFillColor(Color::Magenta);
						back2Triangle.setFillColor(Color::Magenta);
						pauseBar1.setFillColor(Color::Blue);
						pauseBar2.setFillColor(Color::Blue);
						pauseOutline.setOutlineColor(Color::Blue);
						lRect.setFillColor(Color::Blue);
						rRect.setFillColor(Color::Blue);
						outRect.setOutlineColor(Color::Blue);
					}
				}
				else {
					menu = false;
				}
				break;
			}
		}
		if (!menu) {
			if (currentTime.getElapsedTime().asMilliseconds() > 500 / xTime) {
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
				window.draw(forwardOutline);
				window.draw(backwardOutline);
				window.draw(for1Triangle);
				window.draw(for2Triangle);
				window.draw(back1Triangle);
				window.draw(back2Triangle);
			}
			number.setString(std::to_string(iteration));
			window.draw(number);
			speed.setString("x" + std::to_string(xTime));
			window.draw(speed);
			window.display();
		}
		else {
			Text title;
			title.setFont(font);
			title.setFillColor(Color::Black);
			title.setCharacterSize(80);
			title.setString("Game of life");
			title.setPosition(column / 2 - 250, row / 3 - 180);
			RectangleShape playRect({ 400, 100 });
			playRect.setFillColor(Color::White);
			playRect.setOutlineColor(Color::Black);
			playRect.setOutlineThickness(3.f);
			playRect.setPosition({ column / 2 - 200, row / 2 - 150 });
			CircleShape playTriangle(40, 3);
			playTriangle.setPosition({ column / 2 + 40, row / 2 - 140 });
			playTriangle.setRotation(90);
			playTriangle.setFillColor(Color::Black);

			window.clear(Color::White);
			window.draw(playRect);
			window.draw(playTriangle);
			window.draw(title);
			window.display();
			sleep(milliseconds(500));
		}
	}

	return 0;
}