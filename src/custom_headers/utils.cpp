// Utils.cpp
#include "utils.hpp"

void setTitleBarColor(RenderWindow& window, COLORREF color) {
#ifdef _WIN32
  HWND hwnd = window.getSystemHandle();
  const DWORD DWMWA_CAPTION_COLOR = 35;
  DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &color, sizeof(color));
#endif
}

void centerWindow(RenderWindow& window) {
  VideoMode desktop = VideoMode::getDesktopMode();
  Vector2u windowSize = window.getSize();
  int posX = (desktop.width - windowSize.x) / 2;
  int posY = (desktop.height - windowSize.y) / 2;
  window.setPosition(Vector2i(posX, posY));
}
