#include "../include/Board.h"
#include <GL/glut.h>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "../include/Utils.h"

namespace SeaBattle {
    Board::Board(float offsetX, float offsetY, float cellSize)
        : cellSize(cellSize), offsetX(offsetX), offsetY(offsetY) {
        // створюємо 10x10 сітку клітинок
        cells.resize(BOARD_SIZE, std::vector<Cell *>(BOARD_SIZE));
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                cells[y][x] = new Cell(offsetX + x * cellSize,
                                       offsetY + y * cellSize,
                                       cellSize);
            }
        }
    }

    Board::~Board() {
        // видаляємо кораблі бо вони мають вказівники на клітинки
        for (Ship *ship: ships) {
            delete ship;
        }
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                delete cells[y][x];
            }
        }
    }

    void Board::draw(bool isPlayerBoard) {
        float boardLength = BOARD_SIZE * cellSize;
        float boardWidth = BOARD_SIZE * cellSize;
        float boardThickness = cellSize;
        float shipHeight = cellSize;
        float markerSize = cellSize;
        float markerHeight = cellSize;

        float platformCenterX = offsetX + boardLength / 2.0f;
        float platformCenterY = boardThickness / 2.0f;
        float platformCenterZ = offsetY + boardWidth / 2.0f;

        // платформа (вода)
        glPushMatrix();
        glTranslatef(platformCenterX, platformCenterY, platformCenterZ);
        glColor3f(0.2f, 0.4f, 0.8f);
        GLfloat waterAmbient[] = {0.1f, 0.2f, 0.4f, 1.0f};
        GLfloat waterDiffuse[] = {0.2f, 0.4f, 0.8f, 1.0f};
        GLfloat waterSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat waterShininess = 64.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, waterAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, waterDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, waterSpecular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, waterShininess);
        drawParallelepiped(boardLength, boardThickness, boardWidth);
        glPopMatrix();

        // сітка (лінії)
        glDisable(GL_LIGHTING);
        glColor3f(0.75f, 0.75f, 1.0f);
        float topZ = offsetY;
        float bottomZ = offsetY + boardWidth;
        float leftX = offsetX;
        float rightX = offsetX + boardLength;
        float lineY = boardThickness + 0.2f;
        float lineTh = 1.25f; // товщина лінії

        glBegin(GL_QUADS);
        // вертикальні лінії
        for (int i = 0; i <= BOARD_SIZE; ++i) {
            float x = offsetX + i * cellSize;
            glVertex3f(x - lineTh, lineY, topZ);
            glVertex3f(x + lineTh, lineY, topZ);
            glVertex3f(x + lineTh, lineY, bottomZ);
            glVertex3f(x - lineTh, lineY, bottomZ);
        }
        // горизонтальні лінії
        for (int j = 0; j <= BOARD_SIZE; ++j) {
            float z = offsetY + j * cellSize;
            glVertex3f(leftX, lineY, z - lineTh);
            glVertex3f(rightX, lineY, z - lineTh);
            glVertex3f(rightX, lineY, z + lineTh);
            glVertex3f(leftX, lineY, z + lineTh);
        }
        glEnd();
        glEnable(GL_LIGHTING);

        // малюємо свої кораблі
        if (isPlayerBoard) {
            for (Ship *ship: ships) {
                ship->draw(boardThickness, shipHeight, cellSize);
            }
        }

        // малюємо влучання/промахи
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                int state = cells[y][x]->getState();
                if (state == 2 || state == 3) {
                    float cx = offsetX + x * cellSize + cellSize / 2.0f;
                    float cz = offsetY + y * cellSize + cellSize / 2.0f;
                    float h = (state == 2 ? markerHeight : 0.1f);
                    float cy = boardThickness + h / 2.0f;
                    glPushMatrix();
                    glTranslatef(cx, cy, cz);
                    glColor3f(state == 2 ? 1.0f : 0.6f, 0.0f, state == 2 ? 0.0f : 0.6f);
                    drawParallelepiped(markerSize, h, markerSize);
                    glPopMatrix();
                }
            }
        }
    }

    bool Board::makeShot(float x, float y) {
        int bx = (int) ((x - offsetX) / cellSize);
        int by = (int) ((y - offsetY) / cellSize);
        if (bx < 0 || bx >= BOARD_SIZE || by < 0 || by >= BOARD_SIZE) {
            lastShotWasHit = false;
            return false;
        }
        Cell *cell = cells[by][bx];
        if (cell->getState() != 0 && cell->getState() != 1) {
            lastShotWasHit = false;
            return false;
        }
        if (cell->getState() == 1) {
            cell->setState(2);
            lastShotWasHit = true;
        } else {
            cell->setState(3);
            lastShotWasHit = false;
        }
        return true;
    }

    bool Board::isGameOver() const {
        // перевіряємо чи всі кораблі потоплені
        for (Ship *ship: ships) {
            if (!ship->isSunk()) {
                return false;
            }
        }
        return true;
    }

    bool Board::canPlaceShip(int x, int y, int size, bool horizontal) const {
        // перевірка виходу корабля за межі
        if ((horizontal && x + size > BOARD_SIZE) ||
            (!horizontal && y + size > BOARD_SIZE)) {
            return false;
        }

        // перевірка сусідніх клітинок щоб кораблі не торкались друг друга
        int xStart = std::max(0, x - 1);
        int yStart = std::max(0, y - 1);
        int xEnd = std::min(BOARD_SIZE - 1, x + (horizontal ? size : 1));
        int yEnd = std::min(BOARD_SIZE - 1, y + (horizontal ? 1 : size));
        for (int j = yStart; j <= yEnd; ++j) {
            for (int i = xStart; i <= xEnd; ++i) {
                if (cells[j][i]->getState() == 1) {
                    return false; // біля іншого корабля
                }
            }
        }
        return true;
    }

    void Board::placeShip(int x, int y, int size, bool horizontal) {
        // створюємо новий корабель і додаємо його до списку
        Ship *ship = new Ship(size, x, y, cellSize, horizontal, cells);
        ships.push_back(ship);
        // позначаємо клітинки як зайняті (state = 1)
        for (int i = 0; i < size; ++i) {
            if (horizontal) {
                cells[y][x + i]->setState(1);
            } else {
                cells[y + i][x]->setState(1);
            }
        }
    }

    void Board::generateShips() {
        // ставимо кораблі випадковим чином
        srand(static_cast<unsigned>(time(nullptr)) + reinterpret_cast<uintptr_t>(this));
        int shipsToPlace[] = {5, 4, 4, 3, 3};
        for (int s = 0; s < 5; ++s) {
            int size = shipsToPlace[s];
            bool placed = false;
            while (!placed) {
                int x = rand() % BOARD_SIZE;
                int y = rand() % BOARD_SIZE;
                bool horizontal = (rand() % 2 == 1);
                if (canPlaceShip(x, y, size, horizontal)) {
                    placeShip(x, y, size, horizontal);
                    placed = true;
                }
            }
        }
    }
}
