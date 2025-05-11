#include "../include/Cell.h"
#include <GL/glut.h>

namespace SeaBattle {

Cell::Cell(float x, float y, float size)
    : Shape(x, y, size, size), state(0) {
}

void Cell::draw() {
    // Draw cell background
    switch (state) {
        case 0: // Empty
            setColor(0.2f, 0.4f, 0.8f);
            break;
        case 1: // Ship (hidden)
            setColor(0.2f, 0.4f, 0.8f);
            break;
        case 2: // Hit
            setColor(1.0f, 0.0f, 0.0f);
            break;
        case 3: // Miss
            setColor(0.5f, 0.5f, 0.5f);
            break;
    }

    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw grid lines
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