#ifndef SEABATTLE_SHAPE_H
#define SEABATTLE_SHAPE_H

namespace SeaBattle {

class Shape {
protected:
    float x, y;
    float width, height;
    float r, g, b;

public:
    Shape(float x, float y, float width, float height);
    virtual ~Shape() = default;

    virtual void draw() = 0;
    virtual bool contains(float px, float py) const;
    
    void setColor(float r, float g, float b);
    void setPosition(float x, float y);
    void setSize(float width, float height);
};

} // namespace SeaBattle

#endif // SEABATTLE_SHAPE_H
