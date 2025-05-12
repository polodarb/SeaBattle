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
        // Construct a ship of given size at board grid position (x,y)
        Ship(int size, int startX, int startY, float cellSize, bool horizontal,
             const std::vector<std::vector<Cell*>>& boardCells);
        ~Ship();

        bool isHit() const;
        bool isSunk() const;
        bool contains(float px, float py) const;
        void draw(float boardThickness, float shipHeight, float cellSize) const;

        bool isHorizontalDir() const { return isHorizontal; }
        const std::vector<Cell*>& getCells() const { return cells; }
    };
}

#endif // SEABATTLE_SHIP_H