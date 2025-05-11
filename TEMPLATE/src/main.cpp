#define _CRT_SECURE_NO_WARNINGS
#include <gl/glut.h>
#include <stdio.h>
#include "Scene.h"
using DiskGame::Scene;

Scene *scene;

void on_paint() {
    scene->on_paint();
}

void on_size(int width, int height) {
    scene->on_size(width, height);
}

void on_mouse(int button, int state, int x, int y) {
    scene->on_mouse(button, state, x, y);
}

void on_motion(int x, int y) {
    scene->on_motion(x, y);
}

void on_special(int key, int x, int y) {
    // Викликаємо відповідну функцію класу Scene:
    scene->on_special(key, x, y);
}

void on_keyboard(unsigned char key, int x, int y) {
    // Обробка подій від клавіатури:
    if (key == 27)
        exit(0);
}

void on_timer(int value) {
    // Обробка події від таймера
    scene->on_timer(value);
    glutTimerFunc(25, on_timer, 0); // зa 25 мс викличеться ця функція
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    scene = new Scene(0.4, 0.4);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Disks");
    glutDisplayFunc(on_paint);
    glutReshapeFunc(on_size);
    glutMotionFunc(on_motion);
    glutMouseFunc(on_mouse);
    glutKeyboardFunc(on_keyboard);
    glutSpecialFunc(on_special);
    glutTimerFunc(25, on_timer, 0);
    glutMainLoop();
    delete scene;
    return (0);
}
