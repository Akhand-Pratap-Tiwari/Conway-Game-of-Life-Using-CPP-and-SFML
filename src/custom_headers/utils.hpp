// Utils.hpp
#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>

#ifdef _WIN32
#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

using namespace sf;

void setTitleBarColor(sf::RenderWindow& window, COLORREF color);
void centerWindow(sf::RenderWindow& window);

#endif  // UTILS_HPP
