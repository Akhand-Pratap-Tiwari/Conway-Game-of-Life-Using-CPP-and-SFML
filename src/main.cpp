#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int main() {
  RenderWindow window(VideoMode(800, 600), "Grid of Points");

  const int rows = 50;
  const int cols = 50;
  const float spacing = 20.0f;
  const float pointRadius = 2.0f;
  float lenX = (cols - 1) * (spacing) + (2 * pointRadius);
  ;
  float lenY = (rows - 1) * (spacing) + (2 * pointRadius);
  vector<CircleShape> gridPoints;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      CircleShape point(pointRadius);
      point.setFillColor(Color::White);
      float x = j * spacing;
      float y = i * spacing;
      point.setPosition(x - pointRadius + window.getSize().x / 2 - lenX / 2, y - pointRadius + window.getSize().y / 2 - lenY / 2);
      gridPoints.push_back(point);
    }
  }

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
        default:
          break;
      }
    }

    window.clear();

    for (const auto& point : gridPoints) {
      window.draw(point);
    }
    // window.draw(line);
    window.display();
  }

  return 0;
}
