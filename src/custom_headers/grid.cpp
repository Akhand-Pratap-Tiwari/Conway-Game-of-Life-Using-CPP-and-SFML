// Grid.cpp
#include "grid.hpp"

int Grid::delta[8][2] = {
    {1, 0},
    {0, 1},
    {1, 1},
    {-1, 0},
    {0, -1},
    {1, -1},
    {-1, 1},
    {-1, -1}};

Grid::Grid(
    int rows,
    int cols,
    float circleRadius,
    float inBetweenSpacing,
    bool (*randBoolGen)(int, int),
    float posX,
    float posY) : rows(rows),
                  cols(cols),
                  circleRadius(circleRadius),
                  inBetweenSpacing(inBetweenSpacing),
                  posX(posX),
                  posY(posY) {
  gridPoints = V2D_CIRCLE_SHAPE(rows, V1D_CIRCLE_SHAPE(cols));
  float diameter = 2 * circleRadius;

  for (int r = 0; r <= rows - 1; ++r) {
    for (int c = 0; c <= cols - 1; ++c) {
      sf::CircleShape point(circleRadius);
      point.setFillColor(randBoolGen(r, c) ? sf::Color::White : sf::Color::Black);
      float xCoord = posX + (diameter + inBetweenSpacing) * c;
      float yCoord = posY + (diameter + inBetweenSpacing) * r;
      point.setPosition(xCoord, yCoord);
      gridPoints[r][c] = point;
    }
  }
  tempGridPoints = gridPoints;
}

bool Grid::isOut(int r, int c) {
  return (r < 0 || r >= rows || c < 0 || c >= cols);
}

int Grid::getAliveNbrs(int r, int c) {
  int count = 0;
  for (auto d : delta) {
    int dr = r + d[0];
    int dc = c + d[1];
    if (!isOut(dr, dc) && gridPoints[dr][dc].getFillColor() == sf::Color::White) {
      count++;
    }
  }
  return count;
}

void Grid::update() {
  for (int r = 0; r <= rows - 1; ++r) {
    for (int c = 0; c <= cols - 1; ++c) {
      int nbrs = getAliveNbrs(r, c);
      if (gridPoints[r][c].getFillColor() == sf::Color::White) {
        if (nbrs < 2 || nbrs > 3)
          tempGridPoints[r][c].setFillColor(sf::Color::Black);
      } else {
        if (nbrs == 3) tempGridPoints[r][c].setFillColor(sf::Color::White);
      }
    }
  }
  gridPoints = tempGridPoints;
}

void Grid::draw(sf::RenderWindow& window) {
  for (const auto& pointRow : gridPoints)
    for (const auto& point : pointRow)
      window.draw(point);
}

void Grid::reposition(float newPosX, float newPosY) {
    float dx = newPosX - posX;
    float dy = newPosY - posY;
    for(auto& pointRow : gridPoints) {
        for(auto& point : pointRow) {
            float px = point.getPosition().x;
            float py = point.getPosition().y;
            point.setPosition(px+dx, py+dy);
        }
    }
    posX = newPosX;
    posY = newPosY;
    dx = newPosX - tempGridPoints[0][0].getPosition().x;
    dy = newPosY - tempGridPoints[0][0].getPosition().y;
    for(auto& tempPointRow : tempGridPoints) {
        for(auto& tempPoint : tempPointRow) {
            float px = tempPoint.getPosition().x;
            float py = tempPoint.getPosition().y;
            tempPoint.setPosition(px+dx, py+dy);
        }
    }
}