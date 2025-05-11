#include "../include/Cell.h"
#include <GL/glut.h>

namespace SeaBattle {
    Cell::Cell(float x, float y, float size)
        : Shape(x, y, size, size), state(0) {
    }

    void Cell::draw() {
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    }

    void Cell::setState(int newState) {
        state = newState;
    }
} // namespace SeaBattle
