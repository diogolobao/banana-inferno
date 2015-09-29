#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#define PI 3.14159265

using namespace std;

class Bola
{
private:
    double v ,angle;// angle
    double x ,y; // x and y
    double seno;
    double coseno;
    bool active;

public:
    Bola()
    {
        this->x = 0;
        this->y = 0;
        this->v = 0;
        this->active = true;
    }

    void SetV(int vel)  ///modifica a velocidade
    {
        this->v = vel;
    }
    int GetV(void)  /// pega a velocidade
    {
        return this->v;
    }
    void SetAngle(int ang)  ///modifica o angulo
    {
        this->angle = ang;
        this->SetCos();
        this->SetSen();
    }
    void setActive(bool a){
        this->active = a;
    }

    bool getActive(){
        return this->active;
    }
    void SetCos(void)  ///modifica o valor do coseno
    {
        this->coseno = cos(this->angle*PI/180);
    }
    int GetX(void)  /// pega o x
    {
        return this->x;
    }
    int SetX(double x) ///modfica o valor do x
    {
        this->x = x;
    }
    int SetSen(void)  ///modifica o valor de seno
    {
        this->seno = sin(this->angle*PI/180);
    }
    int GetY(void)  /// pega o y
    {
        return this->y;
    }
    int SetY(double y) ///modifica o valor do y
    {
        this->y = y;
    }
    int GetAngle(void)  /// pega o angulo
    {
        return this->angle;
    }

    int Colisao(int x , int y)
    {

        if(x >= 165 || x <= -220) ///barra direita e esquerda
        {
            this->coseno = -this->coseno;
        }
        if(y <= -190 || y >= 95) ///base inferior e superior
        {
            this->seno = -this->seno;
        }

    }
    void Atrito()
    {
        this->v -= 1;
    }

    /**
    Chamada para dar uma tacada nesta bola.
    */
    void Tacada()
    {
        int angulo,velocidade;
        cout<<"Informe o angulo dentro de 0 e 360 e a velocidade desejada: \n";
        cin>>angulo;
        cin>>velocidade;
        SetAngle(angulo);
        SetV(velocidade);
    }

    void Draw()
    {
        this->x += this->v*this->coseno;
        this->y += this->v*this->seno;
        this->Atrito();

        glPushMatrix(); // ~Todo objeto desenhado fica entre Push e Pop
        glLineWidth(1.5);
        glBegin(GL_LINE_STRIP); //ball
        glVertex3d(this->GetX(),this->GetY()+20,0);
        glVertex3d(this->GetX()+20,this->GetY()+20,0);
        glVertex3d(this->GetX()+20,this->GetY(),0);
        glVertex3d(this->GetX(),this->GetY(),0);
        glVertex3d(this->GetX(),this->GetY()+20,0);
        glEnd();
    }
};

class Mesa
{
private:
    double x,y;
    double largura = 490;
    double altura = 340;
    Bola branca;
    Bola bolas[1];

public:
    Mesa(double larguraTela, double alturaTela){
        this->x = larguraTela * 5 / 100;
        this->y = alturaTela * 5 / 100;
        this->largura = larguraTela * 95 / 100;
        this->altura = alturaTela * 95 / 100;

        ///Bola branca
        Bola branca;
        branca.SetX(larguraTela * 15/100);
        branca.SetY(alturaTela* 50/100);
        this->branca = branca;

        /// Outras Bolas

    }

    void adicionarBola(Bola b, int i){
        this->bolas[i] = b;
    }
    void setX(double x){
        this->x = x;
    }
    double getX(){
        return this->x;
    }
    void setY(double y){
        this->y = y;
    }
    double getY(){
        return this->y;
    }

    ///Retorna os 4 pontos de colisão, x baixo, x alto, y baixo, y alto
    double* getColisao(){
        double colisao[4] = {this->x,this->largura, this->y, this->altura};
        return colisao;
    }

    void Draw(){
        glColor3f(0,0,0); //mesa de sinuca
        glBegin(GL_LINE_STRIP);
        glVertex3d(this->x,this->y,0);
        glVertex3d(this->largura,this->y,0);
        glVertex3d(this->largura,this->altura,0);
        glVertex3d(this->x,this->altura,0);
        glVertex3d(this->x,this->y,0);
        glEnd();
        glPopMatrix(); // ~Todo objeto desenhado fica entre Push e Pop


        if(this->branca.GetV() == 0){
            this->branca.Tacada();
        }
        this->branca.Draw();

        for(int i = 0; i<1;i++){
            if(this->bolas[i].getActive()){
                this->bolas[i].Draw();
            }
        }
    }

};

static void resize(int width, int height);
static void display(void);
static void idle(void); // ~Esta função é chamada pelo main ao longo do tempo. Ela faz a interpolação

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glutMainLoop();

    return EXIT_SUCCESS;
}


static void resize(int width, int height)
{

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,width,0,height,-100,100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    Mesa mesa = Mesa(640,480);

    ///Desenhando quadrado
    mesa.Draw();

    glutSwapBuffers();

}
static void idle(void) // ~Esta função é chamada pelo main ao longo do tempo. Ela faz a interpolação
{

    glutPostRedisplay();

}
