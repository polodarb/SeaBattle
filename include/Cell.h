#ifndef SEABATTLE_CELL_H
#define SEABATTLE_CELL_H

#include "Shape.h"

namespace SeaBattle {

class Cell : public Shape {
private:
    int state; // 0: empty, 1: ship, 2: hit, 3: miss

public:
    Cell(float x, float y, float size);
    
    void draw() override;
    void setState(int newState);
    int getState() const { return state; }
};

} // namespace SeaBattle

#endif // SEABATTLE_CELL_H 