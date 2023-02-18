#ifndef Ponto_hpp
#define Ponto_hpp

#include <cmath>
#include <iostream>

using namespace std;

class Ponto {

public:
    float x,y,z;
    Ponto ();
    Ponto(float x, float y, float z=0);
    void set(float x, float y, float z=0);
    double modulo();
    void rotacionaZ(float angulo);
};

Ponto ObtemMinimo (Ponto P1, Ponto P2);
Ponto ObtemMaximo (Ponto P1, Ponto P2);

Ponto operator+(Ponto P1, Ponto P2);
Ponto operator- (Ponto P1, Ponto P2);
Ponto operator* (Ponto P1, float k);

#endif