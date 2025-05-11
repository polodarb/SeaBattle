#include "../include/Shape.h"

namespace SeaBattle {

Shape::Shape(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height), r(1.0f), g(1.0f), b(1.0f) {
}

void Shape::setColor(float r, float g, float b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

void Shape::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

void Shape::setSize(float width, float height) {
    this->width = width;
    this->height = height;
}

bool Shape::contains(float px, float py) const {
    return px >= x && px <= x + width && py >= y && py <= y + height;
}

} // namespace SeaBattle
