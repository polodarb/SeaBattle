#include <gl/glut.h>
#include "Board.h" // Ensure this header file properly defines the Board class and its members
#include "utils.h"

namespace DiskGame {
    void Board::draw() { // Ensure the Board class and its draw method are correctly declared in Board.h
        glMaterialfv(GL_FRONT, GL_AMBIENT, getAmbiColor());
        glMaterialfv(GL_FRONT, GL_DIFFUSE, getDiffColor());
        glMaterialfv(GL_FRONT, GL_SPECULAR, getSpecColor());
        glMaterialf(GL_FRONT, GL_SHININESS, GraphUtils::shininess);
        glPushMatrix();
        glTranslatef(getXCenter(), getYCenter(), getZCenter());
        GraphUtils::parallelepiped(getXSize(), getYSize(), getZSize());
        glPopMatrix();
    }
}
