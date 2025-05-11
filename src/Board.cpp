#include "../include/Board.h"
#include <random>
#include <algorithm>

namespace SeaBattle {
    Board::Board(float offsetX, float offsetY, float cellSize)
        : cellSize(cellSize), offsetX(offsetX), offsetY(offsetY) {
        // Initialize cells
        cells.resize(BOARD_SIZE, std::vector<Cell *>(BOARD_SIZE));
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                cells[y][x] = new Cell(offsetX + x * cellSize, offsetY + y * cellSize, cellSize);
            }
        }
    }

    Board::~Board() {
        // Clean up cells
        for (auto &row: cells) {
            for (auto cell: row) {
                delete cell;
            }
        }

        // Clean up ships
        for (auto ship: ships) {
            delete ship;
        }
    }

    void Board::draw() {
        for (const auto &row: cells) {
            for (auto cell: row) {
                cell->draw();
            }
        }
    }

    bool Board::makeShot(float x, float y) {
        // Convert screen coordinates to board coordinates
        int boardX = static_cast<int>((x - offsetX) / cellSize);
        int boardY = static_cast<int>((y - offsetY) / cellSize);

        if (boardX < 0 || boardX >= BOARD_SIZE || boardY < 0 || boardY >= BOARD_SIZE) {
            return false;
        }

        Cell *cell = cells[boardY][boardX];
        if (cell->getState() != 0 && cell->getState() != 1) {
            return false; // Already shot here
        }

        if (cell->getState() == 1) {
            cell->setState(2); // Hit
            return true;
        } else {
            cell->setState(3); // Miss
            return true;
        }
    }

    bool Board::isGameOver() const {
        for (const auto &row: cells) {
            for (auto cell: row) {
                if (cell->getState() == 1) {
                    // If any ship cell is still hidden
                    return false;
                }
            }
        }
        return true;
    }

    bool Board::canPlaceShip(int x, int y, int size, bool horizontal) const {
        if (horizontal) {
            if (x + size > BOARD_SIZE) return false;
            for (int i = std::max(0, x - 1); i <= std::min(BOARD_SIZE - 1, x + size); i++) {
                for (int j = std::max(0, y - 1); j <= std::min(BOARD_SIZE - 1, y + 1); j++) {
                    if (cells[j][i]->getState() == 1) return false;
                }
            }
        } else {
            if (y + size > BOARD_SIZE) return false;
            for (int i = std::max(0, x - 1); i <= std::min(BOARD_SIZE - 1, x + 1); i++) {
                for (int j = std::max(0, y - 1); j <= std::min(BOARD_SIZE - 1, y + size); j++) {
                    if (cells[j][i]->getState() == 1) return false;
                }
            }
        }
        return true;
    }

    void Board::placeShip(int x, int y, int size, bool horizontal) {
        ships.push_back(new Ship(size, offsetX + x * cellSize, offsetY + y * cellSize, cellSize, horizontal));
    }

    void Board::generateShips() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, BOARD_SIZE - 1);
        std::uniform_int_distribution<> boolDis(0, 1);

        int shipsToPlace[] = {5, 4, 3, 3, 2};
        for (int size: shipsToPlace) {
            bool placed = false;
            while (!placed) {
                int x = dis(gen);
                int y = dis(gen);
                bool horizontal = boolDis(gen) == 1;

                if (canPlaceShip(x, y, size, horizontal)) {
                    placeShip(x, y, size, horizontal);
                    placed = true;
                }
            }
        }
    }
} // namespace SeaBattle
