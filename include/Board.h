#ifndef SEABATTLE_BOARD_H
#define SEABATTLE_BOARD_H

#include <vector>
#include "Cell.h"
#include "Ship.h"

namespace SeaBattle {
    class Board {
    private:
        static const int BOARD_SIZE = 10;
        std::vector<std::vector<Cell *> > cells;
        std::vector<Ship *> ships;
        float cellSize;
        float offsetX, offsetY;

    public:
        Board(float offsetX, float offsetY, float cellSize);

        ~Board();

        void draw();

        bool makeShot(float x, float y);

        bool isGameOver() const;

        void generateShips();

        bool canPlaceShip(int x, int y, int size, bool horizontal) const;

        void placeShip(int x, int y, int size, bool horizontal);

        int getBoardSize() const { return BOARD_SIZE; }
        float getCellSize() const { return cellSize; }
        float getOffsetX() const { return offsetX; }
        float getOffsetY() const { return offsetY; }
    };
} // namespace SeaBattle

#endif // SEABATTLE_BOARD_H
