#ifndef CELL_H
#define CELL_H

namespace SeaBattle {
    class Cell {
        float x, y, size;
        int state;

    public:
        Cell(float x, float y, float size);
        float getX() const;
        float getY() const;
        float getSize() const;
        int getState() const;
        void setState(int s);
        bool contains(float px, float py) const;
    };
}

#endif
