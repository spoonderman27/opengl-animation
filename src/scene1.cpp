#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <thread>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
using namespace std;

float swingPhase = 0.0f;
float angle = 0.0f;
float crackProgress = 0.0f;
int scenePhase = -1;
int timerCount = 0;
float cameraX = 0.0f, cameraY = 5.0f, cameraZ = 15.0f;
float cameraAngle = 0.0f;
float startAngle = 0.0f;
bool rotationComplete = false;
void playThunderAsync();
void playCrackAsync();
void playThunderSound();
void playCrackSound();

void drawHairBall(float cx, float cy, float cz, float radius, bool cutQuarter = false) {
  const int stacks = 18; // 纬度划分
  const int slices = 36; // 经度划分

  glDisable(GL_LIGHTING);

  glColor3f(0.36f, 0.23f, 0.15f); // 深棕色
  for (int i = 0; i < stacks; ++i) {
    float lat0 = M_PI * (-0.5 + (float)i / stacks);     // -π/2 到 π/2
    float lat1 = M_PI * (-0.5 + (float)(i + 1) / stacks);

    float z0 = sin(lat0), zr0 = cos(lat0);
    float z1 = sin(lat1), zr1 = cos(lat1);
        for (int j = 0; j < slices; ++j) {
          float lng0 = 2 * M_PI * (float)j / slices;           // 0 到 2π
            float lng1 = 2 * M_PI * (float)(j + 1) / slices;

            float x0 = cos(lng0), y0 = sin(lng0);
            float x1 = cos(lng1), y1 = sin(lng1);

            // 可选：跳过右下 1/4 区域
            if (cutQuarter && x0 > 0 && z0 < 0) continue;
            if (cutQuarter && x1 > 0 && z1 < 0) continue;

            glBegin(GL_QUADS);
            glVertex3f(cx + radius * x0 * zr0, cy + radius * y0 * zr0, cz + radius * z0);
            glVertex3f(cx + radius * x1 * zr0, cy + radius * y1 * zr0, cz + radius * z0);
            glVertex3f(cx + radius * x1 * zr1, cy + radius * y1 * zr1, cz + radius * z1);
            glVertex3f(cx + radius * x0 * zr1, cy + radius * y0 * zr1, cz + radius * z1);
            glEnd();
        }
    }
}

void drawFringe(float xOffset, float yOffset, float zOffset) {
  glColor3f(0.36f, 0.23f, 0.15f); // 深棕色
    glDisable(GL_LIGHTING);

    glBegin(GL_POLYGON);
    glVertex3f(400 - 3 + xOffset, 420 + yOffset, 411 + zOffset);
    glVertex3f(410 + xOffset, 420 + yOffset, 411 + zOffset);
    glVertex3f(410 + xOffset, 390 + yOffset, 411 + zOffset);

    glVertex3f(400 - 3 + xOffset, 390 + yOffset, 411 + zOffset);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(400 - 10 + xOffset, 420 + yOffset, 411 + zOffset);
    glVertex3f(410 - 10 + xOffset, 420 + yOffset, 411 + zOffset);
    glVertex3f(410 - 10 + xOffset, 415 + yOffset, 411 + zOffset);
    glVertex3f(405 - 10 + xOffset, 389 + yOffset, 411 + zOffset);
glVertex3f(400 - 10 + xOffset, 389 + yOffset, 411 + zOffset);

glEnd();

/*glVertex3f(410 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(390 + xOffset, 390 + yOffset, 411 + zOffset);
glVertex3f(390 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(385 + xOffset, 390 + yOffset, 411 + zOffset);*/
glBegin(GL_POLYGON);
glVertex3f(400 - 15 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 15 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 15 + xOffset, 415 + yOffset, 411 + zOffset);
glVertex3f(405 - 15 + xOffset, 389 + yOffset, 411 + zOffset);
glVertex3f(400 - 15 + xOffset, 389 + yOffset, 411 + zOffset);

glEnd();

glBegin(GL_POLYGON);
glVertex3f(400 - 20 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 20 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 20 + xOffset, 390 + yOffset, 411 + zOffset);

glVertex3f(405 - 20 + xOffset, 390 + yOffset, 411 + zOffset);

glVertex3f(410 - 20 + xOffset, 415 + yOffset, 411 + zOffset);
glEnd();



glBegin(GL_POLYGON);
glVertex3f(400 - 28 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 28 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 28 + xOffset, 390 + yOffset, 411 + zOffset);

glVertex3f(400 - 28 + xOffset, 390 + yOffset, 411 + zOffset);
glEnd();

glBegin(GL_POLYGON);
glVertex3f(400 - 32 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 32 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 32 + xOffset, 390 + yOffset, 411 + zOffset);

glVertex3f(400 - 32 + xOffset, 390 + yOffset, 411 + zOffset);
glEnd();

glBegin(GL_POLYGON);
glVertex3f(400 - 40 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 40 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 40 + xOffset, 415 + yOffset, 411 + zOffset);
glVertex3f(405 - 40 + xOffset, 390 + yOffset, 411 + zOffset);
glVertex3f(400 - 40 + xOffset, 390 + yOffset, 411 + zOffset);

glEnd();

glBegin(GL_POLYGON);
glVertex3f(400 - 48 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 48 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 48 + xOffset, 390 + yOffset, 411 + zOffset);

glVertex3f(400 - 48 + xOffset, 390 + yOffset, 411 + zOffset);
glEnd();

glBegin(GL_POLYGON);
glVertex3f(400 - 57 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 57 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 57 + xOffset, 415 + yOffset, 411 + zOffset);
glVertex3f(405 - 57 + xOffset, 389 + yOffset, 411 + zOffset);
glVertex3f(400 - 57 + xOffset, 389 + yOffset, 411 + zOffset);

glEnd();

glBegin(GL_POLYGON);
glVertex3f(400 - 67 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 67 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 67 + xOffset, 389 + yOffset, 411 + zOffset);

glVertex3f(400 - 67 + xOffset, 389 + yOffset, 411 + zOffset);
glEnd();

glBegin(GL_POLYGON);
glVertex3f(400 - 77 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 77 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 77 + xOffset, 389 + yOffset, 411 + zOffset);

glVertex3f(405 - 77 + xOffset, 389 + yOffset, 411 + zOffset);

glVertex3f(410 - 77 + xOffset, 415 + yOffset, 411 + zOffset);

glEnd();



glBegin(GL_POLYGON);
glVertex3f(400 - 84 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 84 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 84 + xOffset, 390 + yOffset, 411 + zOffset);

glVertex3f(400 - 84 + xOffset, 390 + yOffset, 411 + zOffset);
glEnd();

glBegin(GL_POLYGON);
glVertex3f(400 - 97 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 94 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 94 + xOffset, 390 + yOffset, 411 + zOffset);

glVertex3f(405 - 97 + xOffset, 390 + yOffset, 411 + zOffset);

glVertex3f(410 - 97 + xOffset, 415 + yOffset, 411 + zOffset);

glEnd();

glBegin(GL_POLYGON);
glVertex3f(400 - 110 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 110 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 110 + xOffset, 390 + yOffset, 411 + zOffset);
glVertex3f(400 - 110 + xOffset, 390 + yOffset, 411 + zOffset);
glEnd();



glBegin(GL_POLYGON);
glVertex3f(400 - 105 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 105 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(410 - 105 + xOffset, 390 + yOffset, 411 + zOffset);

glVertex3f(400 - 105 + xOffset, 390 + yOffset, 411 + zOffset);
glEnd();

//left side
glBegin(GL_POLYGON);
glVertex3f(289 + xOffset, 420 + yOffset, 411 + zOffset);
glVertex3f(289 + xOffset, 420 + yOffset, 291 + zOffset);
glVertex3f(289 + xOffset, 390 + yOffset, 291 + zOffset);

glVertex3f(289 + xOffset, 390 + yOffset, 411 + zOffset);
glEnd();



glBegin(GL_POLYGON);
glVertex3f(289 + xOffset, 420 + yOffset, 360 - 20 + zOffset);
glVertex3f(289 + xOffset, 420 + yOffset, 410 - 20 + zOffset);
glVertex3f(289 + xOffset, 415 + yOffset, 410 - 20 + zOffset);
glVertex3f(289 + xOffset, 370 + yOffset, 390 - 20 + zOffset);
glVertex3f(289 + xOffset, 370 + yOffset, 360 - 20 + zOffset);
glEnd();

glBegin(GL_POLYGON);
glVertex3f(289 + xOffset, 420 + yOffset, 290 + zOffset);
glVertex3f(289 + xOffset, 420 + yOffset, 410 - 50 + zOffset);
glVertex3f(289 + xOffset, 415 + yOffset, 410 - 50 + zOffset);
glVertex3f(289 + xOffset, 310 + yOffset, 390 - 50 + zOffset);
glVertex3f(289 + xOffset, 310 + yOffset, 290 + zOffset);
glEnd();
    //right side
    glBegin(GL_POLYGON);
    glVertex3f(411 + xOffset, 420 + yOffset, 411 + zOffset);
    glVertex3f(411 + xOffset, 420 + yOffset, 291 + zOffset);
    glVertex3f(411 + xOffset, 390 + yOffset, 291 + zOffset);

    glVertex3f(411 + xOffset, 390 + yOffset, 411 + zOffset);
    glEnd();



    glBegin(GL_POLYGON);
    glVertex3f(411 + xOffset, 420 + yOffset, 360 - 20 + zOffset);
    glVertex3f(411 + xOffset, 420 + yOffset, 410 - 20 + zOffset);
    glVertex3f(411 + xOffset, 415 + yOffset, 410 - 20 + zOffset);
    glVertex3f(411 + xOffset, 370 + yOffset, 390 - 20 + zOffset);
    glVertex3f(411 + xOffset, 370 + yOffset, 360 - 20 + zOffset);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(411 + xOffset, 420 + yOffset, 290 + zOffset);
    glVertex3f(411 + xOffset, 420 + yOffset, 410 - 50 + zOffset);
    glVertex3f(411 + xOffset, 415 + yOffset, 410 - 50 + zOffset);
    glVertex3f(411 + xOffset, 310 + yOffset, 390 - 50 + zOffset);
    glVertex3f(411 + xOffset, 310 + yOffset, 290 + zOffset);
    glEnd();



    glBegin(GL_QUADS);
    glVertex3f(290 + xOffset, 310 + yOffset, 290 + zOffset);
    glVertex3f(410 + xOffset, 310 + yOffset, 290 + zOffset);
    glVertex3f(410 + xOffset, 420 + yOffset, 290 + zOffset);
    glVertex3f(290 + xOffset, 420 + yOffset, 290 + zOffset);
    glEnd();



}
// Enhanced lighting setup
void setupLighting() {
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);

    // Main light (white, from above)
    GLfloat light0_pos[] = { 2.0f, 3.0f, 2.0f, 1.0f };
    GLfloat light0_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light0_specular[] = { 0.7f, 0.7f, 0.7f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    // Secondary light (blue-ish, from side)
    GLfloat light1_pos[] = { -2.0f, 1.0f, 2.0f, 1.0f };
    GLfloat light1_ambient[] = { 0.05f, 0.05f, 0.1f, 1.0f };
    GLfloat light1_diffuse[] = { 0.4f, 0.4f, 0.6f, 1.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);

    // Global ambient light
    GLfloat global_ambient[] = { 0.5f, 0.5f, 0.45f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
}

// Enhanced material properties
void setMaterial(float r, float g, float b, float shininess = 32.0f) {
   GLfloat mat_ambient[] = { r * 0.3f, g * 0.3f, b * 0.3f, 1.0f };
   GLfloat mat_diffuse[] = { r, g, b, 1.0f };
   GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_shininess[] = { shininess };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

// Cube drawing function from the first code
void drawCube(float size) {
   float h = size / 2.0f;
   glBegin(GL_QUADS);

    // Front face
    glNormal3f(0, 0, 1);
    glVertex3f(-h, -h, h);
    glVertex3f(h, -h, h);
    glVertex3f(h, h, h);
    glVertex3f(-h, h, h);

    // Back face
    glNormal3f(0, 0, -1);
    glVertex3f(-h, -h, -h);
    glVertex3f(-h, h, -h);
    glVertex3f(h, h, -h);
    glVertex3f(h, -h, -h);

    // Top face
    glNormal3f(0, 1, 0);
    glVertex3f(-h, h, -h);
    glVertex3f(-h, h, h);
    glVertex3f(h, h, h);
    glVertex3f(h, h, -h);

    // Bottom face
    glNormal3f(0, -1, 0);
    glVertex3f(-h, -h, -h);
    glVertex3f(h, -h, -h);
    glVertex3f(h, -h, h);
    glVertex3f(-h, -h, h);

    // Right face
    glNormal3f(1, 0, 0);
    glVertex3f(h, -h, -h);
    glVertex3f(h, h, -h);
    glVertex3f(h, h, h);
    glVertex3f(h, -h, h);

    // Left face
    glNormal3f(-1, 0, 0);
    glVertex3f(-h, -h, -h);
    glVertex3f(-h, -h, h);
    glVertex3f(-h, h, h);
    glVertex3f(-h, h, -h);

    glEnd();
}

// Liang character function from the first code
void drawLiang(int x, int y, int z, int movecha_x1, int movecha_y1, int movecha_z1, int
movecha_x2, int movecha_y2, int movecha_z2) {

    float armAngle = sin(swingPhase) * 15.0f; // 手臂摆动角度
    float legAngle = -sin(swingPhase) * 15.0f; // 腿摆动角度
    /*drawHairFringe(350, 380, 411);*/
    drawFringe(0 + x, -10 + y, 0 + z);



    drawHairBall(290 + x, 410 + y, 290 + z, 22, true);

    // 右边头发球（去掉右下 1/4）
    drawHairBall(410 + x, 410 + y, 290 + z, 22, true);

    //black inside eyes
    glColor3f(0.0f, 0.0f, 0.0f); // 黑色
    glBegin(GL_POLYGON);
for (int i = 0; i <= 40; ++i) {
  float angle = i * 3.14159f / 40; // 从 0 到 π
  float dx = cos(angle) * 11.5f;
  float dy = -sin(angle) * 14.5f;
  glVertex3f(325 + dx + x, 370 + dy + y, 412 + z);
}
glEnd();



glColor3f(0.0f, 0.0f, 0.0f); // 黑色
glBegin(GL_POLYGON);
for (int i = 0; i <= 40; ++i) {
  float angle = i * 3.14159f / 40; // 从 0 到 π
  float dx = cos(angle) * 11.5f;
  float dy = -sin(angle) * 14.5f;
  glVertex3f(375 + dx + x, 370 + dy + y, 412 + z);
}
glEnd();

//white inside eyes
glColor3f(1.0f, 1.0f, 1.0f); // 白色
glBegin(GL_POLYGON);

for (int i = 0; i <= 40; ++i) {
  float angle = i * 3.14159f / 40; // 从 0 到 π
  float dx = cos(angle) * 4.5f;
  float dy = -sin(angle) * 4.5f;
  glVertex3f(325 + dx + x, 368 + dy + y, 412.5 + z);
}
glEnd();



glColor3f(1.0f, 1.0f, 1.0f); // 白色
glBegin(GL_POLYGON);
for (int i = 0; i <= 40; ++i) {
  float angle = i * 3.14159f / 40; // 从 0 到 π
    float dx = cos(angle) * 4.5f;
    float dy = -sin(angle) * 4.5f;
    glVertex3f(375 + dx + x, 368 + y + dy, 412.5 + z);
}

glEnd();

//eyes
glBegin(GL_QUADS);
glColor3f(1.0f, 1.0f, 1.0f);
glVertex3f(310 + x, 345 + y, 411 + z);
glVertex3f(340 + x, 345 + y, 411 + z);
glVertex3f(340 + x, 370 + y, 411 + z);
glVertex3f(310 + x, 370 + y, 411 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 1.0f, 1.0f);
glVertex3f(360 + x, 345 + y, 411 + z);
glVertex3f(390 + x, 345 + y, 411 + z);
glVertex3f(390 + x, 370 + y, 411 + z);
glVertex3f(360 + x, 370 + y, 411 + z);
glEnd();

//eyebrow (left)
glBegin(GL_QUADS);
glColor3f(0.0f, 0.0f, 0.0f);
glVertex3f(310 + x, 375 + y, 411 + z);
glVertex3f(340 + x, 375 + y, 411 + z);
glVertex3f(340 + x, 380 + y, 411 + z);
glVertex3f(310 + x, 380 + y, 411 + z);
glEnd();

//eyebrow (right)
glBegin(GL_QUADS);
glColor3f(0.0f, 0.0f, 0.0f);
glVertex3f(360 + x, 375 + y, 411 + z);
glVertex3f(390 + x, 375 + y, 411 + z);
glVertex3f(390 + x, 380 + y, 411 + z);
glVertex3f(360 + x, 380 + y, 411 + z);
glEnd();

//mouth
glBegin(GL_QUADS);
glColor3f(0.0f, 0.0f, 0.0f);
glVertex3f(335 + x, 320 + y, 412 + z);
glVertex3f(375 + x, 320 + y, 412 + z);
glVertex3f(375 + x, 325 + y, 412 + z);
glVertex3f(335 + x, 325 + y, 412 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.0f, 0.0f, 0.0f);
glVertex3f(315 + x, 320 + y, 412 + z);
glVertex3f(325 + x, 320 + y, 412 + z);
glVertex3f(325 + x, 325 + y, 412 + z);
glVertex3f(315 + x, 325 + y, 412 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 1.0f, 1.0f);
glVertex3f(325 + x, 320 + y, 412 + z);
glVertex3f(365 + x, 320 + y, 412 + z);
glVertex3f(365 + x, 325 + y, 412 + z);
glVertex3f(325 + x, 325 + y, 412 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.0f, 0.0f, 0.0f);
glVertex3f(375 + x, 320 + y, 412 + z);
glVertex3f(385 + x, 320 + y, 412 + z);
glVertex3f(385 + x, 325 + y, 412 + z);
glVertex3f(375 + x, 325 + y, 412 + z);
glEnd();

//Left smiley pixel
glBegin(GL_QUADS);
glColor3f(0.0f, 0.0f, 0.0f);
glVertex3f(315 + x, 325 + y, 412 + z);
glVertex3f(320 + x, 325 + y, 412 + z);
glVertex3f(320 + x, 330 + y, 412 + z);
glVertex3f(315 + x, 330 + y, 412 + z);
glEnd();

// Right smiley pixel
glBegin(GL_QUADS);
glColor3f(0.0f, 0.0f, 0.0f);
glVertex3f(380 + x, 325 + y, 412 + z);
glVertex3f(385 + x, 325 + y, 412 + z);
glVertex3f(385 + x, 330 + y, 412 + z);
glVertex3f(380 + x, 330 + y, 412 + z);
glEnd();

//front face
glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(290 + x, 300 + y, 410 + z);
glVertex3f(410 + x, 300 + y, 410 + z);
glVertex3f(410 + x, 410 + y, 410 + z);
glVertex3f(290 + x, 410 + y, 410 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.36f, 0.23f, 0.15f); // 深棕色
glVertex3f(290 + x, 300 + y, 290 + z);
glVertex3f(410 + x, 300 + y, 290 + z);
glVertex3f(410 + x, 410 + y, 290 + z);
glVertex3f(290 + x, 410 + y, 290 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(290 + x, 300 + y, 290 + z);
glVertex3f(290 + x, 300 + y, 410 + z);
glVertex3f(290 + x, 410 + y, 410 + z);
glVertex3f(290 + x, 410 + y, 290 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(410 + x, 300 + y, 290 + z);
glVertex3f(410 + x, 300 + y, 410 + z);
glVertex3f(410 + x, 410 + y, 410 + z);
glVertex3f(410 + x, 410 + y, 290 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.0f, 0.0f, 1.0f);
glVertex3f(290 + x, 300 + y, 290 + z);
glVertex3f(410 + x, 300 + y, 290 + z);
glVertex3f(410 + x, 300 + y, 410 + z);
glVertex3f(290 + x, 300 + y, 410 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.36f, 0.23f, 0.15f); // 深棕色
glVertex3f(290 + x, 410 + y, 290 + z);
glVertex3f(410 + x, 410 + y, 290 + z);
glVertex3f(410 + x, 410 + y, 410 + z);
glVertex3f(290 + x, 410 + y, 410 + z);
glEnd();

//neck
glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(330 + x, 300 + y, 370 + z);
glVertex3f(370 + x, 300 + y, 370 + z);
glVertex3f(370 + x, 280 + y, 370 + z);
glVertex3f(330 + x, 280 + y, 370 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(330 + x, 300 + y, 330 + z);
glVertex3f(370 + x, 300 + y, 330 + z);
glVertex3f(370 + x, 280 + y, 330 + z);
glVertex3f(330 + x, 280 + y, 330 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(330 + x, 300 + y, 330 + z);
glVertex3f(330 + x, 300 + y, 370 + z);
glVertex3f(330 + x, 280 + y, 370 + z);
glVertex3f(330 + x, 280 + y, 330 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(370 + x, 300 + y, 330 + z);
glVertex3f(370 + x, 300 + y, 370 + z);
glVertex3f(370 + x, 280 + y, 370 + z);
glVertex3f(370 + x, 280 + y, 330 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(330 + x, 300 + y, 330 + z);
glVertex3f(370 + x, 300 + y, 330 + z);
glVertex3f(370 + x, 300 + y, 370 + z);
glVertex3f(330 + x, 300 + y, 370 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(330 + x, 280 + y, 330 + z);
glVertex3f(370 + x, 280 + y, 330 + z);
glVertex3f(370 + x, 280 + y, 370 + z);
glVertex3f(330 + x, 280 + y, 370 + z);
glEnd();
//body
glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(250 + x, 160 + y, 425 + z);
glVertex3f(450 + x, 160 + y, 425 + z);
glVertex3f(450 + x, 280 + y, 425 + z);
glVertex3f(250 + x, 280 + y, 425 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(250 + x, 160 + y, 275 + z);
glVertex3f(450 + x, 160 + y, 275 + z);
glVertex3f(450 + x, 280 + y, 275 + z);
glVertex3f(250 + x, 280 + y, 275 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(250 + x, 160 + y, 275 + z);
glVertex3f(250 + x, 160 + y, 425 + z);
glVertex3f(250 + x, 280 + y, 425 + z);
glVertex3f(250 + x, 280 + y, 275 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(450 + x, 160 + y, 275 + z);
glVertex3f(450 + x, 160 + y, 425 + z);
glVertex3f(450 + x, 280 + y, 425 + z);
glVertex3f(450 + x, 280 + y, 275 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(250 + x, 160 + y, 275 + z);
glVertex3f(450 + x, 160 + y, 275 + z);
glVertex3f(450 + x, 160 + y, 425 + z);
glVertex3f(250 + x, 160 + y, 425 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(250 + x, 280 + y, 275 + z);
glVertex3f(450 + x, 280 + y, 275 + z);
glVertex3f(450 + x, 280 + y, 425 + z);
glVertex3f(250 + x, 280 + y, 425 + z);
glEnd();

//上衣 left side
glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(250 + x, 160 + y, 426 + z);
glVertex3f(320 + x, 160 + y, 426 + z);
glVertex3f(320 + x, 280 + y, 426 + z);
glVertex3f(250 + x, 280 + y, 426 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(250 + x, 160 + y, 274 + z);
glVertex3f(450 + x, 160 + y, 274 + z);
glVertex3f(450 + x, 280 + y, 274 + z);
glVertex3f(250 + x, 280 + y, 274 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 1.0f, 0.0f);//skin colour
glVertex3f(300 + x, 160 + y, 426.1 + z);
glVertex3f(310 + x, 160 + y, 426.1 + z);
glVertex3f(310 + x, 280 + y, 426.1 + z);
glVertex3f(300 + x, 280 + y, 426.1 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 1.0f, 0.0f);//skin colour
glVertex3f(300 + x, 260 + y, 273.9 + z);
glVertex3f(310 + x, 260 + y, 273.9 + z);
glVertex3f(310 + x, 280 + y, 273.9 + z);
glVertex3f(300 + x, 280 + y, 273.9 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 1.0f, 0.0f);//skin colour
glVertex3f(300 + x, 260 + y, 273.9 + z);
glVertex3f(400 + x, 260 + y, 273.9 + z);
glVertex3f(400 + x, 255 + y, 273.9 + z);
glVertex3f(300 + x, 255 + y, 273.9 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(249 + x, 160 + y, 275 + z);
glVertex3f(249 + x, 160 + y, 425 + z);
glVertex3f(249 + x, 280 + y, 425 + z);
glVertex3f(249 + x, 280 + y, 275 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(451 + x, 160 + y, 275 + z);
glVertex3f(451 + x, 160 + y, 425 + z);
glVertex3f(451 + x, 280 + y, 425 + z);
glVertex3f(451 + x, 280 + y, 275 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(250 + x, 159 + y, 275 + z);
glVertex3f(320 + x, 159 + y, 275 + z);
glVertex3f(320 + x, 159 + y, 425 + z);
glVertex3f(250 + x, 159 + y, 425 + z);
glEnd();
glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(250 + x, 281 + y, 275 + z);
glVertex3f(320 + x, 281 + y, 275 + z);
glVertex3f(320 + x, 281 + y, 425 + z);
glVertex3f(250 + x, 281 + y, 425 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 1.0f, 0.0f);//skin colour
glVertex3f(300 + x, 281.1 + y, 275 + z);
glVertex3f(310 + x, 281.1 + y, 275 + z);
glVertex3f(310 + x, 281.1 + y, 425 + z);
glVertex3f(300 + x, 281.1 + y, 425 + z);
glEnd();

//上衣 right side
glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(380 + x, 160 + y, 426 + z);
glVertex3f(450 + x, 160 + y, 426 + z);
glVertex3f(450 + x, 280 + y, 426 + z);
glVertex3f(380 + x, 280 + y, 426 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(380 + x, 160 + y, 274 + z);
glVertex3f(450 + x, 160 + y, 274 + z);
glVertex3f(450 + x, 280 + y, 274 + z);
glVertex3f(380 + x, 280 + y, 274 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 1.0f, 0.0f);//skin colour
glVertex3f(390 + x, 160 + y, 426.1 + z);
glVertex3f(400 + x, 160 + y, 426.1 + z);
glVertex3f(400 + x, 280 + y, 426.1 + z);
glVertex3f(390 + x, 280 + y, 426.1 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 1.0f, 0.0f);//skin colour
glVertex3f(390 + x, 260 + y, 273.9 + z);
glVertex3f(400 + x, 260 + y, 273.9 + z);
glVertex3f(400 + x, 280 + y, 273.9 + z);
glVertex3f(390 + x, 280 + y, 273.9 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(379 + x, 160 + y, 275 + z);
glVertex3f(379 + x, 160 + y, 425 + z);
glVertex3f(379 + x, 280 + y, 425 + z);
glVertex3f(379 + x, 280 + y, 275 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(451 + x, 160 + y, 275 + z);
glVertex3f(451 + x, 160 + y, 425 + z);
glVertex3f(451 + x, 280 + y, 425 + z);
glVertex3f(451 + x, 280 + y, 275 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(380 + x, 159 + y, 275 + z);
glVertex3f(450 + x, 159 + y, 275 + z);
glVertex3f(450 + x, 159 + y, 425 + z);
glVertex3f(380 + x, 159 + y, 425 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 0.0f, 0.0f);//skin colour
glVertex3f(380 + x, 281 + y, 275 + z);
glVertex3f(450 + x, 281 + y, 275 + z);
glVertex3f(450 + x, 281 + y, 425 + z);
glVertex3f(380 + x, 281 + y, 425 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(1.0f, 1.0f, 0.0f);//skin colour
glVertex3f(390 + x, 281.1 + y, 275 + z);
glVertex3f(400 + x, 281.1 + y, 275 + z);
glVertex3f(400 + x, 281.1 + y, 425 + z);
glVertex3f(390 + x, 281.1 + y, 425 + z);
glEnd();

////////////////////////////////////////////////////////pants
//glPushMatrix();
//glTranslatef(350 + x, 130 + y, 350 + z);

//// 2. 围绕 Y 轴旋转
//glRotatef(armAngle, 1, 0, 0);

//// 3. 把模型移回原位（等效于反向平移）
//glTranslatef(-350 - x, -130 - y, -350 - z);

glBegin(GL_QUADS);
glColor3f(0.36f, 0.20f, 0.09f); // 深褐色
glVertex3f(250 + x + movecha_x1, 100 + y + movecha_y1, 426 + z + movecha_z1);
glVertex3f(350 + x + movecha_x1, 100 + y + movecha_y1, 426 + z + movecha_z1);
glVertex3f(350 + x + movecha_x1, 160 + y + movecha_y1, 426 + z + movecha_z1);
glVertex3f(250 + x + movecha_x1, 160 + y + movecha_y1, 426 + z + movecha_z1);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.36f, 0.20f, 0.09f); // 深褐色
glVertex3f(250 + x + movecha_x1, 99 + y + movecha_y1, 275 + z + movecha_z1);
glVertex3f(350 + x + movecha_x1, 99 + y + movecha_y1, 275 + z + movecha_z1);
glVertex3f(350 + x + movecha_x1, 99 + y + movecha_y1, 425 + z + movecha_z1);
glVertex3f(250 + x + movecha_x1, 99 + y + movecha_y1, 425 + z + movecha_z1);
     glEnd();

     glBegin(GL_QUADS);
     glColor3f(0.36f, 0.20f, 0.09f); // 深褐色
     glVertex3f(250 + x + movecha_x1, 100 + y + movecha_y1, 274 + z + movecha_z1);
     glVertex3f(350 + x + movecha_x1, 100 + y + movecha_y1, 274 + z + movecha_z1);
     glVertex3f(350 + x + movecha_x1, 160 + y + movecha_y1, 274 + z + movecha_z1);
     glVertex3f(250 + x + movecha_x1, 160 + y + movecha_y1, 274 + z + movecha_z1);
     glEnd();

     glBegin(GL_QUADS);
     glColor3f(0.36f, 0.20f, 0.09f); // 深褐色
     glVertex3f(250 + x + movecha_x1, 161 + y + movecha_y1, 275 + z + movecha_z1);
     glVertex3f(350 + x + movecha_x1, 161 + y + movecha_y1, 275 + z + movecha_z1);
     glVertex3f(350 + x + movecha_x1, 161 + y + movecha_y1, 425 + z + movecha_z1);
     glVertex3f(250 + x + movecha_x1, 161 + y + movecha_y1, 425 + z + movecha_z1);
     glEnd();

     glBegin(GL_QUADS);
     glColor3f(0.36f, 0.20f, 0.09f); // 深褐色
     glVertex3f(249.1 + x + movecha_x1, 100 + y + movecha_y1, 275 + z + movecha_z1);
     glVertex3f(249.1 + x + movecha_x1, 100 + y + movecha_y1, 425 + z + movecha_z1);
     glVertex3f(249.1 + x + movecha_x1, 160 + y + movecha_y1, 425 + z + movecha_z1);
     glVertex3f(249.1 + x + movecha_x1, 160 + y + movecha_y1, 275 + z + movecha_z1);
     glEnd();

     glPopMatrix();

     /*glPushMatrix();
     glTranslatef(350 + x, 130 + y, 350 + z);

     glRotatef(-armAngle, 1, 0, 0);
     glTranslatef(-350 - x, -130 - y, -350 - z);
*/
     glBegin(GL_QUADS);
     glColor3f(0.36f, 0.20f, 0.09f); // 深褐色
     glVertex3f(450.9 + x + movecha_x2, 100 + y + movecha_y2, 275 + z + movecha_z2);
     glVertex3f(450.9 + x + movecha_x2, 100 + y + movecha_y2, 425 + z + movecha_z2);
glVertex3f(450.9 + x + movecha_x2, 160 + y + movecha_y2, 425 + z + movecha_z2);
glVertex3f(450.9 + x + movecha_x2, 160 + y + movecha_y2, 275 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.36f, 0.20f, 0.09f); // 深褐色
glVertex3f(350 + x + movecha_x2, 100 + y + movecha_y2, 426 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 100 + y + movecha_y2, 426 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 160 + y + movecha_y2, 426 + z + movecha_z2);
glVertex3f(350 + x + movecha_x2, 160 + y + movecha_y2, 426 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.36f, 0.20f, 0.09f); // 深褐色
glVertex3f(350 + x + movecha_x2, 99 + y + movecha_y2, 275 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 99 + y + movecha_y2, 275 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 99 + y + movecha_y2, 425 + z + movecha_z2);
glVertex3f(350 + x + movecha_x2, 99 + y + movecha_y2, 425 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.36f, 0.20f, 0.09f); // 深褐色
glVertex3f(350 + x + movecha_x2, 161 + y + movecha_y2, 275 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 161 + y + movecha_y2, 275 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 161 + y + movecha_y2, 425 + z + movecha_z2);
glVertex3f(350 + x + movecha_x2, 161 + y + movecha_y2, 425 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.36f, 0.20f, 0.09f); // 深褐色
glVertex3f(350 + x + movecha_x2, 100 + y + movecha_y2, 274 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 100 + y + movecha_y2, 274 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 160 + y + movecha_y2, 274 + z + movecha_z2);
glVertex3f(350 + x + movecha_x2, 160 + y + movecha_y2, 274 + z + movecha_z2);
glEnd();

glPopMatrix();
 //pants desigb
 /*glPushMatrix();
 glTranslatef(350 + x, 130 + y, 350 + z);

 glRotatef(armAngle, 1, 0, 0);
 glTranslatef(-350 - x, -130 - y, -350 - z);*/
 for (int x_design = 0; x_design <= 40; x_design = x_design + 10) {
   int y_design = x_design / 2;
   glBegin(GL_QUADS);
   glColor3f(0.66f, 0.60f, 0.09f); // 深褐色
   glVertex3f(290 + x + movecha_x1 + x_design, 110 + y + movecha_y1 + y_design, 427 + z +
movecha_z1);
   glVertex3f(300 + x + movecha_x1 + x_design, 110 + y + movecha_y1 + y_design, 427 + z +
movecha_z1);
   glVertex3f(300 + x + movecha_x1 + x_design, 120 + y + movecha_y1 + y_design, 427 + z +
movecha_z1);
   glVertex3f(290 + x + movecha_x1 + x_design, 120 + y + movecha_y1 + y_design, 427 + z +
movecha_z1);
   glEnd();

 }
 glLineWidth(10.0f);
 glBegin(GL_LINES);
 glColor3f(0.66f, 0.60f, 0.09f); // 深褐色
 glVertex3f(350 + x + movecha_x1, 145 + y + movecha_y1, 427 + z + movecha_z1);
 glVertex3f(300 + x + movecha_x1, 145 + y + movecha_y1, 427 + z + movecha_z1);
 glVertex3f(300 + x + movecha_x1, 145 + y + movecha_y1, 427 + z + movecha_z1);
 glVertex3f(300 + x + movecha_x1, 160 + y + movecha_y1, 427 + z + movecha_z1);
 /*glVertex3f(350 + x + movecha_x1, 160 + y + movecha_y1 , 452 + z + movecha_z1);*/
 glEnd();

 /*glPopMatrix();

 glPushMatrix();
 glTranslatef(350 + x, 130 + y, 350 + z);

 glRotatef(-armAngle, 1, 0, 0);
 glTranslatef(-350 - x, -130 - y, -350 - z);*/
 for (int x_design = 0; x_design <= 40; x_design = x_design + 10) {
   int y_design = x_design / 2;
   glBegin(GL_QUADS);
   glColor3f(0.66f, 0.60f, 0.09f); // 深褐色
   glVertex3f(390 + x + movecha_x2 - x_design, 110 + y + movecha_y2 + y_design, 427 + z +
movecha_z2);
   glVertex3f(400 + x + movecha_x2 - x_design, 110 + y + movecha_y2 + y_design, 427 + z +
movecha_z2);
   glVertex3f(400 + x + movecha_x2 - x_design, 120 + y + movecha_y2 + y_design, 427 + z +
movecha_z2);
   glVertex3f(390 + x + movecha_x2 - x_design, 120 + y + movecha_y2 + y_design, 427 + z +
movecha_z2);
   glEnd();

 }

 glLineWidth(10.0f);
 glBegin(GL_LINES);
 glColor3f(0.66f, 0.60f, 0.09f); // 深褐色
 glVertex3f(400 + x + movecha_x2, 145 + y + movecha_y2, 427 + z + movecha_z2);
 glVertex3f(350 + x + movecha_x2, 145 + y + movecha_y2, 427 + z + movecha_z2);
 glVertex3f(400 + x + movecha_x2, 145 + y + movecha_y2, 427 + z + movecha_z2);
 glVertex3f(400 + x + movecha_x2, 160 + y + movecha_y2, 427 + z + movecha_z2);
 /*glVertex3f(350 + x + movecha_x1, 160 + y + movecha_y1 , 452 + z + movecha_z1);*/
 glEnd();

 glPopMatrix();

 glPushMatrix();
 glTranslatef(350 + x, 130 + y, 350 + z);

 glRotatef(armAngle, 1, 0, 0);
 glTranslatef(-350 - x, -130 - y, -350 - z);

 //left leg
 glBegin(GL_QUADS);
 glColor3f(0.98f, 0.89f, 0.76f);//skin colour
 glVertex3f(275 + x + movecha_x1, 100 + y + movecha_y1, 326 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 100 + y + movecha_y1, 326 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 60 + y + movecha_y1, 326 + z + movecha_z1);
glVertex3f(275 + x + movecha_x1, 60 + y + movecha_y1, 326 + z + movecha_z1);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(275 + x + movecha_x1, 100 + y + movecha_y1, 374 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 100 + y + movecha_y1, 374 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 60 + y + movecha_y1, 374 + z + movecha_z1);
glVertex3f(275 + x + movecha_x1, 60 + y + movecha_y1, 374 + z + movecha_z1);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(274 + x + movecha_x1, 100 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(274 + x + movecha_x1, 100 + y + movecha_y1, 325 + z + movecha_z1);
glVertex3f(274 + x + movecha_x1, 60 + y + movecha_y1, 325 + z + movecha_z1);
glVertex3f(274 + x + movecha_x1, 60 + y + movecha_y1, 375 + z + movecha_z1);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(275 + x + movecha_x1, 98 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 98 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 98 + y + movecha_y1, 325 + z + movecha_z1);
glVertex3f(275 + x + movecha_x1, 98 + y + movecha_y1, 325 + z + movecha_z1);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(275 + x + movecha_x1, 61 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 61 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 61 + y + movecha_y1, 325 + z + movecha_z1);
glVertex3f(275 + x + movecha_x1, 61 + y + movecha_y1, 325 + z + movecha_z1);
glEnd();

glPopMatrix();
//right leg
glPushMatrix();
glTranslatef(350 + x, 130 + y, 350 + z);

glRotatef(-armAngle, 1, 0, 0);
glTranslatef(-350 - x, -130 - y, -350 - z);

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(375 + x + movecha_x2, 100 + y + movecha_y2, 326 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 100 + y + movecha_y2, 326 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 60 + y + movecha_y2, 326 + z + movecha_z2);
glVertex3f(375 + x + movecha_x2, 60 + y + movecha_y2, 326 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(375 + x + movecha_x2, 100 + y + movecha_y2, 374 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 100 + y + movecha_y2, 374 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 60 + y + movecha_y2, 374 + z + movecha_z2);
glVertex3f(375 + x + movecha_x2, 60 + y + movecha_y2, 374 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(426 + x + movecha_x2, 100 + y + movecha_y2, 375 + z + movecha_z2);
glVertex3f(426 + x + movecha_x2, 100 + y + movecha_y2, 325 + z + movecha_z2);
glVertex3f(426 + x + movecha_x2, 60 + y + movecha_y2, 325 + z + movecha_z2);
glVertex3f(426 + x + movecha_x2, 60 + y + movecha_y2, 375 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(375 + x + movecha_x2, 98 + y + movecha_y2, 375 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 98 + y + movecha_y2, 375 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 98 + y + movecha_y2, 325 + z + movecha_z2);
glVertex3f(375 + x + movecha_x2, 98 + y + movecha_y2, 325 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(375 + x + movecha_x2, 61 + y + movecha_y2, 375 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 61 + y + movecha_y2, 375 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 61 + y + movecha_y2, 325 + z + movecha_z2);
glVertex3f(375 + x + movecha_x2, 61 + y + movecha_y2, 325 + z + movecha_z2);
glEnd();

glPopMatrix();

glPushMatrix();
glTranslatef(225 + x, 280 + y, 350 + z);

glRotatef(armAngle, 1, 0, 0);
glTranslatef(-225 - x, -280 - y, -350 - z);
//right hand
glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(500 + x + movecha_x2, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(450 + x + movecha_x2, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glEnd();

glPopMatrix();

glPushMatrix();

glTranslatef(225 + x, 280 + y, 350 + z);

glRotatef(-armAngle, 1, 0, 0);
glTranslatef(-225 - x, -280 - y, -350 - z);
//left hand
glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(200 + x + movecha_x1, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + x + movecha_x1, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.98f, 0.89f, 0.76f);//skin colour
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.98f, 0.89f, 0.76f);//skin colour
    glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
    glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
    glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
    glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
    glEnd();

    glPopMatrix();

}

// Draw 3D ground plane for better depth perception
void drawGround() {
   glPushMatrix();
   setMaterial(0.2f, 0.4f, 0.2f, 8.0f);
   glTranslatef(0.0f, -1.0f, 0.0f);
   glScalef(8.0f, 0.1f, 8.0f);
   glutSolidCube(1.0f);
   glPopMatrix();
}

// Enhanced 3D oval egg with shading
void drawOvalEgg() {
   glDisable(GL_LIGHTING);
   glColor3f(1.0f, 1.0f, 1.0f);
   glPushMatrix();
   glTranslatef(0.0f, 0.3f, 0.0f);
   glRotatef(angle, 0.0f, 1.0f, 0.0f);
   glScalef(1.0f, 1.3f, 1.0f);
   glutSolidSphere(1.0, 50, 50);
   glPopMatrix();
   glEnable(GL_LIGHTING);
}

// Enhanced egg half with proper normals and materials
void draw3DCrackPolygon() {
    glPushMatrix();
    glTranslatef(-0.25f, 0.3f, 1.01f); // 放置在左蛋壳前表面
    glScalef(1.0f, 1.3f, 1.0f);     // 拉高比例匹配蛋壳
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

    float crackWidth = 0.02f;
    float z = 0.0f;
    std::vector<float> leftX, leftY, rightX, rightY;

    for (int i = 0; i <= 10; i++) {
      float y = 1.2f - (i * 0.16f);
      float x = (i % 2 == 0) ? -0.08f : 0.08f;
      leftX.push_back(x - crackWidth);
      leftY.push_back(y);
      rightX.push_back(x + crackWidth);
      rightY.push_back(y);
    }

    glBegin(GL_POLYGON);
    for (int i = 0; i <= 10; i++)
      glVertex3f(rightX[i], rightY[i], z);
    for (int i = 10; i >= 0; i--)
      glVertex3f(leftX[i], leftY[i], z);
    glEnd();

    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void draw3DCrackPolygon_1(float x, float y, float z, float scale) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glScalef(scale, scale, scale);
  draw3DCrackPolygon();

    glPopMatrix();
}
void draw3DCrackPolygon_2(float x, float y, float z, float scale,float angle) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(angle, 0.0f, 1.0f, 0.0f);
  glScalef(scale, scale, scale);



    draw3DCrackPolygon();

    glPopMatrix();
}



void drawEggHalf(bool leftHalf) {
  glDisable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 0.0f);
    glScalef(1.0f, 1.3f, 1.0f);

    double planeEq[4];
    if (leftHalf) {
       planeEq[0] = -0.5; planeEq[1] = 0.0; planeEq[2] = 0.0; planeEq[3] = 0.0; // x >= 0
    }
    else {
       planeEq[0] = 0.5; planeEq[1] = 0.0; planeEq[2] = 0.0; planeEq[3] = 0.0; // x <= 0
    }

    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, planeEq);
    glutSolidSphere(1.0, 50, 50);
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
    glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    if (leftHalf) {

        //draw3DCrackPolygon_1(-0.4, -1.3, -3, 1.8); // 把裂缝画在左壳上
    }
}

// 3D crack with depth - creates a visible gap between egg halves
void draw3DCrack() {
   glPushMatrix();
   glTranslatef(0.0f, 0.0f, 0.0f);
   glDisable(GL_LIGHTING);
   glColor3f(0.0f, 0.0f, 0.0f);
   glLineWidth(3.0f);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 10; i++) {
      float y = 1.2f - (i * 0.16f);
      float x = (i % 2 == 0) ? -0.08f : 0.08f;
      float z = 0.6f;
      glVertex3f(x, y, z);
    }
    glEnd();

    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// Enhanced 3D thunderbolt with volumetric effect
void drawThunderbolt(float progress) {
   glPushMatrix();

    // Disable lighting for glowing effect
    glDisable(GL_LIGHTING);
// Flash background with 3D depth
float flashAlpha = (progress < 0.5f) ? (1.0f - progress * 2.0f) : 0.0f;
if (flashAlpha > 0.0f) {
   glEnable(GL_BLEND);
   glColor4f(1.0f, 1.0f, 0.8f, flashAlpha * 0.3f);

    // Create multiple flash layers for depth
    for (int i = 0; i < 3; i++) {
       glPushMatrix();
       glTranslatef(0.0f, 0.0f, -1.0f + i * 0.5f);
       glScalef(8.0f - i, 8.0f - i, 1.0f);
       glBegin(GL_QUADS);
       glVertex3f(-1.0f, -1.0f, 0.0f);
       glVertex3f(1.0f, -1.0f, 0.0f);
       glVertex3f(1.0f, 1.0f, 0.0f);
       glVertex3f(-1.0f, 1.0f, 0.0f);
       glEnd();
       glPopMatrix();
    }
    glDisable(GL_BLEND);
}

// 3D Thunderbolt with multiple segments
glTranslatef(0.0f, 2.6f, 0.0f);

// Main bolt (bright yellow-white)
glColor3f(1.0f, 1.0f, 0.8f);
glLineWidth(6.0f);
glBegin(GL_LINE_STRIP);
glVertex3f(0.0f, 0.0f, 0.0f);
glVertex3f(-0.1f, -0.2f, 0.0f);
glVertex3f(0.05f, -0.4f, 0.0f);
glVertex3f(-0.1f, -0.6f, 0.0f);
glVertex3f(0.0f, -0.8f, 0.0f);
glVertex3f(-0.05f, -1.0f, 0.0f);
glVertex3f(0.0f, -1.2f, 0.0f);
glEnd();
    // Secondary bolts for depth
    glColor3f(0.8f, 0.8f, 1.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.02f, 0.0f, 0.1f);
    glVertex3f(-0.08f, -0.2f, 0.1f);
    glVertex3f(0.07f, -0.4f, 0.1f);
    glVertex3f(-0.08f, -0.6f, 0.1f);
    glVertex3f(0.02f, -0.8f, 0.1f);
    glVertex3f(-0.03f, -1.0f, 0.1f);
    glVertex3f(0.02f, -1.2f, 0.1f);
    glEnd();

    glEnable(GL_LIGHTING);
    glPopMatrix();
}



// Draw glitching stars in the sky
void drawGlitchingStars() {
   glDisable(GL_LIGHTING);
   glPointSize(2.0f);

    // Static stars
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 100; i++) {
       float x = -10.0f + (rand() % 50);
       float y = 3.0f + (rand() % 6);
       float z = -10.0f + (rand() % 20);
       glVertex3f(x, y, z);
    }
    glEnd();

    // Glitching stars that flicker
    float glitchTime = timerCount * 0.1f;
    if (sin(glitchTime * 3.0f) > 0.3f) {
       glColor3f(1.0f, 0.3f, 0.3f);
        glPointSize(4.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < 20; i++) {
          float x = -20.0f + (rand() % 40);
          float y = 5.0f + (rand() % 10);
          float z = -12.0f - (rand() % 3);
          if (sin(glitchTime * 5.0f + i) > 0.0f) {
             glVertex3f(x, y, z);
          }
        }
        glEnd();
    }

    // Constellation lines that glitch
    if (sin(glitchTime * 2.0f) > -0.5f) {
       glColor3f(0.5f, 0.8f, 1.0f);
       glLineWidth(1.0f);
       glBegin(GL_LINES);
       glVertex3f(-10.0f, 8.0f, -12.0f);
       glVertex3f(-5.0f, 12.0f, -12.0f);
       glVertex3f(-5.0f, 12.0f, -12.0f);
       glVertex3f(2.0f, 10.0f, -12.0f);
       glVertex3f(2.0f, 10.0f, -12.0f);
       glVertex3f(8.0f, 15.0f, -12.0f);
       glEnd();
    }

    glEnable(GL_LIGHTING);
}

// Draw ancient temple columns
void drawTempleColumn(float x, float z, float height) {
   glPushMatrix();
   glTranslatef(x, height / 2 - 1.0f, z);

    // Column base
    setMaterial(0.7f, 0.6f, 0.4f, 16.0f);
    glPushMatrix();
    glTranslatef(0, -height / 2 + 0.2f, 0);
    glScalef(0.8f, 0.4f, 0.8f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Column shaft
    glPushMatrix();
    glScalef(0.5f, height, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Column capital
    glPushMatrix();
    glTranslatef(0, height / 2 - 0.2f, 0);
    glScalef(0.8f, 0.4f, 0.8f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

// Draw temple structure
void drawTempleBackground() {
   glDisable(GL_LIGHTING);

    // Sky gradient background
    glPushMatrix();
    glBegin(GL_QUADS);
    // Top - dark purple/blue
    glColor3f(0.08f, 0.05f, 0.15f);
    glVertex3f(-20, 10, -12);
    glVertex3f(20, 10, -12);
    // Middle - lighter with golden tint
    glColor3f(0.12f, 0.08f, 0.18f);
    glVertex3f(20, 2, -12);
    glVertex3f(-20, 2, -12);
    glEnd();
glBegin(GL_QUADS);
glColor3f(0.12f, 0.08f, 0.18f);
glVertex3f(-20, 2, -12);
glVertex3f(20, 2, -12);
// Bottom - darkest with slight orange glow
glColor3f(0.05f, 0.03f, 0.08f);
glVertex3f(20, -2, -12);
glVertex3f(-20, -2, -12);
glEnd();
glPopMatrix();

// Main temple structure
glPushMatrix();
glTranslatef(0, -1, -10);

// Temple base platform
glColor3f(0.3f, 0.25f, 0.2f);
glBegin(GL_QUADS);
glVertex3f(-8, 0, 0);
glVertex3f(8, 0, 0);
glVertex3f(8, 0.5f, 0);
glVertex3f(-8, 0.5f, 0);
glEnd();

// Main temple wall
glColor3f(0.4f, 0.35f, 0.25f);
glBegin(GL_QUADS);
glVertex3f(-6, 0.5f, 0);
glVertex3f(6, 0.5f, 0);
glVertex3f(6, 4, 0);
glVertex3f(-6, 4, 0);
glEnd();

// Temple pillars
glColor3f(0.45f, 0.4f, 0.3f);
for (int i = -2; i <= 2; i++) {
   if (i != 0) { // Skip center for door
      glBegin(GL_QUADS);
        glVertex3f(i * 2.5f - 0.2f, 0.5f, 0.1f);
        glVertex3f(i * 2.5f + 0.2f, 0.5f, 0.1f);
        glVertex3f(i * 2.5f + 0.2f, 4, 0.1f);
        glVertex3f(i * 2.5f - 0.2f, 4, 0.1f);
        glEnd();
    }
}

// Temple roof - traditional curved style
glColor3f(0.2f, 0.15f, 0.1f);

// Main roof section
glBegin(GL_TRIANGLES);
glVertex3f(-7, 4, 0.2f);
glVertex3f(0, 6, 0.2f);
glVertex3f(7, 4, 0.2f);
glEnd();

// Roof edges with slight curve effect
glBegin(GL_QUADS);
glVertex3f(-7.5f, 3.8f, 0.3f);
glVertex3f(7.5f, 3.8f, 0.3f);
glVertex3f(7, 4, 0.2f);
glVertex3f(-7, 4, 0.2f);
glEnd();

// Temple door
glColor3f(0.15f, 0.1f, 0.05f);
glBegin(GL_QUADS);
glVertex3f(-1, 0.5f, 0.1f);
glVertex3f(1, 0.5f, 0.1f);
glVertex3f(1, 3, 0.1f);
glVertex3f(-1, 3, 0.1f);
glEnd();

// Door frame
glColor3f(0.5f, 0.4f, 0.3f);
glLineWidth(3.0f);
glBegin(GL_LINE_LOOP);
glVertex3f(-1.1f, 0.5f, 0.11f);
glVertex3f(1.1f, 0.5f, 0.11f);
glVertex3f(1.1f, 3.1f, 0.11f);
glVertex3f(-1.1f, 3.1f, 0.11f);
glEnd();

glPopMatrix();

// Side temple buildings
// Left temple
glPushMatrix();
glTranslatef(-12, -1, -9);
glColor3f(0.35f, 0.3f, 0.2f);
glBegin(GL_QUADS);
glVertex3f(-2, 0, 0);
glVertex3f(2, 0, 0);
glVertex3f(2, 2.5f, 0);
glVertex3f(-2, 2.5f, 0);
glEnd();

// Left temple roof
glColor3f(0.2f, 0.15f, 0.1f);
glBegin(GL_TRIANGLES);
glVertex3f(-2.5f, 2.5f, 0);
glVertex3f(0, 3.5f, 0);
glVertex3f(2.5f, 2.5f, 0);
glEnd();
glPopMatrix();

// Right temple
glPushMatrix();
glTranslatef(12, -1, -9);
glColor3f(0.35f, 0.3f, 0.2f);
glBegin(GL_QUADS);
glVertex3f(-2, 0, 0);
glVertex3f(2, 0, 0);
glVertex3f(2, 2.5f, 0);
glVertex3f(-2, 2.5f, 0);
glEnd();

// Right temple roof
glColor3f(0.2f, 0.15f, 0.1f);
glBegin(GL_TRIANGLES);
glVertex3f(-2.5f, 2.5f, 0);
glVertex3f(0, 3.5f, 0);
glVertex3f(2.5f, 2.5f, 0);
glEnd();
glPopMatrix();

// Temple lanterns
glColor3f(0.8f, 0.6f, 0.2f);
for (int i = -1; i <= 1; i += 2) {
   glPushMatrix();
   glTranslatef(i * 4, 1, -9.5f);

    // Lantern body
    glScalef(0.3f, 0.5f, 0.3f);
    glutSolidSphere(1.0, 8, 8);
    glPopMatrix();

    // Lantern glow
    glColor4f(1.0f, 0.8f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(i * 4, 1, -9.5f);
    glScalef(0.5f, 0.7f, 0.5f);
    glutSolidSphere(1.0, 8, 8);
    glPopMatrix();
    glColor3f(0.8f, 0.6f, 0.2f);
}

// Distant mountains behind temple
glColor3f(0.15f, 0.12f, 0.18f);
glBegin(GL_TRIANGLES);
// Left mountain
glVertex3f(-20, -1, -11);
    glVertex3f(-12, 2, -11);
    glVertex3f(-4, -1, -11);

    // Right mountain
    glVertex3f(4, -1, -11);
    glVertex3f(12, 2, -11);
    glVertex3f(20, -1, -11);
    glEnd();

    // Temple steps
    glColor3f(0.25f, 0.2f, 0.15f);
    for (int step = 0; step < 3; step++) {
       glBegin(GL_QUADS);
       glVertex3f(-7 + step * 0.2f, -1 + step * 0.2f, 1 + step * 0.3f);
       glVertex3f(7 - step * 0.2f, -1 + step * 0.2f, 1 + step * 0.3f);
       glVertex3f(7 - step * 0.2f, -1 + step * 0.2f + 0.2f, 1 + step * 0.3f);
       glVertex3f(-7 + step * 0.2f, -1 + step * 0.2f + 0.2f, 1 + step * 0.3f);
       glEnd();
    }

    glEnable(GL_LIGHTING);
}

// Draw mysterious temple mist
void drawTempleMist() {
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_LIGHTING);



    float mistTime = timerCount * 0.02f;

    int timecount = 0;
    // Multiple layers of mist
    for (int layer = 0; layer < 3; layer++) {
       glColor4f(0.6f, 0.7f, 0.9f, 0.1f + 0.05f * layer);

      for (int i = 0; i < 4; i++) {
          /*timecount++;
          if (timecount > 10)
             break;*/
          glPushMatrix();
          int rand1 = rand() % 16;
          float mistX = -8.0f + (rand1);
          float mistY = -0.3f + layer * 0.5f + 0.3f * sin(mistTime + i);
          int rand2 = rand() % 4;
          float mistZ = -10.0f + (rand2);

          glTranslatef(mistX, mistY, mistZ);
          glScalef(2.0f + layer, 0.5f, 2.0f + layer);

          // Render mist as transparent spheres
          glutSolidSphere(1.0f, 8, 8);
          glPopMatrix();
      }
    }
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnd();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

    float camX = 6.0f * sin(cameraAngle * M_PI / 180.0f);
    float camZ = 6.0f * cos(cameraAngle * M_PI / 180.0f);
    float camY = 1.5f + sin(cameraAngle * 0.5f * M_PI / 180.0f) * 0.3f;

    gluLookAt(camX, camY, camZ,
      0.0, 0.0, 0.0,
      0.0, 1.0, 0.0);

    if (scenePhase == -1) {

      glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(-1.0f, -0.8f, -5.0f);
    const char* message = "Once upon a time...";
    for (const char* c = message; *c != '\0'; c++) {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
    return;
}

// Draw background and stars
drawGlitchingStars();
drawTempleBackground();

// Ground
glDisable(GL_LIGHTING);
glColor3f(0.1f, 0.1f, 0.08f);
glBegin(GL_QUADS);
glVertex3f(-15, -1, -15);
glVertex3f(15, -1, -15);
glVertex3f(15, -1, 15);
glVertex3f(-15, -1, 15);
glEnd();
glEnable(GL_LIGHTING);

if (scenePhase == 0) {
   drawOvalEgg();
}
else if (scenePhase == 1) {
   drawOvalEgg();
   float thunderProgress = (timerCount <= 15) ? (timerCount / 15.0f) : 1.0f;
   drawThunderbolt(thunderProgress);
}
else if (scenePhase == 2) {
   float maxAngle = 60.0f;
   float currentAngle = crackProgress * maxAngle;
      float separation = crackProgress * 0.5f;

      glPushMatrix();
      glTranslatef(-0.5f * 0.7f - separation, 0.0f, 0.0f);
      glRotatef(10, 0.0f, 1.0f, 0.0f);
      glTranslatef(0.5f * 0.7f, 0.0f, 0.0f);
      drawEggHalf(true);
      draw3DCrackPolygon_1(0.35, -0.6, -0.7, 1.18); // 把裂缝画在左壳上

      glPopMatrix();

      glPushMatrix();
      glTranslatef(0.5f * 0.7f + separation, 0.0f, 0.0f);
      glRotatef(0, 0.0f, 1.0f, 0.0f);
      glTranslatef(-0.5f * 0.7f, 0.0f, 0.0f);
      drawEggHalf(false);
      draw3DCrackPolygon_2(-0.33, -0.57, 1.6, 1.15,180); // 把裂缝画在左壳上

      glPopMatrix();

    }
    else if (scenePhase == 3) {
      if (!rotationComplete) {
         cameraAngle += 3.0f;
         if (cameraAngle - startAngle >= 360.0f) {
            rotationComplete = true;
         }
      }
      drawTempleMist();
      glScalef(0.004, 0.004, 0.004);
      drawLiang(-350, -200, -350, 0, 0, 0, 0, 0, 0);
    }

    glutSwapBuffers();
}

void playThunderAsync() {
  std::thread(playThunderSound).detach();
}

void playCrackAsync() {
  std::thread(playCrackSound).detach();
}

void update(int value) {
  timerCount++;

    if (scenePhase == -1) {
       if (timerCount >= 50) { // 4 seconds
          scenePhase = 0;
          timerCount = 0;
       }
    }
    else if (scenePhase == 0) {
       if (timerCount > 20) {
          scenePhase = 1;
          timerCount = 0;
       }
    }
    else if (scenePhase == 1) {
       if (timerCount == 1)
          playThunderAsync();

      if (timerCount > 55) {
         scenePhase = 2;
         timerCount = 0;
         crackProgress = 0.8f;
      }
    }
    else if (scenePhase == 2) {
      if (timerCount == 1)
         playCrackAsync();

      if (timerCount <= 10)
         crackProgress = 0.8f + (timerCount / 10.0f) * 0.2f;
        if (timerCount > 32) {
           scenePhase = 3;
           timerCount = 0;
           startAngle = cameraAngle;
           rotationComplete = false;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(80, update, 0);
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50.0, (float)w / (float)h, 0.1, 1000000.0);
  glMatrixMode(GL_MODELVIEW);
}

void playThunderSound() {
  mciSendStringA("open \"sounds\\Thunder (1).wav\" type waveaudio alias thunder", NULL, 0, NULL);
  mciSendStringA("play thunder", NULL, 0, NULL);

    Sleep(4600);
    mciSendStringA("close thunder", NULL, 0, NULL);
}

void playCrackSound() {
  mciSendStringA("open \"sounds\\EggCracking.wav\" type waveaudio alias crack", NULL, 0, NULL);
  mciSendStringA("play crack", NULL, 0, NULL);

    Sleep(2000);
    mciSendStringA("close crack", NULL, 0, NULL);
}
void init() {
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
  printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    setupLighting();

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable line smoothing
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Enable point smoothing
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    scenePhase = -1;
    srand((unsigned int)time(NULL));
    printf("Enhanced OpenGL initialization complete.\n");
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1280, 760);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Scene 1");
    if (glutGet(GLUT_WINDOW_COLORMAP_SIZE) == 0) {
       printf("Warning: Could not create OpenGL context with current settings\n");
    }

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(80, update, 0);
    glutMainLoop();
    return 0;
}
