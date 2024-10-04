#ifndef GRID_H
#define GRID_H
#include <SFML/Graphics.hpp>

class Grid {
public:
    Grid(const sf::Shape& name, int age);

    void printInfo() const;
    void setName(const std::string& name);
    void setAge(int age);
    
    std::string getName() const;
    int getAge() const;

private:
    std::string name;
    int age;
};

// Additional function declarations
int add(int a, int b);
double multiply(double x, double y);
void printMessage(const std::string& message);

#endif // GRID_H
