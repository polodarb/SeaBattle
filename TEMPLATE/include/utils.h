#ifndef utils_h
#define utils_h

namespace GraphUtils {
    extern float diffWhite[], ambiWhite[], specWhite[];
    extern float diffBlue[], ambiBlue[], specBlue[];
    extern float diffGray[], ambiGray[], specGray[];
    extern float diffRed[], ambiRed[], specRed[];
    extern float diffYellow[], ambiYellow[], specYellow[];
    extern float diffGreen[], ambiGreen[], specGreen[];
    extern float diffOrange[], ambiOrange[], specOrange[];
    extern float diffLightBlue[], ambiLightBlue[], specLightBlue[];
    extern float diffViolet[], ambiViolet[], specViolet[];
    const float shininess = 64;

    void shuffle(int *a, int size);

    void parallelepiped(float length, float width, float height);

    void drawString(void *font, const char *text, float x, float y);
}

#endif
