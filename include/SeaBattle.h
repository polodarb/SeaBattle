#ifndef SEABATTLE_H
#define SEABATTLE_H

#include <vector>
#include <random>

class SeaBattle {
private:
    static const int BOARD_SIZE = 10;
    std::vector<std::vector<int>> board;
    std::vector<std::vector<int>> playerBoard;
    bool gameOver;
    int remainingShips;

    bool canPlaceShip(int x, int y, int size, bool horizontal);
    void placeShip(int x, int y, int size, bool horizontal);
    void generateShips();

public:
    SeaBattle();
    void init();
    bool makeShot(int x, int y);
    bool isGameOver() const { return gameOver; }
    const std::vector<std::vector<int>>& getBoard() const { return board; }
    const std::vector<std::vector<int>>& getPlayerBoard() const { return playerBoard; }
    int getBoardSize() const { return BOARD_SIZE; }
};

#endif // SEABATTLE_H 