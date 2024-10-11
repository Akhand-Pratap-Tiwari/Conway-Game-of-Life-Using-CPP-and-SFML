#include <SFML/Graphics.hpp>
// #include <iostream>
/*
  TODO: Add readme
  Add init state from image
  Add init state from mouse clicks
*/
#include "./custom_headers/grid.hpp"
#include "./custom_headers/randoms.hpp"
#include "./custom_headers/utils.hpp"

using namespace std;
using namespace sf;

constexpr int ROWS = 160;
constexpr int COLS = 315;

bool getRandBoolCustom(int r, int c) {
  return (r == c - COLS/4 || r == -c + COLS/2 + COLS/4);
  // return r*r + c*c < 50;
}

int main() {
  VideoMode desktop = VideoMode::getDesktopMode();
  RenderWindow window(VideoMode(desktop.width, desktop.height), "Grid of Points");
  COLORREF customColor = RGB(0, 0, 0);
  setTitleBarColor(window, customColor);
  centerWindow(window);

  constexpr int ROWS = 160;
  constexpr int COLS = 315;
  Grid gridOfPoints(ROWS, COLS, 2, 2, getRandBoolCustom);

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case Event::Closed: {
          window.close();
          break;
        }
        case Event::Resized: {
          FloatRect view(0, 0, event.size.width, event.size.height);
          window.setView(View(view));
          break;
        }
        case Event::KeyPressed: {
          if (event.key.code == Keyboard::Escape) window.close();
        }
        default:
          break;
      }
    }

    window.clear();
    gridOfPoints.draw(window);
    gridOfPoints.update();
    window.display();
  }

  return 0;
}