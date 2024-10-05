#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#ifdef _WIN32
#include <dwmapi.h>  // For DwmSetWindowAttribute
#include <windows.h>
#pragma comment(lib, "dwmapi.lib")  // Link dwmapi library
#endif

using namespace std;
using namespace sf;

namespace globalConfigsForRandoms {
random_device randomDevice;
mt19937 gen(randomDevice());
bernoulli_distribution distro(0.5);
}  // namespace globalConfigsForRandoms

class Grid {
  typedef vector<CircleShape> V1D_CIRCLE_SHAPE;
  typedef vector<V1D_CIRCLE_SHAPE> V2D_CIRCLE_SHAPE;

 private:
  Grid() {}
  V2D_CIRCLE_SHAPE gridPoints;
  V2D_CIRCLE_SHAPE tempGridPoints;
  int rows;
  int cols;
  float circleRadius;
  float inBetweenSpacing;
  float posX = 0.0;
  float posY = 0.0;
  int delta[8][2] = {
      {1, 0},
      {0, 1},
      {1, 1},
      {-1, 0},
      {0, -1},
      {1, -1},
      {-1, 1},
      {-1, -1},
  };

  bool isOut(int r, int c);
  int getAliveNbrs(int r, int c);

 public:
  Grid(
      int rows,
      int cols,
      float circleRadius,
      float inBetweenSpacing,
      bool (*randBoolGen)(),
      float posX = 0.0,
      float posY = 0.0);
  void update();
  void draw(RenderWindow& window);
};

bool getRandBool() {
  return globalConfigsForRandoms::distro(globalConfigsForRandoms::gen);
}

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

int main() {
  VideoMode desktop = VideoMode::getDesktopMode();
  RenderWindow window(VideoMode(desktop.width, desktop.height), "Grid of Points");
  COLORREF customColor = RGB(0, 0, 0);
  setTitleBarColor(window, customColor);
  centerWindow(window);

  constexpr int ROWS = 175;
  constexpr int COLS = 375;
  Grid gridOfPoints(ROWS, COLS, 2, 3, getRandBool);

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

bool Grid::isOut(int r, int c) {
  if (r <= 0 || r >= rows) {
    return true;
  }
  if (c <= 0 || c >= cols) {
    return true;
  }
  return false;
}

int Grid::getAliveNbrs(int r, int c) {
  int count = 0;
  for (auto d : delta) {
    int dr = r + d[0];
    int dc = c + d[1];
    if (!isOut(dr, dc) && gridPoints[dr][dc].getFillColor() == Color::White) {
      count++;
    }
  }
  return count;
}

Grid::Grid(
    int rows,
    int cols,
    float circleRadius,
    float inBetweenSpacing,
    bool (*randBoolGen)(),
    float posX,
    float posY) {
  this->rows = rows;
  this->cols = cols;
  this->posX = posX;
  this->posY = posY;
  this->circleRadius = circleRadius;
  this->inBetweenSpacing = inBetweenSpacing;

  gridPoints = V2D_CIRCLE_SHAPE(rows, V1D_CIRCLE_SHAPE(cols));
  for (int r = 0; r <= rows - 1; ++r) {
    for (int c = 0; c <= cols - 1; ++c) {
      CircleShape point(circleRadius);
      if (randBoolGen())
        point.setFillColor(Color::White);
      else
        point.setFillColor(Color::Black);
      float xCoord = posX + (circleRadius + inBetweenSpacing) * c;
      float yCoord = posY + (circleRadius + inBetweenSpacing) * r;
      point.setPosition(xCoord, yCoord);
      gridPoints[r][c] = point;
    }
  }
  tempGridPoints = gridPoints;
}

void Grid::update() {
  for (int r = 0; r <= rows - 1; ++r) {
    for (int c = 0; c <= cols - 1; ++c) {
      int nbrs = getAliveNbrs(r, c);
      if (gridPoints[r][c].getFillColor() == Color::White) {
        if (nbrs < 2 || nbrs > 3)
          tempGridPoints[r][c].setFillColor(Color::Black);
      } else {
        if (nbrs == 3) tempGridPoints[r][c].setFillColor(Color::White);
      }
    }
  }
  gridPoints = tempGridPoints;
}

void Grid::draw(RenderWindow& window) {
  for (const auto& pointRow : gridPoints)
    for (const auto& point : pointRow)
      window.draw(point);
}