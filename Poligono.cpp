#include "Poligono.h"

Poligono::Poligono(){
}

void Poligono::insereVertice(Ponto p){
    Vertices.push_back(p);
}

Ponto Poligono::getVertice(int i){
    return Vertices[i];
}

void Poligono::desenhaPoligono(){
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < Vertices.size(); i++)
        glVertex3f(Vertices[i].x, Vertices[i].y, Vertices[i].z);
    glEnd();
}

void Poligono::desenhaVertices(){
    for (int i = 0; i < Vertices.size(); i++){
        glBegin(GL_POINTS);
        glVertex3f(Vertices[i].x, Vertices[i].y, Vertices[i].z);
    }
    glEnd();
}

unsigned long Poligono::getNVertices(){
    return Vertices.size();
}

void Poligono::obtemLimites(Ponto &Min, Ponto &Max){
    Max = Min = Vertices[0];
    for (int i = 0; i < Vertices.size(); i++)
    {
        Min = ObtemMinimo(Vertices[i], Min);
        Max = ObtemMaximo(Vertices[i], Max);
    }
}

void Poligono::alteraVertice(int i, Ponto P){
    Vertices[i] = P;
}

void Poligono::createEnvelope(bool lines){
    Poligono envelope;

    Ponto min, max;
    Ponto minXminY, maxXminY, minXmaxY, maxXmaxY;

    this->obtemLimites(min, max);

    minXminY.set(min.x, min.y, 0);
    maxXminY.set(max.x, min.y, 0);
    minXmaxY.set(min.x, max.y, 0);
    maxXmaxY.set(max.x, max.y, 0);

    envelope.insereVertice(minXminY);
    envelope.insereVertice(minXmaxY);
    envelope.insereVertice(maxXmaxY);
    envelope.insereVertice(maxXminY);

    if(lines)
        envelope.desenhaPoligono();
    //return envelope;
}

void Poligono::clear(){
    Vertices.clear();
}