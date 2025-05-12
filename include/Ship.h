#ifndef SEABATTLE_SHIP_H
#define SEABATTLE_SHIP_H

#include <vector>
#include "Cell.h"

namespace SeaBattle {
    class Ship {
    private:
        std::vector<Cell*> cells; // список клітинок, на яких розташований корабель
        bool isHorizontal;        // чи розташований корабель горизонтально
        bool isDestroyed;         // прапорець для збереження стану знищення (може бути зайвим)

    public:
        // Створює корабель заданого розміру в координатах (x, y) на дошці
        Ship(int size, int startX, int startY, float cellSize, bool horizontal,
             const std::vector<std::vector<Cell*>>& boardCells);
        ~Ship();

        // Перевіряє, чи хоча б одна частина корабля була влучена
        bool isHit() const;

        // Перевіряє, чи всі частини корабля були влучені (корабель знищено)
        bool isSunk() const;

        // Перевіряє, чи точка (px, py) знаходиться в межах цього корабля
        bool contains(float px, float py) const;

        // Малює корабель (у вигляді прямокутних паралелепіпедів або пірамід)
        void draw(float boardThickness, float shipHeight, float cellSize) const;

        // Повертає напрям корабля (true якщо горизонтально)
        bool isHorizontalDir() const { return isHorizontal; }

        // Повертає список клітинок корабля
        const std::vector<Cell*>& getCells() const { return cells; }
    };
}

#endif // SEABATTLE_SHIP_H
