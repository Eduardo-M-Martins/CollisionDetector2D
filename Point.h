#ifndef Ponto_hpp
#define Ponto_hpp

#include <cmath>
#include <iostream>

using namespace std;

class Point {

public:
    float x,y,z;
    Point ();
    Point(float x, float y, float z=0);
    void set(float x, float y, float z=0);
    double magnitude();
    void rotateZ(float angle);
};

Point getMin (Point P1, Point P2);
Point getMax (Point P1, Point P2);

Point operator+(Point P1, Point P2);
Point operator- (Point P1, Point P2);
Point operator* (Point P1, float k);

#endif