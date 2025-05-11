#include <gl/glut.h>
#include "Disk.h"
#include "utils.h"

namespace DiskGame {
    void Disk::draw() {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getAmbiColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getDiffColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, getSpecColor());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, GraphUtils::shininess);
        glPushMatrix();
        glTranslatef(getXCenter(), getYCenter() + getYSize() / 2, getZCenter());
        glRotatef(90, 1, 0, 0);
        GLUquadricObj *quadricObj = gluNewQuadric();
        gluCylinder(quadricObj, getXSize() / 2, getXSize() / 2, getYSize(), 20, 2);
        glRotatef(180, 1, 0, 0);
        gluDisk(quadricObj, innerRadius, getXSize() / 2, 20, 20);
        glTranslatef(0, 0, -getYSize());
        gluDisk(quadricObj, innerRadius, getXSize() / 2, 20, 20);
        gluDeleteQuadric(quadricObj);
        glPopMatrix();
    }
}
