#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace sf;

namespace globalConfigs {
random_device randomDevice;
mt19937 gen(randomDevice());
bernoulli_distribution distro(0.5);
}  // namespace globalConfigs

class Grid {
  typedef vector<CircleShape> V1D_CIRCLE_SHAPE;
  typedef vector<V1D_CIRCLE_SHAPE> V2D_CIRCLE_SHAPE;

 private:
  Grid() {}
  V2D_CIRCLE_SHAPE gridPoints;
  V2D_CIRCLE_SHAPE tempGridPoints;
  vector<vector<int>> delta = {
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
  int rows;
  int cols;
  float circleRadius;
  float inBetweenSpacing;
  float posX = 0.0;
  float posY = 0.0;

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
  return globalConfigs::distro(globalConfigs::gen);
}

int main() {
  RenderWindow window(VideoMode(800, 600), "Grid of Points");
  constexpr int ROWS = 80;
  constexpr int COLS = 150;
  Grid gridOfPoints(ROWS, COLS, 1, 5.0, getRandBool);

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case Event::Closed: {
          window.close();
          break;
        }
        case Event::Resized: {
          // window.clear();
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