#include "../include/Utils.h"

namespace SeaBattle {
    void drawParallelepiped(float length, float height, float depth) {
        float lx = length / 2.0f;
        float ly = height / 2.0f;
        float lz = depth / 2.0f;
        glBegin(GL_QUADS);
          // Top face (+Y)
          glNormal3f(0.0f, 1.0f, 0.0f);
          glVertex3f(-lx,  ly, -lz);
          glVertex3f(-lx,  ly,  lz);
          glVertex3f( lx,  ly,  lz);
          glVertex3f( lx,  ly, -lz);
          // Bottom face (-Y)
          glNormal3f(0.0f, -1.0f, 0.0f);
          glVertex3f(-lx, -ly, -lz);
          glVertex3f( lx, -ly, -lz);
          glVertex3f( lx, -ly,  lz);
          glVertex3f(-lx, -ly,  lz);
          // Front face (+Z)
          glNormal3f(0.0f, 0.0f, 1.0f);
          glVertex3f(-lx, -ly,  lz);
          glVertex3f( lx, -ly,  lz);
          glVertex3f( lx,  ly,  lz);
          glVertex3f(-lx,  ly,  lz);
          // Back face (-Z)
          glNormal3f(0.0f, 0.0f, -1.0f);
          glVertex3f(-lx, -ly, -lz);
          glVertex3f(-lx,  ly, -lz);
          glVertex3f( lx,  ly, -lz);
          glVertex3f( lx, -ly, -lz);
          // Left face (-X)
          glNormal3f(-1.0f, 0.0f, 0.0f);
          glVertex3f(-lx, -ly, -lz);
          glVertex3f(-lx, -ly,  lz);
          glVertex3f(-lx,  ly,  lz);
          glVertex3f(-lx,  ly, -lz);
          // Right face (+X)
          glNormal3f(1.0f, 0.0f, 0.0f);
          glVertex3f( lx, -ly, -lz);
          glVertex3f( lx,  ly, -lz);
          glVertex3f( lx,  ly,  lz);
          glVertex3f( lx, -ly,  lz);
        glEnd();
    }

    void drawPyramid(float baseSize, float height) {
        float half = baseSize / 2.0f;
        float tipZ =  height / 2.0f;
        float baseZ = -height / 2.0f;
        glBegin(GL_TRIANGLES);
          // Front triangle
          glNormal3f(0.0f, 0.0f, 1.0f);
          glVertex3f(-half, -half, baseZ);
          glVertex3f( half, -half, baseZ);
          glVertex3f( 0.0f,  0.0f, tipZ);
          // Right triangle
          glNormal3f(1.0f, 0.0f, 0.0f);
          glVertex3f( half, -half, baseZ);
          glVertex3f( half,  half, baseZ);
          glVertex3f( 0.0f,  0.0f, tipZ);
          // Back triangle
          glNormal3f(0.0f, 0.0f, -1.0f);
          glVertex3f( half,  half, baseZ);
          glVertex3f(-half,  half, baseZ);
          glVertex3f( 0.0f,  0.0f, tipZ);
          // Left triangle
          glNormal3f(-1.0f, 0.0f, 0.0f);
          glVertex3f(-half,  half, baseZ);
          glVertex3f(-half, -half, baseZ);
          glVertex3f( 0.0f,  0.0f, tipZ);
        glEnd();
    }
} // namespace SeaBattle
