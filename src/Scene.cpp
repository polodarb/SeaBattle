#include "../include/Scene.h"
#include <GL/glut.h>
#include <cstdio>
#include <random>

// глобальний вказівник на сцену (щоб передати в колбек таймера)
SeaBattle::Scene *sceneForTimer = nullptr;

namespace SeaBattle {
    // функція для виводу тексту в 2D-координатах екрану
    void Scene::renderText(int x, int y, const char *text) {
        glRasterPos2i(x, y);
        for (const char *c = text; *c != '\0'; ++c) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
        }
    }

    // конструктор сцени. Ініціалізує дошки гравця та бота
    Scene::Scene(float startX, float startY, float cellSize)
        : waitingForBot(false), playerWon(false),
          gameOver(false), playerTurn(true), button(-1),
          angleX(-20.0f), angleY(30.0f), distZ(-1000.0f),
          cellSize(cellSize) {
        // створення двох дощок: гравця та комп'ютера
        playerBoard = new Board(startX, startY, cellSize);
        computerBoard = new Board(startX + playerBoard->getBoardSize() * cellSize + BOARD_SPACING,
                                  startY, cellSize);
    }

    // деструктор - очищає пам'ять
    Scene::~Scene() {
        delete playerBoard;
        delete computerBoard;
    }

    // ініціалізація OpenGL параметрів та генерація кораблів на дошках
    void Scene::init() {
        // стандартні OpenGL налаштування
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);
        glShadeModel(GL_SMOOTH);

        // параметри матеріалу (освітлення)
        GLfloat matAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
        GLfloat matDiffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
        GLfloat matSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat matShininess = 0.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

        // параметри джерела світла
        GLfloat lightAmbient[] = {0.6f, 0.6f, 0.6f, 1.0f};
        GLfloat lightDiffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};
        GLfloat lightSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat lightPosition[] = {0.0f, 10.0f, 10.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        // зберігаємо посилання на сцену для колбека таймера
        sceneForTimer = this;

        // створення кораблів для обох дощок
        playerBoard->generateShips();
        computerBoard->generateShips();
        gameOver = false;
        playerTurn = true;
    }

    // головна функція малювання 3D-сцени
    void Scene::draw() {
        drawBoards(); // малюємо дошки
    }

    // малювання дощок
    void Scene::drawBoards() {
        playerBoard->draw(true); // дошка гравця з кораблями
        computerBoard->draw(false); // дошка комп’ютера без кораблів
    }

    // підписи над дошками ("Your board", "Bot's board")
    void Scene::drawLabels() {
        glColor3f(0.0f, 0.0f, 0.0f);
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        char buffer[256];
        sprintf(buffer, "Your hits: %d", playerHits);
        renderText(30, 60, buffer);
        sprintf(buffer, "Your board");
        renderText(windowWidth / 2 - 200, (int) playerBoard->getOffsetY(), buffer);
        sprintf(buffer, "Bot's board");
        renderText(windowWidth / 2 + 100, (int) computerBoard->getOffsetY(), buffer);
    }

    // показує, чий зараз хід або хто виграв
    void Scene::drawGameStatus() {
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        glColor3f(0.0f, 0.0f, 0.0f);
        char buffer[256];
        if (!gameOver) {
            sprintf(buffer, "%s", playerTurn ? "Your turn" : "Bot's turn");
            renderText(windowWidth / 2 - 50, 30, buffer);
        } else {
            glColor3f(1.0f, 0.0f, 0.0f);
            sprintf(buffer, "%s", playerWon ? "You won!" : "Bot won!");
            renderText(windowWidth / 2 - 50, 30, buffer);
        }
    }

    // HUD-текст поверх сцени
    void Scene::drawTextOverlay() {
        drawLabels(); // підписи над дошками
        drawGameStatus(); // інфа про стан гри
    }

    // обробка кліку мишкою по дошці комп’ютера
    void Scene::handleMouseClick(float x, float y) {
        if (gameOver || !playerTurn || waitingForBot) return;

        // гравець стріляє по комп'ютеру
        if (computerBoard->makeShot(x, y)) {
            if (computerBoard->wasLastShotHit()) {
                playerHits++;
            }
            if (computerBoard->isGameOver()) {
                gameOver = true;
                playerWon = true;
                return;
            }
            playerTurn = false;
            waitingForBot = true;
            glutTimerFunc(1500, [](int) { sceneForTimer->handleComputerTurn(); }, 0);
        }
    }

    // хід комп’ютера (рандомний постріл по дошці гравця)
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

    // повертаємо координати дощок (для центрування)
    float Scene::getPlayerBoardX() const { return playerBoard->getOffsetX(); }
    float Scene::getComputerBoardX() const { return computerBoard->getOffsetX(); }
    float Scene::getPlayerBoardY() const { return playerBoard->getOffsetY(); }

    // основна функція перемальовки сцени
    void Scene::on_paint() {
        glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

        // налаштування освітлення
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

        // Set up perspective projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, glutGet(GLUT_WINDOW_WIDTH) / (float) glutGet(GLUT_WINDOW_HEIGHT),
                       1.0, 3000.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // Position camera
        glTranslatef(0.0f, 0.0f, distZ);
        glRotatef(angleX, 0.0f, 1.0f, 0.0f);
        glRotatef(angleY, 1.0f, 0.0f, 0.0f);

        // Центруємо сцену так, щоб обидві дошки були посередині екрану:
        // - по X — між двома дошками (із запасом у 5 клітинок, бо кораблі виходять за межі),
        // - по Y — піднімаємо сцену вгору (бо за замовчуванням центр внизу),
        // - по Z — відносно позиції дошки гравця, щоб усе було симетрично.
        float centerX = (getPlayerBoardX() + getComputerBoardX()) / 2.0f + 5 * cellSize;
        float centerZ = getPlayerBoardY();
        glTranslatef(-centerX, centerZ, -centerZ);

        // Вмикаємо глибину і освітлення для 3D-вигляду
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        draw(); // Малюємо всі об'єкти сцени (дошки, кораблі)

        // зберігаємо матриці для визначення координати кліку
        glGetDoublev(GL_MODELVIEW_MATRIX, lastModelview);
        glGetDoublev(GL_PROJECTION_MATRIX, lastProjection);
        glGetIntegerv(GL_VIEWPORT, lastViewport);

        // перемикаємось на ортографічну проекцію для HUD
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        drawTextOverlay(); // текстові елементи

        glEnable(GL_DEPTH_TEST);
        glFlush();
        glutSwapBuffers();
    }

    // обертання сцени при натиснутій правій кнопці миші
    void Scene::on_motion(int x, int y) {
        if (button == GLUT_RIGHT_BUTTON) {
            angleX += (x - mouseX);
            angleY += (y - mouseY);
            mouseX = x;
            mouseY = y;
            glutPostRedisplay();
        }
    }

    // обробка кліків миші (стрільба або обертання)
    void Scene::on_mouse(int button, int state, int x, int y) {
        mouseX = x;
        mouseY = y;
        if (state == GLUT_UP) {
            this->button = -1;
            return;
        }

        this->button = button;

        if (button == GLUT_LEFT_BUTTON) {
            // визначаємо позицію кліку в 3D-просторі
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

    // обробка функціональних клавіш (зум)
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
