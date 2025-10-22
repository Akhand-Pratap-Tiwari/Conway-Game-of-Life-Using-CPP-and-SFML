// Utils.cpp
#include "utils.hpp"

void setTitleBarColor(sf::RenderWindow& window, COLORREF color) {
#ifdef _WIN32
  HWND hwnd = window.getSystemHandle();
  const DWORD DWMWA_CAPTION_COLOR = 35;
  DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &color, sizeof(color));
#endif
}

void centerWindow(sf::RenderWindow& window) {
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  sf::Vector2u windowSize = window.getSize();
  int posX = (desktop.width - windowSize.x) / 2;
  int posY = (desktop.height - windowSize.y) / 2;
  window.setPosition(sf::Vector2i(posX, posY));
}
