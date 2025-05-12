#ifndef SEABATTLE_SCENE_H
#define SEABATTLE_SCENE_H

#include "Board.h"
#include <GL/glut.h>

namespace SeaBattle {
    class Scene {
        static const int BOARD_SPACING = 50;
        bool waitingForBot = false;
        bool playerWon;

        Board* playerBoard;
        Board* computerBoard;

        bool gameOver;
        bool playerTurn;

        float cellSize;
        int button;
        float mouseX, mouseY;
        float angleX, angleY;
        float distZ;

        GLdouble lastModelview[16];
        GLdouble lastProjection[16];
        GLint    lastViewport[4];

    public:
        Scene(float startX, float startY, float cellSize);
        ~Scene();

        void init();
        void draw();
        void drawTextOverlay();
        void handleMouseClick(float x, float y);

        // Event handlers for GLUT callbacks
        void on_paint();
        void on_motion(int x, int y);
        void on_mouse(int button, int state, int x, int y);
        void on_special(int key);

        float getPlayerBoardX() const;
        float getComputerBoardX() const;
        float getPlayerBoardY() const;

    private:
        void renderText(int x, int y, const char *text);
        void drawBoards();
        void drawLabels();
        void drawGameStatus();
        void handleComputerTurn();
    };
}

#endif // SEABATTLE_SCENE_H