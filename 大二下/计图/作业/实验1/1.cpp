#include <GL/glut.h>
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT); // 使用单一颜色着色
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除颜色缓冲和深度缓冲
	glLoadIdentity();									// 重置当前模型视图矩阵

	glTranslatef(-1.5f, 0.0f, -6.0f);						// 向左移动 1.5 个单位，并向屏幕内移动 6.0 个单位
	glBegin(GL_TRIANGLES);								// 使用三角形绘制
	glVertex3f(0.0f, 1.0f, 0.0f);					// 顶点（上）
	glVertex3f(-1.0f, -1.0f, 0.0f);					// 顶点（左下）
	glVertex3f(1.0f, -1.0f, 0.0f);					// 顶点（右下）
	glEnd();											// 三角形绘制结束
	glTranslatef(3.0f, 0.0f, 0.0f);						// 向右移动 3 个单位
	glBegin(GL_QUADS);									// 绘制四边形
	glVertex3f(-1.0f, 1.0f, 0.0f);					// 顶点（左上）
	glVertex3f(1.0f, 1.0f, 0.0f);					// 顶点（右上）
	glVertex3f(1.0f, -1.0f, 0.0f);					// 顶点（右下）
	glVertex3f(-1.0f, -1.0f, 0.0f);					// 顶点（左下）
	glEnd();

	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100); // 设置窗口在屏幕左上角的位置
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
