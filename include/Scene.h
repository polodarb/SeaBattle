#ifndef SCENE_H
#define SCENE_H

#include "Board.h"

namespace SeaBattle {

    class Scene {
        static const int BOARD_SPACING = 50;
        Board* playerBoard;
        Board* computerBoard;
        bool gameOver;
        bool playerTurn;

    public:
        Scene(float startX, float startY, float cellSize);
        ~Scene();

        void init();
        void draw();
        void handleMouseClick(float x, float y);

    private:
        void renderText(int x, int y, const char* text);
        void drawBackground();
        void drawBoards();
        void drawLabels();
        void drawGameStatus();
        void handleComputerTurn();
    };

} // namespace SeaBattle

#endif
