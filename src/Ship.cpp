#include <GL/glut.h>
#include "../include/Ship.h"
#include "../include/Utils.h"

using namespace SeaBattle;

Ship::Ship(int size, int startX, int startY, float cellSize, bool horizontal,
           const std::vector<std::vector<Cell*>>& boardCells)
    : isHorizontal(horizontal), isDestroyed(false)
{
    // Collect pointers to the existing board cells for this ship’s positions
    for (int i = 0; i < size; ++i) {
        Cell* cell = horizontal ? boardCells[startY][startX + i]
                                : boardCells[startY + i][startX];
        cells.push_back(cell);
        // (Board::placeShip will set cell states to 1 after this)
    }
}

Ship::~Ship() {
    // Do NOT delete cells here (they belong to the Board)
    cells.clear();
}

bool Ship::isHit() const {
    // Returns true if any segment of the ship has been hit
    for (const Cell* cell : cells) {
        if (cell->getState() == 2) { // state 2 = hit
            return true;
        }
    }
    return false;
}

bool Ship::isSunk() const {
    // Returns true if all segments of the ship have been hit
    for (const Cell* cell : cells) {
        if (cell->getState() != 2) { // any segment not hit yet
            return false;
        }
    }
    return true;
}

bool Ship::contains(float px, float py) const {
    // Check if the point (px, py) lies within any cell occupied by this ship
    for (const Cell* cell : cells) {
        if (cell->contains(px, py)) {
            return true;
        }
    }
    return false;
}

void Ship::draw(float boardThickness, float shipHeight, float cellSize) const {
    int size = cells.size();
    if (size == 0) return;
    // Color gradient based on ship size (larger ships have a more reddish hue)
    float r = 0.1f + 0.1f * size;
    float g = 0.2f + 0.05f * size;
    float b = 0.4f - 0.05f * size;
    bool horizontal = isHorizontal;  // orientation of the ship

    for (int i = 0; i < size; ++i) {
        const Cell* cell = cells[i];
        // Compute the center of the ship segment in world coordinates
        float centerX = cell->getX() + cellSize / 2.0f;
        float centerZ = cell->getY() + cellSize / 2.0f;
        float centerY = boardThickness + shipHeight / 2.0f;
        glPushMatrix();
        glTranslatef(centerX, centerY, centerZ);
        glColor3f(r, g, b);
        // Use a pyramid for the ship’s bow and stern, and a rectangular prism for mid-sections
        if ((i == 0 || i == size - 1) && size > 1) {
            // Rotate the pyramid to face outward from the ship
            glRotatef(horizontal ? (i == 0 ? 270.0f : 90.0f)
                                 : (i == 0 ? 180.0f : 0.0f),
                      0.0f, 1.0f, 0.0f);
            SeaBattle::drawPyramid(cellSize, cellSize);
        } else {
            SeaBattle::drawParallelepiped(cellSize, shipHeight, cellSize);
        }
        glPopMatrix();
    }
}