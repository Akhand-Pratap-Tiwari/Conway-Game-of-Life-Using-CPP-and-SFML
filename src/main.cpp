#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
using namespace std;
using namespace sf;

void drawGrid(vector<vector<CircleShape>>& gridPoints, RenderWindow& window) {
  for (const auto& pointRow : gridPoints) {
    for (const auto& point : pointRow)
      window.draw(point);
  }
}

vector<vector<CircleShape>> getGridBySpacing(
    int rows = 3,
    int cols = 3,
    float inBetweenSpacing = 30.0f,
    float pointRadius = 10.0f,
    float posX = 0.0,
    float posY = 0.0) {
  vector<vector<CircleShape>> gridPoints(rows, vector<CircleShape>(cols));

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      CircleShape point(pointRadius);
      point.setFillColor(Color::White);
      float xCoord = posX + (pointRadius + inBetweenSpacing) * c;
      float yCoord = posY + (pointRadius + inBetweenSpacing) * r;
      point.setPosition(xCoord, yCoord);
      gridPoints[r][c] = point;
    }
  }
  return gridPoints;
}

bool isOut(int rows, int cols, int r, int c) {
  if (r >= 0 && r <= rows - 1 && c >= 0 && c <= cols - 1) return false;
  return true;
}

int getNumNeighbours(vector<vector<bool>>& isAlive, int rows, int cols, int r, int c) {
  vector<vector<int>> delta = {
      {1, 0},
      {0, 1},
      {1, 1},
      {-1, 0},
      {0, -1},
      {1, -1},
      {-1, 1},
      {-1, -1}};
  int count = 0;
  for (auto d : delta) {
    int dr = r + d[0];
    int dc = c + d[1];
    if (!isOut(rows, cols, dr, dc) && isAlive[dr][dc]) {
      count++;
    }
  }
  return count;
}

void updateIsAlive(vector<vector<bool>>& isAlive) {
  int rows = isAlive.size();
  int cols = isAlive[0].size();
  vector<vector<bool>> tempIsAlive(rows, vector<bool>(cols));

  for (int r = 0; r <= rows - 1; ++r) {
    for (int c = 0; c <= cols - 1; ++c) {
      int nbrs = getNumNeighbours(isAlive, rows, cols, r, c);
      if (isAlive[r][c]) {
        if (nbrs < 2 || nbrs > 3)
          tempIsAlive[r][c] = false;
        else if (nbrs == 2 || nbrs == 3)
          tempIsAlive[r][c] = true;
      } else {
        if (nbrs == 3) tempIsAlive[r][c] = true;
      }
    }
  }
  isAlive = tempIsAlive;
}

void updateGrid(vector<vector<CircleShape>>& gridPoints, vector<vector<bool>>& isAlive) {
  int rows = isAlive.size();
  int cols = isAlive[0].size();

  for (int r = 0; r <= rows - 1; ++r) {
    for (int c = 0; c <= cols - 1; ++c) {
      if (isAlive[r][c])
        gridPoints[r][c].setFillColor(Color::White);
      else
        gridPoints[r][c].setFillColor(Color::Black);
    }
  }
}

int main() {
  RenderWindow window(VideoMode(800, 600), "Grid of Points");
  int rows = 75;
  int cols = 125;
  vector<vector<bool>> isAlive(rows, vector<bool>(cols));
  vector<vector<CircleShape>> gridPoints = getGridBySpacing(rows, cols, 10, 2, 0, 0);

  random_device randomDevice;
  mt19937 gen(randomDevice());
  bernoulli_distribution distro(0.5);

  for (int r = 0; r <= rows - 1; ++r) {
    for (int c = 0; c <= cols - 1; ++c) {
      isAlive[r][c] = distro(gen);
    }
  }
  // int r = 0;
  // float r = 0;
  // CircleShape c(r);
  while (window.isOpen()) {
    // c.setRadius(r);
    // r += 0.5;
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
    drawGrid(gridPoints, window);
    // cout << isAlive[0][0];
    updateIsAlive(isAlive);
    updateGrid(gridPoints, isAlive);
    // sleep(seconds(1));

    // window.draw(c);
    // VertexArray line(Lines, 2);
    // line[0].position = Vector2f(0, 90);
    // line[0].color = Color::White;
    // line[1].position = Vector2f(100, 90);
    // line[1].color = Color::White;
    // window.draw(line);
    window.display();
  }

  return 0;
}
