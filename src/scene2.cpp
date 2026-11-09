#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")



#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



// Villager and camera position
float villagerX = -7.0f;
float cameraX = -8.0f;
float speed = 0.06f;
float liangX = cameraX + 1.0f;
float liangY = -300;     // or whatever Y you want
float liangZ = -150;

bool showDialog = true;
const char* dialogText = "Let me plant seeds"; // Start with first dialog
int startTime = 0;
bool started = false;
bool showFire = false;
bool showSmoke = false;
int smokeStartTime = 0;
bool showVillagerDialog = false;
const char* villagerDialogText = "";
bool showExplosion = false;
int explosionStartTime = 0;

bool soundAPlayed = false;
bool soundBPlayed = false;




void drawGround() {
  glColor3f(0.4f, 0.5f, 0.2f); // Ancient mossy grass
  glBegin(GL_QUADS);
  glVertex3f(-50, -1, -50);
  glVertex3f(50, -1, -50);
  glVertex3f(50, -1, 50);
  glVertex3f(-50, -1, 50);
  glEnd();
}

void drawRunway() {
  int numTiles = 50;
  float tileLength = 1.0f;
  float tileWidth = 1.0f;

  for (int i = 0; i < numTiles; ++i) {
    // Alternate color for cobblestone pattern
    float base = 0.45f + ((i % 2) * 0.05f);
    glColor3f(base, 0.35f, 0.25f);
    glBegin(GL_QUADS);
    glVertex3f(i * tileLength - 25.0f, -0.98f, -0.5f);
    glVertex3f(i * tileLength - 24.0f, -0.98f, -0.5f);
        glVertex3f(i * tileLength - 24.0f, -0.98f, 0.5f);
        glVertex3f(i * tileLength - 25.0f, -0.98f, 0.5f);
        glEnd();
    }
}

void drawHut(float x, float z) {
  glPushMatrix();
  glTranslatef(x, -1.0f, z);

    glScalef(1.5f, 1.5f, 1.5f);

    // Base (dark mud brick)
    glColor3f(0.35f, 0.2f, 0.1f);
    glutSolidCube(1.0f);



    glColor3f(0.15f, 0.07f, 0.02f); // Darker wood
    glBegin(GL_QUADS);

    glVertex3f(-0.25f, -0.5f, 0.51f);
    glVertex3f(0.25f, -0.5f, 0.51f);
    glVertex3f(0.25f, 0.0f, 0.51f);
    glVertex3f(-0.25f, 0.0f, 0.51f);
    glEnd();



    glColor3f(0.6f, 0.6f, 0.5f);
    glBegin(GL_QUADS);
    glVertex3f(-0.51f, 0.1f, -0.2f);
    glVertex3f(-0.51f, 0.1f, 0.2f);
    glVertex3f(-0.51f, 0.4f, 0.2f);
    glVertex3f(-0.51f, 0.4f, -0.2f);
    glEnd();



    glBegin(GL_QUADS);
  glVertex3f(0.51f, 0.1f, -0.2f);
  glVertex3f(0.51f, 0.1f, 0.2f);
  glVertex3f(0.51f, 0.4f, 0.2f);
  glVertex3f(0.51f, 0.4f, -0.2f);
  glEnd();



  glTranslatef(0, 0.5f, 0);
  glColor3f(0.7f, 0.5f, 0.2f);

  glBegin(GL_TRIANGLES);
  // Front face
  glVertex3f(-0.5f, 0.0f, 0.5f);
  glVertex3f(0.5f, 0.0f, 0.5f);
  glVertex3f(0.0f, 0.8f, 0.0f);

  // Right face
  glVertex3f(0.5f, 0.0f, 0.5f);
  glVertex3f(0.5f, 0.0f, -0.5f);
  glVertex3f(0.0f, 0.8f, 0.0f);

  // Back face
  glVertex3f(0.5f, 0.0f, -0.5f);
  glVertex3f(-0.5f, 0.0f, -0.5f);
  glVertex3f(0.0f, 0.8f, 0.0f);

  // Left face
  glVertex3f(-0.5f, 0.0f, -0.5f);
  glVertex3f(-0.5f, 0.0f, 0.5f);
  glVertex3f(0.0f, 0.8f, 0.0f);
  glEnd();

  glPopMatrix();
}
void drawCave(float x, float z) {
  glPushMatrix();
  glTranslatef(x, -1.0f, z);
  glScalef(2.0f, 2.0f, 1.5f);
// Colors
GLfloat caveColor1[] = { 0.3f, 0.25f, 0.2f };
GLfloat caveColor2[] = { 0.2f, 0.18f, 0.15f };
GLfloat black[] = { 0.0f, 0.0f, 0.0f };

// Arch shape for cave
GLfloat archX[] = { -1.0f, -0.8f, -0.4f, 0.0f, 0.4f, 0.8f, 1.0f, 1.0f, -1.0f };
GLfloat archY[] = { 0.0f, 0.6f, 1.0f, 1.2f, 1.0f, 0.6f, 0.0f, -1.0f, -1.0f };
int archPoints = 9;



glBegin(GL_POLYGON);
glColor3fv(caveColor2);
for (int i = 0; i < archPoints; i++)
  glVertex3f(archX[i], archY[i], -1.0f);
glEnd();



glBegin(GL_POLYGON);
glColor3fv(caveColor1);
for (int i = 0; i < archPoints; i++)
  glVertex3f(archX[i], archY[i], 1.0f);
glEnd();



glBegin(GL_POLYGON);
glColor3fv(black);
for (int i = 0; i < archPoints; i++) {
  glVertex3f(archX[i] * 0.85f, archY[i] * 0.85f, 1.01f);
}
glEnd();



glBegin(GL_QUAD_STRIP);
for (int i = 0; i < archPoints; i++) {
  glColor3fv(i % 2 == 0 ? caveColor1 : caveColor2);
  glVertex3f(archX[i], archY[i], 1.0f);
      glVertex3f(archX[i], archY[i], -1.0f);
    }
    glEnd();

    glPopMatrix();
}

void drawRock(float x, float z) {
  glPushMatrix();
  glTranslatef(x, -1.0f, z);
  glColor3f(0.3f, 0.3f, 0.3f);
  glScalef(0.3f, 0.2f, 0.3f);
  glutSolidIcosahedron();
  glPopMatrix();
}

void drawHairFringe(float xOffset, float yOffset, float zOffset) {
  glColor3f(0.36f, 0.23f, 0.15f);



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
void drawHairBall(float cx, float cy, float cz, float radius, bool cutQuarter = false) {
  const int stacks = 18;
  const int slices = 36;

  glColor3f(0.36f, 0.23f, 0.15f);
  for (int i = 0; i < stacks; ++i) {
    float lat0 = M_PI * (-0.5 + (float)i / stacks);
    float lat1 = M_PI * (-0.5 + (float)(i + 1) / stacks);
        float z0 = sin(lat0), zr0 = cos(lat0);
        float z1 = sin(lat1), zr1 = cos(lat1);

        for (int j = 0; j < slices; ++j) {
          float lng0 = 2 * M_PI * (float)j / slices;
          float lng1 = 2 * M_PI * (float)(j + 1) / slices;

            float x0 = cos(lng0), y0 = sin(lng0);
            float x1 = cos(lng1), y1 = sin(lng1);

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
  glColor3f(0.36f, 0.23f, 0.15f);

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
void drawLiang(int x, int y, int z, int movecha_x1, int movecha_y1, int movecha_z1, int
movecha_x2, int movecha_y2, int movecha_z2) {

 float swingPhase = 0.0f;
float armAngle = sin(swingPhase) * 15.0f;
float legAngle = -sin(swingPhase) * 15.0f;
drawHairFringe(0 + x, -10 + y, 0 + z);



drawHairBall(290 + x, 410 + y, 290 + z, 22, true);



drawHairBall(410 + x, 410 + y, 290 + z, 22, true);

glColor3f(0.0f, 0.0f, 0.0f);
glBegin(GL_POLYGON);

for (int i = 0; i <= 40; ++i) {
  float angle = i * 3.14159f / 40;
  float dx = cos(angle) * 11.5f;
  float dy = -sin(angle) * 14.5f;
  glVertex3f(325 + dx + x, 370 + dy + y, 412 + z);
}
glEnd();



glColor3f(0.0f, 0.0f, 0.0f);
glBegin(GL_POLYGON);
for (int i = 0; i <= 40; ++i) {
  float angle = i * 3.14159f / 40;
  float dx = cos(angle) * 11.5f;
  float dy = -sin(angle) * 14.5f;
  glVertex3f(375 + dx + x, 370 + dy + y, 412 + z);
}
glEnd();



glColor3f(1.0f, 1.0f, 1.0f);
glBegin(GL_POLYGON);

for (int i = 0; i <= 40; ++i) {
  float angle = i * 3.14159f / 40;
  float dx = cos(angle) * 4.5f;
  float dy = -sin(angle) * 4.5f;
  glVertex3f(325 + dx + x, 368 + dy + y, 412.5 + z);
}
glEnd();



glColor3f(1.0f, 1.0f, 1.0f);
glBegin(GL_POLYGON);
for (int i = 0; i <= 40; ++i) {
  float angle = i * 3.14159f / 40;
  float dx = cos(angle) * 4.5f;
  float dy = -sin(angle) * 4.5f;
  glVertex3f(375 + dx + x, 368 + y + dy, 412.5 + z);
}

glEnd();

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



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(290 + x, 300 + y, 410 + z);
glVertex3f(410 + x, 300 + y, 410 + z);
glVertex3f(410 + x, 410 + y, 410 + z);
glVertex3f(290 + x, 410 + y, 410 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.36f, 0.23f, 0.15f);
glVertex3f(290 + x, 300 + y, 290 + z);
glVertex3f(410 + x, 300 + y, 290 + z);
glVertex3f(410 + x, 410 + y, 290 + z);
glVertex3f(290 + x, 410 + y, 290 + z);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(290 + x, 300 + y, 290 + z);
glVertex3f(290 + x, 300 + y, 410 + z);
glVertex3f(290 + x, 410 + y, 410 + z);
glVertex3f(290 + x, 410 + y, 290 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
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
glColor3f(0.36f, 0.23f, 0.15f);
glVertex3f(290 + x, 410 + y, 290 + z);
glVertex3f(410 + x, 410 + y, 290 + z);
glVertex3f(410 + x, 410 + y, 410 + z);
glVertex3f(290 + x, 410 + y, 410 + z);

glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(330 + x, 300 + y, 370 + z);
glVertex3f(370 + x, 300 + y, 370 + z);
glVertex3f(370 + x, 280 + y, 370 + z);
glVertex3f(330 + x, 280 + y, 370 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(330 + x, 300 + y, 330 + z);
glVertex3f(370 + x, 300 + y, 330 + z);
glVertex3f(370 + x, 280 + y, 330 + z);
glVertex3f(330 + x, 280 + y, 330 + z);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(330 + x, 300 + y, 330 + z);
glVertex3f(330 + x, 300 + y, 370 + z);
glVertex3f(330 + x, 280 + y, 370 + z);
glVertex3f(330 + x, 280 + y, 330 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(370 + x, 300 + y, 330 + z);
glVertex3f(370 + x, 300 + y, 370 + z);
glVertex3f(370 + x, 280 + y, 370 + z);
glVertex3f(370 + x, 280 + y, 330 + z);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(330 + x, 300 + y, 330 + z);
glVertex3f(370 + x, 300 + y, 330 + z);
glVertex3f(370 + x, 300 + y, 370 + z);
glVertex3f(330 + x, 300 + y, 370 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(330 + x, 280 + y, 330 + z);
glVertex3f(370 + x, 280 + y, 330 + z);
glVertex3f(370 + x, 280 + y, 370 + z);
glVertex3f(330 + x, 280 + y, 370 + z);

glEnd();
//body
glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(250 + x, 160 + y, 425 + z);
glVertex3f(450 + x, 160 + y, 425 + z);
glVertex3f(450 + x, 280 + y, 425 + z);
glVertex3f(250 + x, 280 + y, 425 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(250 + x, 160 + y, 275 + z);
glVertex3f(450 + x, 160 + y, 275 + z);
glVertex3f(450 + x, 280 + y, 275 + z);
glVertex3f(250 + x, 280 + y, 275 + z);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(250 + x, 160 + y, 275 + z);
glVertex3f(250 + x, 160 + y, 425 + z);
glVertex3f(250 + x, 280 + y, 425 + z);
glVertex3f(250 + x, 280 + y, 275 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(450 + x, 160 + y, 275 + z);
glVertex3f(450 + x, 160 + y, 425 + z);
glVertex3f(450 + x, 280 + y, 425 + z);
glVertex3f(450 + x, 280 + y, 275 + z);
glEnd();
glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(250 + x, 160 + y, 275 + z);
glVertex3f(450 + x, 160 + y, 275 + z);
glVertex3f(450 + x, 160 + y, 425 + z);
glVertex3f(250 + x, 160 + y, 425 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(250 + x, 280 + y, 275 + z);
glVertex3f(450 + x, 280 + y, 275 + z);
glVertex3f(450 + x, 280 + y, 425 + z);
glVertex3f(250 + x, 280 + y, 425 + z);

glEnd();



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




glBegin(GL_QUADS);
glColor3f(0.36f, 0.20f, 0.09f); // 深褐色
glVertex3f(450.9 + x + movecha_x2, 100 + y + movecha_y2, 275 + z + movecha_z2);
glVertex3f(450.9 + x + movecha_x2, 100 + y + movecha_y2, 425 + z + movecha_z2);
glVertex3f(450.9 + x + movecha_x2, 160 + y + movecha_y2, 425 + z + movecha_z2);
glVertex3f(450.9 + x + movecha_x2, 160 + y + movecha_y2, 275 + z + movecha_z2);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.36f, 0.20f, 0.09f);
glVertex3f(350 + x + movecha_x2, 100 + y + movecha_y2, 426 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 100 + y + movecha_y2, 426 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 160 + y + movecha_y2, 426 + z + movecha_z2);
glVertex3f(350 + x + movecha_x2, 160 + y + movecha_y2, 426 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.36f, 0.20f, 0.09f);
glVertex3f(350 + x + movecha_x2, 99 + y + movecha_y2, 275 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 99 + y + movecha_y2, 275 + z + movecha_z2);
glVertex3f(450 + x + movecha_x2, 99 + y + movecha_y2, 425 + z + movecha_z2);
glVertex3f(350 + x + movecha_x2, 99 + y + movecha_y2, 425 + z + movecha_z2);
glEnd();
 glBegin(GL_QUADS);
 glColor3f(0.36f, 0.20f, 0.09f);
 glVertex3f(350 + x + movecha_x2, 161 + y + movecha_y2, 275 + z + movecha_z2);
 glVertex3f(450 + x + movecha_x2, 161 + y + movecha_y2, 275 + z + movecha_z2);
 glVertex3f(450 + x + movecha_x2, 161 + y + movecha_y2, 425 + z + movecha_z2);
 glVertex3f(350 + x + movecha_x2, 161 + y + movecha_y2, 425 + z + movecha_z2);

 glEnd();

 glBegin(GL_QUADS);
 glColor3f(0.36f, 0.20f, 0.09f);
 glVertex3f(350 + x + movecha_x2, 100 + y + movecha_y2, 274 + z + movecha_z2);
 glVertex3f(450 + x + movecha_x2, 100 + y + movecha_y2, 274 + z + movecha_z2);
 glVertex3f(450 + x + movecha_x2, 160 + y + movecha_y2, 274 + z + movecha_z2);
 glVertex3f(350 + x + movecha_x2, 160 + y + movecha_y2, 274 + z + movecha_z2);
 glEnd();

 glPopMatrix();
 for (int x_design = 0; x_design <= 40; x_design = x_design + 10) {
   int y_design = x_design / 2;
   glBegin(GL_QUADS);
   glColor3f(0.66f, 0.60f, 0.09f);
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
 glColor3f(0.66f, 0.60f, 0.09f);
 glVertex3f(350 + x + movecha_x1, 145 + y + movecha_y1, 427 + z + movecha_z1);
 glVertex3f(300 + x + movecha_x1, 145 + y + movecha_y1, 427 + z + movecha_z1);
 glVertex3f(300 + x + movecha_x1, 145 + y + movecha_y1, 427 + z + movecha_z1);
 glVertex3f(300 + x + movecha_x1, 160 + y + movecha_y1, 427 + z + movecha_z1);
 glVertex3f(350 + x + movecha_x1, 160 + y + movecha_y1, 452 + z + movecha_z1);
 glEnd();



 for (int x_design = 0; x_design <= 40; x_design = x_design + 10) {
   int y_design = x_design / 2;
   glBegin(GL_QUADS);
   glColor3f(0.66f, 0.60f, 0.09f);
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
 glColor3f(0.66f, 0.60f, 0.09f);
 glVertex3f(400 + x + movecha_x2, 145 + y + movecha_y2, 427 + z + movecha_z2);
 glVertex3f(350 + x + movecha_x2, 145 + y + movecha_y2, 427 + z + movecha_z2);
 glVertex3f(400 + x + movecha_x2, 145 + y + movecha_y2, 427 + z + movecha_z2);
 glVertex3f(400 + x + movecha_x2, 160 + y + movecha_y2, 427 + z + movecha_z2);
 glVertex3f(350 + x + movecha_x1, 160 + y + movecha_y1, 452 + z + movecha_z1);
 glEnd();
glPopMatrix();



glPushMatrix();
glTranslatef(350 + x, 130 + y, 350 + z);

glRotatef(armAngle, 1, 0, 0);
glTranslatef(-350 - x, -130 - y, -350 - z);

glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(275 + x + movecha_x1, 100 + y + movecha_y1, 326 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 100 + y + movecha_y1, 326 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 60 + y + movecha_y1, 326 + z + movecha_z1);
glVertex3f(275 + x + movecha_x1, 60 + y + movecha_y1, 326 + z + movecha_z1);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(275 + x + movecha_x1, 100 + y + movecha_y1, 374 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 100 + y + movecha_y1, 374 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 60 + y + movecha_y1, 374 + z + movecha_z1);
glVertex3f(275 + x + movecha_x1, 60 + y + movecha_y1, 374 + z + movecha_z1);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(274 + x + movecha_x1, 100 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(274 + x + movecha_x1, 100 + y + movecha_y1, 325 + z + movecha_z1);
glVertex3f(274 + x + movecha_x1, 60 + y + movecha_y1, 325 + z + movecha_z1);
glVertex3f(274 + x + movecha_x1, 60 + y + movecha_y1, 375 + z + movecha_z1);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(275 + x + movecha_x1, 98 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 98 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 98 + y + movecha_y1, 325 + z + movecha_z1);
glVertex3f(275 + x + movecha_x1, 98 + y + movecha_y1, 325 + z + movecha_z1);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(275 + x + movecha_x1, 61 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 61 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 61 + y + movecha_y1, 325 + z + movecha_z1);
glVertex3f(275 + x + movecha_x1, 61 + y + movecha_y1, 325 + z + movecha_z1);

glEnd();



glPopMatrix();

glPushMatrix();
glTranslatef(350 + x, 130 + y, 350 + z);

glRotatef(-armAngle, 1, 0, 0);
glTranslatef(-350 - x, -130 - y, -350 - z);

glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(375 + x + movecha_x2, 100 + y + movecha_y2, 326 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 100 + y + movecha_y2, 326 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 60 + y + movecha_y2, 326 + z + movecha_z2);
glVertex3f(375 + x + movecha_x2, 60 + y + movecha_y2, 326 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(375 + x + movecha_x2, 100 + y + movecha_y2, 374 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 100 + y + movecha_y2, 374 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 60 + y + movecha_y2, 374 + z + movecha_z2);
glVertex3f(375 + x + movecha_x2, 60 + y + movecha_y2, 374 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(426 + x + movecha_x2, 100 + y + movecha_y2, 375 + z + movecha_z2);
glVertex3f(426 + x + movecha_x2, 100 + y + movecha_y2, 325 + z + movecha_z2);
glVertex3f(426 + x + movecha_x2, 60 + y + movecha_y2, 325 + z + movecha_z2);
glVertex3f(426 + x + movecha_x2, 60 + y + movecha_y2, 375 + z + movecha_z2);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(375 + x + movecha_x2, 98 + y + movecha_y2, 375 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 98 + y + movecha_y2, 375 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 98 + y + movecha_y2, 325 + z + movecha_z2);
glVertex3f(375 + x + movecha_x2, 98 + y + movecha_y2, 325 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
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

glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(500 + x + movecha_x2, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(450 + x + movecha_x2, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
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

glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(200 + x + movecha_x1, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + x + movecha_x1, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
    glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
    glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
    glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
    glEnd();



    glBegin(GL_QUADS);
    glColor3f(0.91f, 0.72f, 0.52f);
    glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
    glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
    glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
    glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);

    glEnd();
    glPopMatrix();

}

void drawVillager(float x, float z) {
  glPushMatrix();
  glTranslatef(x, -0.5f, z);



    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(0, -0.2f, 0);
    glScalef(0.3f, 0.6f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(0.9f, 0.75f, 0.6f);
    glPushMatrix();
    glTranslatef(0, 0.2f, 0);
    glutSolidCube(0.25f);
    glPopMatrix();

    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
glTranslatef(-0.2f, -0.1f, 0);
glScalef(0.1f, 0.4f, 0.1f);
glutSolidCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0.2f, -0.1f, 0);
glScalef(0.1f, 0.4f, 0.1f);
glutSolidCube(1.0f);
glPopMatrix();

glColor3f(0.2f, 0.1f, 0.1f);
glPushMatrix();
glTranslatef(-0.1f, -0.7f, 0);
glScalef(0.1f, 0.3f, 0.1f);
glutSolidCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0.1f, -0.7f, 0);
glScalef(0.1f, 0.3f, 0.1f);
glutSolidCube(1.0f);
glPopMatrix();



if (showSmoke) {
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor4f(0.3f, 0.3f, 0.3f, 0.5f);

  for (int i = 0; i < 3; i++) {
    glPushMatrix();
    glTranslatef(0.0f, 0.4f + i * 0.2f, 0.0f);
    glutSolidSphere(0.08f - (i * 0.02f), 12, 12);
    glPopMatrix();
  }
        glDisable(GL_BLEND);
    }

    glPopMatrix();
}



void drawSignboard(float x, float z) {
  glPushMatrix();
  glTranslatef(x, -1.0f, z);

    glColor3f(0.3f, 0.15f, 0.05f);
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0);
    glScalef(0.1f, 1.0f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(0.7f, 0.6f, 0.3f);
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0);
    glScalef(1.0f, 0.5f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glDisable(GL_LIGHTING);

    glColor3f(0.0f, 0.0f, 0.0f);



    glRasterPos3f(-0.45f, 1.10f, 0.10f);
    const char* line1 = "No Magic";
    for (const char* c = line1; *c != '\0'; c++) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glRasterPos3f(-0.45f, 0.90f, 0.10f);
    const char* line2 = "No Liang";
    for (const char* c = line2; *c != '\0'; c++) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glPopMatrix();
}



void drawTorch(float x, float z) {
  glPushMatrix();
  glTranslatef(x, -1.0f, z);



    glColor3f(0.3f, 0.15f, 0.05f);
    glPushMatrix();
    glTranslatef(0, 0.5f, 0);
    glScalef(0.05f, 1.0f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glTranslatef(0.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 0.4f, 0.0f);
    glutSolidSphere(0.15f, 12, 12);

    glPopMatrix();
}

void drawBrokenStone(float x, float z) {
  glPushMatrix();
  glTranslatef(x, -1.0f, z);
  glColor3f(0.3f, 0.25f, 0.2f);
  glScalef(1.0f, 0.3f, 0.8f);
  glutSolidCube(0.6f);
  glPopMatrix();
}
void drawPaddyField(float xOffset) {
  glPushMatrix();
  glTranslatef(xOffset, -0.96f, -3.0f);
    glColor3f(0.2f, 0.5f, 0.2f);



    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.5f, 0.0f, -2.0f);
    glVertex3f(1.5f, 0.0f, -2.0f);
    glVertex3f(1.5f, 0.0f, 2.0f);
    glVertex3f(-1.5f, 0.0f, 2.0f);
    glEnd();



    for (float i = -1.2f; i <= 1.2f; i += 0.4f) {
      for (float j = -1.8f; j <= 1.8f; j += 0.4f) {
         glPushMatrix();
         glTranslatef(i, 0.22f, j);



            glColor3f(1.0f, 1.0f, 0.0f);
            glutSolidSphere(0.03f, 8, 8);



            glTranslatef(0.0f, -0.10f, 0.0f);
            GLUquadric* quad = gluNewQuadric();
            gluCylinder(quad, 0.015f, 0.01f, 0.1f, 8, 1);
            gluDeleteQuadric(quad);

            glPopMatrix();
        }
    }

    glPopMatrix();
}

void drawRiver() {
  glPushMatrix();
  glTranslatef(0.0f, -0.99f, 0.0f);
  glColor3f(0.2f, 0.4f, 0.8f);
    glBegin(GL_QUADS);
    glVertex3f(-2.0f, 0.0f, -3.5f);
    glVertex3f(2.0f, 0.0f, -3.5f);
    glVertex3f(2.0f, 0.0f, -2.5f);
    glVertex3f(-2.0f, 0.0f, -2.5f);
    glEnd();

    glPopMatrix();
}




void drawSmallBridge() {
  glPushMatrix();

    glTranslatef(0.0f, -0.95f, -3.0f);

    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glColor3f(0.5f, 0.35f, 0.2f);

    glScalef(1.0f, 0.1f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawPaddyFieldFireCone(float xOffset) {
  glPushMatrix();
  glTranslatef(xOffset, -0.74f, -3.0f);



    for (int i = 0; i < 8; ++i) {
      glPushMatrix();

      float randX = ((rand() % 100) / 100.0f - 0.5f) * 1.5f;
      float randZ = ((rand() % 100) / 100.0f - 0.5f) * 2.0f;

      glTranslatef(randX, 0.0f, randZ);
      glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(1.0f, 0.4f + (rand() % 5) / 10.0f, 0.0f);
        GLUquadric* flame = gluNewQuadric();
        gluCylinder(flame, 0.15f, 0.0f, 0.5f, 12, 1);
        gluDeleteQuadric(flame);

        glPopMatrix();
    }

    glPopMatrix();
}
void drawExplosion(float x, float y, float z) {
  int points = 10;
  float innerRadius = 0.5f;
  float outerRadius = 1.2f;

    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= points * 2; ++i) {
      float angle = i * M_PI / points;
      float radius = (i % 2 == 0) ? outerRadius : innerRadius;
      float dx = cos(angle) * radius;
      float dz = sin(angle) * radius;
      glVertex3f(dx, 0.0f, dz);
    }

    glEnd();
    glPopMatrix();
}




void drawDialogBubble(float x, float y, float z, const char* text) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(1.5f, 0.6f, 1.0f);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();

    glDisable(GL_DEPTH_TEST);

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos3f(x - 0.60f, y, z + 0.01f);

    for (const char* c = text; *c != '\0'; ++c) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glEnable(GL_DEPTH_TEST);
}

void playSoundA() {
  mciSendStringA("open \"sounds\\Explosion Sound Effect.wav\" type waveaudio alias explosion", NULL, 0, NULL);
  mciSendStringA("play soundA", NULL, 0, NULL);
}

void playSoundB() {
  mciSendStringA("open \"sounds\\Bridge Cracking.wav\" type waveaudio alias bridge", NULL, 0, NULL);
  mciSendStringA("play soundB", NULL, 0, NULL);
}




void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
gluLookAt(cameraX, 2.5f, 6.0f,
  cameraX, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f);

drawGround();
drawRunway();



drawHut(0.0f, 2.0f);
drawHut(0.0f, -3.0f);
drawHut(3.0f, 2.0f);
drawHut(4.0f, -2.5f);



drawCave(8.0f, -2.0f);
drawSignboard(2.0f, -2.5f);



float torchX[3] = { -4.0f, -1.0f, 2.0f };
for (int i = 0; i < 3; ++i) {
  drawTorch(torchX[i], 0.8f);
  drawTorch(torchX[i], -0.8f);
}

if (showVillagerDialog) {
   drawDialogBubble(-9.5f, 0.5f, -1.0f, villagerDialogText);
}

drawBrokenStone(-1.0f, 1.5f);
drawBrokenStone(0.0f, -2.0f);
drawBrokenStone(2.0f, 1.8f);

drawRock(5.8f, -1.0f);
drawRock(5.5f, -2.0f);
drawRock(6.0f, -0.5f);
drawRock(6.5f, 0.0f);
drawRock(6.8f, -0.2f);
drawRock(7.5f, -0.1f);
drawRock(8.1f, -0.1f);
drawRock(9.4f, -0.1f);
drawRock(10.f, -1.0f);
drawRock(10.5f, -2.0f);

drawVillager(-3.0f, -6.0f);
drawVillager(-9.0f, -6.0f);
drawVillager(-5.5f, -5.0f);
drawVillager(-9.5f, -1.f);
glPushMatrix();



drawPaddyField(-9.0f);
drawPaddyField(-3.0f);

if (showFire) {
   drawPaddyFieldFireCone(-3.0);
   drawPaddyFieldFireCone(-9.0);

    glPushMatrix();
    glTranslatef(-6.0f, 0.0f, 0.0f);
    glPopMatrix();
}

if (showDialog) {
   drawDialogBubble(cameraX + 1.0f, -0.6f, -0.45f, dialogText);
}
if (showExplosion) {
   drawExplosion(-6.0f, -0.7f, -3.0f);
}




glPushMatrix();
drawSmallBridge();
glPopMatrix();
    glPushMatrix();

    glTranslatef(-6.0f, 0.0f, 0.0f);
    drawRiver();
    drawSmallBridge();
    glPopMatrix();

    glPopMatrix();



    glScalef(0.0030, 0.0030, 0.0030);
    drawLiang((cameraX + 1.0f) / 0.0030, -300, -150, 0, 0, 0, 0, 0, 0);



    glutSwapBuffers();
}

void update(int value) {
  int currentTime = glutGet(GLUT_ELAPSED_TIME);
  int elapsed = currentTime - startTime;

    if (elapsed < 3000) {
       showDialog = true;
       dialogText = "Let me plant seeds ?";

        showFire = false;
        showSmoke = false;
        showVillagerDialog = false;
    }

    else if (elapsed < 5000) {
      showFire = true;
      showSmoke = true;

        showDialog = false;
        showVillagerDialog = true;
        villagerDialogText = "You almost kill us!";
    }
else if (elapsed < 6000) {
  showDialog = true;
  dialogText = "I help fix bridge?";

    showVillagerDialog = false;
}

else if (elapsed < 7000) {
  showExplosion = true;
  if (explosionStartTime == 0) {
     explosionStartTime = glutGet(GLUT_ELAPSED_TIME);
  }
  showDialog = false;
}



else if (elapsed < 8000) {
  showDialog = true;
  dialogText = "This is all my fault..";
}

else if (elapsed < 10800) {
  started = true;
  showDialog = false;
  showVillagerDialog = false;

    if (cameraX < 3.5f) {
       villagerX += speed;
       cameraX += speed;
    }
    else {
       speed = 0.0f;

    }
}

else if (elapsed < 14000) {
    showDialog = true;
    dialogText = "Why they hate me?";
    showVillagerDialog = false;
    soundAPlayed = false;
    soundBPlayed = false;

    mciSendStringA("close explode", NULL, 0, NULL);
    mciSendStringA("close crack", NULL, 0, NULL);
}

else {
  showDialog = false;
  showVillagerDialog = false;

    if (speed == 0.0f) speed = 0.6f;

    villagerX += speed;
    cameraX += speed;

    if (cameraX > 7.0f) {
       villagerX = -7.0f;
       cameraX = -8.0f;

      started = false;
      showFire = false;
      showSmoke = false;

      showDialog = true;
      dialogText = "Let me plant seeds ?";

      speed = 0.06f;
      startTime = glutGet(GLUT_ELAPSED_TIME);

      soundAPlayed = false;
      soundBPlayed = false;

      mciSendStringA("close explode", NULL, 0, NULL);
      mciSendStringA("close crack", NULL, 0, NULL);
        }
    }



  if (elapsed >= 1000 && !soundAPlayed && elapsed<=7000) {
     mciSendStringA("open \"sounds\\Explosion Sound.wav\" type waveaudio alias explode", NULL, 0, NULL);
     mciSendStringA("play explode", NULL, 0, NULL);
     soundAPlayed = true;
  }
 /* if (elapsed <= 7000) {
     soundAPlayed = false;
     mciSendStringA("close explode", NULL, 0, NULL);

    }*/

  if (elapsed >= 6000 && !soundBPlayed&&elapsed<=7000) {
     mciSendStringA("open \"sounds\\Bridge Cracking.wav\" type waveaudio alias crack", NULL, 0, NULL);
     mciSendStringA("play crack", NULL, 0, NULL);
     soundBPlayed = true;
  }
  /*if (elapsed <= 7000) {
     soundBPlayed = false;
     mciSendStringA("close crack", NULL, 0, NULL);

    }*/



    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}



void reshape(int w, int h) {
  if (h == 0) h = 1;
    float ratio = (float)w / (float)h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1280, 760);
  glutCreateWindow("Scene 2");

    glEnable(GL_DEPTH_TEST);

    startTime = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(30, update, 0);

    glutMainLoop();
    return 0;
}
