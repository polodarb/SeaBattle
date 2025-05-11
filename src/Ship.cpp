#include "../include/Ship.h"

namespace SeaBattle {
    Ship::Ship(int size, float startX, float startY, float cellSize, bool horizontal)
        : isHorizontal(horizontal), isDestroyed(false) {
        for (int i = 0; i < size; i++) {
            float x = startX + (horizontal ? i * cellSize : 0);
            float y = startY + (horizontal ? 0 : i * cellSize);
            cells.push_back(new Cell(x, y, cellSize));
            cells.back()->setState(1); // Mark as ship
        }
    }

    Ship::~Ship() {
        for (auto cell: cells) {
            delete cell;
        }
    }

    bool Ship::isHit() const {
        for (const auto &cell: cells) {
            if (cell->getState() == 2) {
                // If any cell is hit
                return true;
            }
        }
        return false;
    }

    bool Ship::contains(float x, float y) const {
        for (const auto &cell: cells) {
            if (cell->contains(x, y)) {
                return true;
            }
        }
        return false;
    }
} // namespace SeaBattle
