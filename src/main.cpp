#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
// #include <iostream>
/*
  TODO: Add readme
  Add init state from image
  Add init state from mouse clicks
*/
#include <windows.h>
#pragma comment(lib, "user32.lib")
#include "./custom_headers/grid.hpp"
#include "./custom_headers/randoms.hpp"
#include "./custom_headers/utils.hpp"

using namespace std;
using namespace sf;

constexpr int ROWS = 200;
constexpr int COLS = 200;

bool getRandBoolCustom(int r, int c) {
  return (r == c || r + c == COLS - 1);
  // return r*r + c*c < 50;
}

void drawDebugLines(RenderWindow& window) {
  sf::Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

  // Define the horizontal line
  sf::RectangleShape horizontalLine(sf::Vector2f(window.getSize().x, 2));  // Line width 2px
  horizontalLine.setFillColor(sf::Color::White);
  horizontalLine.setPosition(0, center.y);  // Set Y to the center

  // Define the vertical line
  sf::RectangleShape verticalLine(sf::Vector2f(window.getSize().y, 2));  // Line width 2px
  verticalLine.setFillColor(sf::Color::White);
  verticalLine.setRotation(90);           // Rotate to make it vertical
  verticalLine.setPosition(center.x, 0);  // Set X to the center
  window.draw(horizontalLine);
  window.draw(verticalLine);
}

int main() {
  VideoMode desktop = VideoMode::getDesktopMode();
  RenderWindow window(desktop, "Conway's Game of Life");
  COLORREF customColor = RGB(0, 0, 0);
  setTitleBarColor(window, customColor);
  // centerWindow(window);
  HWND hwnd = window.getSystemHandle();
  ShowWindow(hwnd, SW_MAXIMIZE);
  window.clear();
  window.display();

  const float WINDOW_WIDTH = window.getSize().x;
  const float WINDOW_HEIGHT = window.getSize().y;
  
  constexpr float RADIUS = 1;
  constexpr float SPACING = 2;
  
  const float POSX = WINDOW_WIDTH / 2 - 2 * COLS - 1;
  const float POSY = WINDOW_HEIGHT / 2 - 2 * ROWS - 1;

  
  Grid gridOfPoints(ROWS, COLS, RADIUS, SPACING, getRandBoolCustom, POSX, POSY);
  gridOfPoints.draw(window);

  // window.display();
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

    // gridOfPoints.update();
    drawDebugLines(window);
    gridOfPoints.draw(window);
    window.display();
    window.clear();
  }

  return 0;
}