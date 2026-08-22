#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

const float PI = 3.1415926535f;

// 奥运五环标准色
float colors[5][3] = {
    {0.0f, 0.50f, 0.78f}, // 蓝
    {0.0f, 0.0f, 0.0f},   // 黑
    {0.87f, 0.10f, 0.23f},// 红
    {0.99f, 0.73f, 0.16f},// 黄
    {0.0f, 0.61f, 0.31f}  // 绿
};

// 环中心坐标
float centers[5][2] = {
    {-2.2f, 0.0f}, {0.0f, 0.0f}, {2.2f, 0.0f}, // 上三环
    {-1.1f, -1.1f}, {1.1f, -1.1f}             // 下二环
};

// 绘制圆环函数：指定中心、颜色、起始角度和结束角度
void drawRing(float x, float y, float* color, float startAngle = 0, float endAngle = 360) {
    float radius = 1.0f;
    float thickness = 0.15f; // 环的厚度
    int segments = 100;

    glColor3fv(color);
    glLineWidth(8.0f); // 也可以用多边形画，这里用粗线条实现最快

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segments; i++) {
        float angle = (startAngle + (endAngle - startAngle) * i / segments) * PI / 180.0f;
        float curX = x + cos(angle) * radius;
        float curY = y + sin(angle) * radius;
        glVertex2f(curX, curY);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.5f, -6.0f); // 整体视角居中

    // 1. 先绘制基础的五个环
    for (int i = 0; i < 5; i++) {
        drawRing(centers[i][0], centers[i][1], colors[i]);
    }

    // 2. 核心：通过重绘局部弧段解决“互锁”遮挡问题
    // 逻辑：让上层的环在特定角度范围内重新覆盖下层的环
    drawRing(centers[0][0], centers[0][1], colors[0], 270, 360); // 蓝环覆盖黄环
    drawRing(centers[1][0], centers[1][1], colors[1], 240, 310); // 黑环覆盖黄环
    drawRing(centers[1][0], centers[1][1], colors[1], 330, 360); // 黑环覆盖绿环
    drawRing(centers[2][0], centers[2][1], colors[2], 230, 300); // 红环覆盖绿环

    glutSwapBuffers();
}

// 附加题：保持比例不变
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // 关键点：动态计算窗口宽高比 (aspect ratio)
    float aspect = (float)w / (float)h;
    if (w <= h) {
        // 窗口太窄时，扩大垂直视野
        glOrtho(-4.0, 4.0, -4.0 / aspect, 4.0 / aspect, -10.0, 10.0);
    } else {
        // 窗口太扁时，扩大水平视野
        glOrtho(-4.0 * aspect, 4.0 * aspect, -4.0, 4.0, -10.0, 10.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // 白色背景
    glEnable(GL_LINE_SMOOTH);         // 开启线段抗锯齿
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 500);
    glutCreateWindow("Task 3: Olympic Rings");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}