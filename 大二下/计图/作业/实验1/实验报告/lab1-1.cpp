#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

static GLfloat spin = 0.0;
const float PI = 3.1415926535f;

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
}

void drawEightSectors(float radius) {
    int numSectors = 8;
    int slicesPerSector = 30;

    float colors[8][3] = {
        {1.0f, 0.0f, 0.0f}, // 红色
        {1.0f, 1.0f, 0.0f}, // 黄色
        {0.0f, 1.0f, 0.0f}, // 绿色
        {0.0f, 0.0f, 1.0f}, // 蓝色
        {1.0f, 0.0f, 1.0f}, // 品红
        {1.0f, 0.5f, 0.0f}, // 橙色
        {0.0f, 0.75f, 1.0f},// 天蓝色
        {0.75f, 0.75f, 0.75f} // 灰色
    };

    for (int i = 0; i < numSectors; i++) {
        glColor3fv(colors[i]); // 应用预设颜色

        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, 0.0f, 0.0f); 

        float startAngle = 2.0f * PI * i / numSectors;
        float endAngle = 2.0f * PI * (i + 1) / numSectors;

        for (int j = 0; j <= slicesPerSector; j++) {
            float currentAngle = startAngle + (endAngle - startAngle) * j / slicesPerSector;
            glVertex3f(cos(currentAngle) * radius, sin(currentAngle) * radius, 0.0f);
        }
        glEnd();
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(spin, 0.0, 0.0, 1.0);

    drawEightSectors(1.5f);

    glutSwapBuffers();
}

void spinDisplay(void) {
    spin += 0.5;
    if (spin > 360.0) spin -= 360.0;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Task 2: 8 Colored Sectors Circle");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(spinDisplay);
    glutMainLoop();
    return 0;
}