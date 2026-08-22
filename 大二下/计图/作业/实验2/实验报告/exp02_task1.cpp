#include<GL/glut.h>
#include<iostream>
#include<cmath>

using namespace std;
int mode;
float t=0,ang=0,scl=1,ds=0.01;
struct P{float x,y;};

void draw(P a,P b,P c,int d){
    if(mode==1) glColor3f(0.2,0.4,1.0);
    else if(mode==2) glColor3f((d%3==0)*0.8+0.2,(d%3==1)*0.8+0.2,(d%3==2)*0.8+0.2);
    else glColor3f(0.5+0.5*sin(t+d),0.5+0.5*sin(t+d*2+2),0.5+0.5*sin(t+d*3+4));
    glBegin(GL_TRIANGLES);
    glVertex2f(a.x,a.y); glVertex2f(b.x,b.y); glVertex2f(c.x,c.y);
    glEnd();
}

void div_tri(P a,P b,P c,int m,int d){
    if(m){
        P v0={(a.x+b.x)/2,(a.y+b.y)/2},v1={(a.x+c.x)/2,(a.y+c.y)/2},v2={(b.x+c.x)/2,(b.y+c.y)/2};
        div_tri(a,v0,v1,m-1,d*3);
        div_tri(c,v1,v2,m-1,d*3+1);
        div_tri(b,v2,v0,m-1,d*3+2);
    }else draw(a,b,c,d);
}

void disp(){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    if(mode==3){glScalef(scl,scl,1); glRotatef(ang,0,0,1);}
    P a={-0.8,-0.8},b={0.8,-0.8},c={0,0.8};
    div_tri(a,b,c,6,1);
    glutSwapBuffers();
}

void idle(){
    if(mode==3){
        t+=0.05; ang+=0.5; if(ang>360) ang-=360;
        scl+=ds; if(scl>1.5||scl<0.5) ds=-ds;
        glutPostRedisplay();
    }
}

int main(int argc,char** argv){
    cout<<"请输入运行模式（1-静态单色，2-静态彩色，3-动态变换）：";
    cin>>mode;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(600,600);
    glutCreateWindow("Task1 - Sierpinski");
    glClearColor(0,0,0,1);
    glutDisplayFunc(disp);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}