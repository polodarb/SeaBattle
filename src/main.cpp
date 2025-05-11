#include <GL/glut.h>
#include "../include/Scene.h"

namespace SeaBattle {
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 800;
    const int CELL_SIZE = 40;
    const int BOARD_OFFSET_X = 100;
    const int BOARD_OFFSET_Y = 150;

    // Параметры камеры
    static float angleX = -20.0f;
    static float angleY = 30.0f;
    static float distZ = -1000.0f;
    static int lastMouseX = 0, lastMouseY = 0;
    static int currentButton = -1;

    Scene *scene;

    void init() {
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        float ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
        float diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
        float specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

        scene = new Scene(BOARD_OFFSET_X, BOARD_OFFSET_Y, CELL_SIZE);
        scene->init();
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Установка проекции и камеры
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (double) glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 1.0, 3000.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // Источник света (направленный из камеры)
        float lightPos[] = {0.0f, 0.0f, -1.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        // Трансформации камеры
        glTranslatef(0.0f, 0.0f, distZ);
        glRotatef(angleX, 0.0f, 1.0f, 0.0f);
        glRotatef(angleY, 1.0f, 0.0f, 0.0f);
        float centerX = (scene->getPlayerBoardX() + scene->getComputerBoardX()) / 2.0f + 5 * CELL_SIZE;
        float centerZ = scene->getPlayerBoardY() + 5 * CELL_SIZE;
        glTranslatef(-centerX, -10.0f, -centerZ);

        // Включаем освещение и рисуем 3D-сцену
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        scene->draw(); // рисование игровых объектов (доски и корабли)
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);

        // Переключаемся в 2D для отображения текста
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);
        scene->drawTextOverlay();
        glEnable(GL_DEPTH_TEST);

        glutSwapBuffers();
    }

    void mouse(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            GLint viewport[4];
            GLdouble modelview[16], projection[16];
            GLfloat winX, winY, winZ;
            GLdouble posX, posY, posZ;

            glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
            glGetDoublev(GL_PROJECTION_MATRIX, projection);
            glGetIntegerv(GL_VIEWPORT, viewport);

            winX = static_cast<float>(x);
            winY = static_cast<float>(viewport[3] - y);
            glReadPixels(x, viewport[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

            gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

            scene->handleMouseClick((float)posX, (float)posZ); // ⚠️ Z — это наша глубина, Z → Y
            glutPostRedisplay();
        }

        if (button == GLUT_RIGHT_BUTTON) {
            if (state == GLUT_DOWN) {
                currentButton = GLUT_RIGHT_BUTTON;
                lastMouseX = x;
                lastMouseY = y;
            } else if (state == GLUT_UP) {
                currentButton = -1;
            }
        }
    }

    void motion(int x, int y) {
        if (currentButton == GLUT_RIGHT_BUTTON) {
            angleX += (float) (x - lastMouseX);
            angleY += (float) (y - lastMouseY);
            lastMouseX = x;
            lastMouseY = y;
            glutPostRedisplay();
        }
    }

    void special(int key, int, int) {
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

    void reshape(int w, int h) {
        if (h == 0) h = 1;
        glViewport(0, 0, w, h);
    }
} // namespace SeaBattle

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(SeaBattle::WINDOW_WIDTH, SeaBattle::WINDOW_HEIGHT);
    glutCreateWindow("Sea Battle");

    SeaBattle::init();

    glutDisplayFunc(SeaBattle::display);
    glutMouseFunc(SeaBattle::mouse);
    glutMotionFunc(SeaBattle::motion);
    glutSpecialFunc(SeaBattle::special);
    glutReshapeFunc(SeaBattle::reshape);

    glutMainLoop();

    delete SeaBattle::scene;
    return 0;
}
