#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <GL/glut.h>
using namespace std;

float V[40005][3], VN[40005][3];
int F[80005][3];
int vc = 0, fc = 0;
float angle = 0.0;

void loadPly() {
    ifstream fin("lizhenxiout.ply");
    string s;
    while(getline(fin, s)) {
        if(s.find("element vertex") != string::npos) sscanf(s.c_str(), "element vertex %d", &vc);
        if(s.find("element face") != string::npos) sscanf(s.c_str(), "element face %d", &fc);
        if(s == "end_header") break;
    }
    float mn[3]={1e9,1e9,1e9}, mx[3]={-1e9,-1e9,-1e9};
    for(int i=0; i<vc; ++i) {
        fin >> V[i][0] >> V[i][1] >> V[i][2] >> VN[i][0] >> VN[i][1] >> VN[i][2];
        for(int j=0; j<3; ++j) { mn[j]=min(mn[j],V[i][j]); mx[j]=max(mx[j],V[i][j]); }
    }
    float cx=(mx[0]+mn[0])/2, cy=(mx[1]+mn[1])/2, cz=(mx[2]+mn[2])/2;
    float scale = 2.0 / max({mx[0]-mn[0], mx[1]-mn[1], mx[2]-mn[2]});
    for(int i=0; i<vc; ++i) {
        V[i][0]=(V[i][0]-cx)*scale; V[i][1]=(V[i][1]-cy)*scale; V[i][2]=(V[i][2]-cz)*scale;
    }
    for(int i=0; i<fc; ++i) { int n; fin >> n >> F[i][0] >> F[i][1] >> F[i][2]; }
}

void init() {
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // 设置基础材质
    float mat_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    float mat_specular[] = { 0.3, 0.3, 0.3, 1.0 };
    float mat_shininess[] = { 10.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0,0,3, 0,0,0, 0,1,0);
    
    // 动态更新光源位置
    float lx = 3.0 * sin(angle);
    float lz = 3.0 * cos(angle);
    float light_pos[] = { lx, 1.0, lz, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glBegin(GL_TRIANGLES);
    for(int i=0; i<fc; ++i) {
        for(int j=0; j<3; ++j) {
            glNormal3fv(VN[F[i][j]]);
            glVertex3fv(V[F[i][j]]);
        }
    }
    glEnd();
    glutSwapBuffers();
}

void idle() {
    angle += 0.01;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h?h:1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w/h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    loadPly();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bonus 1");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}