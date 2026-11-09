#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <thread>



#define M_PI 3.1412654

float angle = 0.0f;
float swingPhase = 0.0f;

//float cameraX = 0.0f, cameraY = 5.0f, cameraZ = 15.0f; zjde
float cameraAngle = 0.0f;

float cloudYPhase = 0.0f;
float cloudOffsetLeft = 0.0f;
float cloudOffsetRight = 0.0f;

int scene = 1;

int case1x = 0;
int case1L_z = 0;
int case5x = 0;

int case5L_z = -600;

int scene10_x = 0;
int scene10_z = 0;



float tearFallOffset = 0.0f;

float particleScale = 3.5f;
float effectScale = 8.0f;
using namespace std;

//part2
int liang2_12x = 0;
int liang2_12y = 0;
int liang2_12z = 0;



int ene11_z = 0;
int ene11_y = 0;
int ene11_x = 0;
int ene11_angle = 0;

float glowScale = 0.0f; // 初始大小为 0
bool glow = true;
float floatPhase_liang2 = 0.0f;
float floatOffset_liang2 = 0.0f;

float floatOffset_bubble = 0.0f; // 上下浮动偏移
float floatPhase_bubble = 0.0f; // 浮动相位
float particleScale_bubble = 1.5f; // 粒子尺寸放大倍数
float effectScale_bubble = 6.0f; // 整体特效空间位置放大倍数

// ==== 粒子结构 ====
struct Particle_bubble {
   float x, y, z;
   float alpha;
   float speed;
   float size;
   bool transparent;
};

std::vector<Particle_bubble> particles;

void initParticles_bubble() {
  particles.clear();
  for (int i = 0; i < 50; ++i) {
    Particle_bubble p;
    p.x = 300 + rand() % 20 - 10;
    p.y = 150 + rand() % 20 - 10;
    p.z = 450 + rand() % 20 - 10;
    p.alpha = (rand() % 100) / 100.0f;
        p.speed = 0.3f + (rand() % 100) / 500.0f;
        p.size = (3.0f + (rand() % 100) / 10.0f) * particleScale_bubble;
        p.transparent = rand() % 2;
        particles.push_back(p);
    }
}

void updateParticles_bubble() {
  for (auto& p : particles) {
    p.y += p.speed * effectScale_bubble;
    p.alpha -= 0.01f;
    if (p.alpha <= 0.0f) {
       p.y = 150 + rand() % 10;
       p.alpha = 1.0f;
       p.x = 300 + rand() % 20 - 10;
       p.z = 450 + rand() % 20 - 10;
       p.speed = 0.01f + (rand() % 100) / 500.0f;
       p.size = (3.0f + (rand() % 100) / 10.0f) * particleScale_bubble;
    }
  }
}

void drawParticles_bubble() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_POINT_SMOOTH);

    glPushMatrix();
    glScalef(effectScale_bubble, effectScale_bubble, effectScale_bubble); // 整体空间放大

    for (auto& p : particles) {
      if (p.transparent)
         glColor4f(1.0f, 0.2f, 0.2f, p.alpha);
      else
         glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

        glPointSize(p.size);
        glBegin(GL_POINTS);
        glVertex3f(p.x, p.y, p.z);
        glEnd();
    }

    glPopMatrix();
    glDisable(GL_BLEND);
}



void drawparticle1(float x, float y, float z) {
  glPushMatrix();
  glTranslatef(x, y, z);



    drawParticles_bubble(); // 调用粒子绘制函数
    glPopMatrix();

}

// ==== 光球绘制函数 ====
void drawGlowBall(float x, float y, float z, float scale) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 中心白色光球
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y, z);
    for (int i = 0; i <= 40; i++) {
      float angle = i * 2 * 3.14159f / 40;
      float dx = cos(angle) * 10.0f * scale;
      float dy = sin(angle) * 10.0f * scale;
      glVertex3f(x + dx, y + dy, z);
    }
    glEnd();

    // 外围光晕
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y, z);
    for (int i = 0; i <= 40; i++) {
      float angle = i * 2 * 3.14159f / 40;
      float dx = cos(angle) * 20.0f * scale;
      float dy = sin(angle) * 20.0f * scale;
      glVertex3f(x + dx, y + dy, z);
    }
    glEnd();

    glDisable(GL_BLEND);
}

void drawGlowBall1(float x, float y, float z, float scale, float angle) {
  glPushMatrix();
  glRotatef(angle, 0.0f, 1.0f, 0.0f);

    drawGlowBall(x, y, z, scale);
    glPopMatrix();
}
float liangPositionX = -5.0f; // Initial X position of Liang
float wheelAngle = 0.0f;
struct Color {
   float r, g, b;
   Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

// Colors for Liang
Color SKIN(0.95f, 0.75f, 0.65f);



float guardianPositionX = 1.0f; // Initial X position of guardian
float guardianPositionY = 3.0f; // Initial Y position of guardian
float hoverOffset = 0.0f; // New variable for vertical hover movement
float hoverAngle = 0.0f;
float tridentX = 7.0f;

void drawCube1(float size) {
float h = size / 2.0f;
glBegin(GL_QUADS);

glNormal3f(0, 0, 1);
glVertex3f(-h, -h, h);
glVertex3f(h, -h, h);
glVertex3f(h, h, h);
glVertex3f(-h, h, h);

glNormal3f(0, 0, -1);
glVertex3f(-h, -h, -h);
glVertex3f(-h, h, -h);
glVertex3f(h, h, -h);
glVertex3f(h, -h, -h);

glNormal3f(0, 1, 0);
glVertex3f(-h, h, -h);
glVertex3f(-h, h, h);
glVertex3f(h, h, h);
glVertex3f(h, h, -h);

glNormal3f(0, -1, 0);
glVertex3f(-h, -h, -h);
glVertex3f(h, -h, -h);
glVertex3f(h, -h, h);
glVertex3f(-h, -h, h);

glNormal3f(1, 0, 0);
glVertex3f(h, -h, -h);
glVertex3f(h, h, -h);
glVertex3f(h, h, h);
glVertex3f(h, -h, h);

glNormal3f(-1, 0, 0);
glVertex3f(-h, -h, -h);
glVertex3f(-h, -h, h);
glVertex3f(-h, h, h);
glVertex3f(-h, h, -h);
    glEnd();

}

void drawTrident()
{
  glPushMatrix();
  GLUquadric* q = gluNewQuadric();

    /*                                              ──              shaft
  // ───────────────────────────────────────
──── */
    glColor3f(0.30f, 0.10f, 0.40f);     // dark-purple shaft
    gluCylinder(q, 0.12, 0.12, 3.5, 20, 4);

    /* decorative collar where prongs begin */
    glPushMatrix();
    glColor3f(0.60f, 0.10f, 0.80f); // brighter purple ring
    glTranslatef(0.0f, 0.0f, 3.45f);
    gluCylinder(q, 0.14, 0.14, 0.15, 20, 2);
    glPopMatrix();

    /* ── trident head (three prongs) ───────────────────── */

         glPushAttrib(GL_LIGHTING_BIT);
         GLfloat em[] = { 0.35f, 0.00f, 0.55f, 1.0f };
         glMaterialfv(GL_FRONT, GL_EMISSION, em);

         glPushMatrix();
         glTranslatef(0.0f, 0.0f, 3.6f); // start at end of shaft

         /* base block that ties the prongs together */
         glPushMatrix();
         glColor3f(0.60f, 0.10f, 0.80f);
         glScalef(0.70f, 0.15f, 0.28f);
         drawCube1(1.0f);
         glPopMatrix();
      /* center prong (taller) */
      glPushMatrix();
      glTranslatef(0.0f, 0.0f, 0.28f);
      gluCylinder(q, 0.12, 0.0, 1.25, 20, 4);
      glPopMatrix();

      /* left prong */
      glPushMatrix();
      glTranslatef(-0.32f, 0.0f, 0.28f);
      gluCylinder(q, 0.12, 0.0, 1.05, 20, 4);
      glPopMatrix();

      /* right prong */
      glPushMatrix();
      glTranslatef(0.32f, 0.0f, 0.28f);
      gluCylinder(q, 0.12, 0.0, 1.05, 20, 4);
      glPopMatrix();
      glPopMatrix();

      GLfloat noEm[] = { 0,0,0,1 };
      glMaterialfv(GL_FRONT, GL_EMISSION, noEm);
      glPopAttrib();



    gluDeleteQuadric(q);
    glPopMatrix();
}



void drawHoveringSphere(float size) {
  glPushMatrix();

    // Dark purple sphere
    glColor3f(0.3f, 0.0f, 0.4f); // Dark purple color
    glutSolidSphere(0.50 * size, 20, 20);

    glPopMatrix();
}

void drawEvilGuardian() {
  glPushMatrix();
  glTranslatef(guardianPositionX, guardianPositionY, 2.0f); // Position on right side




    glRotatef(-70, 0, 1, 0); // Face towards left (Liang)
    glScalef(1.30f, 1.30f, 1.30f); // Make Evil Guardian 25% of original size

    //hovering sphere under Evil Guardian
    glPushMatrix();

    // Lower the sphere to just beneath the guardian's feet, with hover offset
    const float HOVER_Y_LOCAL = -1.20f;
    glTranslatef(0.0f, HOVER_Y_LOCAL + hoverOffset, 0.0f);

    // Rotate the sphere
    glRotatef(30.0f, 0, 1, 1);
    glRotatef(hoverAngle, 0, 1, 1);

    // Draw the sphere
    drawHoveringSphere(3.0f);

    glPopMatrix();

    // Head
    glPushMatrix();
    glTranslatef(0, 7, 0);

    // Face (purple demonic color)
    glPushMatrix();
    glColor3f(0.4f, 0.2f, 0.6f);
    glScalef(2, 2, 2);
    drawCube1(1.0f);
    glPopMatrix();
// Hair (darker purple/black)
glPushMatrix();
glTranslatef(0, 0.7f, 0);
glColor3f(0.2f, 0.0f, 0.3f);
glScalef(2.1f, 0.6f, 2.1f);
drawCube1(1.0f);
glPopMatrix();

// Left devil horn (L-shaped)
glColor3f(0.8f, 0.1f, 0.1f);
glPushMatrix();
glTranslatef(-1.3f, 0.9f, 0);
glScalef(1.6f, 0.25f, 0.25f);
drawCube1(1.0f);
glPopMatrix();
glPushMatrix();
glTranslatef(-2.0f, 1.3f, 0);
glScalef(0.25f, 0.8f, 0.25f);
drawCube1(1.0f);
glPopMatrix();

// Right devil horn (L-shaped)
glPushMatrix();
glTranslatef(1.3f, 0.9f, 0);
glScalef(1.6f, 0.25f, 0.25f);
drawCube1(1.0f);
glPopMatrix();
glPushMatrix();
glTranslatef(2.0f, 1.3f, 0);
glScalef(0.25f, 0.8f, 0.25f);
drawCube1(1.0f);
glPopMatrix();

// Face details on front
glPushMatrix();
glTranslatef(0, 0, 1.01f);

// Left Eye (demonic red eyes)
glPushMatrix();
glTranslatef(-0.4f, -0.1f, 0);

glPushMatrix();
glScalef(0.6f, 0.3f, 0.1f);
glColor3f(1.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0, 0, 0.01f);
glScalef(0.4f, 0.15f, 0.1f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

glPopMatrix();

// Right Eye (demonic red eyes)
glPushMatrix();
glTranslatef(0.4f, -0.1f, 0);

glPushMatrix();
glScalef(0.6f, 0.3f, 0.1f);
glColor3f(1.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0, 0, 0.01f);
glScalef(0.4f, 0.15f, 0.1f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

glPopMatrix();

// Third eye on forehead
glPushMatrix();
glTranslatef(0, 0.25f, 0.01f);
glScalef(0.6f, 0.3f, 0.1f);
glColor3f(1.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0, 0.25f, 0.03f);
glScalef(0.4f, 0.15f, 0.1f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

// Dark nose
glPushMatrix();
glTranslatef(0, -0.4f, 0.01f);
glScalef(0.15f, 0.15f, 0.1f);
glColor3f(0.3f, 0.1f, 0.4f);
drawCube1(1.0f);
glPopMatrix();

// Demonic mouth (fanged appearance)
glPushMatrix();
glTranslatef(0, -0.7f, 0.01f);
glScalef(1.3f, 0.2f, 0.15f);

glPushMatrix();
glColor3f(0.5f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

// Left fang
glPushMatrix();
glTranslatef(-0.3f, -0.2f, 0.01f);
glScalef(0.15f, 0.4f, 1.0f);
glColor3f(1.0f, 1.0f, 1.0f);
drawCube1(1.0f);
glPopMatrix();

// Right fang
glPushMatrix();
glTranslatef(0.3f, -0.2f, 0.01f);
glScalef(0.15f, 0.4f, 1.0f);
glColor3f(1.0f, 1.0f, 1.0f);
drawCube1(1.0f);
glPopMatrix();

glPopMatrix();

glPopMatrix();
glPopMatrix();

// Body with demonic design
glPushMatrix();
glTranslatef(0, 5, 0);

glPushMatrix();
glColor3f(0.6f, 0.1f, 0.4f);
glScalef(2, 3, 1);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glColor3f(0.5f, 0.2f, 0.7f);
glTranslatef(0, 0, 0.51f);
glScalef(0.8f, 3.0f, 0.02f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glColor3f(0.8f, 0.1f, 0.1f);
glTranslatef(-0.8f, 0, 0.51f);
glScalef(0.4f, 3.0f, 0.02f);
drawCube1(1.0f);
glPopMatrix();
glPushMatrix();
glColor3f(0.8f, 0.1f, 0.1f);
glTranslatef(0.8f, 0, 0.51f);
glScalef(0.4f, 3.0f, 0.02f);
drawCube1(1.0f);
glPopMatrix();

glColor3f(0.1f, 0.0f, 0.2f);
glPushMatrix();
glTranslatef(0, -1.45f, 0.52f);
glScalef(2.0f, 0.25f, 0.01f);
drawCube1(1.0f);
glPopMatrix();

glPopMatrix();

// Arms (dark purple skin)
for (int i = 0; i < 2; i++) {
   float side = i == 0 ? -1.0f : 1.0f;

  glPushMatrix();
  glTranslatef(side * 1.5f, 5, 0);
  glColor3f(0.4f, 0.2f, 0.6f);
  glScalef(0.5f, 1.5f, 0.5f);
  drawCube1(1.0f);
  glPopMatrix();

  if (i == 1) // right arm trident
  {
     glPushMatrix();
     glTranslatef(0, 4.0, -tridentX); // Use tridentX for position
     glTranslatef(2.0f, 0.0f, 4.0f); // to hand
     glRotatef(-70, -30, 200, -700); // point left (towards Liang)
     glScalef(2.3f, 2.3f, 2.3f); // overall trident size

    drawTrident();       // Draw full trident before it breaks
            glPopMatrix();
        }
    }

    // Pants (black with red trim)
    glPushMatrix();
    glColor3f(0.1f, 0.0f, 0.1f);
    glTranslatef(0, 2.5f, 0);
    glScalef(1, 1.5f, 1);
    drawCube1(1.0f);
    glPopMatrix();

    // Legs and feet (dark purple skin)
    for (int i = 0; i < 2; i++) {
       float side = i == 0 ? -1.0f : 1.0f;

        glPushMatrix();
        glTranslatef(side * 0.5f, 1.5f, 0);

        glPushMatrix();
        glColor3f(0.4f, 0.2f, 0.6f);
        glScalef(0.5f, 1.5f, 0.5f);
        drawCube1(1.0f);
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.3f, 0.1f, 0.5f);
        glTranslatef(0, -0.75f, 0.05f);
        glScalef(0.55f, 0.3f, 0.8f);
        drawCube1(1.0f);
        glPopMatrix();

        glPopMatrix();
    }

    glPopMatrix();
}
void drawenemy(int x,int y,int z,int scale,int angle) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glScalef(scale, scale, scale);
  glRotatef(angle, 0.0f, 1.0f, 0.0f);

    drawEvilGuardian();
    glPopMatrix();
}

void drawFireWheel(float innerR, float outerR, int bandCount)
{
  glPushAttrib(GL_LIGHTING_BIT);
  GLfloat emit[] = { 1.0f, 0.5f, 0.1f, 1.0f }; // soft orange glow
  glMaterialfv(GL_FRONT, GL_EMISSION, emit);

    // Three repeating band‐colors: red, orange, yellow
    const float colors[3][3] = {
       {1.0f, 0.2f, 0.0f}, // red-orange
       {1.0f, 0.6f, 0.0f}, // orange
       {1.0f, 0.9f, 0.2f} // yellow
    };

    // Draw 'bandCount' identical tori, each rotated around Z by an equal slice,
    // so that when the whole wheel spins around its axle
    float sliceAngle = 360.0f / bandCount;
    for (int i = 0; i < bandCount; ++i) {
       glPushMatrix();
       glRotatef(i * sliceAngle, 0, 0, 1); // rotate around hole-axis (Z)
       glColor3f(
          colors[i % 3][0],
          colors[i % 3][1],
          colors[i % 3][2]
       );
       glutSolidTorus(innerR, outerR, 16, 32);
       glPopMatrix();
    }
    // Turn off emission again:
    GLfloat noEmit[] = { 0, 0, 0, 1 };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmit);
    glPopAttrib();
}



void drawLiang1() {
  glPushMatrix();
  glTranslatef(liangPositionX, 2, 0); // Use liangPositionX instead of fixed position

    //twin fire wheels under Liang
    glPushMatrix();

    // 1) lower them to just beneath Liang's feet (in Liang's own local coordinates)
    const float WHEEL_Y_LOCAL = -0.45f;
    glTranslatef(0.0f, WHEEL_Y_LOCAL, 0.0f);

    // 2) stand the torus upright: default hole axis = +Z → rotate so hole = +X
    // Because a coin's hole/axis must point toward +X (Liang's "forward")
    glRotatef(30.0f, 0, 1, 1);

    // 3) spin the wheel "forward" around its vertical axle (axis = +Y) - FIXED!
    glRotatef(wheelAngle, 0, 1, 1); // Changed from (1, 0, 0) to (0, 1, 0)

    // Left wheel under left foot
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    drawFireWheel( 0.08f, 0.35f, 3);
    glPopMatrix();

    // Right wheel under right foot
    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0.0f);
    drawFireWheel(0.08f,0.35f, 3);
    glPopMatrix();
glPopMatrix();



glRotatef(90, 0, 1, 0); // Face towards right (guardian)
glScalef(0.30f, 0.30f, 0.30f); // Make Liang 25% of original size

// Head
glPushMatrix();
glTranslatef(0, 7, 0);

// Face (brown color - same as body parts)
glPushMatrix();
glColor3f(1.1f, 0.80f, 0.635f);
glScalef(2, 2, 2);
drawCube1(1.0f);
glPopMatrix();

// Hair (top part of head - black, shorter)
glPushMatrix();
glTranslatef(0, 0.7f, 0);
glColor3f(0.0f, 0.0f, 0.0f);
glScalef(2.1f, 0.6f, 2.1f);
drawCube1(1.0f);
glPopMatrix();

// Left hair bun
glPushMatrix();
glTranslatef(-1.2f, 1.1f, 0);
glColor3f(0.0f, 0.0f, 0.0f);
glScalef(0.6f, 0.6f, 0.6f);
drawCube1(1.0f);
glPopMatrix();

// Right hair bun
glPushMatrix();
glTranslatef(1.2f, 1.1f, 0);
glColor3f(0.0f, 0.0f, 0.0f);
glScalef(0.6f, 0.6f, 0.6f);
drawCube1(1.0f);
glPopMatrix();

// Left hairband
glColor3f(1.0f, 0.0f, 0.0f);
glPushMatrix();
glTranslatef(-0.9f, 1.0f, 0);
glScalef(0.6f, 0.1f, 0.1f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(-1.3f, 0.3f, 0);
glScalef(0.1f, 1.4f, 0.1f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(-1.35f, 0.5f, 0);
glScalef(0.3f, 0.1f, 0.1f);
drawCube1(1.0f);
glPopMatrix();

// Right hairband
glPushMatrix();
glTranslatef(0.9f, 1.0f, 0);
glScalef(0.6f, 0.1f, 0.1f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(1.35f, 0.3f, 0);
glScalef(0.1f, 1.4f, 0.1f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(1.35f, 0.5f, 0);
glScalef(0.3f, 0.1f, 0.1f);
drawCube1(1.0f);
glPopMatrix();

// Face details on front
glPushMatrix();
glTranslatef(0, 0, 1.01f);

// Left Eye
glPushMatrix();
glTranslatef(-0.4f, 0.1f, 0);

glPushMatrix();
glTranslatef(-0.15f, 0, 0);
glScalef(0.3f, 0.4f, 0.1f);
glColor3f(1.0f, 1.0f, 1.0f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0.15f, 0, 0.01f);
glScalef(0.3f, 0.4f, 0.1f);
glColor3f(1.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

glPopMatrix();

// Right Eye
glPushMatrix();
glTranslatef(0.4f, 0.1f, 0);

glPushMatrix();
glTranslatef(-0.15f, 0, 0.01f);
glScalef(0.3f, 0.4f, 0.1f);
glColor3f(1.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();
glPushMatrix();
glTranslatef(0.15f, 0, 0);
glScalef(0.3f, 0.4f, 0.1f);
glColor3f(1.0f, 1.0f, 1.0f);
drawCube1(1.0f);
glPopMatrix();

glPopMatrix();

// Red dot on forehead
glPushMatrix();
glTranslatef(0., 0.5f, 0.01f);
glScalef(0.15f, 0.15f, 0.1f);
glColor3f(1.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

// Brown nose
glPushMatrix();
glTranslatef(0, -0.4f, 0.01f);
glScalef(0.15f, 0.15f, 0.1f);
glColor3f(0.95f, 0.75f, 0.65f);
drawCube1(1.0f);
glPopMatrix();

// Mouth
glPushMatrix();
glTranslatef(0, -0.7f, 0.01f);
glScalef(1.3f, 0.11f, 0.15f);

glPushMatrix();
glTranslatef(-0.3f, 0, 0);
glScalef(0.4f, 1.0f, 1.0f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();
glPushMatrix();
glTranslatef(0, 0, 0.01f);
glScalef(0.7f, 1.0f, 1.0f);
glColor3f(1.0f, 1.0f, 1.0f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0.3f, 0, 0);
glScalef(0.4f, 1.0f, 1.0f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

glPopMatrix();

// Smiley pixels
glPushMatrix();
glTranslatef(-0.6f, -0.59f, 0.01f);
glScalef(0.1f, 0.1f, 0.1f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0.6f, -0.59f, 0.01f);
glScalef(0.1f, 0.1f, 0.1f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube1(1.0f);
glPopMatrix();

glPopMatrix();
glPopMatrix();

// Body with jacket design
glPushMatrix();
glTranslatef(0, 5, 0);
glPushMatrix();
glColor3f(0.9f, 0.2f, 0.1f);
glScalef(2, 3, 1);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glColor3f(0.95f, 0.75f, 0.65f);
glTranslatef(0, 0, 0.51f);
glScalef(0.8f, 3.0f, 0.02f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glColor3f(1.0f, 1.0f, 0.0f);
glTranslatef(-0.8f, 0, 0.51f);
glScalef(0.4f, 3.0f, 0.02f);
drawCube1(1.0f);
glPopMatrix();

glPushMatrix();
glColor3f(1.0f, 1.0f, 0.0f);
glTranslatef(0.8f, 0, 0.51f);
glScalef(0.4f, 3.0f, 0.02f);
drawCube1(1.0f);
glPopMatrix();

glColor3f(0.95f, 0.65f, 0.2f);
glPushMatrix();
glTranslatef(0, -1.45f, 0.52f);
glScalef(2.0f, 0.1f, 0.01f);
drawCube1(1.0f);
glPopMatrix();

glPopMatrix();

// Arms
for (int i = 0; i < 2; ++i)
{
    float side = (i == 0 ? -1.0f : 1.0f);

    glPushMatrix();
    glTranslatef(side * 1.5f, 5, 0);
    glColor3f(SKIN.r, SKIN.g, SKIN.b);
    glScalef(0.5f, 1.5f, 0.5f);
    drawCube1(1.0f);
    glPopMatrix();



}

// Pants
glPushMatrix();
glColor3f(0.0f, 0.0f, 0.0f);
glTranslatef(0, 2.5f, 0);
glScalef(1, 1.5f, 1);
drawCube1(1.0f);
glPopMatrix();

// Legs and feet
for (int i = 0; i < 2; i++) {
   float side = i == 0 ? -1.0f : 1.0f;

    glPushMatrix();
    glTranslatef(side * 0.5f, 1.5f, 0);

    glPushMatrix();
    glColor3f(0.95f, 0.75f, 0.65f);
    glScalef(0.5f, 1.5f, 0.5f);
    drawCube1(1.0f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.95f, 0.75f, 0.65f);
    glTranslatef(0, -0.75f, 0.05f);
    glScalef(0.55f, 0.3f, 0.6f);
        drawCube1(1.0f);
        glPopMatrix();

        glPopMatrix();
    }

    glPopMatrix();
}



void drawLiang2(int x, int y, int z, int scale, int angle) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glScalef(scale, scale, scale);
  glRotatef(angle, 0.0f, 1.0f, 0.0f);

    drawLiang1();
    glPopMatrix();
}

float flameTime = 0.0f;

struct FlameTri {
   float x, y, z;
   float size;
   float offset;
   float r, g, b;
};

std::vector<FlameTri> flameTris;

void initFlameTriangles() {
  flameTris.clear();
  for (int i = 0; i < 10; ++i) {
     FlameTri tri;
     tri.x = rand() % 20 - 10;
     tri.y = 0;
     tri.z = rand() % 20 - 10;
        tri.size = 10 + rand() % 10;
        tri.offset = rand() % 100 / 100.0f * 3.14f;

        float t = (rand() % 100) / 100.0f;
        tri.r = 1.0f;
        tri.g = t * 0.2f + 0.2f;
        tri.b = 0.0f;

        flameTris.push_back(tri);
    }
}

void drawFlameTriangles(float baseX, float baseY, float baseZ, float scale) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glPushMatrix();

    for (auto& tri : flameTris) {
      float yOffset = sin(flameTime + tri.offset) * 5.0f * scale;

        float x = baseX + tri.x * scale;
        float y = baseY + yOffset;
        float z = baseZ + tri.z * scale;
        float size = tri.size * scale;

        glColor4f(tri.r, tri.g, tri.b, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex3f(x, y, z);
        glVertex3f(x - size / 2, y - size, z);
        glVertex3f(x + size / 2, y - size, z);
        glEnd();
    }

    glPopMatrix();
    glDisable(GL_BLEND);
}
void drawfire(int x,int y,int z) {
  glPushMatrix();
  glTranslatef(x, y, z);
  drawFlameTriangles(4300, 650, 0, 5.5f);
  drawFlameTriangles(4900, 850, 0, 5.5f);

    drawFlameTriangles(3150, 650, -200, 5.5f);
    glPopMatrix();

}

struct Particle {
   float x, y, z;
   float alpha;
   float speed;
   float size;
   float r, g, b;
};



vector<Particle> smokeParticles;



void initSmokeParticles() {
  smokeParticles.clear();
  for (int i = 0; i < 100; ++i) {
    Particle p;
    p.x = 370 + rand() % 20 - 10;
    p.y = 350 + rand() % 10;
    p.z = 450 + rand() % 20 - 10;
    p.alpha = 0.7f;
    p.speed = 0.2f + (rand() % 100) / 400.0f;
    p.size = (6.0f + (rand() % 100) / 8.0f) * particleScale;

     float gray = 0.2f + (rand() % 50) / 100.0f;
     p.r = gray;
        p.g = gray;
        p.b = gray;

        smokeParticles.push_back(p);
    }
}




void updateSmokeParticles() {
  for (auto& p : smokeParticles) {
    p.y += p.speed;
    p.x += sin(p.y * 0.05f) * 0.1f;
    p.z += cos(p.y * 0.05f) * 0.1f;
    p.alpha -= 0.1f;
    if (p.alpha <= 0.0f) {
       p.x = 370 + rand() % 20 - 10;
       p.y = 10 + rand() % 60;
       p.z = 450 + rand() % 20 - 10;
       p.alpha = 0.7f;
       p.speed = 0.2f + (rand() % 100) / 400.0f;
       p.size = (6.0f + (rand() % 100) / 8.0f) * particleScale;

            float gray = 0.2f + (rand() % 50) / 100.0f;
            p.r = gray;
            p.g = gray;
            p.b = gray;
        }
    }
}




void drawSmokeParticles(int x, int y, int z) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_POINT_SMOOTH);
    glPushMatrix();
    glScalef(effectScale, effectScale, effectScale);
    glTranslatef(x, y, z);

    for (auto& p : smokeParticles) {
      glColor4f(p.r, p.g, p.b, p.alpha);
      glPointSize(p.size);
      glBegin(GL_POINTS);
      glVertex3f(p.x, p.y, p.z);
      glEnd();
    }

    glPopMatrix();
    glDisable(GL_BLEND);
}



void drawTree(float x, float z, float scale) {
  // 树干
    glColor3f(0.4f, 0.25f, 0.1f);
    glPushMatrix();
    glTranslatef(x, 0, z);
    glScalef(scale, scale, scale);

    glBegin(GL_QUADS);
    glVertex3f(-1, 0, -1); glVertex3f(1, 0, -1);
    glVertex3f(1, 10, -1); glVertex3f(-1, 10, -1);
    glVertex3f(-1, 0, 1); glVertex3f(1, 0, 1);
    glVertex3f(1, 10, 1); glVertex3f(-1, 10, 1);
    glEnd();

    // 树冠
    glColor3f(0.1f, 0.5f, 0.1f);
    glTranslatef(0, 13, 0);
    glutSolidSphere(5, 16, 16);

    glPopMatrix();
}
void drawFence(float x, float zStart, float zEnd, float scale) {
  for (float z = zStart; z <= zEnd; z += 10 * scale) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glScalef(scale, scale, scale);

        glColor3f(0.4f, 0.2f, 0.1f);
        glBegin(GL_QUADS);
        glVertex3f(-1, 0, 0); glVertex3f(1, 0, 0);
        glVertex3f(1, 15, 0); glVertex3f(-1, 15, 0);
        glEnd();

        glPopMatrix();
    }
}



void drawGround() {
  glColor3f(0.3f, 0.6f, 0.3f);
  glBegin(GL_QUADS);
  glVertex3f(-40000.0f, 0.0f, -40000.0f);
  glVertex3f(40000.0f, 0.0f, -40000.0f);
  glVertex3f(40000.0f, 0.0f, 40000.0f);
  glVertex3f(-40000.0f, 0.0f, 40000.0f);
  glEnd();
}



void drawHouse(float x, float y, float z, float scale) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glScalef(scale, scale, scale);

    glColor3f(0.5f, 0.35f, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f(-10, 0, 10); glVertex3f(10, 0, 10); glVertex3f(10, 20, 10); glVertex3f(-10, 20, 10);
    glVertex3f(-10, 0, -10); glVertex3f(-10, 20, -10); glVertex3f(10, 20, -10); glVertex3f(10, 0, -10);
  glVertex3f(-10, 0, -10); glVertex3f(-10, 0, 10); glVertex3f(-10, 20, 10); glVertex3f(-10, 20, -10);
  glVertex3f(10, 0, -10); glVertex3f(10, 0, 10); glVertex3f(10, 20, 10); glVertex3f(10, 20, -10);
  glVertex3f(-10, 20, -10); glVertex3f(-10, 20, 10); glVertex3f(10, 20, 10); glVertex3f(10, 20, -
10);
  glEnd();

    glColor3f(0.6f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(-10, 20, 10); glVertex3f(10, 20, 10); glVertex3f(0, 30, 10);
    glVertex3f(-10, 20, -10); glVertex3f(10, 20, -10); glVertex3f(0, 30, -10);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-10, 20, -10); glVertex3f(-10, 20, 10); glVertex3f(0, 30, 10); glVertex3f(0, 30, -10);
    glVertex3f(10, 20, -10); glVertex3f(10, 20, 10); glVertex3f(0, 30, 10); glVertex3f(0, 30, -10);
    glEnd();

    glPopMatrix();
}



void drawvillage(int x,int y, int z) {
  drawHouse(600+x, 0, 300+z, 36.0f);
  drawHouse(1600 + x, 0, 300 + z, 36.0f);
  drawHouse(2600 + x, 0, 300 + z, 36.0f);
  drawHouse(-1400 + x, 0, 300 + z, 36.0f);
  drawHouse(3600 + x, 0, 1300 + z, 36.0f);
  drawHouse(1600 + x, 0, 2300 + z, 36.0f);
  drawHouse(2600 + x, 0, 3300 + z, 36.0f);
  drawHouse(-1400 + x, 0, 4300 + z, 36.0f);
  drawHouse(2000 + x, 0, 4300 + z, 36.0f);
  drawHouse(-2400 + x, 0, 4300 + z, 36.0f);
  drawHouse(600 + x, 0, 5300 + z, 36.0f);

    drawTree(-2600 + x, 300 + z, 92);
    drawTree(-3600 + x, 1400 + z, 92);

    drawTree(-4600 + x, 850 + z, 92);
    drawTree(-560 + x, 700 + z, 92);
    drawTree(-5600 + x, 700 + z, 92);
    drawTree(-6600 + x, 500 + z, 92);
    drawTree(-7600 + x, 550 + z, 92);
    drawTree(-8600 + x, 1900 + z, 92);

    drawTree(600 + x, -300 + z, 92);
    drawTree(3600 + x, 1400 + z, 92);

    drawTree(4600 + x, 850 + z, 92);
    drawTree(5600 + x, 700 + z, 92);
    drawTree(6600 + x, 500 + z, 92);
    drawTree(7600 + x, 550 + z, 92);
    drawTree(8600 + x, 1900 + z, 92);

    drawTree(-2600 + x, 1300 + z, 92);
    drawTree(-3600 + x, 2400 + z, 92);

    drawTree(-4600 + x, 3850 + z, 92);
    drawTree(-5600 + x, 1700 + z, 92);
    drawTree(-6600 + x, 2500 + z, 92);
    drawTree(-7600 + x, 3550 + z, 92);
    drawTree(-8600 + x, 4900 + z, 92);

    drawTree(600 + x, -2300 + z, 92);
    drawTree(3600 + x, 5400 + z, 92);

    drawTree(4600 + x, 5050 + z, 92);
    drawTree(5600 + x, 6700 + z, 92);
    drawTree(6600 + x, 6500 + z, 92);
    drawTree(7600 + x, 7550 + z, 92);
    drawTree(8600 + x, 4900 + z, 92);




}

//part 1
void drawText(float x, float y, float z, const char* text) {
  //glDisable(GL_LIGHTING); // 禁用光照，确保文字不受影响
    glColor3f(0.6f, 0.0f, 0.0f); // 白色文字，可修改

    glRasterPos3f(x, y, z); // 设置文字位置（3D 空间）

    // 使用大字体逐字符绘制
    for (const char* c = text; *c != '\0'; ++c) {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
      // 其他可选字体：
        // GLUT_BITMAP_HELVETICA_18
        // GLUT_BITMAP_TIMES_ROMAN_24
    }

    //glEnable(GL_LIGHTING); // 恢复光照（如有用）
}

void drawText1(float x, float y, float z, const char* text) {
  //glDisable(GL_LIGHTING); // 禁用光照，确保文字不受影响
    glColor3f(1.0f, 1.0f, 0.0f); // 白色文字，可修改

    glRasterPos3f(x, y, z); // 设置文字位置（3D 空间）

    // 使用大字体逐字符绘制
    for (const char* c = text; *c != '\0'; ++c) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
      // 其他可选字体：
        // GLUT_BITMAP_HELVETICA_18
        // GLUT_BITMAP_TIMES_ROMAN_24
    }

    //glEnable(GL_LIGHTING); // 恢复光照（如有用）
}

void init() {
 /* if (scene == 9) {
    glEnable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
    else {*/
      glEnable(GL_DEPTH_TEST);
      glClearColor(0.8f, 0.65f, 0.4f, 1.0f);
}

void drawRockFinalDirect(float x, float y, float z, float size, float angleDeg, float brightness) {
  if (brightness < 0.0f) brightness = 0.0f;
  if (brightness > 1.0f) brightness = 1.0f;

    float baseR = 0.36f + 0.2f * brightness;
    float baseG = 0.20f + 0.15f * brightness;
    float baseB = 0.09f + 0.1f * brightness;

    float darkR = baseR * 0.7f;
    float darkG = baseG * 0.7f;
    float darkB = baseB * 0.7f;

    float lightR = baseR * 1.2f;
    float lightG = baseG * 1.2f;
    float lightB = baseB * 1.2f;

    float s = size * 0.5f;

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angleDeg, 0.0f, 1.0f, 0.0f);
    glTranslatef(-x, -y, -z);

    // 底面
    glBegin(GL_QUADS);
    glColor3f(darkR, darkG, darkB);
    glVertex3f(x - s, y, z - s);
    glVertex3f(x + s, y, z - s * 0.8f);
    glVertex3f(x + s * 1.2f, y, z + s);
glVertex3f(x - s * 0.6f, y, z + s * 1.2f);
glEnd();



// 面 1：底左后 → 底右后 → 顶左后
glBegin(GL_TRIANGLES);
glColor3f(baseR, baseG, baseB);
glVertex3f(x - s, y, z - s);
glVertex3f(x + s, y, z - s * 0.8f);
glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
glEnd();

// 面 2：底右后 → 顶右后 → 顶左后
glBegin(GL_TRIANGLES);
glColor3f(lightR, lightG, lightB);
glVertex3f(x + s, y, z - s * 0.8f);
glVertex3f(x + s * 0.5f, y + s * 2.2f, z - s);
glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
glEnd();

// 面 3：底右后 → 底右前 → 顶右前
glBegin(GL_TRIANGLES);
glColor3f(baseR, baseG, baseB);
glVertex3f(x + s, y, z - s * 0.8f);
glVertex3f(x + s * 1.2f, y, z + s);
glVertex3f(x + s, y + s * 2.1f, z + s * 0.3f);
glEnd();

// 面 4：底右前 → 底左前 → 顶左前
glBegin(GL_TRIANGLES);
glColor3f(baseR, baseG, baseB);
glVertex3f(x + s * 1.2f, y, z + s);
glVertex3f(x - s * 0.6f, y, z + s * 1.2f);
glVertex3f(x + s * 0.0f, y + s * 1.5f, z + s * 1.2f);
glEnd();

// 面 5：底左前 → 底左后 → 顶左后
glBegin(GL_TRIANGLES);
    glColor3f(lightR, lightG, lightB);
    glVertex3f(x - s * 0.6f, y, z + s * 1.2f);
    glVertex3f(x - s, y, z - s);
    glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
    glEnd();

    // 面 6：顶部四边封面（左后 → 右前 → 左前 → 左前底）
    glBegin(GL_QUADS);
    glColor3f((baseR + lightR) * 0.5f, (baseG + lightG) * 0.5f, (baseB + lightB) * 0.5f);
    glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f); // 顶左后
    glVertex3f(x + s, y + s * 2.1f, z + s * 0.3f); // 顶右前
    glVertex3f(x + s * 0.0f, y + s * 1.5f, z + s * 1.2f); // 顶左前
    glVertex3f(x - s * 0.6f, y, z + s * 1.2f);       // 底左前
    glEnd();

    // 面 7：顶后斜面封闭（顶左后 → 顶右后 → 顶右前）
    glBegin(GL_TRIANGLES);
    glColor3f(darkR, darkG, darkB);
    glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
    glVertex3f(x + s * 0.5f, y + s * 2.2f, z - s);
    glVertex3f(x + s, y + s * 2.1f, z + s * 0.3f);
    glEnd();

    glPopMatrix();
}



void drawRockFinalDirect1(float x, float y, float z, float size, float angleDeg, float brightness)
{
  if (brightness < 0.0f) brightness = 0.0f;
  if (brightness > 1.0f) brightness = 1.0f;

    float baseR = 0.36f + 0.2f * brightness;
    float baseG = 0.20f + 0.15f * brightness;
    float baseB = 0.09f + 0.1f * brightness;

    float darkR = baseR * 0.7f;
float darkG = baseG * 0.7f;
float darkB = baseB * 0.7f;

float lightR = baseR * 1.2f;
float lightG = baseG * 1.2f;
float lightB = baseB * 1.2f;

float s = size * 0.5f;

glPushMatrix();
glTranslatef(x, y, z);
glRotatef(angleDeg, 0.0f, 0.0f, 1.0f);
glTranslatef(-x, -y, -z);




// 底面
glBegin(GL_QUADS);
glColor3f(darkR, darkG, darkB);
glVertex3f(x - s, y, z - s);
glVertex3f(x + s, y, z - s * 0.8f);
glVertex3f(x + s * 1.2f, y, z + s);
glVertex3f(x - s * 0.6f, y, z + s * 1.2f);
glEnd();

// 面 1：底左后 → 底右后 → 顶左后
glBegin(GL_TRIANGLES);
glColor3f(baseR, baseG, baseB);
glVertex3f(x - s, y, z - s);
glVertex3f(x + s, y, z - s * 0.8f);
glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
glEnd();




// 面 2：底右后 → 顶右后 → 顶左后
glBegin(GL_TRIANGLES);
glColor3f(lightR, lightG, lightB);
glVertex3f(x + s, y, z - s * 0.8f);
glVertex3f(x + s * 0.5f, y + s * 2.2f, z - s);
glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
glEnd();

// 面 3：底右后 → 底右前 → 顶右前
glBegin(GL_TRIANGLES);
glColor3f(baseR, baseG, baseB);
glVertex3f(x + s, y, z - s * 0.8f);
glVertex3f(x + s * 1.2f, y, z + s);
glVertex3f(x + s, y + s * 2.1f, z + s * 0.3f);
glEnd();

// 面 4：底右前 → 底左前 → 顶左前
glBegin(GL_TRIANGLES);
glColor3f(baseR, baseG, baseB);
glVertex3f(x + s * 1.2f, y, z + s);
glVertex3f(x - s * 0.6f, y, z + s * 1.2f);
glVertex3f(x + s * 0.0f, y + s * 1.5f, z + s * 1.2f);
glEnd();

// 面 5：底左前 → 底左后 → 顶左后
glBegin(GL_TRIANGLES);
glColor3f(lightR, lightG, lightB);
glVertex3f(x - s * 0.6f, y, z + s * 1.2f);
glVertex3f(x - s, y, z - s);
glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
glEnd();

// 面 6：顶部四边封面（左后 → 右前 → 左前 → 左前底）
glBegin(GL_QUADS);
glColor3f((baseR + lightR) * 0.5f, (baseG + lightG) * 0.5f, (baseB + lightB) * 0.5f);
glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f); // 顶左后
glVertex3f(x + s, y + s * 2.1f, z + s * 0.3f); // 顶右前
glVertex3f(x + s * 0.0f, y + s * 1.5f, z + s * 1.2f); // 顶左前
glVertex3f(x - s * 0.6f, y, z + s * 1.2f);       // 底左前
glEnd();
    // 面 7：顶后斜面封闭（顶左后 → 顶右后 → 顶右前）
    glBegin(GL_TRIANGLES);
    glColor3f(darkR, darkG, darkB);
    glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
    glVertex3f(x + s * 0.5f, y + s * 2.2f, z - s);
    glVertex3f(x + s, y + s * 2.1f, z + s * 0.3f);
    glEnd();

    glPopMatrix();
}

void drawRockFinalDirect2(float x, float y, float z, float size, float angleDeg, float brightness)
{
  if (brightness < 0.0f) brightness = 0.0f;
  if (brightness > 1.0f) brightness = 1.0f;

    float baseR = 0.36f + 0.2f * brightness;
    float baseG = 0.20f + 0.15f * brightness;
    float baseB = 0.09f + 0.1f * brightness;

    float darkR = baseR * 0.7f;
    float darkG = baseG * 0.7f;
    float darkB = baseB * 0.7f;

    float lightR = baseR * 1.2f;
    float lightG = baseG * 1.2f;
    float lightB = baseB * 1.2f;

    float s = size * 0.5f;

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angleDeg, 1.0f, 0.0f, 0.0f);
    glTranslatef(-x, -y, -z);



    // 底面
glBegin(GL_QUADS);
glColor3f(darkR, darkG, darkB);
glVertex3f(x - s, y, z - s);
glVertex3f(x + s, y, z - s * 0.8f);
glVertex3f(x + s * 1.2f, y, z + s);
glVertex3f(x - s * 0.6f, y, z + s * 1.2f);
glEnd();

// 面 1：底左后 → 底右后 → 顶左后
glBegin(GL_TRIANGLES);
glColor3f(baseR, baseG, baseB);
glVertex3f(x - s, y, z - s);
glVertex3f(x + s, y, z - s * 0.8f);
glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
glEnd();




// 面 2：底右后 → 顶右后 → 顶左后
glBegin(GL_TRIANGLES);
glColor3f(lightR, lightG, lightB);
glVertex3f(x + s, y, z - s * 0.8f);
glVertex3f(x + s * 0.5f, y + s * 2.2f, z - s);
glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
glEnd();

// 面 3：底右后 → 底右前 → 顶右前
glBegin(GL_TRIANGLES);
glColor3f(baseR, baseG, baseB);
glVertex3f(x + s, y, z - s * 0.8f);
glVertex3f(x + s * 1.2f, y, z + s);
glVertex3f(x + s, y + s * 2.1f, z + s * 0.3f);
glEnd();

// 面 4：底右前 → 底左前 → 顶左前
glBegin(GL_TRIANGLES);
glColor3f(baseR, baseG, baseB);
glVertex3f(x + s * 1.2f, y, z + s);
    glVertex3f(x - s * 0.6f, y, z + s * 1.2f);
    glVertex3f(x + s * 0.0f, y + s * 1.5f, z + s * 1.2f);
    glEnd();

    // 面 5：底左前 → 底左后 → 顶左后
    glBegin(GL_TRIANGLES);
    glColor3f(lightR, lightG, lightB);
    glVertex3f(x - s * 0.6f, y, z + s * 1.2f);
    glVertex3f(x - s, y, z - s);
    glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
    glEnd();

    // 面 6：顶部四边封面（左后 → 右前 → 左前 → 左前底）
    glBegin(GL_QUADS);
    glColor3f((baseR + lightR) * 0.5f, (baseG + lightG) * 0.5f, (baseB + lightB) * 0.5f);
    glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f); // 顶左后
    glVertex3f(x + s, y + s * 2.1f, z + s * 0.3f); // 顶右前
    glVertex3f(x + s * 0.0f, y + s * 1.5f, z + s * 1.2f); // 顶左前
    glVertex3f(x - s * 0.6f, y, z + s * 1.2f);       // 底左前
    glEnd();

    // 面 7：顶后斜面封闭（顶左后 → 顶右后 → 顶右前）
    glBegin(GL_TRIANGLES);
    glColor3f(darkR, darkG, darkB);
    glVertex3f(x - s * 0.3f, y + s * 1.8f, z - s * 0.5f);
    glVertex3f(x + s * 0.5f, y + s * 2.2f, z - s);
    glVertex3f(x + s, y + s * 2.1f, z + s * 0.3f);
    glEnd();

    glPopMatrix();
}



void drawHairBall(float cx, float cy, float cz, float radius, bool cutQuarter = false) {
  const int stacks = 18; // 纬度划分
    const int slices = 36; // 经度划分
    glColor3f(0.36f, 0.23f, 0.15f); // 深棕色
    for (int i = 0; i < stacks; ++i) {
      float lat0 = M_PI * (-0.5 + (float)i / stacks);           // -π/2 到 π/2
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

void drawHairFringe(float x, float y, float z) {
  glColor3f(0.25f, 0.15f, 0.05f); // 深褐色刘海

    glBegin(GL_QUADS);

    // 中间刘海片
    glVertex3f(x - 5, y + 30, z); // 左上
    glVertex3f(x + 5, y + 30, z); // 右上
    glVertex3f(x + 8, y - 20, z - 2); // 右下
    glVertex3f(x - 8, y - 20, z - 2); // 左下

    // 左前刘海片
    glVertex3f(x - 20, y + 28, z + 0.5);
    glVertex3f(x - 10, y + 28, z + 0.5);
    glVertex3f(x - 12, y - 15, z - 1.5);
    glVertex3f(x - 22, y - 15, z - 1.5);

    // 右前刘海片
    glVertex3f(x + 10, y + 28, z + 0.5);
    glVertex3f(x + 20, y + 28, z + 0.5);
    glVertex3f(x + 22, y - 15, z - 1.5);
    glVertex3f(x + 12, y - 15, z - 1.5);

    // 左斜刘海片
    glVertex3f(x - 30, y + 26, z + 1);
    glVertex3f(x - 22, y + 26, z + 1);
    glVertex3f(x - 26, y - 10, z - 1.5);
    glVertex3f(x - 34, y - 10, z - 1.5);

    // 右斜刘海片
    glVertex3f(x + 22, y + 26, z + 1);
    glVertex3f(x + 30, y + 26, z + 1);
    glVertex3f(x + 34, y - 10, z - 1.5);
    glVertex3f(x + 26, y - 10, z - 1.5);

    glEnd();
}

void drawFringe(float xOffset, float yOffset, float zOffset) {
  glColor3f(0.36f, 0.23f, 0.15f); // 深棕色



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

void drawPuffCloud(float cx, float cy, float cz) {
  glColor3f(1.0f, 1.0f, 1.0f); // 云朵白色

    for (int i = 0; i < 3; ++i) {
      float dx = (i - 1) * 8.0f;
      glBegin(GL_POLYGON);
      for (int j = 0; j <= 30; ++j) {
         float angle = j * 2 * 3.14159f / 30;
         float px = cos(angle) * 6.0f;
         float py = sin(angle) * 6.0f;
         glVertex3f(cx + dx + px, cy + py, cz);
      }
      glEnd();
    }
}



void drawLiang(int x, int y, int z, int movecha_x1, int movecha_y1, int movecha_z1, int
movecha_x2, int movecha_y2, int movecha_z2,int emojinum) {



    float armAngle = sin(swingPhase) * 15.0f; // 手臂摆动角度
    float legAngle = -sin(swingPhase) * 15.0f; // 腿摆动角度
    /*drawHairFringe(350, 380, 411);*/
    drawFringe(0 + x, -10 + y, 0 + z);
drawHairBall(290 + x, 410 + y, 290 + z, 22, true);

// 右边头发球（去掉右下 1/4）
drawHairBall(410 + x, 410 + y, 290 + z, 22, true);
if (emojinum == 1) {



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

 /////

}
if (emojinum == 2) {
//// === 左眼泪滴：往左下滴落 ===
//glColor4f(0.4f, 0.8f, 1.0f, 0.9f); // 淡蓝半透明
//glBegin(GL_POLYGON);
//for (int i = 0; i <= 40; ++i) {
// float angle = i * 2 * 3.14159f / 40;
// float dx = cos(angle) * 2.5f;
// float dy = sin(angle) * 5.0f;

// glVertex3f(
//   320 + dx + x - tearFallOffset * 1.8f,   // 左偏落
//   340 + dy + y - tearFallOffset,       // 垂直下落
//    412 + z
// );
//}
//glEnd();

//// === 右眼泪滴：往右下滴落 ===
//glColor4f(0.4f, 0.8f, 1.0f, 0.9f);
//glBegin(GL_POLYGON);
//for (int i = 0; i <= 40; ++i) {
// float angle = i * 2 * 3.14159f / 40;
// float dx = cos(angle) * 2.5f;
// float dy = sin(angle) * 5.0f;

// glVertex3f(
//   380 + dx + x + tearFallOffset * 1.8f,   // 右偏落
//   340 + dy + y - tearFallOffset,       // 垂直下落
//    412 + z
// );
//}
//glEnd();

float curveRadius = 100.0f; // 控制整个弯曲的半径
float maxFall = 100.0f;

float theta = tearFallOffset / maxFall * (3.14159f / 2.0f);
float offsetX_L = -curveRadius * sin(theta); // 左侧弯曲
float offsetY = -curveRadius * (1.0f - cos(theta));
float offsetX_R = +curveRadius * sin(theta);

// 更弯、自然的左泪滴（圆弧向左下滑落）
glColor4f(0.4f, 0.8f, 1.0f, 0.9f);
glBegin(GL_POLYGON);
for (int i = 0; i <= 40; ++i) {
  float angle = i * 2 * 3.14159f / 40;
  float dx = cos(angle) * 9.5f;
  float dy = sin(angle) * 9.0f;

    glVertex3f(
       290 + dx + x + offsetX_L,
       335 + dy + y + offsetY,
       412 + z
    );
}

glEnd();



// === 弯曲 + 更大颗的右眼泪滴 ===
glColor4f(0.4f, 0.8f, 1.0f, 0.9f);
glBegin(GL_POLYGON);
for (int i = 0; i <= 40; ++i) {
  float angle = i * 2 * 3.14159f / 40;
  float dx = cos(angle) * 9.5f;
  float dy = sin(angle) * 9.0f;

    glVertex3f(
       410 + dx + x + offsetX_R,
       335 + dy + y + offsetY,
       412 + z
    );
}
glEnd();
glEnd();

glColor3f(0.0f, 0.0f, 0.0f); // 黑色
glLineWidth(3.0f);
glBegin(GL_LINES);
// 左眼睛
glVertex3f(310 + x, 370 + y, 411 + z);
glVertex3f(340 + x, 360 + y, 411 + z);

glColor3f(0.0f, 0.0f, 0.0f); // 黑色
glLineWidth(3.0f);
glBegin(GL_LINES);
// 左眼睛
glVertex3f(310 + x, 350 + y, 411 + z);
glVertex3f(340 + x, 360 + y, 411 + z);
glEnd();

glColor3f(0.0f, 0.0f, 0.0f); // 黑色
glLineWidth(3.0f);
glBegin(GL_LINES);
// 右眼睛
glVertex3f(390 + x, 370 + y, 411 + z);
glVertex3f(360 + x, 360 + y, 411 + z);

glColor3f(0.0f, 0.0f, 0.0f); // 黑色
glLineWidth(3.0f);
glBegin(GL_LINES);
// 右眼睛
glVertex3f(390 + x, 350 + y, 411 + z);
glVertex3f(360 + x, 360 + y, 411 + z);

glEnd();



glColor3f(0.0f, 0.0f, 0.0f); // 黑色
  glLineWidth(3.0f);
  glBegin(GL_LINES);
  // 左眉毛
  glVertex3f(310 + x, 390 + y, 411 + z);
  glVertex3f(330 + x, 400 + y, 411 + z);
  // 右眉毛
  glVertex3f(370 + x, 400 + y, 411 + z);
  glVertex3f(390 + x, 390 + y, 411 + z);
  glEnd();

  // 【2】嘴巴（下弯半弧）
  glColor3f(0.0f, 0.0f, 0.0f); // 黑色
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i <= 40; ++i) {
    float angle = i * 3.14159f / 40;
    float dx = cos(angle) * 20;
    float dy = sin(angle) * 8; // 向下弯
    glVertex3f(350 + dx + x, 310 + dy + y, 411 + z);
  }
  glEnd();




}
if (emojinum == 3) {
   glColor3f(0.0f, 0.0f, 0.0f);
   //glLineWidth(4.0f);
   //glBegin(GL_LINES);
   //// 左眉毛（下斜）
  //glVertex3f(310 + x, 405 + y, 411 + z);
  //glVertex3f(330 + x, 395 + y, 411 + z);
  //// 右眉毛（下斜）
  //glVertex3f(370 + x, 395 + y, 411 + z);
  //glVertex3f(390 + x, 405 + y, 411 + z);
  //glEnd();

  //// 左边喷气位置（向上 + 浮动）
    //drawPuffCloud(310 + x, 430 + y + cloudOffsetLeft, 412 + z);

    //// 右边喷气位置
    //drawPuffCloud(390 + x, 430 + y + cloudOffsetRight, 412 + z);

    glLineWidth(8.0f);
    glBegin(GL_LINES);
    // 左眉毛（下斜）
    glVertex3f(315 + x, 375 + y, 411 + z);
    glVertex3f(340 + x, 365 + y, 411 + z);
    // 右眉毛（下斜）
    glVertex3f(360 + x, 365 + y, 411 + z);
    glVertex3f(385 + x, 375 + y, 411 + z);
    glEnd();

    glPointSize(20.0f);
    glBegin(GL_POINTS);
    glVertex3f(375 + x, 355 + y, 412 + z);
    glVertex3f(325 + x, 355 + y, 412 + z);

    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f); // 黑色
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 40; ++i) {
      float angle = i * 3.14159f / 40;
      float dx = cos(angle) * 20;
      float dy = sin(angle) * 8; // 向下弯
      glVertex3f(350 + dx + x, 310 + dy + y, 411 + z);
    }
    glEnd();




}

//front face
glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
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
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(290 + x, 300 + y, 290 + z);
glVertex3f(290 + x, 300 + y, 410 + z);
glVertex3f(290 + x, 410 + y, 410 + z);
glVertex3f(290 + x, 410 + y, 290 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
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
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
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
 for (int x_design = 0;x_design <= 40;x_design = x_design + 10) {
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
 for (int x_design = 0;x_design <= 40;x_design = x_design + 10) {
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
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(275 + x + movecha_x1, 100 + y + movecha_y1, 326 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 100 + y + movecha_y1, 326 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 60 + y + movecha_y1, 326 + z + movecha_z1);
glVertex3f(275 + x + movecha_x1, 60 + y + movecha_y1, 326 + z + movecha_z1);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(275 + x + movecha_x1, 100 + y + movecha_y1, 374 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 100 + y + movecha_y1, 374 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 60 + y + movecha_y1, 374 + z + movecha_z1);
glVertex3f(275 + x + movecha_x1, 60 + y + movecha_y1, 374 + z + movecha_z1);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(274 + x + movecha_x1, 100 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(274 + x + movecha_x1, 100 + y + movecha_y1, 325 + z + movecha_z1);
glVertex3f(274 + x + movecha_x1, 60 + y + movecha_y1, 325 + z + movecha_z1);
glVertex3f(274 + x + movecha_x1, 60 + y + movecha_y1, 375 + z + movecha_z1);
glEnd();
glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(275 + x + movecha_x1, 98 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 98 + y + movecha_y1, 375 + z + movecha_z1);
glVertex3f(325 + x + movecha_x1, 98 + y + movecha_y1, 325 + z + movecha_z1);
glVertex3f(275 + x + movecha_x1, 98 + y + movecha_y1, 325 + z + movecha_z1);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
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
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(375 + x + movecha_x2, 100 + y + movecha_y2, 326 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 100 + y + movecha_y2, 326 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 60 + y + movecha_y2, 326 + z + movecha_z2);
glVertex3f(375 + x + movecha_x2, 60 + y + movecha_y2, 326 + z + movecha_z2);
glEnd();
glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(375 + x + movecha_x2, 100 + y + movecha_y2, 374 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 100 + y + movecha_y2, 374 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 60 + y + movecha_y2, 374 + z + movecha_z2);
glVertex3f(375 + x + movecha_x2, 60 + y + movecha_y2, 374 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(426 + x + movecha_x2, 100 + y + movecha_y2, 375 + z + movecha_z2);
glVertex3f(426 + x + movecha_x2, 100 + y + movecha_y2, 325 + z + movecha_z2);
glVertex3f(426 + x + movecha_x2, 60 + y + movecha_y2, 325 + z + movecha_z2);
glVertex3f(426 + x + movecha_x2, 60 + y + movecha_y2, 375 + z + movecha_z2);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(375 + x + movecha_x2, 98 + y + movecha_y2, 375 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 98 + y + movecha_y2, 375 + z + movecha_z2);
glVertex3f(425 + x + movecha_x2, 98 + y + movecha_y2, 325 + z + movecha_z2);
glVertex3f(375 + x + movecha_x2, 98 + y + movecha_y2, 325 + z + movecha_z2);
glEnd();

glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
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
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(500 + x + movecha_x2, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(450 + x + movecha_x2, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(500 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(450 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 375 + movecha_z2 + z);
glVertex3f(450 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glVertex3f(500 + movecha_x2 + x, 130 + movecha_y2 + y, 325 + movecha_z2 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
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
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(200 + x + movecha_x1, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + x + movecha_x1, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();




glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
glEnd();



glBegin(GL_QUADS);
glColor3f(0.91f, 0.72f, 0.52f);//skin colour
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 375 + movecha_z1 + z);
glVertex3f(250 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glVertex3f(200 + movecha_x1 + x, 130 + movecha_y1 + y, 325 + movecha_z1 + z);
glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.91f, 0.72f, 0.52f);//skin colour
    glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
    glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 375 + movecha_z1 + z);
    glVertex3f(250 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);
    glVertex3f(200 + movecha_x1 + x, 280 + movecha_y1 + y, 325 + movecha_z1 + z);

    glEnd();



    glPopMatrix();
}



void drawFloor(int x, int y, int z) {

    glBegin(GL_QUADS);
    glColor3ub(66, 66, 58);//skin colour

    glVertex3f(-1900 + x, -100 + y, -3500 + z);
    glVertex3f(1900 + x, -100 + y, -3500 + z);
    glVertex3f(1900 + x, -100 + y, 3000 + z);

    glVertex3f(-1900 + x, -100 + y, 3000 + z);
    glEnd();




    //float centerX = -1250.0f;    // 拱圆中心 X
    //float centerY = -100.0f;    // 拱圆中心 Y
    //float centerZ = 450.0f;     // 固定 Z

    //float radius = 800.0f;     // 圆弧半径（越大越平缓）
    //int count = 17;          // 石块数（图中 5 块建议设为 6）
    //int angle1 = 0;
    //for (int h = 0;h < count;h++) {
    // float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
    // float z = centerZ - radius * sinf(angle2) * 5; // 向 shang

    // for (int i = 0; i < count; ++i) {
    //   float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
    //    float x = centerX - radius * cosf(angle); // 向左
    //    float y = centerY + radius * sinf(angle) * 2; // 向 shang
    //   angle1 += 37;
    //   drawRockFinalDirect(x, y, z, 500.0f, angle + angle1, 0.5f);
    // }
    //}



}



void drawcave1() {
  //drawRock1(0.0f, 0.0f, -0.98f, 0.02f, 25.0f, 0.6f);

    //int x, y, z,x1,y1,z1;
    //x = -1650.0f;
    //y = 100.0f;
    //z = 450.0f;
    //x1 = 0;
    //y1 = 100;
    //z1 = 0;

    //for (x1 = 0;x1 <= 2000;x1 += 50) {
    // y1 +=25;
    // drawRockFinalDirect(/*x1 +*/ x+x1, y1 + y, z1+ z, 300.0f, 0.0f, 0.6f);




    //}
    float centerX = -1450.0f;      // 拱圆中心 X
    float centerY = -100.0f;      // 拱圆中心 Y
    float centerZ = 450.0f;      // 固定 Z

    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;           // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
      float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float z = centerZ + radius * sinf(angle2) * 5; // 向 shang
        angle1 += 127;
        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX - radius * cosf(angle); // 向左
            float y = centerY + radius * sinf(angle) * 2; // 向 shang
            angle1 += 67;
            drawRockFinalDirect(x, y, z, 700.0f, angle + angle1, 0.6f);
        }
    }
}



void drawcave2() {




    float centerX = -1250.0f;      // 拱圆中心 X
    float centerY = -100.0f;      // 拱圆中心 Y
    float centerZ = 450.0f;      // 固定 Z

    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;           // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
      float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float z = centerZ - radius * sinf(angle2) * 5; // 向 shang
        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX - radius * cosf(angle); // 向左
            float y = centerY + radius * sinf(angle) * 2; // 向 shang
            angle1 += 37;
            drawRockFinalDirect(x, y, z, 500.0f, angle + angle1, 0.5f);
        }
    }
}

void drawcave3() {
  //drawRock1(0.0f, 0.0f, -0.98f, 0.02f, 25.0f, 0.6f);

    //int x, y, z,x1,y1,z1;
    //x = -1650.0f;
    //y = 100.0f;
    //z = 450.0f;
    //x1 = 0;
    //y1 = 100;
    //z1 = 0;

    //for (x1 = 0;x1 <= 2000;x1 += 50) {
    // y1 +=25;
    // drawRockFinalDirect(/*x1 +*/ x+x1, y1 + y, z1+ z, 300.0f, 0.0f, 0.6f);




    //}



    float centerX = 1150.0f;       // 拱圆中心 X
    float centerY = -100.0f;      // 拱圆中心 Y
    float centerZ = 450.0f;      // 固定 Z

    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;           // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
      float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float z = centerZ + radius * sinf(angle2) * 5; // 向 shang
        angle1 += 127;
        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX + radius * cosf(angle); // 向左
            float y = centerY + radius * sinf(angle) * 2; // 向 shang
            angle1 += 67;
            drawRockFinalDirect(x, y, z, 700.0f, angle + angle1, 0.6f);
        }
    }
}




void drawcave4() {




    float centerX = 1350.0f;       // 拱圆中心 X
    float centerY = -100.0f;      // 拱圆中心 Y
    float centerZ = 450.0f;      // 固定 Z

    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;           // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
      float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float z = centerZ - radius * sinf(angle2) * 5; // 向 shang

        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX + radius * cosf(angle); // 向左
             float y = centerY + radius * sinf(angle) * 2; // 向 shang
             angle1 += 37;
             drawRockFinalDirect(x, y, z, 700.0f, angle + angle1, 0.6f);
         }
     }
}

//void drawcave7() {
//
//
//
// float centerX = 1350.0f;             // 拱圆中心 X
// float centerY = 100.0f;            // 拱圆中心 Y
// float centerZ = 1050.0f;             // 固定 Z
//
// float radius = 800.0f;            // 圆弧半径（越大越平缓）
// int count = 17;                // 石块数（图中 5 块建议设为 6）
// int angle1 = 0;
// for (int h = 0;h < count;h++) {
//   float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
//           float z = centerZ - radius * sinf(angle2) * 5; // 向 shang
//
//           for (int i = 0; i < count; ++i) {
//             float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
//             float x = centerX + radius * cosf(angle); // 向左
//             float y = centerY + radius * sinf(angle) * 2; // 向 shang
//     angle1 += 37;
//     drawRockFinalDirect(x, y, z, 700.0f, angle + angle1, 0.6f);
//   }
// }
//}

void drawcave5() {




     float centerX = -1050.0f;         // 拱圆中心 X
 float centerY = 2200.0f;       // 拱圆中心 Y
 float centerZ = 450.0f;      // 固定 Z

 float radius = 800.0f;      // 圆弧半径（越大越平缓）
 int count = 17;           // 石块数（图中 5 块建议设为 6）
 int angle1 = 0;
 for (int h = 0;h < count;h++) {
   float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
     float z = centerZ - radius * sinf(angle2) * 5; // 向 shang

     for (int i = 0; i < count; ++i) {
       float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
         float x = centerX + radius * cosf(angle); // 向左
         float y = centerY;/*+ radius * sinf(angle) * 2;*/ // 向 shang
         angle1 += 37;
         drawRockFinalDirect1(x, y, z, 600.0f, angle + angle1, 0.6f);
     }
 }
}
void drawcave6() {




 float centerX = -150.0f;      // 拱圆中心 X
 float centerY = 2400.0f;       // 拱圆中心 Y
 float centerZ = 450.0f;      // 固定 Z

 float radius = 800.0f;      // 圆弧半径（越大越平缓）
 int count = 17;           // 石块数（图中 5 块建议设为 6）
 int angle1 = 0;
 for (int h = 0;h < count;h++) {
   float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
     float z = centerZ - radius * sinf(angle2) * 5; // 向 shang

     for (int i = 0; i < count; ++i) {
       float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX + radius * cosf(angle); // 向左
            float y = centerY;/*+ radius * sinf(angle) * 2;*/ // 向 shang
            angle1 += 37;
            drawRockFinalDirect1(x, y, z, 600.0f, angle + angle1, 0.6f);
        }

    }
}



void drawcave5a() {




    float centerX = -1050.0f;      // 拱圆中心 X
    float centerY = 2200.0f;       // 拱圆中心 Y
    float centerZ = 3000.0f;       // 固定 Z

    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;           // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
      float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float z = centerZ - radius * sinf(angle2) * 5; // 向 shang

        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX + radius * cosf(angle); // 向左
            float y = centerY;/*+ radius * sinf(angle) * 2;*/ // 向 shang
            angle1 += 37;
            drawRockFinalDirect1(x, y, z, 600.0f, angle + angle1, 0.6f);
        }
    }
}
void drawcave6a() {
    float centerX = -150.0f;      // 拱圆中心 X
    float centerY = 2400.0f;       // 拱圆中心 Y
    float centerZ = 3000.0f;       // 固定 Z

    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;           // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
      float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float z = centerZ - radius * sinf(angle2) * 5; // 向 shang

        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX + radius * cosf(angle); // 向左
            float y = centerY;/*+ radius * sinf(angle) * 2;*/ // 向 shang
            angle1 += 37;
            drawRockFinalDirect1(x, y, z, 600.0f, angle + angle1, 0.6f);
        }

    }
}

void drawcaveInside1() {




    float centerX = -1350.0f;      // 拱圆中心 X
    float centerY = 400.0f;      // 拱圆中心 Y
    float centerZ = 3250.0f;       // 固定 Z
    float z = centerZ;
    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;           // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
      float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float y = centerY + radius * sinf(angle2) * 2; // 向 shang

        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX + radius * cosf(angle); // 向左

            angle1 += 37;
            drawRockFinalDirect1(x, y, z, 700.0f, angle + angle1, 0.6f);
        }
    }
}

void drawcaveInside2() {




    float centerX = 0.0f;     // 拱圆中心 X
    float centerY = 400.0f;      // 拱圆中心 Y
    float centerZ = 3250.0f;       // 固定 Z
    float z = centerZ;
    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;          // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
      float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float y = centerY + radius * sinf(angle2) * 2; // 向 shang

        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX + radius * cosf(angle); // 向左

            angle1 += 37;
            drawRockFinalDirect1(x, y, z, 700.0f, angle + angle1, 0.6f);
        }
    }
}
void drawcaveInside3() {




    float centerX = 500.0f;      // 拱圆中心 X
    float centerY = 400.0f;      // 拱圆中心 Y
    float centerZ = 3250.0f;       // 固定 Z
    float z = centerZ;
    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;          // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
      float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float y = centerY + radius * sinf(angle2) * 2; // 向 shang

        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX + radius * cosf(angle); // 向左

            angle1 += 37;
            drawRockFinalDirect1(x, y, z, 700.0f, angle + angle1, 0.6f);
        }
    }
}

void drawcaveInside1a() {




    float centerX = -1350.0f;      // 拱圆中心 X
    float centerY = 400.0f;      // 拱圆中心 Y
    float centerZ = -3250.0f;      // 固定 Z
    float z = centerZ;
    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;          // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
        float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float y = centerY + radius * sinf(angle2) * 2; // 向 shang

        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX + radius * cosf(angle); // 向左

            angle1 += 37;
            drawRockFinalDirect1(x, y, z, 700.0f, angle + angle1, 0.6f);
        }
    }
}

void drawcaveInside2a() {




    float centerX = 0.0f;     // 拱圆中心 X
    float centerY = 400.0f;      // 拱圆中心 Y
    float centerZ = -3250.0f;      // 固定 Z
    float z = centerZ;
    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;          // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
      float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float y = centerY + radius * sinf(angle2) * 2; // 向 shang

        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX + radius * cosf(angle); // 向左

            angle1 += 37;
            drawRockFinalDirect1(x, y, z, 700.0f, angle + angle1, 0.6f);
        }
    }
}
void drawcaveInside3a() {




    float centerX = 500.0f;      // 拱圆中心 X
    float centerY = 400.0f;      // 拱圆中心 Y
    float centerZ = -3250.0f;      // 固定 Z
    float z = centerZ;
    float radius = 800.0f;      // 圆弧半径（越大越平缓）
    int count = 17;          // 石块数（图中 5 块建议设为 6）
    int angle1 = 0;
    for (int h = 0;h < count;h++) {
      float angle2 = (M_PI / 2.0f) * ((float)h / (count - 1)); // 0 到 π/2
        float y = centerY + radius * sinf(angle2) * 2; // 向 shang

        for (int i = 0; i < count; ++i) {
          float angle = (M_PI / 2.0f) * ((float)i / (count - 1)); // 0 到 π/2
            float x = centerX + radius * cosf(angle); // 向左

            angle1 += 37;
            drawRockFinalDirect1(x, y, z, 700.0f, angle + angle1, 0.6f);
        }
    }
}

float pulseTime = 0.0f;
float expandProgress = 0.0f;

void drawGreenScreenEffect() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float centerX = 150.0f;
    float centerY = 650.0f;
    float z = 2050.0f;
float maxW = 600.0f; // 最大宽度范围（左右扩展）
float maxH = 400.0f; // 最大高度范围（上下扩展）

float width = 0.0f, height = 0.0f;
if (expandProgress < 1.0f) {
   width = expandProgress * maxW;
   height = 5.0f; // 起初是很薄的一条横线
}
else {
  width = maxW;
  height = (expandProgress - 1.0f) * maxH;
  if (height > maxH) height = maxH;
}




// 发光雾效边缘（呼吸闪动）
if (expandProgress >= 2.0f) {
   float glowScale = 1.08f + 0.03f * sin(pulseTime);
   float alpha = 0.35f + 0.25f * sin(pulseTime);

    float glowW = maxW * glowScale;
    float glowH = maxH * glowScale;

    //glColor4f(0.4f, 1.0f, 0.4f, alpha);
    glColor3f(1.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);
    glVertex3f(centerX - glowW, centerY - glowH, z+2);
    glVertex3f(centerX + glowW, centerY - glowH, z+2);
    glVertex3f(centerX + glowW, centerY + glowH, z+2);
    glVertex3f(centerX - glowW, centerY + glowH, z+2);
    glEnd();

}



// 主绿色区域
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(centerX - width, centerY - height, z);
    glVertex3f(centerX + width, centerY - height, z);
    glVertex3f(centerX + width, centerY + height, z);
    glVertex3f(centerX - width, centerY + height, z);
    glEnd();
    glDisable(GL_BLEND);
}
void drawoutcave(int x, int z, float scale) {
  glPushMatrix();
  glTranslatef(x, 0, z);     // 移动到位置
    glScalef(scale, scale, scale);   // 缩放
    drawcave1();
    drawcave2();
    drawcave3();
    drawcave4();
    drawcave5a();
    drawcave6a();
    drawcaveInside1a();
    drawcaveInside2a();
    drawcaveInside3a();

    glPopMatrix();

}



//part 1
void switchToScene13(int) {
  scene = 13;
  glutPostRedisplay();
  initParticles_bubble();

    //glutTimerFunc(5000, switchToScene13, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）

}
void switchToScene12(int) {
  scene = 12;
  initParticles_bubble();

    glutPostRedisplay();
    glutTimerFunc(5000, switchToScene13, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）

}
void switchToScene111(int) {
  scene = 111;
  glutPostRedisplay();
  glutTimerFunc(2000, switchToScene12, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}

void switchToScene11(int) {
  scene = 11;
  glutPostRedisplay();
  glutTimerFunc(3000, switchToScene111, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}

void switchToScene10(int) {
  scene = 10;
  initSmokeParticles();
  initFlameTriangles();
  glutPostRedisplay();
  glutTimerFunc(4000, switchToScene11, 0);
 // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}

void switchToScene9(int) {
  scene = 9;
  initSmokeParticles();
  initFlameTriangles();

    glutPostRedisplay();
    glutTimerFunc(3000, switchToScene10, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}
//part 1 switchscene
void switchToScene81(int) {
  scene = 81;
  glutPostRedisplay();
  glutTimerFunc(3000, switchToScene9, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}

void switchToScene8(int) {
  scene = 8;
  glutPostRedisplay();
  glutTimerFunc(3000, switchToScene81, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}

void switchToScene15(int) {
  scene = 15;
  glutPostRedisplay();
  glutTimerFunc(2000, switchToScene8, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}

void switchToScene14(int) {
  scene = 14;
  glutPostRedisplay();
  glutTimerFunc(2000, switchToScene15, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}



void switchToScene7(int) {
  scene = 7;
  glutPostRedisplay();
  glutTimerFunc(2000, switchToScene14, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}

void switchToScene6(int) {
  scene = 6;
  glutPostRedisplay();
    glutTimerFunc(3000, switchToScene7, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}



void switchToScene5(int) {
  scene = 5;
  glutPostRedisplay();
  glutTimerFunc(2000, switchToScene6, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}

void switchToScene4(int) {
  scene = 4;
  glutPostRedisplay();
  glutTimerFunc(3000, switchToScene5, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}



void switchToScene3(int) {
  scene = 3;
  glutPostRedisplay();
  glutTimerFunc(1000, switchToScene4, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}

void switchToScene2(int) {
  scene = 2;
  glutPostRedisplay();
  glutTimerFunc(500, switchToScene3, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
}



void idle2() {
  pulseTime += 0.05f;

    if (expandProgress < 2.0f)
       expandProgress += 0.01f;

    glutPostRedisplay();
    if ((scene == 3||scene==4) && case5x > -700) {
       case5x -= 5;
       glutPostRedisplay();
    }
}



void idle1() {



    if (case1x > -400) {
       case1x -= 1;
       glutPostRedisplay();

    }
    if (case1L_z <= 2500) {
       case1L_z += 5;
       tearFallOffset += 3.9f; // 控制下落速度
        swingPhase += 0.05f;



        if (tearFallOffset > 100.0f) // 眼泪落到底后重新开始
         tearFallOffset = 0.0f;

        if (swingPhase > 2 * 3.14159f) swingPhase -= 2 * 3.14159f;

        cloudYPhase += 0.08f;
        if (cloudYPhase > 6.28f) cloudYPhase -= 6.28f;

        // 上浮偏移（左右不同步）
        cloudOffsetLeft = sin(cloudYPhase) * 5.0f;
        cloudOffsetRight = cos(cloudYPhase) * 5.0f;

        glutPostRedisplay();
    }
}



void idle3() {




    if (case5L_z <= 200) {
       case5L_z += 5;
       swingPhase += 0.05f;
       if (swingPhase > 2 * 3.14159f) swingPhase -= 2 * 3.14159f;




        glutPostRedisplay();
    }
    if(case5L_z <= 4000 && case5L_z >200&& scene==81) {

        case5L_z += 10;
        swingPhase += 0.1f;
        if (swingPhase > 2 * 3.14159f) swingPhase -= 2 * 3.14159f;

        glutPostRedisplay();

    }



}




void idle4() {

    updateSmokeParticles();
    flameTime += 0.02f;

    if (scene == 10&& scene10_x>= -3000) {
       scene10_x -= 10;
        if (scene10_x <= -1000&& scene10_z<1001) {
           scene10_z += 5;
        }
    }
    glutPostRedisplay();
}

void idle5() {
  if (glow == true) {
     if (glowScale < 21.0f)
        glowScale += 0.075f; // 缓慢放大，直到 scale = 1.0f

        else {
          glow = false;
        }
    }
    else {
      if (glowScale > 0) {
         glowScale -= 0.35f;
      }
      floatPhase_liang2 += 0.05f;
      if (floatPhase_liang2 > 6.28f) floatPhase_liang2 -= 6.28f;
      floatOffset_liang2 = sin(floatPhase_liang2) * 10.0f; // 浮动幅度为 ±10

        updateParticles_bubble(); // 更新粒子动画

        glutPostRedisplay();
    }

    if (scene == 13) {
       liang2_12y += 15;
       liang2_12z += 15;
       floatPhase_liang2 += 0.05f;
       if (floatPhase_liang2 > 6.28f) floatPhase_liang2 -= 6.28f;
       floatOffset_liang2 = sin(floatPhase_liang2) * 10.0f; // 浮动幅度为 ±10

        updateParticles_bubble(); // 更新粒子动画
    }
    glutPostRedisplay();
}

void idle6() {
  if (ene11_angle <= 180) {
     ene11_angle += 2;
  }
  else {
     ene11_y += 10;
     ene11_z += 10;
  }

    glutPostRedisplay();
}




void display() {



    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Enhanced camera movement
    switch (scene) {
    case 1:

     gluLookAt(1450 + case1x, 650, 2900, // 眼睛位置
       350.0, 350.0, 450.0, // 看向中心
       0.0, 1.0, 0.0);



     drawcave1();
     drawcave2();
     drawcave3();
     drawcave4();
 drawcave5();
 drawcave6();
 drawcaveInside1();
 drawcaveInside2();
 drawcaveInside3();
 drawFloor(0, 0, 0);

 //glPushMatrix();
 //glTranslatef(350, 350, 450); // 你之前的角色坐标中心

 //glRotatef(180, 0, 1, 0);
 //glTranslatef(-350, -350, -450);

 //glPopMatrix();
 drawLiang(0, 0, -1600 + case1L_z, 0, 0, 0, 0, 0, 0, 2);
 glutIdleFunc(idle1);



 break;

case 2:
  gluLookAt(1450 + case1x, 650, -1000, // 眼睛位置
   350.0, 350.0, 450.0, // 看向中心
   0.0, 1.0, 0.0);



 drawcave1();
 drawcave2();
 drawcave3();
 drawcave4();
 drawcave5();
 drawcave6();
 drawcaveInside1();
 drawcaveInside2();
 drawcaveInside3();
 drawFloor(0, 0, 0);
 drawLiang(0, 0, -1600 + case1L_z, 0, 0, 0, 0, 0, 0, 1);




 break;

case 3:
  gluLookAt(1450 + case1x+ case5x, 650, -1000, // 眼睛位置
   350.0, 350.0, 450.0, // 看向中心
   0.0, 1.0, 0.0);



 drawcave1();
 drawcave2();
 drawcave3();
 drawcave4();
 drawcave5();
 drawcave6();
 drawcaveInside1();
 drawcaveInside2();
 drawcaveInside3();
 drawFloor(0, 0, 0);




 drawText1(470, 460, 400, "!!!");

 drawLiang(0, 0, -1600 + case1L_z, 0, 0, 0, 0, 0, 0, 1);
 break;

case 4:

 gluLookAt(1450 + case1x+ case5x, 650, -1000, // 眼睛位置
   350.0, 350.0, 450.0, // 看向中心
   0.0, 1.0, 0.0);
 drawcave1();
 drawcave2();
 drawcave3();
 drawcave4();
 drawcave5();
 drawcave6();
 drawcaveInside1();
 drawcaveInside2();
 drawcaveInside3();
 drawFloor(0, 0, 0);




 drawLiang(0, 0, -1600 + case1L_z, 0, 0, 0, 0, 0, 0,1);
 drawGreenScreenEffect();
 glutIdleFunc(idle2);

 break;

case 5:

 //gluLookAt(350+case5x , 650, -1000, // 眼睛位置
 // 350.0, 350.0, 450.0, // 看向中心
 // 0.0, 1.0, 0.0);
 gluLookAt(1450 + case1x+case5x, 650, -1000, // 眼睛位置
   350.0, 350.0, 450.0, // 看向中心
   0.0, 1.0, 0.0);

 drawcave1();
 drawcave2();
 drawcave3();
 drawcave4();
 drawcave5();
 drawcave6();
 drawcaveInside1();
 drawcaveInside2();
 drawcaveInside3();
 drawFloor(0, 0, 0);




 drawLiang(0, 0, -1600 + case1L_z, 0, 0, 0, 0, 0, 0, 1);
 drawGreenScreenEffect();

 drawText1(600, 850, 2040, "Destiny Alignment : Liang – Hero Class");
 drawText1(400, 650, 2040, "Essence Transfer:");

 drawText1(680, 580, 2040, "Redirected to: Ren, Son of Azure Minister");




 glutIdleFunc(idle2);

  break;
case 6:

 //gluLookAt(350+case5x , 650, -1000, // 眼睛位置
 // 350.0, 350.0, 450.0, // 看向中心
 // 0.0, 1.0, 0.0);
 gluLookAt(1450 + case1x + case5x, 650, -1000, // 眼睛位置
   350.0, 350.0, 450.0, // 看向中心
   0.0, 1.0, 0.0);

 drawcave1();
 drawcave2();
 drawcave3();
 drawcave4();
 drawcave5();
 drawcave6();
 drawcaveInside1();
 drawcaveInside2();
 drawcaveInside3();
 drawFloor(0, 0, 0);
 drawLiang(0, 0, -1600 + case1L_z, 0, 0, 0, 0, 0, 0, 1);
 drawGreenScreenEffect();

 drawText1(600, 850, 2040, "You weren’t destined to be wrong. ");
 drawText1(400, 650, 2040, "Your fate was stolen.");




 glutIdleFunc(idle2);

 break;

case 7:
  gluLookAt(350 , 450, 1800, // 眼睛位置
   350.0, 350.0, 450.0, // 看向中心
   0.0, 1.0, 0.0);

 drawcave1();
 drawcave2();
 drawcave3();
 drawcave4();
 drawcave5();
 drawcave6();
 /*drawcaveInside1();
 drawcaveInside2();
 drawcaveInside3();*/
 drawFloor(0, 0, 0);



 drawText1(600, 850, 2040, "????");
 drawText1(400, 650, 2040, "!!!!!!!!!");

 drawLiang(0, 0, -1600 + case1L_z, 0, 0, 0, 0, 0, 0, 3);
  break;
case 8:
  gluLookAt(-1450 , 650, -2800, // 眼睛位置
   350.0, 350.0, 450.0, // 看向中心
   0.0, 1.0, 0.0);



 drawcave1();
 drawcave2();
 drawcave3();
 drawcave4();
 drawcave5a();
 drawcave6a();
 drawcaveInside1a();
 drawcaveInside2a();
 drawcaveInside3a();
 drawFloor(0, 0, 0);

 drawLiang(-700, 0, -4600 +case1L_z+case5L_z, 0, 0, 0, 0, 0, 0, 1);
 glutIdleFunc(idle3);

 if (case5L_z > 200) {
    drawText1(0, 0, 0, "!!!!!!!!!");
    drawText(-900, 800, 0, "HELP !!!!!!!!!!!");
 }

  break;
//// part 2
case 81:
  gluLookAt(-1450, 650, -2800, // 眼睛位置
   350.0, 350.0, 450.0, // 看向中心
   0.0, 1.0, 0.0);



 drawcave1();
 drawcave2();
 drawcave3();
 drawcave4();
 drawcave5a();
 drawcave6a();
 drawcaveInside1a();
 drawcaveInside2a();
 drawcaveInside3a();
 drawFloor(0, 0, 0);

 drawLiang(-700, 0, -4600 + case1L_z + case5L_z, 0, 0, 0, 0, 0, 0, 1);
 glutIdleFunc(idle3);

  break;
case 9:
  //gluLookAt(3800, 350, -3000, // 眼睛位置
 // 3800.0, 350.0, -2000.0, // 看向中心
 // 0.0, 1.0, 0.0);    gluLookAt(cameraX, cameraY, cameraZ, 0, 1, -20, 0, 1, 0);




 //float cameraX = -120.0f, cameraY = 1.0f, cameraZ = -72.0f;
 gluLookAt(3700, 350, -3000, 3800, 350, -2000, 0, 1, 0);

 drawGround();

 drawvillage(1200,0,2200);
 drawoutcave(3800, -4000, 0.5);

 drawLiang(3400, 0,-2000, 0, 0, 0, 0, 0, 0, 1);

 drawSmokeParticles(300, 125, -300);
 drawSmokeParticles(200, 80, -300);
 drawSmokeParticles(-15, 90, -300);
 drawSmokeParticles(0, 100, 300);
 drawSmokeParticles(-400, 100, 300);

 drawfire(0,0,0);
 drawenemy(3800,750,-1000,24.0f,250);
 glutIdleFunc(idle4);
 break;

case 10:
  if (scene10_x > -1000) {
     //gluLookAt(3700, 350, -3000, 3800, 350, -2000, 0, 1, 0);
     gluLookAt(3700 + scene10_x, 350, -3000 + scene10_z, 3800, 350, -2000, 0, 1, 0);

   drawGround();

   drawvillage(1200, 0, 2200);
   drawoutcave(3800, -4000, 0.5);

   drawLiang(3400, 0, -2000, 0, 0, 0, 0, 0, 0, 1);

   drawSmokeParticles(300, 125, -300);
   drawSmokeParticles(200, 80, -300);
   drawSmokeParticles(-15, 90, -300);
   drawSmokeParticles(0, 100, 300);
   drawSmokeParticles(-400, 100, 300);

   drawfire(0, 0, 0);
   drawenemy(3800, 750, -1000, 24.0f, 250);
   glutIdleFunc(idle4);

 }
 else {
   gluLookAt(3700 + scene10_x, 350,-3000 + scene10_z,
     3800, 350, -2000,
     0, 1, 0);

   //gluLookAt(2700, 350, -3000, 3800, 350, -2000, 0, 1, 0);

   //gluLookAt(700, 350, -2000, 3800, 350, -2000, 0, 1, 0);
   drawGround();
   drawvillage(1200, 0, -400);
   drawoutcave(3800, -5000, 0.5);

   drawLiang(3400, 0, -2800, 0, 0, 0, 0, 0, 0, 1);

   drawSmokeParticles(300, 125, -900);
   drawSmokeParticles(200, 80, -900);
   drawSmokeParticles(-15, 90, -900);
   drawSmokeParticles(0, 100, 300);
   drawSmokeParticles(-400, 100, 300);

   drawfire(-100, 200, -400);
   drawenemy(3800, 750, -1000, 44.0f, 250);
   drawText(5000, 750 + ene11_y, -2000 + ene11_z, "THERE IS NOTHING YOU CAN DO");

   glutIdleFunc(idle4);
 }
 break;

case 11:
 /* gluLookAt(3700 + scene10_x, 350, -3000 + scene10_z,
    3800, 350, -2000,
    0, 1, 0);*/

 //gluLookAt(2700, 350, -3000, 3800, 350, -2000, 0, 1, 0);

 gluLookAt(700, 350, -2000, 3800, 350, -2000, 0, 1, 0);
 drawGround();

 drawvillage(1200, 0, -400);
 drawoutcave(3800, -5000, 0.5);

 drawLiang(3400, 0, -2800, 0, 0, 0, 0, 0, 0, 1);

 drawSmokeParticles(300, 125, -900);
 drawSmokeParticles(200, 80, -900);
 drawSmokeParticles(-15, 90, -900);
 drawSmokeParticles(0, 100, 300);
 drawSmokeParticles(-400, 100, 300);

 drawfire(-100, 200, -400);
 //drawText(5000, 750 + ene11_y, -1000 + ene11_z, "NOOBBBBB");
 drawText(5000, 750 + ene11_y, -1500 + ene11_z, "HAHAHHAHAHAHA ");
 drawenemy(3800+ene11_x, 750+ene11_y, -1000+ene11_z, 44.0f, 250+ene11_angle);
 glutIdleFunc(idle6);

  break;
case 111:
  gluLookAt(700, 350, -2000, 3800, 350, -2000, 0, 1, 0);
  drawGround();

 drawvillage(1200, 0, -400);
 drawoutcave(3800, -5000, 0.5);

 drawLiang(3400, 0, -2800, 0, 0, 0, 0, 0, 0, 1);

 drawSmokeParticles(300, 125, -900);
 drawSmokeParticles(200, 80, -900);
 drawSmokeParticles(-15, 90, -900);
 drawSmokeParticles(0, 100, 300);
 drawSmokeParticles(-400, 100, 300);

 drawfire(-100, 200, -400);
 //drawText(5000, 750 + ene11_y, -1000 + ene11_z, "NOOBBBBB");
 drawText1(5000, 550, -2500, "I WILL PROTECT EVERYONE HERE !!!!! ");
 glutIdleFunc(idle6);

 break;

case 12:
  //float cameraX = 0.0f, cameraY = 5.0f, cameraZ = 12.0f;

 /*gluLookAt(3803, 355, -3014,
   3800, 352, -2000,
   0, 1, 0);*/
gluLookAt(700, 350, -2000,
  3800, 350, -2000,
  0, 1, 0);



/*drawparticle1(3900, 0, -3550);
glutIdleFunc(idle5);*/

drawGround();

drawvillage(1200, 0, -400);
drawoutcave(3800, -5000, 0.5);

drawfire(-100, 200, -400);

drawSmokeParticles(300, 125, -900);
drawSmokeParticles(200, 80, -900);
drawSmokeParticles(-15, 90, -900);
drawSmokeParticles(0, 100, 300);
drawSmokeParticles(-400, 100, 300);

if (glow == true) {

  drawGlowBall1(2220, 310, 550, glowScale, 77); // 在角色头顶右侧发光
  drawLiang(3400, 0, -2800, 0, 0, 0, 0, 0, 0, 1);

  glutIdleFunc(idle5);
}
else {
  drawparticle1(1300, -750 + floatOffset_liang2, -4900);

  drawLiang2(3202, -50 + floatOffset_liang2, -1300, 164.0f, 270.0f);
  if (glowScale > 0) {
     drawGlowBall1(2220, 310, 550, glowScale, 77); // 在角色头顶右侧发光
  }
  drawparticle1(3700, -1000 + floatOffset_liang2, -5050);
   glutIdleFunc(idle5);



   //gluLookAt(700, 650, -2000, // 相机位置
   // 3800, 350, -2000,       // 观察中心
   // 0, 1, 0);    // 向上方向




 }
 break;

case 13:
  gluLookAt(700, 350, -2000,
    3800, 350, -2000,
    0, 1, 0);



 /*drawparticle1(3900, 0, -3550);
 glutIdleFunc(idle5);*/

 drawGround();

 drawvillage(1200, 0, -400);
 drawoutcave(3800, -5000, 0.5);

 drawfire(-100, 200, -400);

 drawSmokeParticles(300, 125, -900);
 drawSmokeParticles(200, 80, -900);
 drawSmokeParticles(-15, 90, -900);
 drawSmokeParticles(0, 100, 300);
 drawSmokeParticles(-400, 100, 300);
    drawparticle1(1300 + liang2_12x, -750 + floatOffset_liang2 + liang2_12y, -4900 +
liang2_12z);

   drawLiang2(3202+liang2_12x, -50 + floatOffset_liang2 + liang2_12y, -1300 + liang2_12z,
164.0f, 270.0f);



   glutIdleFunc(idle5);

   break;
 case 14:

   //gluLookAt(3700 + case1x + case5x, 350, -3000, // 眼睛位置
   // 3800.0, 350.0, -2000.0, // 看向中心
   // 0.0, 1.0, 0.0);




   gluLookAt(3700, 350, -3000, 3800, 350, -2000, 0, 1, 0);
   drawenemy(3800, -5600, -1000, 504.0f, 250);
   drawText(5550, 850, 40, "Oh crap!He found out everything. ");



   break;

 case 15:
   gluLookAt(3700, 350, -3000, 3800, 350, -2000, 0, 1, 0);
   drawenemy(3800, -5600, -1000, 504.0f, 250);
   drawText(5550, 850, 40, " Time to work > < ");

   break;



 }
 //正前方

 //gluLookAt(1450, 350, 5380, // 眼睛位置
    // 350.0, 350.0, 450.0, // 看向中心
    // 0.0, 1.0, 0.0);

    //drawBentBoxSurface(
    // 350.0f, // centerX
    // 100.0f, // centerY (底面 Y)
    // 450.0f, // centerZ
    // 2000.0f, // width (横向总宽)
    // 200.0f, // height (直立高度)
    // 300.0f, // depth (厚度)
    // 40     // segments (越多越平滑)
    //);

    /*glRotatef(angle, 1.0f, 1.0f, 0.0f);*/
    //glPopMatrix();

    glutSwapBuffers();
}




void reshape(int w, int h) {
  if (h == 0) h = 1;
  float ratio = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, ratio, 1.0, 20000.0);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}



void playMusic1() {
  //for scene1
 mciSendStringA("open \"sounds\\banana-cat-crying.wav\" type waveaudio alias bgm", NULL, 0, NULL);
 mciSendStringA("play bgm", NULL, 0, NULL);
 mciSendStringA("setaudio bgm volume to 50", NULL, 0, NULL);
 Sleep(3500);
 mciSendStringA("pause bgm", NULL, 0, NULL);

    Sleep(20000);

 mciSendStringA("open \"sounds\\fire-sound-222359.wav\" type waveaudio alias fire", NULL, 0, NULL);
 mciSendStringA("play fire", NULL, 0, NULL);
 Sleep(7000);
 mciSendStringA("pause fire", NULL, 0, NULL);

    Sleep(1000);

 mciSendStringA("open \"sounds\\evil-laugh-89423.wav\" type waveaudio alias laugh", NULL, 0, NULL);
 mciSendStringA("play laugh", NULL, 0, NULL);
 Sleep(4000);
 mciSendStringA("pause laugh", NULL, 0, NULL);



  mciSendStringA("open \"sounds\\change.wav\" type waveaudio alias change", NULL, 0, NULL);
  mciSendStringA("play change", NULL, 0, NULL);
  Sleep(7000);
  mciSendStringA("pause change", NULL, 0, NULL);

}



int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 760);
    glutCreateWindow("3D 彩色立方体");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutTimerFunc(3000, switchToScene2, 0); // Scene 2 持续 18 秒（总到达 6+18=24 秒）
    std::thread musicThread(playMusic1); // 启动音乐播放线程
    musicThread.detach();
    glutMainLoop();
    return 0;

}
