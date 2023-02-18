#ifndef Polygon_hpp
#define Polygon_hpp

#include <fstream>
#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

#include "Point.h"

class Polygon
{
    vector <Point> Vertices;
    Point Min, Max;
    
public:
    Polygon();
    Point getVertice(int);
    unsigned long getNVertices();
    void addVertice(Point);
    void drawPolygon();
    void drawVertices();
    void getLimits(Point &Min, Point &Max);
    void setVertice(int i, Point P);
    void createEnvelope(bool lines);
    void clear();
};

#endif 