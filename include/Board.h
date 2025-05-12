#ifndef SEABATTLE_BOARD_H
#define SEABATTLE_BOARD_H

#include <vector>
#include "Cell.h"
#include "Ship.h"

namespace SeaBattle {
    class Board {
    private:
        static const int BOARD_SIZE = 10; // розмір поля
        std::vector<std::vector<Cell *> > cells; // 2д масив клітинок
        std::vector<Ship *> ships; // список кораблів
        float cellSize; // розмір однієї клітинки
        float offsetX, offsetY; //  зсув дошки в просторі

    public:
        // створює нову дошку з заданим зміщенням і розміром клітинки
        Board(float offsetX, float offsetY, float cellSize);

        // чистимо пам'ять
        ~Board();

        // малює дошку
        void draw(bool isPlayerBoard);

        // постріл
        bool makeShot(float x, float y);

        bool isGameOver() const;

        // генерація кораблів
        void generateShips();

        // перевіряє, чи можна поставити корабель щою не було дивних розміщень
        bool canPlaceShip(int x, int y, int size, bool horizontal) const;

        // ставить корабель на поле
        void placeShip(int x, int y, int size, bool horizontal);

        int getBoardSize() const { return BOARD_SIZE; }
        float getCellSize() const { return cellSize; }
        float getOffsetX() const { return offsetX; }
        float getOffsetY() const { return offsetY; }
        const std::vector<std::vector<Cell *> > &getCells() const { return cells; }
    };
}

#endif
