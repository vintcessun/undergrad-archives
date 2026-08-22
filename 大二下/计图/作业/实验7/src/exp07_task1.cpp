#include <GL/freeglut.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

struct Point {
    float x;
    float y;
};

const float PI = 3.14159265358979323846f;

int g_controlCount = 4;
float g_t = 0.0f;
bool g_paused = false;
int g_lastTime = 0;
int g_width = 1000;
int g_height = 700;

Point lerp(const Point& a, const Point& b, float t) {
    return {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t};
}

std::vector<Point> makeControlPoints(int count) {
    std::vector<Point> points;
    points.reserve(count);

    for (int i = 0; i < count; ++i) {
        float u = static_cast<float>(i) / static_cast<float>(count - 1);
        float x = -4.2f + 8.4f * u;
        float y = 1.65f * std::sin((2.55f * u + 0.08f) * PI)
                + 0.55f * std::sin((6.2f * u + 0.25f) * PI);
        points.push_back({x, y});
    }

    return points;
}

std::vector<std::vector<Point>> deCasteljauLevels(const std::vector<Point>& controls, float t) {
    std::vector<std::vector<Point>> levels;
    levels.push_back(controls);

    while (levels.back().size() > 1) {
        const std::vector<Point>& previous = levels.back();
        std::vector<Point> next;
        next.reserve(previous.size() - 1);

        for (size_t i = 0; i + 1 < previous.size(); ++i) {
            next.push_back(lerp(previous[i], previous[i + 1], t));
        }

        levels.push_back(next);
    }

    return levels;
}

Point evaluateBezier(const std::vector<Point>& controls, float t) {
    return deCasteljauLevels(controls, t).back().front();
}

void color(float r, float g, float b) {
    glColor3f(r, g, b);
}

void drawPoint(const Point& p, float size, float r, float g, float b) {
    glPointSize(size);
    color(r, g, b);
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
}

void drawPolyline(const std::vector<Point>& points, float r, float g, float b) {
    if (points.empty()) {
        return;
    }

    color(r, g, b);
    glBegin(GL_LINE_STRIP);
    for (const Point& p : points) {
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void drawText(float x, float y, const std::string& text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, g_width, 0.0, g_height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    color(0.08f, 0.09f, 0.10f);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = g_height == 0 ? 1.0f : static_cast<float>(g_width) / static_cast<float>(g_height);
    if (aspect >= 1.0f) {
        glOrtho(-5.7f * aspect, 5.7f * aspect, -4.0f, 4.0f, -1.0f, 1.0f);
    } else {
        glOrtho(-5.7f, 5.7f, -4.0f / aspect, 4.0f / aspect, -1.0f, 1.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() {
    glClearColor(0.94f, 0.95f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    setupCamera();

    std::vector<Point> controls = makeControlPoints(g_controlCount);
    std::vector<std::vector<Point>> levels = deCasteljauLevels(controls, g_t);

    glLineWidth(2.0f);
    drawPolyline(controls, 0.16f, 0.18f, 0.21f);
    for (const Point& p : controls) {
        drawPoint(p, 9.0f, 0.90f, 0.24f, 0.18f);
    }

    for (size_t level = 1; level < levels.size(); ++level) {
        float f = static_cast<float>(level) / static_cast<float>(levels.size() - 1);
        glLineWidth(1.25f);
        drawPolyline(levels[level], 0.10f + 0.45f * f, 0.34f, 0.88f - 0.50f * f);
        for (const Point& p : levels[level]) {
            drawPoint(p, 5.0f, 0.14f + 0.45f * f, 0.32f, 0.84f - 0.35f * f);
        }
    }

    std::vector<Point> curve;
    int samples = std::max(2, static_cast<int>(150.0f * std::max(g_t, 0.02f)));
    curve.reserve(samples + 1);
    for (int i = 0; i <= samples; ++i) {
        float t = g_t * static_cast<float>(i) / static_cast<float>(samples);
        curve.push_back(evaluateBezier(controls, t));
    }

    glLineWidth(4.0f);
    drawPolyline(curve, 0.02f, 0.54f, 0.32f);
    drawPoint(evaluateBezier(controls, g_t), 12.0f, 0.02f, 0.42f, 0.22f);

    char status[160];
    std::snprintf(
        status,
        sizeof(status),
        "Task1 Bezier Curve | 3-8 control points: %d | Space %s | R reset",
        g_controlCount,
        g_paused ? "resume" : "pause"
    );
    drawText(22.0f, static_cast<float>(g_height - 34), status);

    glutSwapBuffers();
}

void idle() {
    int now = glutGet(GLUT_ELAPSED_TIME);
    float delta = static_cast<float>(now - g_lastTime) / 1000.0f;
    g_lastTime = now;

    if (!g_paused) {
        g_t += delta * 0.22f;
        if (g_t > 1.0f) {
            g_t = 0.0f;
        }
    }

    glutPostRedisplay();
}

void reshape(int width, int height) {
    g_width = std::max(1, width);
    g_height = std::max(1, height);
    glViewport(0, 0, g_width, g_height);
}

void resetAnimation() {
    g_t = 0.0f;
    g_lastTime = glutGet(GLUT_ELAPSED_TIME);
}

void keyboard(unsigned char key, int, int) {
    if (key == 27) {
        std::exit(0);
    } else if (key >= '3' && key <= '8') {
        g_controlCount = key - '0';
        resetAnimation();
    } else if (key == ' ') {
        g_paused = !g_paused;
        g_lastTime = glutGet(GLUT_ELAPSED_TIME);
    } else if (key == 'r' || key == 'R') {
        resetAnimation();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(g_width, g_height);
    glutCreateWindow("exp07_task1 - de Casteljau Bezier Curve");

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    g_lastTime = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
