#include <GL/glut.h>
#include "../include/Scene.h"

SeaBattle::Scene *scene;

void on_paint() { scene->on_paint(); }
void on_size(int width, int height) { glViewport(0, 0, width, height); }
void on_mouse(int button, int state, int x, int y) { scene->on_mouse(button, state, x, y); }
void on_motion(int x, int y) { scene->on_motion(x, y); }
void on_special(int key, int, int) { scene->on_special(key); }

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Sea Battle");

    scene = new SeaBattle::Scene(100.0f, 150.0f, 40.0f);
    scene->init();

    glutDisplayFunc(on_paint);
    glutMouseFunc(on_mouse);
    glutMotionFunc(on_motion);
    glutReshapeFunc(on_size);
    glutSpecialFunc(on_special);
    glutMainLoop();

    delete scene;
    return 0;
}
