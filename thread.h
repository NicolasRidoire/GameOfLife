#pragma once
#include <SFML/Graphics.hpp>

#include "Pixel.h"

using namespace sf;

void gameLogic(int* iteration, const int* column, const int* row, Pixel** screen, Text* number, RenderWindow* window, Mutex* mutex);