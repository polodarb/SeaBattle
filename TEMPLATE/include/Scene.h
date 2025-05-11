#ifndef Scene_h
#define Scene_h

#include "Shape.h"
#include "Disk.h"
#include <vector>

namespace DiskGame {
    const int M = 3, N = 3;

    class Scene {
        std::vector<Shape *> shapes;
        int button;
        float angleX, angleY;
        float mouseX, mouseY;
        float width, height;
        float distZ;
        bool finish;
        Disk *disks[N];
        float xStep, zStep;
        int time;
        int fields[M][N];
        int xFrom, zFrom;
        int xTo, zTo;

    public:
        Scene(float xStep, float zStep);

        ~Scene();

        void on_paint();

        void on_size(int width, int height);

        void on_mouse(int button, int state, int x, int y);

        void on_motion(int x, int y);

        void on_special(int key, int x, int y);

        void on_timer(int value);

    private:
        void initialize();

        void allocateDisks();

        bool moveDisk(int xFrom, int zFrom, int xTo, int zTo);

        void upDisk(int x, int z);

        void downAllDisks();

        bool findNearest(int x, int z, int &x1, int &z1);

        void resetArr();

        float allocX(int i);

        float allocZ(int i);
    };
}

#endif
