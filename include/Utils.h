#ifndef SEABATTLE_GRAPHUTILS_H
#define SEABATTLE_GRAPHUTILS_H

#include <GL/glut.h>

namespace SeaBattle {

    // Малює паралелепіпед заданих розмірів (довжина × висота × глибина)
    // Використовується для малювання клітинок, кораблів, маркерів тощо
    void drawParallelepiped(float length, float height, float depth);

    // Малює піраміду з квадратною основою (наприклад, для носа корабля)
    // baseSize — довжина сторони основи, height — висота піраміди
    void drawPyramid(float baseSize, float height);
}

#endif // SEABATTLE_GRAPHUTILS_H
