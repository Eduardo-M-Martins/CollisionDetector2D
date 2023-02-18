#ifndef Poligono_hpp
#define Poligono_hpp

#include <fstream>
#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

#include "Ponto.h"

class Poligono
{
    vector <Ponto> Vertices;
    Ponto Min, Max;
    
public:
    Poligono();
    Ponto getVertice(int);
    unsigned long getNVertices();
    void insereVertice(Ponto);
    void desenhaPoligono();
    void desenhaVertices();
    void obtemLimites(Ponto &Min, Ponto &Max);
    void alteraVertice(int i, Ponto P);
    void createEnvelope(bool lines);
    void clear();
};

#endif 