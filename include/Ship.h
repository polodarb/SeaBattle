#ifndef SEABATTLE_SHIP_H
#define SEABATTLE_SHIP_H

#include <vector>
#include "Cell.h"

namespace SeaBattle {

class Ship {
private:
    std::vector<Cell*> cells;
    bool isHorizontal;
    bool isDestroyed;

public:
    Ship(int size, float startX, float startY, float cellSize, bool horizontal);
    ~Ship();

    bool isHit() const;
    bool isSunk() const { return isDestroyed; }
    const std::vector<Cell*>& getCells() const { return cells; }
    bool contains(float x, float y) const;
};

} // namespace SeaBattle

#endif // SEABATTLE_SHIP_H 