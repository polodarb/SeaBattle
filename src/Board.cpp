#include "../include/Board.h"
#include <GL/glut.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "../include/Shape.h"


namespace SeaBattle {
    // Вспомогательная функция рисования параллелепипеда (ориентирован вдоль осей, центр в начале координат)
    static void drawParallelepiped(float length, float height, float depth) {
        float lx = length / 2.0f;
        float ly = height / 2.0f;
        float lz = depth / 2.0f;
        glBegin(GL_QUADS);
        // Верхняя грань (y = +ly)
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-lx, ly, -lz);
        glVertex3f(-lx, ly, lz);
        glVertex3f(lx, ly, lz);
        glVertex3f(lx, ly, -lz);
        // Нижняя грань (y = -ly)
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-lx, -ly, -lz);
        glVertex3f(lx, -ly, -lz);
        glVertex3f(lx, -ly, lz);
        glVertex3f(-lx, -ly, lz);
        // Передняя грань (z = +lz)
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-lx, -ly, lz);
        glVertex3f(lx, -ly, lz);
        glVertex3f(lx, ly, lz);
        glVertex3f(-lx, ly, lz);
        // Задняя грань (z = -lz)
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-lx, -ly, -lz);
        glVertex3f(-lx, ly, -lz);
        glVertex3f(lx, ly, -lz);
        glVertex3f(lx, -ly, -lz);
        // Левая грань (x = -lx)
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-lx, -ly, -lz);
        glVertex3f(-lx, -ly, lz);
        glVertex3f(-lx, ly, lz);
        glVertex3f(-lx, ly, -lz);
        // Правая грань (x = +lx)
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(lx, -ly, -lz);
        glVertex3f(lx, ly, -lz);
        glVertex3f(lx, ly, lz);
        glVertex3f(lx, -ly, lz);
        glEnd();
    }

    Board::Board(float offsetX, float offsetY, float cellSize)
        : cellSize(cellSize), offsetX(offsetX), offsetY(offsetY) {
        cells.resize(BOARD_SIZE, std::vector<Cell *>(BOARD_SIZE));
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                cells[y][x] = new Cell(offsetX + x * cellSize, offsetY + y * cellSize, cellSize);
            }
        }
    }

    Board::~Board() {
        for (auto &row: cells) {
            for (auto cell: row) {
                delete cell;
            }
        }
        for (auto ship: ships) {
            delete ship;
        }
    }

    void Board::draw(bool isPlayerBoard) {
        float boardLength = BOARD_SIZE * cellSize;
        float boardWidth = BOARD_SIZE * cellSize;
        float boardThickness = 1 * cellSize;
        float shipHeight = 0.5f * cellSize;
        float markerSize = 1 * cellSize;
        float markerHeight = 0.5f * cellSize;

        float platformCenterX = offsetX + boardLength / 2.0f;
        float platformCenterY = boardThickness / 2.0f;
        float platformCenterZ = offsetY + boardWidth / 2.0f;
        glPushMatrix();
        glTranslatef(platformCenterX, platformCenterY, platformCenterZ);
        glColor3f(0.2f, 0.4f, 0.8f);
        drawParallelepiped(boardLength, boardThickness, boardWidth);
        glPopMatrix();

        glDisable(GL_LIGHTING);
        glColor3f(0.75f, 0.75f, 1.0f);
        float topZ = offsetY;
        float bottomZ = offsetY + boardWidth;
        float leftX = offsetX;
        float rightX = offsetX + boardLength;
        float lineY = boardThickness + 0.2f;
        float lineThickness = 1.25f;
        glBegin(GL_QUADS);
        // Вертикальные линии
        for (int i = 0; i <= BOARD_SIZE; ++i) {
            float x = offsetX + i * cellSize;
            glVertex3f(x - lineThickness, lineY, topZ);
            glVertex3f(x + lineThickness, lineY, topZ);
            glVertex3f(x + lineThickness, lineY, bottomZ);
            glVertex3f(x - lineThickness, lineY, bottomZ);
        }
        // Горизонтальные линии
        for (int j = 0; j <= BOARD_SIZE; ++j) {
            float z = offsetY + j * cellSize;
            glVertex3f(leftX, lineY, z - lineThickness);
            glVertex3f(rightX, lineY, z - lineThickness);
            glVertex3f(rightX, lineY, z + lineThickness);
            glVertex3f(leftX, lineY, z + lineThickness);
        }
        glEnd();
        glEnable(GL_LIGHTING);

        if (isPlayerBoard) {
            for (Ship *ship: ships) {
                int shipSize = ship->getCells().size();
                bool horizontal = false;
                const std::vector<Cell *> &shipCells = ship->getCells();
                if (shipSize > 1) {
                    float x0 = shipCells[0]->getX();
                    float x1 = shipCells[1]->getX();
                    horizontal = (x1 > x0);
                }
                float shipLength = horizontal ? shipSize * cellSize : cellSize;
                float shipDepth = horizontal ? cellSize : shipSize * cellSize;
                float shipCenterX = horizontal
                                        ? shipCells[0]->getX() + shipLength / 2.0f
                                        : shipCells[0]->getX() + cellSize / 2.0f;
                float shipCenterZ = horizontal
                                        ? shipCells[0]->getY() + cellSize / 2.0f
                                        : shipCells[0]->getY() + shipDepth / 2.0f;
                float shipCenterY = boardThickness + shipHeight / 2.0f;
                glPushMatrix();
                glTranslatef(shipCenterX, shipCenterY, shipCenterZ);
                glColor3f(0.0f, 0.0f, 0.0f);
                drawParallelepiped(shipLength, shipHeight, shipDepth);
                glPopMatrix();
            }
        }

        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                int state = cells[y][x]->getState();
                if (state == 2 || state == 3) {
                    float cellCenterX = offsetX + x * cellSize + cellSize / 2.0f;
                    float cellCenterZ = offsetY + y * cellSize + cellSize / 2.0f;
                    float markerH = (state == 2) ? markerHeight : 0.1f;
                    float baseY = boardThickness;
                    if (state == 2 && isPlayerBoard) {
                        baseY = boardThickness + shipHeight;
                    }
                    float markerCenterY = baseY + markerH / 2.0f;

                    glPushMatrix();
                    glTranslatef(cellCenterX, markerCenterY, cellCenterZ);
                    if (state == 2) {
                        glColor3f(1.0f, 0.0f, 0.0f);
                    } else {
                        glColor3f(0.6f, 0.6f, 0.6f);
                    }
                    drawParallelepiped(markerSize, markerH, markerSize);
                    glPopMatrix();
                }
            }
        }
    }

    bool Board::makeShot(float x, float y) {
        int boardX = (int) ((x - offsetX) / cellSize);
        int boardY = (int) ((y - offsetY) / cellSize);
        if (boardX < 0 || boardX >= BOARD_SIZE || boardY < 0 || boardY >= BOARD_SIZE) {
            return false;
        }
        Cell *cell = cells[boardY][boardX];
        if (cell->getState() != 0 && cell->getState() != 1) {
            return false;
        }
        if (cell->getState() == 1) {
            cell->setState(2);
            return true;
        } else {
            cell->setState(3);
            return true;
        }
    }

    bool Board::isGameOver() const {
        for (const auto &row: cells) {
            for (auto cell: row) {
                if (cell->getState() == 1) {
                    return false;
                }
            }
        }
        return true;
    }

    bool Board::canPlaceShip(int x, int y, int size, bool horizontal) const {
        if (horizontal) {
            if (x + size > BOARD_SIZE) return false;
            for (int i = std::max(0, x - 1); i <= std::min(BOARD_SIZE - 1, x + size); ++i) {
                for (int j = std::max(0, y - 1); j <= std::min(BOARD_SIZE - 1, y + 1); ++j) {
                    if (cells[j][i]->getState() == 1) return false;
                }
            }
        } else {
            if (y + size > BOARD_SIZE) return false;
            for (int i = std::max(0, x - 1); i <= std::min(BOARD_SIZE - 1, x + 1); ++i) {
                for (int j = std::max(0, y - 1); j <= std::min(BOARD_SIZE - 1, y + size); ++j) {
                    if (cells[j][i]->getState() == 1) return false;
                }
            }
        }
        return true;
    }

    void Board::placeShip(int x, int y, int size, bool horizontal) {
        ships.push_back(new Ship(size, offsetX + x * cellSize, offsetY + y * cellSize, cellSize, horizontal));
        if (horizontal) {
            for (int i = 0; i < size; ++i) {
                cells[y][x + i]->setState(1);
            }
        } else {
            for (int i = 0; i < size; ++i) {
                cells[y + i][x]->setState(1);
            }
        }
    }

    void Board::generateShips() {
        srand(static_cast<unsigned>(time(nullptr)));
        int shipsToPlace[] = {5, 4, 3, 3, 2};
        for (int size: shipsToPlace) {
            bool placed = false;
            while (!placed) {
                int x = rand() % BOARD_SIZE;
                int y = rand() % BOARD_SIZE;
                bool horizontal = rand() % 2 == 1;
                if (canPlaceShip(x, y, size, horizontal)) {
                    placeShip(x, y, size, horizontal);
                    placed = true;
                }
            }
        }
    }
} // namespace SeaBattle
