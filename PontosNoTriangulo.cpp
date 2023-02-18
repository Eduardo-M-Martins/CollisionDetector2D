// Autores: Eduardo Machado Martins & Nathan dos Reis Ramos de Mello
// Ler README.md

#include <regex>

using namespace std;

#include "Ponto.h"
#include "Poligono.h"

Poligono PontosCenario, CampoVisao, TrianguloBase, PontoVerde, PontoAmarelo;
Ponto Min, Max, Tamanho, Meio, PosicaoCampoVisao;
string modeDesc=" | Mode: Brute force", colorDesc=" | Color: OFF", detailDesc=" | Details: OFF", infoDesc="-> Nº BruteForce tests: ";

vector<Poligono> leafs;
vector<int> idxt;

float AnguloCampoVisao=0.0, moveScale=2, coordMax=500;
int fileNpts=0, modeStatus=0, infoStatus=0, treeNvalue=10;
bool colorStatus=false, lineStatus=false;

// Metodo que auxilia a imprimir os resultados
void clear(int i){
    for (int j=0; j<i; j++){
        system("tput cuu1;tput dl1");
    }
}

// Verifica se String é um numero
bool isNumeric(string const &str){
    auto it = find_if(str.begin(), str.end(), [](char const &c){ return !isdigit(c); });
    return !str.empty() && it == str.end();
}

// Metodo que gera pontos aleatorios no intervalo [Min..Max]
void GeraPontos(unsigned long int qtd, Ponto Min, Ponto Max){
    time_t t;
    Ponto Escala;
    Escala = (Max-Min) * (1.0/1000.0);
    srand((unsigned)time(&t));
    for (int i=0; i<qtd; i++){
        float x = rand() % 1000;
        float y = rand() % 1000;
        x = x*Escala.x + Min.x;
        y = y*Escala.y + Min.y;
        PontosCenario.insereVertice(Ponto(x, y));
    }
}

// Cria um triangulo a partir do vetor (1,0,0), girando este vetor em 45 e -45 graus.
void CriaTrianguloCampoVisao(){
    Ponto vetor = Ponto(1, 0, 0);

    TrianguloBase.insereVertice(Ponto(0, 0, 0));
    CampoVisao.insereVertice(Ponto(0, 0, 0));

    vetor.rotacionaZ(45);
    TrianguloBase.insereVertice(vetor);
    CampoVisao.insereVertice(vetor);

    vetor.rotacionaZ(-90);
    TrianguloBase.insereVertice(vetor);
    CampoVisao.insereVertice(vetor);
}

// Posiciona o campo de visão na posicao PosicaoCampoVisao com a orientacao "AnguloCampoVisao".
void PosicionaTrianguloCampoVisao(){
    float tamanho = Tamanho.x*0.25;
    Ponto temp;
    for (int i=0; i<TrianguloBase.getNVertices(); i++){
        temp = TrianguloBase.getVertice(i);
        temp.rotacionaZ(AnguloCampoVisao);
        CampoVisao.alteraVertice(i, PosicaoCampoVisao + temp * tamanho);
    }
}

// Move o campo de visão "distancia" unidades pra frente ou pra tras.
void AvancaCampoVisao(float distancia){
    Ponto vetor = Ponto(1, 0, 0);
    vetor.rotacionaZ(AnguloCampoVisao);
    PosicaoCampoVisao = PosicaoCampoVisao + vetor * distancia;
}

// Definição do tipo Nodo
struct Nodo{
    Poligono e;
    Ponto eMax, eMin;
    vector<Nodo> filhos;
    vector<int> idx;
    int nPontos = -1, altura;
    bool folha = false;
};
Nodo Tree;

// Metodo que gera a estrutura de uma QuadTree a partir dos pontos no cerario
void geraQuadTree(Ponto Max, Ponto Min, Nodo *nodo, int altura){
    int r = 0;
    nodo->altura=altura;
    altura++;
    Ponto Med = (Max + Min) * 0.5;
    Ponto Tam = (Max - Min);
    vector<int> aux;
    for (int i=0; i<PontosCenario.getNVertices(); i++){
        if (PontosCenario.getVertice(i).x >= Min.x && PontosCenario.getVertice(i).x <= Max.x && PontosCenario.getVertice(i).y >= Min.y && PontosCenario.getVertice(i).y <= Max.y){
            r++;
            aux.push_back(i);
        }
    }

    nodo->e.insereVertice(Ponto(Min.x, Min.y, 0.0));
    nodo->e.insereVertice(Ponto(Max.x, Min.y, 0.0));
    nodo->e.insereVertice(Ponto(Max.x, Max.y, 0.0));
    nodo->e.insereVertice(Ponto(Min.x, Max.y, 0.0));

    if ((r>=treeNvalue) && (nodo->nPontos==-1) && altura<7){
        nodo->nPontos = r;

        for (int i=0; i<4; i++){
            Nodo n;
            nodo->filhos.push_back(n);
        }
        Ponto Ma, Mi;
        Mi = (Ponto(Min.x, Med.y));
        Ma = (Ponto(Med.x, Max.y));
        geraQuadTree(Ma, Mi, &(nodo->filhos.at(0)),altura);
        Mi = (Ponto(Med.x, Med.y));
        Ma = (Ponto(Max.x, Max.y));
        geraQuadTree(Ma, Mi, &(nodo->filhos.at(1)),altura);
        Mi = (Ponto(Med.x, Min.y));
        Ma = (Ponto(Max.x, Med.y));
        geraQuadTree(Ma, Mi, &(nodo->filhos.at(2)),altura);
        Mi = (Ponto(Min.x, Min.y));
        Ma = (Ponto(Med.x, Med.y));
        geraQuadTree(Ma, Mi, &(nodo->filhos.at(3)),altura);
    } else {
        nodo->folha = true;
        nodo->idx=aux;
        nodo->nPontos = r;
        nodo->e.obtemLimites(nodo->eMin, nodo->eMax);
        leafs.push_back(nodo->e);
    }
}

//  Faz as inicializacoes das variaveis de estado da aplicacao
void init(string ler){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if (ler.empty()==1){
        cout << fileNpts << " random points were created!\n" << endl;
        GeraPontos(fileNpts, Ponto(0, 0), Ponto(coordMax, coordMax));
    }

    PontosCenario.obtemLimites(Min, Max);
    Min.x--;
    Min.y--;
    Max.x++;
    Max.y++;

    Meio = (Max + Min) * 0.5;
    Tamanho = (Max - Min);
    moveScale = Tamanho.modulo() * 0.004;

    PosicaoCampoVisao = Meio;
    AnguloCampoVisao = 0;

    idxt.clear();
    leafs.clear();
    geraQuadTree(Max, Min, &Tree, 0);

    CriaTrianguloCampoVisao();
    PosicionaTrianguloCampoVisao();
}

// Define os indexes dos pontos que passaram no filtro da QuadTree
void getIdxt(Nodo T){

    if(!T.folha){
        getIdxt(T.filhos.at(0));
        getIdxt(T.filhos.at(1));
        getIdxt(T.filhos.at(2));
        getIdxt(T.filhos.at(3));
    } else {
        Ponto Emin, Emax;
        CampoVisao.obtemLimites(Emin, Emax);

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

// Metodo da QuadTree
int quad(){
    Ponto min, max;
    bool isGreen;
    CampoVisao.obtemLimites(min, max);
    PontoVerde.clear();
    PontoAmarelo.clear();

    Ponto v1 = operator-(CampoVisao.getVertice(1), CampoVisao.getVertice(0));
    Ponto v2 = operator-(CampoVisao.getVertice(2), CampoVisao.getVertice(1));
    Ponto v3 = operator-(CampoVisao.getVertice(0), CampoVisao.getVertice(2));

    int r = 0;

    #pragma omp parallel for reduction(+ : r) schedule(dynamic, 8)
    for (int i=0; i<idxt.size(); i++){
        isGreen = false;
        if(infoStatus==1)
            r++;
        Ponto ponto = PontosCenario.getVertice(idxt.at(i));
        if (ponto.x > min.x && ponto.x < max.x && ponto.y > min.y && ponto.y < max.y){
            if(infoStatus==0)
                r++;
            Ponto va = operator-(ponto, CampoVisao.getVertice(0));
            if ((v1.x * va.y) - (v1.y * va.x) < 0){
                va = operator-(ponto, CampoVisao.getVertice(1));
                if ((v2.x * va.y) - (v2.y * va.x) < 0){
                    va = operator-(ponto, CampoVisao.getVertice(2));
                    if ((v3.x * va.y) - (v3.y * va.x) < 0){
                        isGreen = true;
                        if(infoStatus==2)
                            r++;
                    }
                }
            }
        }
        if(isGreen){
            #pragma omp critical
            {
                PontoVerde.insereVertice(ponto);
            }
        }else{
            #pragma omp critical
            {
                PontoAmarelo.insereVertice(ponto);
            }
        }
    }
    return r;
}

// Metodo do Envelope
int envelope()
{
    Ponto min, max;
    bool isGreen;
    CampoVisao.obtemLimites(min, max);
    PontoVerde.clear();
    PontoAmarelo.clear();

    Ponto v1 = operator-(CampoVisao.getVertice(1), CampoVisao.getVertice(0));
    Ponto v2 = operator-(CampoVisao.getVertice(2), CampoVisao.getVertice(1));
    Ponto v3 = operator-(CampoVisao.getVertice(0), CampoVisao.getVertice(2));

    int r = 0;

    #pragma omp parallel for reduction(+ : r) schedule(dynamic, 8)
    for (int i = 0; i < PontosCenario.getNVertices(); i++){
        if(infoStatus==1)
                r++;
        Ponto ponto = PontosCenario.getVertice(i);
        if (ponto.x > min.x && ponto.x < max.x && ponto.y > min.y && ponto.y < max.y){
            isGreen = false;
            if(infoStatus==0)
                r++;
            Ponto va = operator-(ponto, CampoVisao.getVertice(0));
            if ((v1.x * va.y) - (v1.y * va.x) < 0){
                va = operator-(ponto, CampoVisao.getVertice(1));
                if ((v2.x * va.y) - (v2.y * va.x) < 0){
                    va = operator-(ponto, CampoVisao.getVertice(2));
                    if ((v3.x * va.y) - (v3.y * va.x) < 0){
                        isGreen = true;
                        if(infoStatus==2)
                            r++;
                    }
                }
            }
            if(isGreen){
                #pragma omp critical
                {
                    PontoVerde.insereVertice(ponto);
                }
            }else{
                #pragma omp critical
                {
                    PontoAmarelo.insereVertice(ponto);
                }
            }
        }
    }
    return r;
}

//Metodo de Força Bruta
int forcaBruta()
{   
    int r = 0;
    PontoVerde.clear();
    Ponto v1 = operator-(CampoVisao.getVertice(1), CampoVisao.getVertice(0));
    Ponto v2 = operator-(CampoVisao.getVertice(2), CampoVisao.getVertice(1));
    Ponto v3 = operator-(CampoVisao.getVertice(0), CampoVisao.getVertice(2));
    #pragma omp parallel for reduction(+ : r) schedule(dynamic, 8)
    for (int i = 0; i < PontosCenario.getNVertices(); i++){
        if(infoStatus==0)
            r++;
        Ponto va = operator-(PontosCenario.getVertice(i), CampoVisao.getVertice(0));
        if ((v1.x * va.y) - (v1.y * va.x) < 0){
            va = operator-(PontosCenario.getVertice(i), CampoVisao.getVertice(1));
            if ((v2.x * va.y) - (v2.y * va.x) < 0){
                va = operator-(PontosCenario.getVertice(i), CampoVisao.getVertice(2));
                if ((v3.x * va.y) - (v3.y * va.x) < 0){
                    #pragma omp critical
                    {
                        PontoVerde.insereVertice(PontosCenario.getVertice(i));
                    }
                    if(infoStatus==2)
                        r++;
                }
            }
        }
    }
    return r;
}

// Metodo que chama os algoritmos
int exe(){
    if (modeStatus == 0){
        return forcaBruta();
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

    PontosCenario.desenhaVertices();

    if (colorStatus){
        if (modeStatus == 1||modeStatus==2){
            for (int i = 0; i < PontoAmarelo.getNVertices(); i++){
                Ponto P;
                P = PontoAmarelo.getVertice(i);
                glBegin(GL_POINTS);
                glColor3f(1, 1, 0);
                glVertex3f(P.x, P.y, P.z);
                glEnd();
            }
        }
        for (int i = 0; i < PontoVerde.getNVertices(); i++){
            Ponto P;
            P = PontoVerde.getVertice(i);
            glBegin(GL_POINTS);
            glColor3f(0, 0.75, 0);
            glVertex3f(P.x, P.y, P.z);
            glEnd();
        }
    }

    if(lineStatus && modeStatus == 2){
        glLineWidth(2);
        glColor3f(0.5, 0.5, 0.5);
        for (Poligono p : leafs){
            p.desenhaPoligono();
        }
    }

    if ((modeStatus == 1 && lineStatus)||(modeStatus == 2 && lineStatus)){
        glLineWidth(2);
        glColor3f(0, 0, 1);
        CampoVisao.createEnvelope(lineStatus);
    }

    glLineWidth(3);
    glColor3f(1, 0, 0);
    CampoVisao.desenhaPoligono();

    glutSwapBuffers();
}

// Metodo para para colocar o triangulo em posições especificas
void PosicionaCampoDeVisao(int n)
{
 switch (n) {
    case 1:
        AnguloCampoVisao = 0;
        PosicaoCampoVisao = Meio;
        break;
    case 2:
        AnguloCampoVisao = 90;
        PosicaoCampoVisao = Meio;
        break;
    case 3:
        AnguloCampoVisao = 90;
        PosicaoCampoVisao = Meio*0.5;
        break;
    case 4:
        AnguloCampoVisao = 0;
        PosicaoCampoVisao = Meio + Meio*0.5;
        break;
    default:
        break;
 }
 
 PosicionaTrianguloCampoVisao();
}

// Metodo para mudar o valor N da QuadTree
void changeNvalue(){
    clear(1);
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
        Nodo New;
        Tree=New;
        geraQuadTree(Max, Min, &Tree, 0);
    }
    clear(2);
}

// Metodo para aumentar/diminuir o campo de visão
void zoom(int i){
    if (i == 1){
        TrianguloBase.alteraVertice(1, (operator+(TrianguloBase.getVertice(1), (operator*(TrianguloBase.getVertice(1), 0.05)))));
        TrianguloBase.alteraVertice(2, (operator+(TrianguloBase.getVertice(2), (operator*(TrianguloBase.getVertice(2), 0.05)))));
    } else {
        TrianguloBase.alteraVertice(1, (operator*(TrianguloBase.getVertice(1), 0.95)));
        TrianguloBase.alteraVertice(2, (operator*(TrianguloBase.getVertice(2), 0.95)));
    }
}

// Entrada do teclado
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
            colorDesc = " | Color: OFF";
        } else {
            colorDesc = " | Color: ON";
        }
        colorStatus = !colorStatus;
        break;

    case 'x':
        if (lineStatus){
            lineStatus = !lineStatus;
            detailDesc = " | Details: OFF";
        }
        if (colorStatus){
            colorStatus = !colorStatus;
            colorDesc = " | Color: OFF";
        }
        if (modeStatus == 0){
            modeStatus = 1;
            modeDesc = " | Mode: Envelope";
        } else if (modeStatus == 1){
            modeStatus = 2;
            modeDesc = " | Mode: QuadTree";
        } else if (modeStatus = 2){
            modeStatus=0;
            modeDesc = " | Mode: Brute force";
        }
        infoDesc="-> Nº of BruteForce tests: ";
        infoStatus=0;
        break;

    case ' ':
        if (lineStatus){
            detailDesc = " | Details: OFF";
        } else {
            detailDesc = " | Details: ON";
        }
        lineStatus = !lineStatus;
        break;

    case 'z':
        if(infoStatus==0){
            infoDesc="-> Nº Envelope tests  : ";
            infoStatus=1;
        } else if (infoStatus==1){
            infoDesc="-> Nº inside triangle : ";
            infoStatus=2;
        } else if (infoStatus==2){
            infoDesc="-> Nº BruteForce tests: ";
            infoStatus=0;
        }
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
        PosicionaCampoDeVisao(i);
        break;

    default:
        break;
    }

    PosicionaTrianguloCampoVisao();
    clear(1);
    cout << infoDesc << exe() <<  modeDesc << detailDesc << colorDesc << endl;
    glutPostRedisplay();
}

void arrow_keys(int a_keys, int x, int y){
    switch (a_keys){
    case GLUT_KEY_LEFT:
        AnguloCampoVisao += 2;
        break;
    case GLUT_KEY_RIGHT:
        AnguloCampoVisao -= 2;
        break;
    case GLUT_KEY_UP:
        AvancaCampoVisao(moveScale);
        break;
    case GLUT_KEY_DOWN:
        AvancaCampoVisao(moveScale * (-1));
        break;
    default:
        break;
    }

    PosicionaTrianguloCampoVisao();
    clear(1);
    cout << infoDesc << exe() <<  modeDesc << detailDesc << colorDesc << endl;
    glutPostRedisplay();
}

// Ler arquivo
void readFile(string *name){
    ifstream input;
    input.open(*name, ios::in);
    if (!input){
        cout << "Unable to find the file " << *name << "... Finishing process!\n" << endl;
        exit(0);
    }

    cout << "Reading the file... " << *name << "...";
    string S;
    unsigned int qtdVertices;
    input >> qtdVertices;

    for (int i = 0; i < qtdVertices; i++){
        double x, y;
        input >> x >> y;
        if (!input)
            break;
        PontosCenario.insereVertice(Ponto(x, y));
    }
    cout << " Done!\n" << endl;
    moveScale = abs(coordMax * 0.004);
}

// Metodo principal
int main(int argc, char **argv){
    system("export OMP_NUM_THREADS=8");
    system("clear");
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "-                OpenGL Program - CG - T1                   -" << endl;
    cout << "- Eduardo Machado Martins & Nathan dos Reis Ramos de Mello  -" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "-  'Esc'...End program                                      -\n-  '+/-'...Change field of vision size                      -\n-  ' '.....Show details                                     -\n-  'c'.....Points with colors                               -\n-  'x'.....Change algorithm                                 -\n-  'z'.....Change info                                      -\n-  'n'.....Change Tree N value                              -" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n" << endl;
    cout << "Input the file name or press Enter to random points:" << endl;
    cout << "\n -> ";

    string input = "";
    getline(cin, input);
    clear(3);

    if (input.empty() == 0){
        readFile(&input);
    } else {
        string verify = "";
        cout << "Input the number of random points (min 10):" << endl;
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
        clear(3);
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