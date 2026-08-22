#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <GL/glut.h>
using namespace std;

float V[40005][3];
int F[80005][3];
int vc = 0, fc = 0;

void loadPly() {
    ifstream fin("lizhenxiout.ply");
    if(!fin) exit(0);
    string s;
    while(getline(fin, s)) {
        if(s.find("element vertex") != string::npos) sscanf(s.c_str(), "element vertex %d", &vc);
        if(s.find("element face") != string::npos) sscanf(s.c_str(), "element face %d", &fc);
        if(s == "end_header") break;
    }
    float mn[3]={1e9,1e9,1e9}, mx[3]={-1e9,-1e9,-1e9};
    for(int i=0; i<vc; ++i) {
        float dummy[3]; // 略过法线数据
        fin >> V[i][0] >> V[i][1] >> V[i][2] >> dummy[0] >> dummy[1] >> dummy[2];
        for(int j=0; j<3; ++j) {
            mn[j] = min(mn[j], V[i][j]);
            mx[j] = max(mx[j], V[i][j]);
        }
    }
    float cx=(mx[0]+mn[0])/2, cy=(mx[1]+mn[1])/2, cz=(mx[2]+mn[2])/2;
    float scale = 2.0 / max({mx[0]-mn[0], mx[1]-mn[1], mx[2]-mn[2]});
    for(int i=0; i<vc; ++i) {
        V[i][0]=(V[i][0]-cx)*scale; V[i][1]=(V[i][1]-cy)*scale; V[i][2]=(V[i][2]-cz)*scale;
    }
    for(int i=0; i<fc; ++i) {
        int n; fin >> n >> F[i][0] >> F[i][1] >> F[i][2];
    }
}

void init() {
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0,0,3, 0,0,0, 0,1,0);
    glColor3f(0.3, 0.7, 0.9); // 纯色浅蓝
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    for(int i=0; i<fc; ++i)
        for(int j=0; j<3; ++j)
            glVertex3fv(V[F[i][j]]);
    glEnd();
    glutSwapBuffers();
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
    glutCreateWindow("Task 1");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}