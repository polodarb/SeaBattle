#include <GL/glut.h>
#include "../include/Scene.h"

namespace SeaBattle {

const int WINDOW_WIDTH = 1200;  // Increased width for two boards
const int WINDOW_HEIGHT = 800;  // Increased height for better visibility
const int CELL_SIZE = 40;
const int BOARD_OFFSET_X = 100;
const int BOARD_OFFSET_Y = 150;  // Moved down to make room for text

Scene* scene;

void init() {
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);  // Light gray background
    scene = new Scene(BOARD_OFFSET_X, BOARD_OFFSET_Y, CELL_SIZE);
    scene->init();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    scene->draw();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        scene->handleMouseClick(static_cast<float>(x), static_cast<float>(y));
        glutPostRedisplay();
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
}

} // namespace SeaBattle

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SeaBattle::WINDOW_WIDTH, SeaBattle::WINDOW_HEIGHT);
    glutCreateWindow("Sea Battle");
    
    SeaBattle::init();
    
    glutDisplayFunc(SeaBattle::display);
    glutMouseFunc(SeaBattle::mouse);
    glutReshapeFunc(SeaBattle::reshape);
    
    glutMainLoop();
    
    delete SeaBattle::scene;
    return 0;
}
