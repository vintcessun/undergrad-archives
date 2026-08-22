#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

const float PI = 3.1415926535f;
int md; // 模式 1~4
int lstX = -1, lstY = -1;
float cx = 0, cy = 0, cz = 5, yw = 0, pt = 0, rot = 0;
bool lck = 0, drg = 0;

// 功能1：不使用glut对象，通过数学公式纯手工绘制线框球体
void drwSph(float r, int s, int t) {
    glColor3f(1, 0, 0);
    // 绘制纬线 (水平圆圈)
    for (int j = 0; j <= t; ++j) {
        float phi = PI * j / t;
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < s; ++i) {
            float th = 2 * PI * i / s;
            glVertex3f(r * sin(phi) * cos(th), r * cos(phi), r * sin(phi) * sin(th));
        }
        glEnd();
    }
    // 绘制经线 (穿过两极的半圆)
    for (int i = 0; i < s; ++i) {
        float th = 2 * PI * i / s;
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= t; ++j) {
            float phi = PI * j / t;
            glVertex3f(r * sin(phi) * cos(th), r * cos(phi), r * sin(phi) * sin(th));
        }
        glEnd();
    }
}

void disp() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    
    // 功能4：启用交互式相机
    if (md >= 4) {
        float dx = cos(pt) * sin(yw), dy = sin(pt), dz = -cos(pt) * cos(yw);
        gluLookAt(cx, cy, cz, cx + dx, cy + dy, cz + dz, 0, 1, 0);
    } else {
        // 模式1/2/3下的固定死相机
        gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
    }
    
    glPushMatrix();
    // 功能2：启用动画旋转
    if (md >= 2) glRotatef(rot, 1, 1, 0);
    
    drwSph(1.5, 30, 30);
    glPopMatrix();
    
    glutSwapBuffers();
}

void idle() {
    // 模式2及以上才启用动画更新
    if (md >= 2 && !lck) {
        rot += 0.1; 
        if (rot > 360) rot -= 360; 
        glutPostRedisplay();
    }
}

void reshp(int w, int h) {
    if (!h) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    
    // 功能3：启用防形变的 reshape
    if (md >= 3) {
        gluPerspective(45.0, (float)w / h, 0.1, 100.0);
    } else {
        // 模式1/2下，故意使用固定的宽高比1.0，展示拉伸形变的错误效果
        gluPerspective(45.0, 1.0, 0.1, 100.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

void keyb(unsigned char k, int x, int y) {
    if (md < 4) return; // 模式4以下拦截所有键盘操作
    if (k == 'l' || k == 'L') { lck = !lck; return; }
    if (lck) return;
    
    float v = 0.5, fx = sin(yw), fz = -cos(yw), rx = cos(yw), rz = sin(yw);
    if (k == 'w' || k == 'W') { cx += fx * v; cz += fz * v; }
    if (k == 's' || k == 'S') { cx -= fx * v; cz -= fz * v; }
    if (k == 'a' || k == 'A') { cx -= rx * v; cz -= rz * v; }
    if (k == 'd' || k == 'D') { cx += rx * v; cz += rz * v; }
    if (k == 'q' || k == 'Q') cy += v;
    if (k == 'e' || k == 'E') cy -= v;
    
    if (k == 27) exit(0);
    glutPostRedisplay();
}

void mous(int b, int s, int x, int y) {
    if (md < 4 || lck) return; // 模式4以下拦截鼠标操作
    if (b == GLUT_LEFT_BUTTON) {
        drg = (s == GLUT_DOWN);
        if (drg) { lstX = x; lstY = y; }
    }
}

void mot(int x, int y) {
    if (md < 4 || lck || !drg) return;
    yw += (x - lstX) * 0.005; 
    pt -= (y - lstY) * 0.005;
    lstX = x; lstY = y;
    if (pt > 1.5) pt = 1.5; 
    if (pt < -1.5) pt = -1.5;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    cout << "===== 递进式测试模式 =====" << endl;
    cout << "1: 静态线框球体 (会形变)" << endl;
    cout << "2: + 自动旋转" << endl;
    cout << "3: + 防形变 (Reshape)" << endl;
    cout << "4: + 交互式相机控制 (最终完整版)" << endl;
    cout << "请输入模式 (1-4): "; 
    cin >> md;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Task 3 - Progressive Wireframe Sphere");
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