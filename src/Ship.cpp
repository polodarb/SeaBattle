#include <GL/glut.h>
#include "../include/Ship.h"
#include "../include/Utils.h"

namespace SeaBattle {

Ship::Ship(int size, int startX, int startY, float cellSize, bool horizontal,
           const std::vector<std::vector<Cell*>>& boardCells)
    : isHorizontal(horizontal), isDestroyed(false)
{
    // Додаємо в список клітинки, які займає цей корабель на дошці
    for (int i = 0; i < size; ++i) {
        Cell* cell = horizontal ? boardCells[startY][startX + i]
                                : boardCells[startY + i][startX];
        cells.push_back(cell);
        // (Стан клітинок буде встановлено у placeShip в класі Board)
    }
}

Ship::~Ship() {
    // Просто очищаємо список — самі клітинки не видаляємо, бо вони належать дошці
    cells.clear();
}

bool Ship::isHit() const {
    // Перевіряємо, чи хоча б одна клітинка корабля була влучена (стан = 2)
    for (const Cell* cell : cells) {
        if (cell->getState() == 2) {
            return true;
        }
    }
    return false;
}

bool Ship::isSunk() const {
    // Перевіряємо, чи всі клітинки корабля влучені (стан != 2 — ще не влучено)
    for (const Cell* cell : cells) {
        if (cell->getState() != 2) {
            return false;
        }
    }
    return true;
}

bool Ship::contains(float px, float py) const {
    // Перевіряємо, чи точка (px, py) потрапляє в одну з клітинок корабля
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

    // Залежно від розміру корабля змінюємо відтінок (більші кораблі червоніші)
    float r = 0.1f + 0.1f * size;
    float g = 0.2f + 0.05f * size;
    float b = 0.4f - 0.05f * size;
    bool horizontal = isHorizontal;

    for (int i = 0; i < size; ++i) {
        const Cell* cell = cells[i];
        // Центр поточного сегмента корабля (у світових координатах)
        float centerX = cell->getX() + cellSize / 2.0f;
        float centerZ = cell->getY() + cellSize / 2.0f;
        float centerY = boardThickness + shipHeight / 2.0f;
        glPushMatrix();
        glTranslatef(centerX, centerY, centerZ);
        glColor3f(r, g, b);

        // Якщо це крайня частина корабля — малюємо піраміду, інакше паралелепіпед
        if ((i == 0 || i == size - 1) && size > 1) {
            // Обертаємо піраміду так, щоб вона «дивилася» назовні
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

} // namespace SeaBattle
