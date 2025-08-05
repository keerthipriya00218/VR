#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

// Car and animation variables
float carX = 0.0f;
float wheelAngle = 0.0f;
float cloudOffset = 0.0f;

// Building data
const int numBuildings = 12;
float buildingHeights[numBuildings];

// Initialize building heights (once)
void initBuildings() {
    for (int i = 0; i < numBuildings; ++i) {
        buildingHeights[i] = 100 + rand() % 100;
    }
}

void drawBuilding(float x, float height) {
    glColor3f(0.4f, 0.4f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(x, 100);
    glVertex2f(x + 60, 100);
    glVertex2f(x + 60, 100 + height);
    glVertex2f(x, 100 + height);
    glEnd();
}

void drawCity() {
    for (int i = 0; i < numBuildings; ++i) {
        drawBuilding(i * 100 - 300, buildingHeights[i]);
    }
}

void drawRoad() {
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(0, 100);
    glVertex2f(800, 100);
    glVertex2f(800, 160);
    glVertex2f(0, 160);
    glEnd();

    glColor3f(1, 1, 1);
    for (int i = 0; i < 800; i += 60) {
        glBegin(GL_QUADS);
        glVertex2f(i, 127);
        glVertex2f(i + 30, 127);
        glVertex2f(i + 30, 133);
        glVertex2f(i, 133);
        glEnd();
    }
}

void drawCloud(float x, float y) {
    glColor3f(1, 1, 1);
    for (int i = 0; i < 3; ++i) {
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; ++j) {
            float angle = j * 3.1416f / 180.0f;
            float radius = 15;
            glVertex2f(x + i * 20 + radius * cos(angle), y + radius * sin(angle));
        }
        glEnd();
    }
}

void drawSun() {
    glColor3f(1.0f, 0.85f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; ++i) {
        float angle = i * 3.1416f / 180.0f;
        glVertex2f(700 + 40 * cos(angle), 500 + 40 * sin(angle));
    }
    glEnd();
}

void drawWheel(float cx, float cy) {
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; ++i) {
        float angle = i * 3.1416f / 180.0f;
        glVertex2f(cx + 15 * cos(angle), cy + 15 * sin(angle));
    }
    glEnd();

    // Spokes
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    for (int i = 0; i < 4; ++i) {
        float angle = wheelAngle + i * 3.1416f / 2;
        glVertex2f(cx, cy);
        glVertex2f(cx + 15 * cos(angle), cy + 15 * sin(angle));
    }
    glEnd();
}

void drawCar() {
    float baseY = 170.0f;

    // Car body
    glColor3f(0.8f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(carX + 50, baseY);
    glVertex2f(carX + 200, baseY);
    glVertex2f(carX + 200, baseY + 40);
    glVertex2f(carX + 170, baseY + 70);
    glVertex2f(carX + 80, baseY + 70);
    glVertex2f(carX + 50, baseY + 40);
    glEnd();

    // Windows
    glColor3f(0.6f, 0.9f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(carX + 90, baseY + 45);
    glVertex2f(carX + 160, baseY + 45);
    glVertex2f(carX + 150, baseY + 65);
    glVertex2f(carX + 100, baseY + 65);
    glEnd();

    // Wheels
    drawWheel(carX + 70, baseY);
    drawWheel(carX + 180, baseY);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Sky
    glColor3f(0.5f, 0.8f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();

    drawSun();
    drawCloud(150 + cloudOffset, 500);
    drawCloud(400 + cloudOffset, 530);
    drawCloud(650 + cloudOffset, 490);

    drawCity();
    drawRoad();
    drawCar();

    glutSwapBuffers();
}

void update(int value) {
    cloudOffset -= 0.5f;
    if (cloudOffset < -800) cloudOffset = 0;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);  // ~60 FPS
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
    case 'a': carX -= 5; wheelAngle -= 0.2f; break;
    case 'd': carX += 5; wheelAngle += 0.2f; break;
    case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 0, 600);
    initBuildings();  // only once
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Car Driving in a City");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
