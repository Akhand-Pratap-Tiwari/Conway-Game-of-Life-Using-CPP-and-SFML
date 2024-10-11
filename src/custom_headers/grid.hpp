// Grid.hpp
#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

typedef std::vector<sf::CircleShape> V1D_CIRCLE_SHAPE;
typedef std::vector<V1D_CIRCLE_SHAPE> V2D_CIRCLE_SHAPE;

class Grid {
 private:
  V2D_CIRCLE_SHAPE gridPoints;
  V2D_CIRCLE_SHAPE tempGridPoints;
  int rows;
  int cols;
  float circleRadius;
  float inBetweenSpacing;
  float posX;
  float posY;
  static int delta[8][2];

  Grid();
  bool isOut(int r, int c);
  int getAliveNbrs(int r, int c);

 public:
  Grid(
      int rows,
      int cols,
      float circleRadius,
      float inBetweenSpacing,
      bool (*randBoolGen)(int, int),
      float posX = 0.0,
      float posY = 0.0);
  void update();
  void draw(sf::RenderWindow& window);
};

#endif  // GRID_HPP
