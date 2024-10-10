#include <SFML/Graphics.hpp>
// #include <iostream>

#include "grid.hpp"
#include "randoms.hpp"
#include "utils.hpp"

using namespace std;
using namespace sf;

int main() {
  VideoMode desktop = VideoMode::getDesktopMode();
  RenderWindow window(VideoMode(desktop.width, desktop.height), "Grid of Points");
  COLORREF customColor = RGB(0, 0, 0);
  setTitleBarColor(window, customColor);
  centerWindow(window);

  constexpr int ROWS = 100;
  constexpr int COLS = 100;
  Grid gridOfPoints(ROWS, COLS, 3, 2, getRandBool);

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