#include "Polygon.h"

Polygon::Polygon(){
}

void Polygon::addVertice(Point p){
    Vertices.push_back(p);
}

Point Polygon::getVertice(int i){
    return Vertices[i];
}

void Polygon::drawPolygon(){
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < Vertices.size(); i++)
        glVertex3f(Vertices[i].x, Vertices[i].y, Vertices[i].z);
    glEnd();
}

void Polygon::drawVertices(){
    for (int i = 0; i < Vertices.size(); i++){
        glBegin(GL_POINTS);
        glVertex3f(Vertices[i].x, Vertices[i].y, Vertices[i].z);
    }
    glEnd();
}

unsigned long Polygon::getNVertices(){
    return Vertices.size();
}

void Polygon::getLimits(Point &Min, Point &Max){
    Max = Min = Vertices[0];
    for (int i = 0; i < Vertices.size(); i++)
    {
        Min = getMin(Vertices[i], Min);
        Max = getMax(Vertices[i], Max);
    }
}

void Polygon::setVertice(int i, Point P){
    Vertices[i] = P;
}

void Polygon::createEnvelope(bool lines){
    Polygon envelope;

    Point min, max;
    Point minXminY, maxXminY, minXmaxY, maxXmaxY;

    this->getLimits(min, max);

    minXminY.set(min.x, min.y, 0);
    maxXminY.set(max.x, min.y, 0);
    minXmaxY.set(min.x, max.y, 0);
    maxXmaxY.set(max.x, max.y, 0);

    envelope.addVertice(minXminY);
    envelope.addVertice(minXmaxY);
    envelope.addVertice(maxXmaxY);
    envelope.addVertice(maxXminY);

    if(lines)
        envelope.drawPolygon();
    //return envelope;
}

void Polygon::clear(){
    Vertices.clear();
}