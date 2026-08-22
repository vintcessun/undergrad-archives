#include <GL/freeglut.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

struct Point3 {
    float x;
    float y;
    float z;
};

const float PI = 3.14159265358979323846f;

int g_gridSize = 4;
float g_t = 0.0f;
bool g_paused = false;
int g_lastTime = 0;
int g_width = 1000;
int g_height = 740;
float g_yaw = -34.0f;
float g_pitch = 25.0f;

Point3 lerp(const Point3& a, const Point3& b, float t) {
    return {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    };
}

std::vector<std::vector<Point3>> makeControlGrid(int n) {
    std::vector<std::vector<Point3>> grid(n, std::vector<Point3>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            float u = static_cast<float>(i) / static_cast<float>(n - 1);
            float v = static_cast<float>(j) / static_cast<float>(n - 1);
            float x = -3.2f + 6.4f * u;
            float z = -3.2f + 6.4f * v;
            float wave = std::sin(u * PI) * std::cos(v * PI);
            float dome = 0.55f * std::sin(u * PI) * std::sin(v * PI);
            float twist = 0.35f * std::sin((u + v) * 2.0f * PI);
            grid[i][j] = {x, 1.30f * wave + dome + twist, z};
        }
    }

    return grid;
}

std::vector<std::vector<Point3>> deCasteljauLevels(const std::vector<Point3>& controls, float t) {
    std::vector<std::vector<Point3>> levels;
    levels.push_back(controls);

    while (levels.back().size() > 1) {
        const std::vector<Point3>& previous = levels.back();
        std::vector<Point3> next;
        next.reserve(previous.size() - 1);

        for (size_t i = 0; i + 1 < previous.size(); ++i) {
            next.push_back(lerp(previous[i], previous[i + 1], t));
        }

        levels.push_back(next);
    }

    return levels;
}

Point3 evaluateCurve(const std::vector<Point3>& controls, float t) {
    return deCasteljauLevels(controls, t).back().front();
}

Point3 evaluateSurface(const std::vector<std::vector<Point3>>& controls, float u, float v) {
    std::vector<Point3> rowPoints;
    rowPoints.reserve(controls.size());

    for (const std::vector<Point3>& row : controls) {
        rowPoints.push_back(evaluateCurve(row, v));
    }

    return evaluateCurve(rowPoints, u);
}

void color(float r, float g, float b, float a = 1.0f) {
    glColor4f(r, g, b, a);
}

void drawPoint(const Point3& p, float size, float r, float g, float b) {
    glPointSize(size);
    color(r, g, b);
    glBegin(GL_POINTS);
    glVertex3f(p.x, p.y, p.z);
    glEnd();
}

void drawPolyline(const std::vector<Point3>& points, float r, float g, float b) {
    if (points.empty()) {
        return;
    }

    color(r, g, b);
    glBegin(GL_LINE_STRIP);
    for (const Point3& p : points) {
        glVertex3f(p.x, p.y, p.z);
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

    glDisable(GL_DEPTH_TEST);
    color(0.08f, 0.09f, 0.10f);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawControlNet(const std::vector<std::vector<Point3>>& controls) {
    int n = static_cast<int>(controls.size());

    glLineWidth(1.5f);
    for (int i = 0; i < n; ++i) {
        drawPolyline(controls[i], 0.17f, 0.19f, 0.22f);
    }

    for (int j = 0; j < n; ++j) {
        std::vector<Point3> column;
        column.reserve(n);
        for (int i = 0; i < n; ++i) {
            column.push_back(controls[i][j]);
        }
        drawPolyline(column, 0.17f, 0.19f, 0.22f);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            drawPoint(controls[i][j], 6.5f, 0.90f, 0.24f, 0.18f);
        }
    }
}

void drawAnimatedSurface(const std::vector<std::vector<Point3>>& controls) {
    int gridSamples = 28;
    int active = std::max(2, static_cast<int>(gridSamples * std::max(g_t, 0.03f)));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    color(0.10f, 0.56f, 0.82f, 0.38f);

    for (int i = 0; i < active; ++i) {
        float u0 = g_t * static_cast<float>(i) / static_cast<float>(active);
        float u1 = g_t * static_cast<float>(i + 1) / static_cast<float>(active);

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= active; ++j) {
            float v = g_t * static_cast<float>(j) / static_cast<float>(active);
            Point3 p0 = evaluateSurface(controls, u0, v);
            Point3 p1 = evaluateSurface(controls, u1, v);
            glVertex3f(p0.x, p0.y, p0.z);
            glVertex3f(p1.x, p1.y, p1.z);
        }
        glEnd();
    }

    glDisable(GL_BLEND);

    glLineWidth(1.1f);
    for (int i = 0; i <= active; ++i) {
        float a = g_t * static_cast<float>(i) / static_cast<float>(active);
        std::vector<Point3> lineU;
        std::vector<Point3> lineV;
        lineU.reserve(active + 1);
        lineV.reserve(active + 1);

        for (int j = 0; j <= active; ++j) {
            float b = g_t * static_cast<float>(j) / static_cast<float>(active);
            lineU.push_back(evaluateSurface(controls, a, b));
            lineV.push_back(evaluateSurface(controls, b, a));
        }

        drawPolyline(lineU, 0.02f, 0.34f, 0.50f);
        drawPolyline(lineV, 0.02f, 0.34f, 0.50f);
    }
}

void drawConstruction(const std::vector<std::vector<Point3>>& controls) {
    std::vector<Point3> rowPoints;
    rowPoints.reserve(controls.size());

    for (const std::vector<Point3>& row : controls) {
        std::vector<std::vector<Point3>> levels = deCasteljauLevels(row, g_t);
        glLineWidth(1.4f);
        for (size_t i = 1; i < levels.size(); ++i) {
            drawPolyline(levels[i], 0.78f, 0.50f, 0.10f);
        }
        rowPoints.push_back(levels.back().front());
    }

    glLineWidth(3.0f);
    drawPolyline(rowPoints, 0.70f, 0.18f, 0.48f);

    std::vector<std::vector<Point3>> columnLevels = deCasteljauLevels(rowPoints, g_t);
    for (size_t i = 1; i < columnLevels.size(); ++i) {
        drawPolyline(columnLevels[i], 0.58f, 0.10f, 0.42f);
    }

    drawPoint(columnLevels.back().front(), 11.0f, 0.02f, 0.42f, 0.22f);
}

void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = g_height == 0 ? 1.0f : static_cast<float>(g_width) / static_cast<float>(g_height);
    if (aspect >= 1.0f) {
        glOrtho(-6.2f * aspect, 6.2f * aspect, -5.2f, 5.2f, -30.0f, 30.0f);
    } else {
        glOrtho(-6.2f, 6.2f, -5.2f / aspect, 5.2f / aspect, -30.0f, 30.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() {
    glClearColor(0.94f, 0.95f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setupCamera();

    std::vector<std::vector<Point3>> controls = makeControlGrid(g_gridSize);

    glPushMatrix();
    glRotatef(g_pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(g_yaw, 0.0f, 1.0f, 0.0f);
    drawControlNet(controls);
    drawAnimatedSurface(controls);
    drawConstruction(controls);
    glPopMatrix();

    char status[180];
    std::snprintf(
        status,
        sizeof(status),
        "Task2 Bezier Surface | 3-5 grid: %dx%d | Space %s | R reset | arrows rotate",
        g_gridSize,
        g_gridSize,
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
        g_t += delta * 0.18f;
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
    } else if (key >= '3' && key <= '5') {
        g_gridSize = key - '0';
        resetAnimation();
    } else if (key == ' ') {
        g_paused = !g_paused;
        g_lastTime = glutGet(GLUT_ELAPSED_TIME);
    } else if (key == 'r' || key == 'R') {
        resetAnimation();
    }
}

void specialKeyboard(int key, int, int) {
    if (key == GLUT_KEY_LEFT) {
        g_yaw -= 4.0f;
    } else if (key == GLUT_KEY_RIGHT) {
        g_yaw += 4.0f;
    } else if (key == GLUT_KEY_UP) {
        g_pitch += 4.0f;
    } else if (key == GLUT_KEY_DOWN) {
        g_pitch -= 4.0f;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_width, g_height);
    glutCreateWindow("exp07_task2 - de Casteljau Bezier Surface");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    g_lastTime = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
