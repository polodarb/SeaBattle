#include <GL/glut.h>
#include "../include/Shape.h"

namespace SeaBattle {
    void drawParallelepiped(float length, float height, float depth) {
        float lx = length / 2.0f;
        float ly = height / 2.0f;
        float lz = depth / 2.0f;
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-lx, ly, -lz); glVertex3f(-lx, ly,  lz);
        glVertex3f( lx, ly,  lz); glVertex3f( lx, ly, -lz);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-lx, -ly, -lz); glVertex3f( lx, -ly, -lz);
        glVertex3f( lx, -ly,  lz); glVertex3f(-lx, -ly,  lz);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-lx, -ly,  lz); glVertex3f( lx, -ly,  lz);
        glVertex3f( lx,  ly,  lz); glVertex3f(-lx,  ly,  lz);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-lx, -ly, -lz); glVertex3f(-lx,  ly, -lz);
        glVertex3f( lx,  ly, -lz); glVertex3f( lx, -ly, -lz);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-lx, -ly, -lz); glVertex3f(-lx, -ly,  lz);
        glVertex3f(-lx,  ly,  lz); glVertex3f(-lx,  ly, -lz);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f( lx, -ly, -lz); glVertex3f( lx,  ly, -lz);
        glVertex3f( lx,  ly,  lz); glVertex3f( lx, -ly,  lz);
        glEnd();
    }
}
