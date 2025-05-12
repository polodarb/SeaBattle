#include "../include/Board.h"
#include <GL/glut.h>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "../include/Utils.h"

using namespace SeaBattle;

Board::Board(float offsetX, float offsetY, float cellSize)
    : cellSize(cellSize), offsetX(offsetX), offsetY(offsetY)
{
    // Initialize 10x10 grid of Cell pointers
    cells.resize(BOARD_SIZE, std::vector<Cell*>(BOARD_SIZE));
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            cells[y][x] = new Cell(offsetX + x * cellSize,
                                    offsetY + y * cellSize,
                                    cellSize);
        }
    }
}

Board::~Board() {
    // Delete all ships first (ships hold pointers to cells)
    for (Ship* ship : ships) {
        delete ship;
    }
    // Delete all cells in the grid
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            delete cells[y][x];
        }
    }
}

void Board::draw(bool isPlayerBoard) {
    // Dimensions for drawing the board and contents
    float boardLength   = BOARD_SIZE * cellSize;
    float boardWidth    = BOARD_SIZE * cellSize;
    float boardThickness = cellSize;
    float shipHeight    = cellSize;
    float markerSize    = cellSize;
    float markerHeight  = cellSize;

    // Position the water platform (board) at the correct world coordinates
    float platformCenterX = offsetX + boardLength / 2.0f;
    float platformCenterY = boardThickness / 2.0f;
    float platformCenterZ = offsetY + boardWidth / 2.0f;

    // Draw the water platform (blue rectangular prism)
    glPushMatrix();
    glTranslatef(platformCenterX, platformCenterY, platformCenterZ);
    glColor3f(0.2f, 0.4f, 0.8f);
    GLfloat waterAmbient[]  = { 0.1f, 0.2f, 0.4f, 1.0f };
    GLfloat waterDiffuse[]  = { 0.2f, 0.4f, 0.8f, 1.0f };
    GLfloat waterSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat waterShininess  = 64.0f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   waterAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   waterDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  waterSpecular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, waterShininess);
    SeaBattle::drawParallelepiped(boardLength, boardThickness, boardWidth);
    glPopMatrix();

    // Draw grid lines on the board (disable lighting for solid color lines)
    glDisable(GL_LIGHTING);
    glColor3f(0.75f, 0.75f, 1.0f);
    float topZ    = offsetY;
    float bottomZ = offsetY + boardWidth;
    float leftX   = offsetX;
    float rightX  = offsetX + boardLength;
    float lineY   = boardThickness + 0.2f;
    float lineTh  = 1.25f; // half-thickness of grid line
    glBegin(GL_QUADS);
    // Vertical grid lines (along X)
    for (int i = 0; i <= BOARD_SIZE; ++i) {
        float x = offsetX + i * cellSize;
        glVertex3f(x - lineTh, lineY, topZ);
        glVertex3f(x + lineTh, lineY, topZ);
        glVertex3f(x + lineTh, lineY, bottomZ);
        glVertex3f(x - lineTh, lineY, bottomZ);
    }
    // Horizontal grid lines (along Z)
    for (int j = 0; j <= BOARD_SIZE; ++j) {
        float z = offsetY + j * cellSize;
        glVertex3f(leftX,  lineY, z - lineTh);
        glVertex3f(rightX, lineY, z - lineTh);
        glVertex3f(rightX, lineY, z + lineTh);
        glVertex3f(leftX,  lineY, z + lineTh);
    }
    glEnd();
    glEnable(GL_LIGHTING);

    // Draw ships on the player's own board (hidden for the opponent's board)
    if (isPlayerBoard) {
        for (Ship* ship : ships) {
            ship->draw(boardThickness, shipHeight, cellSize);
        }
    }

    // Draw hit/miss markers on all boards (red for hits, blue-gray for misses)
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            int state = cells[y][x]->getState();
            if (state == 2 || state == 3) {  // 2 = hit ship, 3 = miss
                float cx = offsetX + x * cellSize + cellSize / 2.0f;
                float cz = offsetY + y * cellSize + cellSize / 2.0f;
                float h  = (state == 2 ? markerHeight : 0.1f);
                float cy = boardThickness + h / 2.0f;
                glPushMatrix();
                glTranslatef(cx, cy, cz);
                // Red marker for hit (state 2), dark cyan for miss (state 3)
                glColor3f(state == 2 ? 1.0f : 0.6f, 0.0f, state == 2 ? 0.0f : 0.6f);
                SeaBattle::drawParallelepiped(markerSize, h, markerSize);
                glPopMatrix();
            }
        }
    }
}

bool Board::makeShot(float x, float y) {
    // Translate world coordinates (x,y) into board grid indices
    int bx = (int)((x - offsetX) / cellSize);
    int by = (int)((y - offsetY) / cellSize);
    if (bx < 0 || bx >= BOARD_SIZE || by < 0 || by >= BOARD_SIZE) {
        return false; // Click was outside this board
    }
    Cell* cell = cells[by][bx];
    // Only allow shooting at cells that are untried (state 0 or 1)
    if (cell->getState() != 0 && cell->getState() != 1) {
        return false;
    }
    // If there was a ship (state 1), mark hit (2); if water (0), mark miss (3)
    cell->setState(cell->getState() == 1 ? 2 : 3);
    return true;
}

bool Board::isGameOver() const {
    // Check if all ships have been sunk
    for (Ship* ship : ships) {
        if (!ship->isSunk()) {
            return false;
        }
    }
    return true;
}

bool Board::canPlaceShip(int x, int y, int size, bool horizontal) const {
    // Check board bounds for the ship placement
    if ((horizontal && x + size > BOARD_SIZE) ||
        (!horizontal && y + size > BOARD_SIZE)) {
        return false;
    }
    // Ensure no neighboring cells (including diagonals) contain a ship
    int xStart = std::max(0, x - 1);
    int yStart = std::max(0, y - 1);
    int xEnd   = std::min(BOARD_SIZE - 1, x + (horizontal ? size : 1));
    int yEnd   = std::min(BOARD_SIZE - 1, y + (horizontal ? 1 : size));
    for (int j = yStart; j <= yEnd; ++j) {
        for (int i = xStart; i <= xEnd; ++i) {
            if (cells[j][i]->getState() == 1) {
                return false; // Adjacent to an existing ship
            }
        }
    }
    return true;
}

void Board::placeShip(int x, int y, int size, bool horizontal) {
    // Create a new ship at the specified grid position/orientation
    Ship* ship = new Ship(size, x, y, cellSize, horizontal, cells);
    ships.push_back(ship);
    // Mark the corresponding cells on the board as occupied (state = 1)
    for (int i = 0; i < size; ++i) {
        if (horizontal) {
            cells[y][x + i]->setState(1);
        } else {
            cells[y + i][x]->setState(1);
        }
    }
}

void Board::generateShips() {
    // Randomly place ships of sizes specified in the array
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