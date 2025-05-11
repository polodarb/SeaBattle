#include "../include/Scene.h"
#include <GL/glut.h>
#include <string>
#include <random>
#include <cstdio>

namespace SeaBattle {
    void Scene::renderText(int x, int y, const char *text) {
        glRasterPos2i(x, y);
        for (const char *c = text; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
        }
    }

    Scene::Scene(float startX, float startY, float cellSize)
        : gameOver(false), playerTurn(true) {
        playerBoard = new Board(startX, startY, cellSize);
        computerBoard = new Board(startX + playerBoard->getBoardSize() * cellSize + BOARD_SPACING, startY, cellSize);
    }

    Scene::~Scene() {
        delete playerBoard;
        delete computerBoard;
    }

    void Scene::init() {
        playerBoard->generateShips();
        computerBoard->generateShips();
        gameOver = false;
        playerTurn = true;
    }

    void Scene::draw() {
        drawBackground();
        drawBoards();
        drawLabels();
        drawGameStatus();
    }

    void Scene::drawBackground() {
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(glutGet(GLUT_WINDOW_WIDTH), 0);
        glVertex2f(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        glVertex2f(0, glutGet(GLUT_WINDOW_HEIGHT));
        glEnd();
    }

    void Scene::drawBoards() {
        playerBoard->draw();
        computerBoard->draw();
    }

    void Scene::drawLabels() {
        glColor3f(0.0f, 0.0f, 0.0f);
        char buffer[256];
        sprintf(buffer, "Your board");
        renderText(static_cast<int>(playerBoard->getOffsetX()), static_cast<int>(playerBoard->getOffsetY() - 30),
                   buffer);
        sprintf(buffer, "Bot's board");
        renderText(static_cast<int>(computerBoard->getOffsetX()), static_cast<int>(computerBoard->getOffsetY() - 30),
                   buffer);
    }

    void Scene::drawGameStatus() {
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        glColor3f(0.0f, 0.0f, 0.0f);
        char buffer[256];
        sprintf(buffer, "%s", playerTurn ? "Your step" : "Bot's step");
        renderText(windowWidth / 2 - 50, 30, buffer);
        if (gameOver) {
            glColor3f(1.0f, 0.0f, 0.0f);
            sprintf(buffer, "Game over!");
            renderText(windowWidth / 2 - 50, 50, buffer);
        }
    }

    void Scene::handleMouseClick(float x, float y) {
        if (gameOver || !playerTurn) return;

        if (computerBoard->makeShot(x, y)) {
            if (computerBoard->isGameOver()) {
                gameOver = true;
                return;
            }

            playerTurn = false;
            handleComputerTurn();
        }
    }

    void Scene::handleComputerTurn() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, playerBoard->getBoardSize() - 1);

        bool shotMade = false;
        while (!shotMade) {
            int compX = dis(gen);
            int compY = dis(gen);

            float shotX = playerBoard->getOffsetX() + compX * playerBoard->getCellSize();
            float shotY = playerBoard->getOffsetY() + compY * playerBoard->getCellSize();

            shotMade = playerBoard->makeShot(shotX, shotY);
        }

        if (playerBoard->isGameOver()) {
            gameOver = true;
        } else {
            playerTurn = true;
        }
    }
} // namespace SeaBattle
