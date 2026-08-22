#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

int md, lstX = -1, lstY = -1;
float cx = 0, cy = 0, cz = 5, yw = 0, pt = 0, rot = 0;
bool lck = 0, drg = 0;

// 模式2：绘制实体彩色正方体
void drwCube() {
    glBegin(GL_QUADS);
    glColor3f(1, 0, 0); glVertex3f(-1, -1, 1); glVertex3f(1, -1, 1); glVertex3f(1, 1, 1); glVertex3f(-1, 1, 1);
    glColor3f(0, 1, 0); glVertex3f(-1, -1, -1); glVertex3f(-1, 1, -1); glVertex3f(1, 1, -1); glVertex3f(1, -1, -1);
    glColor3f(0, 0, 1); glVertex3f(-1, 1, -1); glVertex3f(-1, 1, 1); glVertex3f(1, 1, 1); glVertex3f(1, 1, -1);
    glColor3f(1, 1, 0); glVertex3f(-1, -1, -1); glVertex3f(1, -1, -1); glVertex3f(1, -1, 1); glVertex3f(-1, -1, 1);
    glColor3f(1, 0, 1); glVertex3f(1, -1, -1); glVertex3f(1, 1, -1); glVertex3f(1, 1, 1); glVertex3f(1, -1, 1);
    glColor3f(0, 1, 1); glVertex3f(-1, -1, -1); glVertex3f(-1, -1, 1); glVertex3f(-1, 1, 1); glVertex3f(-1, 1, -1);
    glEnd();
}

// 模式3：绘制线框模型（手工绘制的线框四棱锥）
void drwWireModel() {
    glColor3f(0.0, 1.0, 1.0); // 设置线框颜色为青色
    glLineWidth(2.0);         // 加粗线框便于观察
    
    // 绘制底部正方形线框
    glBegin(GL_LINE_LOOP);
    glVertex3f(-1, -1, 1); glVertex3f(1, -1, 1);
    glVertex3f(1, -1, -1); glVertex3f(-1, -1, -1);
    glEnd();
    
    // 绘制侧面四条棱连向顶点(0, 1.5, 0)
    glBegin(GL_LINES);
    glVertex3f(-1, -1, 1);  glVertex3f(0, 1.5, 0);
    glVertex3f(1, -1, 1);   glVertex3f(0, 1.5, 0);
    glVertex3f(1, -1, -1);  glVertex3f(0, 1.5, 0);
    glVertex3f(-1, -1, -1); glVertex3f(0, 1.5, 0);
    glEnd();
    
    glLineWidth(1.0); // 恢复默认线宽
}

void disp() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    
    // 计算交互式漫游的相机朝向偏移量
    float dx = cos(pt) * sin(yw), dy = sin(pt), dz = -cos(pt) * cos(yw);
    gluLookAt(cx, cy, cz, cx + dx, cy + dy, cz + dz, 0, 1, 0);
    
    glPushMatrix();
    glRotatef(rot, 1, 1, 0); // 整体旋转
    
    if (md == 2) drwCube();
    else if (md == 3) drwWireModel(); // 调用线框模型
    
    glPopMatrix();
    glutSwapBuffers();
}

void idle() {
    if (!lck) { 
        rot += 0.1; 
        if (rot > 360) rot -= 360; 
    }
    glutPostRedisplay();
}

void reshp(int w, int h) {
    if (!h) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyb(unsigned char k, int x, int y) {
    if (k == 'l' || k == 'L') { lck = !lck; return; }
    if (lck) return;
    
    float v = 0.5, fx = sin(yw), fz = -cos(yw), rx = cos(yw), rz = sin(yw);
    
    if (k == 'w' || k == 'W') { cx += fx * v; cz += fz * v; }
    if (k == 's' || k == 'S') { cx -= fx * v; cz -= fz * v; }
    if (k == 'a' || k == 'A') { cx -= rx * v; cz -= rz * v; }
    if (k == 'd' || k == 'D') { cx += rx * v; cz += rz * v; }
    if (k == 'q' || k == 'Q') cy += v;
    if (k == 'e' || k == 'E') cy -= v;
    
    if (k == 27) exit(0); // ESC
    glutPostRedisplay();
}

void mous(int b, int s, int x, int y) {
    if (lck) return;
    if (b == GLUT_LEFT_BUTTON) {
        drg = (s == GLUT_DOWN);
        if (drg) { lstX = x; lstY = y; }
    }
}

void mot(int x, int y) {
    if (lck || !drg) return;
    yw += (x - lstX) * 0.005; 
    pt -= (y - lstY) * 0.005;
    lstX = x; lstY = y;
    if (pt > 1.5) pt = 1.5; 
    if (pt < -1.5) pt = -1.5;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    cout << "请输入模式 (2: 旋转正方体, 3: 旋转线框模型): "; 
    cin >> md;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Task2 - 3D Models");
    glEnable(GL_DEPTH_TEST);
    
    glutDisplayFunc(disp); 
    glutReshapeFunc(reshp);
    glutIdleFunc(idle); 
    glutKeyboardFunc(keyb);
    glutMouseFunc(mous); 
    glutMotionFunc(mot);
    
    glutMainLoop();
    return 0;
}