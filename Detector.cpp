#include <regex>

using namespace std;

#include "Point.h"
#include "Polygon.h"

Polygon ScenarioPoints, FieldVision, Triangle, GreenPoint, YellowPoint;
Point Min, Max, Size, Middle, PosFieldVision;
string modeDesc=" | Mode('x'): Brute force", colorDesc=" | Color('c'): OFF", detailDesc=" | Details(' '): OFF", infoDesc="-> Points : ";

vector<Polygon> leafs;
vector<int> idxt;

float angFieldVision=0.0, moveScale=2, coordMax=500;
int fileNpts=0, modeStatus=0, treeNvalue=10;
bool colorStatus=false, lineStatus=false;

bool isNumeric(string const &str){
    auto it = find_if(str.begin(), str.end(), [](char const &c){ return !isdigit(c); });
    return !str.empty() && it == str.end();
}

void createPoints(unsigned long int qtd, Point Min, Point Max){
    time_t t;
    Point Scale;
    Scale = (Max-Min) * (1.0/1000.0);
    srand((unsigned)time(&t));
    for (int i=0; i<qtd; i++){
        float x = rand() % 1000;
        float y = rand() % 1000;
        x = x*Scale.x + Min.x;
        y = y*Scale.y + Min.y;
        ScenarioPoints.addVertice(Point(x, y));
    }
}

void createTriangle(){
    Point vector = Point(1, 0, 0);

    Triangle.addVertice(Point(0, 0, 0));
    FieldVision.addVertice(Point(0, 0, 0));

    vector.rotateZ(45);
    Triangle.addVertice(vector);
    FieldVision.addVertice(vector);

    vector.rotateZ(-90);
    Triangle.addVertice(vector);
    FieldVision.addVertice(vector);
}

void setTriangle(){
    float size = Size.x*0.25;
    Point temp;
    for (int i=0; i<Triangle.getNVertices(); i++){
        temp = Triangle.getVertice(i);
        temp.rotateZ(angFieldVision);
        FieldVision.setVertice(i, PosFieldVision + temp * size);
    }
}

void setFieldVision(float dist){
    Point vector = Point(1, 0, 0);
    vector.rotateZ(angFieldVision);
    PosFieldVision = PosFieldVision + vector * dist;
}

struct Node{
    Polygon e;
    Point eMax, eMin;
    vector<Node> subNode;
    vector<int> idx;
    int nPoints = -1, height;
    bool leaf = false;
};
Node Tree;

void createQuadTree(Point Max, Point Min, Node *node, int height){
    int r = 0;
    node->height=height;
    height++;
    Point Midd = (Max + Min) * 0.5;
    vector<int> aux;
    for (int i=0; i<ScenarioPoints.getNVertices(); i++){
        if (ScenarioPoints.getVertice(i).x >= Min.x && ScenarioPoints.getVertice(i).x <= Max.x && ScenarioPoints.getVertice(i).y >= Min.y && ScenarioPoints.getVertice(i).y <= Max.y){
            r++;
            aux.push_back(i);
        }
    }

    node->e.addVertice(Point(Min.x, Min.y, 0.0));
    node->e.addVertice(Point(Max.x, Min.y, 0.0));
    node->e.addVertice(Point(Max.x, Max.y, 0.0));
    node->e.addVertice(Point(Min.x, Max.y, 0.0));

    if ((r>=treeNvalue) && (node->nPoints==-1) && height<7){
        node->nPoints = r;

        for (int i=0; i<4; i++){
            Node n;
            node->subNode.push_back(n);
        }
        Point Ma, Mi;
        Mi = (Point(Min.x, Midd.y));
        Ma = (Point(Midd.x, Max.y));
        createQuadTree(Ma, Mi, &(node->subNode.at(0)),height);
        Mi = (Point(Midd.x, Midd.y));
        Ma = (Point(Max.x, Max.y));
        createQuadTree(Ma, Mi, &(node->subNode.at(1)),height);
        Mi = (Point(Midd.x, Min.y));
        Ma = (Point(Max.x, Midd.y));
        createQuadTree(Ma, Mi, &(node->subNode.at(2)),height);
        Mi = (Point(Min.x, Min.y));
        Ma = (Point(Midd.x, Midd.y));
        createQuadTree(Ma, Mi, &(node->subNode.at(3)),height);
    } else {
        node->leaf = true;
        node->idx=aux;
        node->nPoints = r;
        node->e.getLimits(node->eMin, node->eMax);
        leafs.push_back(node->e);
    }
}

void init(string read){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if (read.empty()==1){
        cout << "\n" << fileNpts << " random points were created!\n" << endl;
        createPoints(fileNpts, Point(0, 0), Point(coordMax, coordMax));
    }

    ScenarioPoints.getLimits(Min, Max);
    Min.x--;
    Min.y--;
    Max.x++;
    Max.y++;

    Middle = (Max + Min) * 0.5;
    Size = (Max - Min);
    moveScale = Size.magnitude() * 0.004;

    PosFieldVision = Middle;
    angFieldVision = 0;

    idxt.clear();
    leafs.clear();
    createQuadTree(Max, Min, &Tree, 0);

    createTriangle();
    setTriangle();
}

void getIdxt(Node T){

    if(!T.leaf){
        getIdxt(T.subNode.at(0));
        getIdxt(T.subNode.at(1));
        getIdxt(T.subNode.at(2));
        getIdxt(T.subNode.at(3));
    } else {
        Point Emin, Emax;
        FieldVision.getLimits(Emin, Emax);

        if((T.eMin.x <= Emax.x && T.eMax.x >= Emin.x && T.eMin.y <= Emax.y && T.eMax.y >= Emin.y)){
            idxt.insert(idxt.begin(), T.idx.begin(), T.idx.end());
        }
    }
    auto end = idxt.end();
    for (auto it = idxt.begin(); it != end; ++it) {
        end = remove(it + 1, end, *it);
    }
    idxt.erase(end, idxt.end());
}

int quad(){
    Point min, max;
    bool isGreen;
    FieldVision.getLimits(min, max);
    GreenPoint.clear();
    YellowPoint.clear();

    Point v1 = operator-(FieldVision.getVertice(1), FieldVision.getVertice(0));
    Point v2 = operator-(FieldVision.getVertice(2), FieldVision.getVertice(1));
    Point v3 = operator-(FieldVision.getVertice(0), FieldVision.getVertice(2));

    int r = 0;

    #pragma omp parallel for reduction(+ : r) schedule(dynamic, 8)
    for (int i=0; i<idxt.size(); i++){
        isGreen = false;
        Point ponto = ScenarioPoints.getVertice(idxt.at(i));
        if (ponto.x > min.x && ponto.x < max.x && ponto.y > min.y && ponto.y < max.y){
            Point va = operator-(ponto, FieldVision.getVertice(0));
            if ((v1.x * va.y) - (v1.y * va.x) < 0){
                va = operator-(ponto, FieldVision.getVertice(1));
                if ((v2.x * va.y) - (v2.y * va.x) < 0){
                    va = operator-(ponto, FieldVision.getVertice(2));
                    if ((v3.x * va.y) - (v3.y * va.x) < 0){
                        isGreen = true;
                        r++;
                    }
                }
            }
        }
        if(isGreen){
            #pragma omp critical
            {
                GreenPoint.addVertice(ponto);
            }
        }else{
            #pragma omp critical
            {
                YellowPoint.addVertice(ponto);
            }
        }
    }
    return r;
}

int envelope()
{
    Point min, max;
    bool isGreen;
    FieldVision.getLimits(min, max);
    GreenPoint.clear();
    YellowPoint.clear();

    Point v1 = operator-(FieldVision.getVertice(1), FieldVision.getVertice(0));
    Point v2 = operator-(FieldVision.getVertice(2), FieldVision.getVertice(1));
    Point v3 = operator-(FieldVision.getVertice(0), FieldVision.getVertice(2));

    int r = 0;

    #pragma omp parallel for reduction(+ : r) schedule(dynamic, 8)
    for (int i = 0; i < ScenarioPoints.getNVertices(); i++){
        Point ponto = ScenarioPoints.getVertice(i);
        if (ponto.x > min.x && ponto.x < max.x && ponto.y > min.y && ponto.y < max.y){
            isGreen = false;
            Point va = operator-(ponto, FieldVision.getVertice(0));
            if ((v1.x * va.y) - (v1.y * va.x) < 0){
                va = operator-(ponto, FieldVision.getVertice(1));
                if ((v2.x * va.y) - (v2.y * va.x) < 0){
                    va = operator-(ponto, FieldVision.getVertice(2));
                    if ((v3.x * va.y) - (v3.y * va.x) < 0){
                        isGreen = true;
                        r++;
                    }
                }
            }
            if(isGreen){
                #pragma omp critical
                {
                    GreenPoint.addVertice(ponto);
                }
            }else{
                #pragma omp critical
                {
                    YellowPoint.addVertice(ponto);
                }
            }
        }
    }
    return r;
}

int bruteForce()
{   
    int r = 0;
    GreenPoint.clear();
    Point v1 = operator-(FieldVision.getVertice(1), FieldVision.getVertice(0));
    Point v2 = operator-(FieldVision.getVertice(2), FieldVision.getVertice(1));
    Point v3 = operator-(FieldVision.getVertice(0), FieldVision.getVertice(2));
    #pragma omp parallel for reduction(+ : r) schedule(dynamic, 8)
    for (int i = 0; i < ScenarioPoints.getNVertices(); i++){
        Point va = operator-(ScenarioPoints.getVertice(i), FieldVision.getVertice(0));
        if ((v1.x * va.y) - (v1.y * va.x) < 0){
            va = operator-(ScenarioPoints.getVertice(i), FieldVision.getVertice(1));
            if ((v2.x * va.y) - (v2.y * va.x) < 0){
                va = operator-(ScenarioPoints.getVertice(i), FieldVision.getVertice(2));
                if ((v3.x * va.y) - (v3.y * va.x) < 0){
                    #pragma omp critical
                    {
                        GreenPoint.addVertice(ScenarioPoints.getVertice(i));
                    }
                    r++;
                }
            }
        }
    }
    return r;
}

int exe(){
    if (modeStatus == 0){
        return bruteForce();
    } else if (modeStatus == 1){
        return envelope();
    } else {
        idxt.clear();
        getIdxt(Tree);
        return quad();
    }
}

// Reshape
void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glOrtho(Min.x, Max.x, Min.y, Max.y, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Display
void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPointSize(2);
    if (colorStatus){
        glColor3f(1, 0, 0);
    } else {
        glColor3f(1, 1, 1);
    }

    ScenarioPoints.drawVertices();

    if (colorStatus){
        if (modeStatus == 1||modeStatus==2){
            for (int i = 0; i < YellowPoint.getNVertices(); i++){
                Point P;
                P = YellowPoint.getVertice(i);
                glBegin(GL_POINTS);
                glColor3f(1, 1, 0);
                glVertex3f(P.x, P.y, P.z);
                glEnd();
            }
        }
        for (int i = 0; i < GreenPoint.getNVertices(); i++){
            Point P;
            P = GreenPoint.getVertice(i);
            glBegin(GL_POINTS);
            glColor3f(0, 0.75, 0);
            glVertex3f(P.x, P.y, P.z);
            glEnd();
        }
    }

    if(lineStatus && modeStatus == 2){
        glLineWidth(2);
        glColor3f(0.5, 0.5, 0.5);
        for (Polygon p : leafs){
            p.drawPolygon();
        }
    }

    if ((modeStatus == 1 && lineStatus)||(modeStatus == 2 && lineStatus)){
        glLineWidth(2);
        glColor3f(0, 0, 1);
        FieldVision.createEnvelope(lineStatus);
    }

    glLineWidth(3);
    glColor3f(1, 0, 0);
    FieldVision.drawPolygon();

    glutSwapBuffers();
}

void posFieldVision(int n)
{
 switch (n) {
    case 1:
        angFieldVision = 0;
        PosFieldVision = Middle;
        break;
    case 2:
        angFieldVision = 90;
        PosFieldVision = Middle;
        break;
    case 3:
        angFieldVision = 90;
        PosFieldVision = Middle*0.5;
        break;
    case 4:
        angFieldVision = 0;
        PosFieldVision = Middle + Middle*0.5;
        break;
    default:
        break;
 }
 
 setTriangle();
}

void changeNvalue(){
    string verify = "";
    cout << "Input the new N value (min 5):" << endl;
    cout << "\n -> ";
    getline(cin, verify);
    if ((isNumeric(verify))){
        if (stoi(verify) < 5){
            treeNvalue=5;
        } else {
            treeNvalue = stoi(verify);
        }
        idxt.clear();
        leafs.clear();
        Node New;
        Tree=New;
        createQuadTree(Max, Min, &Tree, 0);
    }
}

void zoom(int i){
    if (i == 1){
        Triangle.setVertice(1, (operator+(Triangle.getVertice(1), (operator*(Triangle.getVertice(1), 0.05)))));
        Triangle.setVertice(2, (operator+(Triangle.getVertice(2), (operator*(Triangle.getVertice(2), 0.05)))));
    } else {
        Triangle.setVertice(1, (operator*(Triangle.getVertice(1), 0.95)));
        Triangle.setVertice(2, (operator*(Triangle.getVertice(2), 0.95)));
    }
}

void keyboard(unsigned char key, int x, int y){
    switch (key){
    case 27:
        system("clear");
        exit(0);
        break;

    case '+':
        zoom(1);
        break;

    case '-':
        zoom(0);
        break;

    case 'c':
        if (colorStatus){
            colorDesc = " | Color('c'): OFF";
        } else {
            colorDesc = " | Color('c'): ON";
        }
        colorStatus = !colorStatus;
        break;

    case 'x':
        if (lineStatus){
            lineStatus = !lineStatus;
            detailDesc = " | Details(' '): OFF";
        }
        if (colorStatus){
            colorStatus = !colorStatus;
            colorDesc = " | Color('c'): OFF";
        }
        if (modeStatus == 0){
            modeStatus = 1;
            modeDesc = " | Mode('x'): Envelope";
        } else if (modeStatus == 1){
            modeStatus = 2;
            modeDesc = " | Mode('x'): QuadTree";
        } else if (modeStatus = 2){
            modeStatus=0;
            modeDesc = " | Mode('x'): Brute force";
        }
        break;

    case ' ':
        if (lineStatus){
            detailDesc = " | Details(' '): OFF";
        } else {
            detailDesc = " | Details(' '): ON";
        }
        lineStatus = !lineStatus;
        break;

    case 'n':
        changeNvalue();
        break;

    case '1':
    case '2':
    case '3':
    case '4':
        int i;
        i = key - '0';
        posFieldVision(i);
        break;

    default:
        break;
    }

    setTriangle();
    cout << infoDesc << exe() <<  modeDesc << detailDesc << colorDesc << endl;
    glutPostRedisplay();
}

void arrow_keys(int a_keys, int x, int y){
    switch (a_keys){
    case GLUT_KEY_LEFT:
        angFieldVision += 2;
        break;
    case GLUT_KEY_RIGHT:
        angFieldVision -= 2;
        break;
    case GLUT_KEY_UP:
        setFieldVision(moveScale);
        break;
    case GLUT_KEY_DOWN:
        setFieldVision(moveScale * (-1));
        break;
    default:
        break;
    }

    setTriangle();
    cout << infoDesc << exe() <<  modeDesc << detailDesc << colorDesc << endl;
    glutPostRedisplay();
}

void readFile(string *name){
    ifstream input;
    input.open(*name, ios::in);
    if (!input){
        cout << "Unable to find the file " << *name << "... Finishing process!\n" << endl;
        exit(0);
    }

    cout << "Reading the file... " << *name << "...";
    string S;
    unsigned int nVertices;
    input >> nVertices;

    for (int i = 0; i < nVertices; i++){
        double x, y;
        input >> x >> y;
        if (!input)
            break;
        ScenarioPoints.addVertice(Point(x, y));
    }
    cout << " Done!\n" << endl;
    moveScale = abs(coordMax * 0.004);
}

int main(int argc, char **argv){
    system("export OMP_NUM_THREADS=8");
    system("clear");
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "-                      OpenGL Program                       -" << endl;
    cout << "- Eduardo Machado Martins & Nathan dos Reis Ramos de Mello  -" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "-  'Esc'...End program                                      -\n-  '+/-'...Change field of vision size                      -\n-  ' '.....Show details                                     -\n-  'c'.....Points with colors                               -\n-  'x'.....Change algorithm                                 -\n-  'n'.....Change Tree N value                              -" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n" << endl;
    cout << "Input the file name or press Enter to random points:" << endl;
    cout << "\n -> ";

    string input = "";
    getline(cin, input);

    if (input.empty() == 0){
        readFile(&input);
    } else {
        string verify = "";
        cout << "\nInput the number of random points (min 10):" << endl;
        cout << "\n -> ";
        getline(cin, verify);
        if ((!isNumeric(verify))){
            fileNpts = 10;
        } else {
            if (stoi(verify) < 10){
                fileNpts = 10;
            } else {
                fileNpts = stoi(verify);
            }
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Field of vision - OpenGL");
    init(input);

    cout << infoDesc << exe() <<  modeDesc << detailDesc << colorDesc << endl;

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow_keys);
    glutMainLoop();

    return 0;
}