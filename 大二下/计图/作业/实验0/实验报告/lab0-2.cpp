#include <GL/freeglut.h>
#include <math.h>

void display(void)
{
    /* 清除窗口 */
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.2f, 0.2f, 0.0f);

    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        for (int i = 0; i <= 365; i++) {
            float angle = 2.0f * 3.14159f * i / 365;
            glColor3f(0.0f, 1.0f, 1.0f);
            glVertex2f(cos(angle) * 0.3f, sin(angle) * 0.3f);
        }
    glEnd();

    /* 强制执行 GL 缓冲 */
    glFlush();
}

void init()
{
    /* 设置清屏颜色为黑色 */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    /* 设置画笔填充颜色为白色 */
    glColor3f(1.0f, 1.0f, 1.0f); 

    /* 下面是正交投影矩阵设置，默认即可，这里保持注释 */
    /* glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); 
    */
}

int main(int argc, char** argv)
{
    /* 初始化 GLUT 并打开窗口 */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    /* 设置窗口大小和初始位置 */
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    
    /* 创建窗口，标题为 "simple" */
    glutCreateWindow("simple");
    
    /* 注册显示回调函数 */
    glutDisplayFunc(display);
    
    /* 调用我们自己的初始化函数 */
    init();
    
    /* 进入 GLUT 事件处理循环 */
    glutMainLoop();

    return 0; // C++ main 函数标准返回值
}