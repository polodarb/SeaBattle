#ifndef SEABATTLE_SCENE_H
#define SEABATTLE_SCENE_H

#include "Board.h"
#include <GL/glut.h>

namespace SeaBattle {
    class Scene {
        static const int BOARD_SPACING = 50; // відстань між дошками
        bool waitingForBot = false; // чи зараз хід бота
        bool playerWon; // виграв гравець чи ні
        int playerHits = 0; // кількість попадань твоих

        Board *playerBoard; // дошка гравця
        Board *computerBoard; // дошка комп'ютера

        bool gameOver; // чи гра завершена
        bool playerTurn; // чи зараз хід гравця

        float cellSize; // розмір клітинки
        int button; // яка кнопка миші натиснута
        float mouseX, mouseY; // координати миші
        float angleX, angleY; // кути повороту сцени
        float distZ; // віддалення камери

        // матриці та в’юпорт, щоб визначити, куди саме клацнув користувач у 3D-сцені
        // в методичці такого нема, бо там об'єкти жорстко задані, а тут клітинки вільно в просторі
        GLdouble lastModelview[16];
        GLdouble lastProjection[16];
        GLint lastViewport[4];

    public:
        Scene(float startX, float startY, float cellSize); // створення сцени з 2 дошками
        ~Scene(); // очищення пам'яті

        void init(); // ініціалізація OpenGL та логіки гри
        void draw(); // малює 3D-сцену (дошки тощо)

        void drawTextOverlay(); // малює текст поверх сцени (HUD):
        // викликає drawLabels() + drawGameStatus()
        // drawLabels() — це тільки підписи дошок
        // drawGameStatus() — це "твій хід", "бот виграв" і т.д.

        void handleMouseClick(float x, float y); // логіка пострілу гравця при натисканні миші

        // обробники подій GLUT
        void on_paint(); // перемальовка вікна
        void on_motion(int x, int y); // рух миші з натиснутою кнопкою
        void on_mouse(int button, int state, int x, int y); // натискання миші
        void on_special(int key); // клавіші типу стрілок

        float getPlayerBoardX() const; // повертає координату X дошки гравця
        float getComputerBoardX() const; // повертає координату X дошки бота
        float getPlayerBoardY() const;

        // повертає координату Y дошки (у всіх дошок вона однакова, тому для бота не треба окремо)

    private:
        void renderText(int x, int y, const char *text); // виводить текст на екран
        void drawBoards(); // малює дошки
        void drawLabels(); // підписи над дошками ("Your board", "Bot's board")
        void drawGameStatus(); // повідомлення про стан гри ("Your turn", "You won!" і т.д.)
        void handleComputerTurn(); // логіка ходу бота
    };
}

#endif // SEABATTLE_SCENE_H
