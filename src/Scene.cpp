#include "../include/Scene.h"
#include <GL/glut.h>
#include <string>
#include <random>
#include <cstdio>
#include <iostream>

SeaBattle::Scene* sceneForTimer;

namespace SeaBattle {
    void Scene::renderText(int x, int y, const char *text) {
        glRasterPos2i(x, y);
        for (const char *c = text; *c != '\0'; ++c) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
        }
    }

    Scene::Scene(float startX, float startY, float cellSize)
        : gameOver(false), playerTurn(true), button(-1),
          angleX(-20.0f), angleY(30.0f), distZ(-1000.0f), cellSize(cellSize) {
        playerBoard = new Board(startX, startY, cellSize);
        computerBoard = new Board(startX + playerBoard->getBoardSize() * cellSize + BOARD_SPACING, startY, cellSize);
    }

    Scene::~Scene() {
        delete playerBoard;
        delete computerBoard;
    }

    void Scene::init() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);
        glShadeModel(GL_SMOOTH);

        GLfloat matAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
        GLfloat matDiffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
        GLfloat matSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat matShininess = 0.0f;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

        GLfloat lightAmbient[] = {0.6f, 0.6f, 0.6f, 1.0f};
        GLfloat lightDiffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};
        GLfloat lightSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat lightPosition[] = {0.0f, 10.0f, 10.0f, 0.0f};

        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        sceneForTimer = this;

        playerBoard->generateShips();
        computerBoard->generateShips();
        gameOver = false;
        playerTurn = true;
    }

    void Scene::draw() {
        drawBoards();
    }

    void Scene::drawBoards() {
        playerBoard->draw(true);
        computerBoard->draw(false);
    }

    void Scene::drawLabels() {
        glColor3f(0.0f, 0.0f, 0.0f);
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        char buffer[256];
        sprintf(buffer, "Your board");
        renderText(windowWidth / 2 - 200, (int) (playerBoard->getOffsetY()), buffer);
        sprintf(buffer, "Bot's board");
        renderText(windowWidth / 2 + 100, (int) (computerBoard->getOffsetY()), buffer);
    }

    void Scene::drawGameStatus() {
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        glColor3f(0.0f, 0.0f, 0.0f);
        char buffer[256];

        if (!gameOver) {
            sprintf(buffer, "%s", playerTurn ? "Your step" : "Bot's step");
            renderText(windowWidth / 2 - 50, 30, buffer);
        } else {
            glColor3f(1.0f, 0.0f, 0.0f);
            if (playerWon)
                sprintf(buffer, "You won!");
            else
                sprintf(buffer, "Bot won!");
            renderText(windowWidth / 2 - 50, 30, buffer);
        }
    }

    void Scene::drawTextOverlay() {
        drawLabels();
        drawGameStatus();
    }

    void Scene::handleMouseClick(float x, float y) {
        if (gameOver || !playerTurn || waitingForBot) return;

        if (computerBoard->makeShot(x, y)) {
            if (computerBoard->isGameOver()) {
                gameOver = true;
                playerWon = true;
                return;
            }
            playerTurn = false;
            waitingForBot = true;
            glutTimerFunc(1500, [](int) {
                sceneForTimer->handleComputerTurn();
            }, 0);
        }
    }

    void Scene::handleComputerTurn() {
        waitingForBot = false;

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
            playerWon = false;
        } else {
            playerTurn = true;
        }

        glutPostRedisplay();
    }

    float Scene::getPlayerBoardX() const {
        return playerBoard->getOffsetX();
    }

    float Scene::getComputerBoardX() const {
        return computerBoard->getOffsetX();
    }

    float Scene::getPlayerBoardY() const {
        return playerBoard->getOffsetY();
    }

    void Scene::on_paint() {
        glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

        float lightAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
        float lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
        float lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        float lightPosition[] = {1.0f, 1.0f, 1.0f, 0.0f};

        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, glutGet(GLUT_WINDOW_WIDTH) / (float) glutGet(GLUT_WINDOW_HEIGHT), 1.0, 3000.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, distZ);
        glRotatef(angleX, 0.0f, 1.0f, 0.0f);
        glRotatef(angleY, 1.0f, 0.0f, 0.0f);

        float centerX = (getPlayerBoardX() + getComputerBoardX()) / 2.0f + 5 * cellSize;
        float centerZ = getPlayerBoardY() + 5 * cellSize;
        glTranslatef(-centerX, -10.0f, -centerZ);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        draw();

        glGetDoublev(GL_MODELVIEW_MATRIX, lastModelview);
        glGetDoublev(GL_PROJECTION_MATRIX, lastProjection);
        glGetIntegerv(GL_VIEWPORT, lastViewport);

        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);
        drawTextOverlay();
        glEnable(GL_DEPTH_TEST);

        glFlush();
        glutSwapBuffers();
    }

    void Scene::on_motion(int x, int y) {
        if (button == GLUT_RIGHT_BUTTON) {
            angleX += x - mouseX;
            angleY += y - mouseY;
            mouseX = x;
            mouseY = y;
            glutPostRedisplay();
        }
    }

    void Scene::on_mouse(int button, int state, int x, int y) {
        mouseX = x;
        mouseY = y;

        if (state == GLUT_UP) {
            this->button = -1;
            return;
        }

        this->button = button;

        if (button == GLUT_LEFT_BUTTON) {
            GLfloat winX = (float) x;
            GLfloat winY = (float) (lastViewport[3] - y);
            GLfloat winZ;
            GLdouble posX, posY, posZ;

            glReadPixels(x, lastViewport[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
            gluUnProject(winX, winY, winZ, lastModelview, lastProjection, lastViewport, &posX, &posY, &posZ);

            handleMouseClick((float) posX, (float) posZ);
            glutPostRedisplay();
        }
    }

    void Scene::on_special(int key) {
        switch (key) {
            case GLUT_KEY_UP:
                if (distZ < -500.0f) distZ += 50.0f;
                break;
            case GLUT_KEY_DOWN:
                if (distZ > -1500.0f) distZ -= 50.0f;
                break;
        }
        glutPostRedisplay();
    }
}
