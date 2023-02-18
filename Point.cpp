#include "Point.h"

Point::Point(){
    x = y = z = 0;
}

Point::Point(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

void Point::set(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

void Point::rotateZ(float angle){
    float xr, yr;
    double angleRad = angle * 3.14159265359 / 180.0;
    xr = x * cos(angleRad) - y * sin(angleRad);
    yr = x * sin(angleRad) + y * cos(angleRad);
    x = xr;
    y = yr;
}

double Point::magnitude(){
    return sqrt(x * x + y * y + z * z);
}

Point getMax(Point P1, Point P2){
    Point Max;
    Max.x = (P2.x > P1.x) ? P2.x : P1.x;
    Max.y = (P2.y > P1.y) ? P2.y : P1.y;
    Max.z = (P2.z > P1.x) ? P2.z : P1.z;
    return Max;
}

Point getMin(Point P1, Point P2){
    Point Min;
    Min.x = (P2.x < P1.x) ? P2.x : P1.x;
    Min.y = (P2.y < P1.y) ? P2.y : P1.y;
    Min.z = (P2.z < P1.x) ? P2.z : P1.z;
    return Min;
}

Point operator+(Point P1, Point P2){
    Point temp;
    temp = P1;
    temp.x += P2.x;
    temp.y += P2.y;
    temp.z += P2.z;
    return temp;
}

Point operator-(Point P1, Point P2){
    Point temp;
    temp = P1;
    temp.x -= P2.x;
    temp.y -= P2.y;
    temp.z -= P2.z;
    return temp;
}

Point operator*(Point P1, float k){
    Point temp;
    temp.x = P1.x * k;
    temp.y = P1.y * k;
    temp.z = P1.z * k;
    return temp;
}