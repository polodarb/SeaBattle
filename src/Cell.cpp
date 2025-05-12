#include "Cell.h"

namespace SeaBattle {
    Cell::Cell(float x, float y, float size)
        : x(x), y(y), size(size), state(0) {
    }

    float Cell::getX() const { return x; }
    float Cell::getY() const { return y; }
    float Cell::getSize() const { return size; }
    int Cell::getState() const { return state; }
    void Cell::setState(int s) { state = s; }

    bool Cell::contains(float px, float py) const {
        // перевіряємо, чи потрапляє точка в межі клітинки
        return (px >= x && px <= x + size && py >= y && py <= y + size);
    }
}
