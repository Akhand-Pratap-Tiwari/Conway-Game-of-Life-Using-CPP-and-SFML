// Utils.hpp
#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>

#ifdef _WIN32
#include <dwmapi.h>
#include <windows.h>
#pragma comment(lib, "dwmapi.lib")
#endif

void setTitleBarColor(sf::RenderWindow& window, COLORREF color);
void centerWindow(sf::RenderWindow& window);

#endif  // UTILS_HPP
