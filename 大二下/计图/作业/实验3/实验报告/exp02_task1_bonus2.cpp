#include <GL/glut.h>
#include <iostream>
using namespace std;

float angleX = 0, angleY = 0;
float intensity = 1.0;

// 顶点数据和法线
float vertices[8][3] = {
    {-0.5,-0.5,-0.5}, {0.5,-0.5,-0.5}, {0.5,0.5,-0.5}, {-0.5,0.5,-0.5},
    {-0.5,-0.5,0.5}, {0.5,-0.5,0.5}, {0.5,0.5,0.5}, {-0.5,0.5,0.5}
};
float normals[6][3] = {
    {0,0,-1}, {0,0,1}, {-1,0,0}, {1,0,0}, {0,-1,0}, {0,1,0}
};
int faces[6][4] = {
    {0,3,2,1}, {4,5,6,7}, {0,4,7,3}, {1,2,6,5}, {0,1,5,4}, {3,7,6,2}
};

void updateLightAndMaterial(char mode = 0) {
    static char matMode = 'm', lightMode = 'o';
    if (mode == 'b' || mode == 'n' || mode == 'm') matMode = mode;
    if (mode == 'o' || mode == 'p') lightMode = mode;

    float l_diff[4];
    if (lightMode == 'o') { l_diff[0]=1*intensity; l_diff[1]=1*intensity; l_diff[2]=1*intensity; l_diff[3]=1; }
    else { l_diff[0]=0.2*intensity; l_diff[1]=0.8*intensity; l_diff[2]=0.4*intensity; l_diff[3]=1; }
    glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diff);

    float m_diff[4], m_spec[4], m_shine[1];
    if (matMode == 'b') { 
        m_diff[0]=0.78; m_diff[1]=0.57; m_diff[2]=0.11;
        m_spec[0]=0.99; m_spec[1]=0.94; m_spec[2]=0.81; m_shine[0]=27.8;
    } else if (matMode == 'n') { 
        m_diff[0]=0.5; m_diff[1]=0.0; m_diff[2]=0.0;
        m_spec[0]=0.7; m_spec[1]=0.6; m_spec[2]=0.6; m_shine[0]=32.0;
    } else { 
        m_diff[0]=1.0; m_diff[1]=1.0; m_diff[2]=1.0;
        m_spec[0]=1.0; m_spec[1]=1.0; m_spec[2]=1.0; m_shine[0]=100.0;
    }
    
    for(int i=0;i<3;++i) m_diff[i] *= intensity; 
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, m_shine);
}

void init() {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    float pos[] = {1.0, 1.0, 1.0, 0.0}; 
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    updateLightAndMaterial();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0,0,2.5, 0,0,0, 0,1,0);
    
    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    glBegin(GL_QUADS);
    for(int i=0; i<6; ++i) {
        glNormal3fv(normals[i]);
        for(int j=0; j<4; ++j) glVertex3fv(vertices[faces[i][j]]);
    }
    glEnd();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if(key=='b' || key=='n' || key=='m' || key=='o' || key=='p') {
        updateLightAndMaterial(key);
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if(key == GLUT_KEY_UP) intensity = min(1.5f, intensity + 0.1f);
    if(key == GLUT_KEY_DOWN) intensity = max(0.0f, intensity - 0.1f);
    updateLightAndMaterial();
    glutPostRedisplay();
}

void timer(int value) {
    angleX += 0.5;
    angleY += 0.7;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h?h:1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w/h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bonus 2");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glutTimerFunc(16, timer, 0); 
    
    glutMainLoop();
    return 0;
}