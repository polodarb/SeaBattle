#include "../include/Utils.h"

namespace SeaBattle {

    // Малює прямокутний паралелепіпед зі сторонами length × height × depth.
    // Центр об'єкта — в точці (0, 0, 0), тому координати вершин рахуються відносно центру.
    void drawParallelepiped(float length, float height, float depth) {
        float lx = length / 2.0f;
        float ly = height / 2.0f;
        float lz = depth / 2.0f;

        glBegin(GL_QUADS);

          // Верхня грань (позитивна вісь Y)
          glNormal3f(0.0f, 1.0f, 0.0f);
          glVertex3f(-lx,  ly, -lz);
          glVertex3f(-lx,  ly,  lz);
          glVertex3f( lx,  ly,  lz);
          glVertex3f( lx,  ly, -lz);

          // Нижня грань (негативна вісь Y)
          glNormal3f(0.0f, -1.0f, 0.0f);
          glVertex3f(-lx, -ly, -lz);
          glVertex3f( lx, -ly, -lz);
          glVertex3f( lx, -ly,  lz);
          glVertex3f(-lx, -ly,  lz);

          // Передня грань (позитивна вісь Z)
          glNormal3f(0.0f, 0.0f, 1.0f);
          glVertex3f(-lx, -ly,  lz);
          glVertex3f( lx, -ly,  lz);
          glVertex3f( lx,  ly,  lz);
          glVertex3f(-lx,  ly,  lz);

          // Задня грань (негативна вісь Z)
          glNormal3f(0.0f, 0.0f, -1.0f);
          glVertex3f(-lx, -ly, -lz);
          glVertex3f(-lx,  ly, -lz);
          glVertex3f( lx,  ly, -lz);
          glVertex3f( lx, -ly, -lz);

          // Ліва грань (негативна вісь X)
          glNormal3f(-1.0f, 0.0f, 0.0f);
          glVertex3f(-lx, -ly, -lz);
          glVertex3f(-lx, -ly,  lz);
          glVertex3f(-lx,  ly,  lz);
          glVertex3f(-lx,  ly, -lz);

          // Права грань (позитивна вісь X)
          glNormal3f(1.0f, 0.0f, 0.0f);
          glVertex3f( lx, -ly, -lz);
          glVertex3f( lx,  ly, -lz);
          glVertex3f( lx,  ly,  lz);
          glVertex3f( lx, -ly,  lz);

        glEnd();
    }

    // Малює піраміду з квадратною основою:
    // Центр — у початку координат, вершина виступає вздовж осі Z.
    void drawPyramid(float baseSize, float height) {
        float half = baseSize / 2.0f;
        float tipZ =  height / 2.0f;
        float baseZ = -height / 2.0f;

        glBegin(GL_TRIANGLES);

          // Передня грань
          glNormal3f(0.0f, 0.0f, 1.0f);
          glVertex3f(-half, -half, baseZ);
          glVertex3f( half, -half, baseZ);
          glVertex3f( 0.0f,  0.0f, tipZ);

          // Права грань
          glNormal3f(1.0f, 0.0f, 0.0f);
          glVertex3f( half, -half, baseZ);
          glVertex3f( half,  half, baseZ);
          glVertex3f( 0.0f,  0.0f, tipZ);

          // Задня грань
          glNormal3f(0.0f, 0.0f, -1.0f);
          glVertex3f( half,  half, baseZ);
          glVertex3f(-half,  half, baseZ);
          glVertex3f( 0.0f,  0.0f, tipZ);

          // Ліва грань
          glNormal3f(-1.0f, 0.0f, 0.0f);
          glVertex3f(-half,  half, baseZ);
          glVertex3f(-half, -half, baseZ);
          glVertex3f( 0.0f,  0.0f, tipZ);

        glEnd();
    }

} // namespace SeaBattle
