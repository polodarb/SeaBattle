#include "../include/Board.h"
#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "../include/Shape.h"

namespace SeaBattle {
    static void drawParallelepiped(float length, float height, float depth) {
        float lx = length / 2.0f;
        float ly = height / 2.0f;
        float lz = depth / 2.0f;
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-lx, ly, -lz);
        glVertex3f(-lx, ly, lz);
        glVertex3f(lx, ly, lz);
        glVertex3f(lx, ly, -lz);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-lx, -ly, -lz);
        glVertex3f(lx, -ly, -lz);
        glVertex3f(lx, -ly, lz);
        glVertex3f(-lx, -ly, lz);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-lx, -ly, lz);
        glVertex3f(lx, -ly, lz);
        glVertex3f(lx, ly, lz);
        glVertex3f(-lx, ly, lz);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-lx, -ly, -lz);
        glVertex3f(-lx, ly, -lz);
        glVertex3f(lx, ly, -lz);
        glVertex3f(lx, -ly, -lz);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-lx, -ly, -lz);
        glVertex3f(-lx, -ly, lz);
        glVertex3f(-lx, ly, lz);
        glVertex3f(-lx, ly, -lz);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(lx, -ly, -lz);
        glVertex3f(lx, ly, -lz);
        glVertex3f(lx, ly, lz);
        glVertex3f(lx, -ly, lz);
        glEnd();
    }

    static void drawPyramid(float baseSize, float height) {
        float half = baseSize / 2.0f;
        float tipZ = height / 2.0f;
        float baseZ = -height / 2.0f;
        glBegin(GL_TRIANGLES);
        glNormal3f(0, 0, 1);
        glVertex3f(-half, -half, baseZ);
        glVertex3f(half, -half, baseZ);
        glVertex3f(0, 0, tipZ);
        glNormal3f(1, 0, 0);
        glVertex3f(half, -half, baseZ);
        glVertex3f(half, half, baseZ);
        glVertex3f(0, 0, tipZ);
        glNormal3f(0, 0, -1);
        glVertex3f(half, half, baseZ);
        glVertex3f(-half, half, baseZ);
        glVertex3f(0, 0, tipZ);
        glNormal3f(-1, 0, 0);
        glVertex3f(-half, half, baseZ);
        glVertex3f(-half, -half, baseZ);
        glVertex3f(0, 0, tipZ);
        glEnd();
    }

    void Board::drawShip(const Ship *ship, float boardThickness, float shipHeight, float cellSize) {
        const std::vector<Cell *> &shipCells = ship->getCells();
        int size = shipCells.size();
        if (size == 0) return;

        bool horizontal = size == 1 || shipCells[1]->getX() > shipCells[0]->getX();
        float r = 0.1f + 0.1f * size;
        float g = 0.2f + 0.05f * size;
        float b = 0.4f - 0.05f * size;

        for (int i = 0; i < size; ++i) {
            Cell *cell = shipCells[i];
            float centerX = cell->getX() + cellSize / 2.0f;
            float centerZ = cell->getY() + cellSize / 2.0f;
            float centerY = boardThickness + shipHeight / 2.0f;

            glPushMatrix();
            glTranslatef(centerX, centerY, centerZ);

            glColor3f(r, g, b);

            if ((i == 0 || i == size - 1) && size > 1) {
                glRotatef(horizontal ? (i == 0 ? 270 : 90) : (i == 0 ? 180 : 0), 0, 1, 0);
                drawPyramid(cellSize, cellSize);
            } else {
                drawParallelepiped(cellSize, shipHeight, cellSize);
            }

            glPopMatrix();
        }
    }

    Board::Board(float offsetX, float offsetY, float cellSize)
        : cellSize(cellSize), offsetX(offsetX), offsetY(offsetY) {
        cells.resize(BOARD_SIZE, std::vector<Cell *>(BOARD_SIZE));
        for (int y = 0; y < BOARD_SIZE; ++y)
            for (int x = 0; x < BOARD_SIZE; ++x)
                cells[y][x] = new Cell(offsetX + x * cellSize, offsetY + y * cellSize, cellSize);
    }

    Board::~Board() {
        for (int y = 0; y < BOARD_SIZE; ++y)
            for (int x = 0; x < BOARD_SIZE; ++x)
                delete cells[y][x];
        for (int i = 0; i < ships.size(); ++i)
            delete ships[i];
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

        glPushMatrix();
        glTranslatef(platformCenterX, platformCenterY, platformCenterZ);
        glColor3f(0.2f, 0.4f, 0.8f);
        GLfloat water_ambient[] = {0.1f, 0.2f, 0.4f, 1.0f};
        GLfloat water_diffuse[] = {0.2f, 0.4f, 0.8f, 1.0f};
        GLfloat water_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat water_shininess = 64.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, water_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, water_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, water_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, water_shininess);
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
        for (int i = 0; i <= BOARD_SIZE; ++i) {
            float x = offsetX + i * cellSize;
            glVertex3f(x - lineThickness, lineY, topZ);
            glVertex3f(x + lineThickness, lineY, topZ);
            glVertex3f(x + lineThickness, lineY, bottomZ);
            glVertex3f(x - lineThickness, lineY, bottomZ);
        }
        for (int j = 0; j <= BOARD_SIZE; ++j) {
            float z = offsetY + j * cellSize;
            glVertex3f(leftX, lineY, z - lineThickness);
            glVertex3f(rightX, lineY, z - lineThickness);
            glVertex3f(rightX, lineY, z + lineThickness);
            glVertex3f(leftX, lineY, z + lineThickness);
        }
        glEnd();
        glEnable(GL_LIGHTING);

        if (isPlayerBoard)
            for (int i = 0; i < ships.size(); ++i)
                drawShip(ships[i], boardThickness, shipHeight, cellSize);

        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                int state = cells[y][x]->getState();
                if (state == 2 || state == 3) {
                    float cx = offsetX + x * cellSize + cellSize / 2.0f;
                    float cz = offsetY + y * cellSize + cellSize / 2.0f;
                    float h = (state == 2) ? markerHeight : 0.1f;
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
        if (bx < 0 || bx >= BOARD_SIZE || by < 0 || by >= BOARD_SIZE) return false;
        Cell *cell = cells[by][bx];
        if (cell->getState() != 0 && cell->getState() != 1) return false;
        cell->setState(cell->getState() == 1 ? 2 : 3);
        return true;
    }

    bool Board::isGameOver() const {
        for (int y = 0; y < BOARD_SIZE; ++y)
            for (int x = 0; x < BOARD_SIZE; ++x)
                if (cells[y][x]->getState() == 1)
                    return false;
        return true;
    }

    bool Board::canPlaceShip(int x, int y, int size, bool horizontal) const {
        if ((horizontal && x + size > BOARD_SIZE) || (!horizontal && y + size > BOARD_SIZE)) return false;
        for (int i = std::max(0, x - 1); i <= std::min(BOARD_SIZE - 1, x + (horizontal ? size : 1)); ++i)
            for (int j = std::max(0, y - 1); j <= std::min(BOARD_SIZE - 1, y + (horizontal ? 1 : size)); ++j)
                if (cells[j][i]->getState() == 1) return false;
        return true;
    }

    void Board::placeShip(int x, int y, int size, bool horizontal) {
        ships.push_back(new Ship(size, offsetX + x * cellSize, offsetY + y * cellSize, cellSize, horizontal));
        for (int i = 0; i < size; ++i)
            horizontal ? cells[y][x + i]->setState(1) : cells[y + i][x]->setState(1);
    }

    void Board::generateShips() {
        srand(static_cast<unsigned>(time(nullptr)) + reinterpret_cast<uintptr_t>(this));
        int shipsToPlace[] = {5, 4, 4, 3, 3};
        for (int s = 0; s < 5; ++s) {
            int size = shipsToPlace[s];
            bool placed = false;
            while (!placed) {
                int x = rand() % BOARD_SIZE;
                int y = rand() % BOARD_SIZE;
                bool horizontal = rand() % 2;
                if (canPlaceShip(x, y, size, horizontal)) {
                    placeShip(x, y, size, horizontal);
                    placed = true;
                }
            }
        }
    }
}
