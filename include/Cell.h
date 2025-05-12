#ifndef SEABATTLE_CELL_H
#define SEABATTLE_CELL_H

namespace SeaBattle {
    class Cell {
        float x, y, size; // координати і розмір клітинки
        int state; // 0 — пусто, 1 — корабель, 2 — влучив, 3 — мимо
    public:
        Cell(float x, float y, float size);

        float getX() const;
        float getY() const;
        float getSize() const;

        int getState() const;
        void setState(int s);

        // чи точка з координатами (px, py) знаходиться в цій клітинці
        bool contains(float px, float py) const;
    };
}

#endif
