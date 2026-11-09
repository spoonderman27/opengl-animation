#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include <mmsystem.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Function declarations
void drawCube(float size);
void drawSpear();
void drawTrident();
void drawHoveringSphere(float size = 1.0f);
void drawLiang();
void drawEvilGuardian();
void drawSkyBox();
void draw3DMountain(float x, float z, float width, float height, float depth, float r, float g, float
b);
void draw3DMountains();
void drawSimpleTree(float x, float z, float height, float trunkRadius, float leafRadius);
void draw3DTrees();
void drawClouds();
void drawCollisionEffect();
void drawWoodenHouse();
void drawFloor();

/* ---- Phase-2 forward decls ---- */
namespace P2 // only the functions you will call from Phase-1
{
  void init();
  void display();
  void reshape(int, int);
  void keyboard(unsigned char, int, int);
  void updateAnimation();

}

namespace P3 // only the functions you will call from Phase-1
{
  clock_t p3StartTime = 0;
  void init();
  void display();
  void reshape(int, int);
  void keyboard(unsigned char, int, int);
  void updateAnimation();
}

namespace P1 {

    void drawTridentStick();
    void drawFireWheel(float innerR = 0.08f, float outerR = 0.35f, int bandCount = 3);
  //                    ───                         scene               hand-off      control
  // ─────────────────────────────────
  bool phase2Started = false; // becomes true ≈22 s in
  float phase1Elapsed = 0.0f; // seconds that have run in phase-1

  bool phase3Started = false; // becomes true ≈30 s in

// Simple collision effect structure
   struct CollisionEffect {
     float x, y, z; // Position
     float size;    // Current size of the effect
     float alpha;     // Transparency
     bool active;     // Whether the effect is currently active

       CollisionEffect() : x(0), y(0), z(0), size(0), alpha(1.0f), active(false) {}

       void start(float _x, float _y, float _z) {
         x = _x;
         y = _y;
         z = _z;
         size = 0.1f; // Start smaller
         alpha = 1.0f;
         active = true;
       }

       void update() {
         if (!active) return;
         size += 0.1f; // Expand more slowly
         alpha -= 0.03f; // Fade out at moderate speed
         if (alpha <= 0) {
            active = false;
         }
       }
  };

  // Emits a bright orange–yellow glow so it looks like fire.
  // Draws one "fire coin" as a torus with 3 colored bands around its rim.
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

// Global variables for collision effect
CollisionEffect collisionEffect;

// Structure for spark particles
struct Spark {
     float x, y, z;    // Position
     float vx, vy, vz; // Velocity
     float life;      // Lifetime remaining
     float r, g, b;    // Color

     Spark(float x, float y, float z) : x(x), y(y), z(z), life(1.0f) {
      // Random velocity in all directions
      float angle = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;
      float elevation = static_cast<float>(rand()) / RAND_MAX * M_PI;
      float speed = 0.1f + static_cast<float>(rand()) / RAND_MAX * 0.2f;

         vx = speed * cos(angle) * sin(elevation);
         vy = speed * sin(angle) * sin(elevation);
         vz = speed * cos(elevation);

         // Golden-orange color with slight variation
         r = 1.0f;
         g = 0.5f + static_cast<float>(rand()) / RAND_MAX * 0.3f;
         b = 0.0f;
     }

     void update() {
       x += vx;
       y += vy;
       z += vz;
       vy -= 0.005f; // Gravity effect
       life -= 0.02f; // Fade out
     }
};

// Global variables for spark system
std::vector<Spark> sparks;
bool createSparks = false;
float sparkX = 0.0f, sparkY = 0.0f, sparkZ = 0.0f;

// Structure to hold 2D screen coordinates
struct ScreenCoords {
   int x, y;
};

// Function to convert 3D world coordinates to 2D screen coordinates
ScreenCoords worldToScreen(float x, float y, float z) {
   GLdouble modelview[16];
   GLdouble projection[16];
   GLint viewport[4];
   GLdouble winX, winY, winZ;

     glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
     glGetDoublev(GL_PROJECTION_MATRIX, projection);
     glGetIntegerv(GL_VIEWPORT, viewport);

     gluProject(x, y, z, modelview, projection, viewport, &winX, &winY, &winZ);

     ScreenCoords coords;
     coords.x = (int)winX;
     coords.y = (int)winY;
     return coords;
}

// Function to render text
void renderText(float x, float y, const char* text) {
   glDisable(GL_LIGHTING);
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

     glMatrixMode(GL_MODELVIEW);
     glPushMatrix();
     glLoadIdentity();

     // Set text color to black
     // glColor3f(0.0f, 0.0f, 0.0f);
     glColor3f(1.0f, 1.0f, 1.0f); // Black text
     glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

// Dialogue state
bool showLiangDialogue = true;
bool showGuardianDialogue = false;
bool showGuardianFinalDialogue = false;
const char* liangText = "YOU DIE, OR I DIE";
const char* guardianText = "daddy chill";
const char* guardianFinalText = "man what can i say";

// Global variables for animation
float cameraX = 0.0f, cameraY = 3.0f, cameraZ = 15.0f; // Moderate camera position
float targetCameraZ = 6.0f; // Target zoom position
float wheelAngle = 0.0f;

// Animation timing
bool animationStarted = false;
int startDelay = 0; // Counter for the 2-second delay
int finalDialogueDelay = 0; // New counter for final dialogue delay

// Weapon animation variables
float spearX = -10.0f; // Starting position for spear
float tridentX = 7.0f; // Starting position for trident
float weaponSpeed = 0.065f; // Speed of weapon movement
bool weaponsCollided = false; // Flag to stop animation when weapons meet

// Global variable for skull rotation
float skullAngle = 0.0f;
 // Global variable for hover rotation
 float hoverAngle = 0.0f;
 float hoverOffset = 0.0f; // New variable for vertical hover movement

  // Animation state
  bool animationComplete = false;
  bool weaponsRetreating = false; // New state for weapon retreat phase
  bool tridentFlying = false; // New state for trident flying phase
  float originalCameraZ = 15.0f; // Store original camera position
  float tridentRotation = 0.0f; // For trident spinning animation
  bool tridentHitHouse = false; // Flag for when trident hits ground
  bool weaponsReturned = false; // New flag to track if weapons have returned to original position

 // Cloud animation variables
 float cloudSwayAngle = 0.0f; // Angle for cloud swaying motion
 float cloudOffsetX = 0.0f; // Horizontal offset for cloud movement

 // New variables for final spear throw animation
 bool spearThrowStarted = false;
 float guardianPositionX = 5.0f; // Initial X position of guardian
 float guardianPositionY = 3.0f; // Initial Y position of guardian
 float throwSpearX = -10.0f; // Initial spear position
 float throwSpearY = 4.0f;     // Initial spear height
 float throwSpearRotation = 0.0f; // Rotation for thrown spear
 bool guardianKnockedOff = false; // Flag to track if guardian is knocked off

 // New variables for dialogue position
 float dialogueX = 5.0f; // Initial X position for dialogue
 float dialogueY = 4.9f; // Initial Y position for dialogue
 float dialogueZ = 0.0f; // Initial Z position for dialogue

 // New variables for Liang's movement
 float liangPositionX = -5.0f; // Initial X position of Liang
 bool liangMoving = false;      // Flag to track if Liang is moving
 float liangMoveSpeed = 0.2f; // Speed of Liang's movement
 bool finalImpact = false;     // Flag for final impact with guardian
// New variables for guardian's knockoff animation
float guardianXVelocity = 0.0f; // X velocity for guardian's movement
float guardianYVelocity = 0.0f; // Y velocity for guardian's movement
float guardianXAccel = 0.005f; // Reduced X acceleration significantly (was 0.05f)
float guardianYAccel = 0.005f; // Reduced Y acceleration significantly (was 0.08f)
float guardianRotation = 0.0f; // Rotation angle for spinning effect

// New variables for broken trident head
float brokenHeadX = 0.0f;
float brokenHeadY = 4.0f;
float brokenHeadZ = 0.0f;
bool tridentBroken = false;
float brokenHeadRotation = 0.0f;
float brokenHeadVelocityY = 0.5f; // Initial upward velocity
float brokenHeadGravity = -0.004f; // Much reduced gravity effect for slower falling
bool brokenHeadLanded = false; // Flag to track if head has landed

// Color definitions
struct Color {
   float r, g, b;
   Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

// Colors for Liang
Color SKIN(0.95f, 0.75f, 0.65f);
Color HAIR(0.0f, 0.0f, 0.0f);
Color SHIRT(0.9f, 0.2f, 0.1f);
Color PANTS(0.0f, 0.0f, 0.0f);
Color TRIM(0.95f, 0.65f, 0.2f);

// Colors for Evil Guardian
Color GUARDIAN_SKIN(0.4f, 0.2f, 0.6f);
Color GUARDIAN_HAIR(0.2f, 0.0f, 0.3f);
Color GUARDIAN_BODY(0.6f, 0.1f, 0.4f);

// Add these global variables after other global variables
float bgColorR = 0.5f; // Initial red component
float bgColorG = 0.1f; // Initial green component
float bgColorB = 0.1f; // Initial blue component

// Target sky blue colors
const float TARGET_R = 0.529f;
const float TARGET_G = 0.808f;
const float TARGET_B = 0.922f;

// Add these global variables after the other background color variables
float skyTopR = 0.5f; // Initial dark red for sky top
float skyTopG = 0.1f;
float skyTopB = 0.1f;
float skyBottomR = 0.3f; // Darker red for horizon
float skyBottomG = 0.05f;
float skyBottomB = 0.05f;

// Target sky colors (sky blue)
const float TARGET_SKY_TOP_R = 0.53f;
const float TARGET_SKY_TOP_G = 0.81f;
const float TARGET_SKY_TOP_B = 0.98f;
const float TARGET_SKY_BOTTOM_R = 0.05f;
const float TARGET_SKY_BOTTOM_G = 0.40f;
const float TARGET_SKY_BOTTOM_B = 0.85f;

// Add these global variables after the sky color variables
bool startColorTransition = false;
int transitionDelay = 0;
float transitionProgress = 0.0f;

// Utility functions
void drawCube(float size) {
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

void drawSpear()
{
  GLUquadric* q = gluNewQuadric();
    //                                            ──                                         shaft
  // ───────────────────────────────────────
  // ─────
    glColor3f(0.55f, 0.27f, 0.07f);         // wood brown
    gluCylinder(q, 0.12, 0.12, 3.5, 12, 1);

    // Decorative red bindings
    glColor3f(0.8f, 1.0f, 0.1f);
    for (float z = 0.4f; z < 3.2f; z += 0.5f)
    {
       glPushMatrix();
       glTranslatef(0, 0, z);
       gluCylinder(q, 0.13, 0.13, 0.15, 12, 1);
       glPopMatrix();
    }

    //                         ──                             flaming                          tip
  // ─────────────────────────────────────
    glPushAttrib(GL_LIGHTING_BIT);
    GLfloat em[] = { 1, 0.4f, 0, 1 };
    glMaterialfv(GL_FRONT, GL_EMISSION, em);
    glColor3f(10, -10.0f, 0);
    glTranslatef(0, 0, 3.5);

    gluCylinder(q, 0.35, 0, 0.8, 12, 1);      // main flame cone

    // Extra flicks
    glPushMatrix(); glRotatef(135, 0, 1, 0); gluCylinder(q, 0.25, 0, 0.6, 8, 1); glPopMatrix();
    glPushMatrix(); glRotatef(-135, 0, 1, 0); gluCylinder(q, 0.25, 0, 0.6, 8, 1); glPopMatrix();

    GLfloat noE[] = { 0,0,0,1 };
    glMaterialfv(GL_FRONT, GL_EMISSION, noE);
    glPopAttrib();

    gluDeleteQuadric(q);
}
 // Structure for trident tips



 // trident
 void drawTrident()
 {
    glPushMatrix();
    GLUquadric* q = gluNewQuadric();

   /*                                             ──               shaft
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
   if (!tridentBroken) { // Only draw tips if not broken
      glPushAttrib(GL_LIGHTING_BIT);
      GLfloat em[] = { 0.35f, 0.00f, 0.55f, 1.0f };
      glMaterialfv(GL_FRONT, GL_EMISSION, em);

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 3.6f); // start at end of shaft

        /* base block that ties the prongs together */
        glPushMatrix();
        glColor3f(0.60f, 0.10f, 0.80f);
        glScalef(0.70f, 0.15f, 0.28f);
        drawCube(1.0f);
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
    }

    gluDeleteQuadric(q);
    glPopMatrix();
}



// Character drawing functions
void drawLiang() {
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
glTranslatef(-0.6f, 0.0f, 0.0f);
P1::drawFireWheel();
glPopMatrix();

// Right wheel under right foot
glPushMatrix();
glTranslatef(0.6f, 0.0f, 0.0f);
P1::drawFireWheel();
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
drawCube(1.0f);
glPopMatrix();
// Hair (top part of head - black, shorter)
glPushMatrix();
glTranslatef(0, 0.7f, 0);
glColor3f(0.0f, 0.0f, 0.0f);
glScalef(2.1f, 0.6f, 2.1f);
drawCube(1.0f);
glPopMatrix();

// Left hair bun
glPushMatrix();
glTranslatef(-1.2f, 1.1f, 0);
glColor3f(0.0f, 0.0f, 0.0f);
glScalef(0.6f, 0.6f, 0.6f);
drawCube(1.0f);
glPopMatrix();

// Right hair bun
glPushMatrix();
glTranslatef(1.2f, 1.1f, 0);
glColor3f(0.0f, 0.0f, 0.0f);
glScalef(0.6f, 0.6f, 0.6f);
drawCube(1.0f);
glPopMatrix();

// Left hairband
glColor3f(1.0f, 0.0f, 0.0f);
glPushMatrix();
glTranslatef(-0.9f, 1.0f, 0);
glScalef(0.6f, 0.1f, 0.1f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(-1.3f, 0.3f, 0);
glScalef(0.1f, 1.4f, 0.1f);
drawCube(1.0f);
glPopMatrix();
glPushMatrix();
glTranslatef(-1.35f, 0.5f, 0);
glScalef(0.3f, 0.1f, 0.1f);
drawCube(1.0f);
glPopMatrix();

// Right hairband
glPushMatrix();
glTranslatef(0.9f, 1.0f, 0);
glScalef(0.6f, 0.1f, 0.1f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(1.35f, 0.3f, 0);
glScalef(0.1f, 1.4f, 0.1f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(1.35f, 0.5f, 0);
glScalef(0.3f, 0.1f, 0.1f);
drawCube(1.0f);
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
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0.15f, 0, 0.01f);
glScalef(0.3f, 0.4f, 0.1f);
glColor3f(1.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

glPopMatrix();

// Right Eye
glPushMatrix();
glTranslatef(0.4f, 0.1f, 0);

glPushMatrix();
glTranslatef(-0.15f, 0, 0.01f);
glScalef(0.3f, 0.4f, 0.1f);
glColor3f(1.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0.15f, 0, 0);
glScalef(0.3f, 0.4f, 0.1f);
glColor3f(1.0f, 1.0f, 1.0f);
drawCube(1.0f);
glPopMatrix();

glPopMatrix();

// Red dot on forehead
glPushMatrix();
glTranslatef(0., 0.5f, 0.01f);
glScalef(0.15f, 0.15f, 0.1f);
glColor3f(1.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

// Brown nose
glPushMatrix();
glTranslatef(0, -0.4f, 0.01f);
glScalef(0.15f, 0.15f, 0.1f);
glColor3f(0.95f, 0.75f, 0.65f);
drawCube(1.0f);
glPopMatrix();

// Mouth
glPushMatrix();
glTranslatef(0, -0.7f, 0.01f);
glScalef(1.3f, 0.11f, 0.15f);

glPushMatrix();
glTranslatef(-0.3f, 0, 0);
glScalef(0.4f, 1.0f, 1.0f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0, 0, 0.01f);
glScalef(0.7f, 1.0f, 1.0f);
glColor3f(1.0f, 1.0f, 1.0f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0.3f, 0, 0);
glScalef(0.4f, 1.0f, 1.0f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

glPopMatrix();
// Smiley pixels
glPushMatrix();
glTranslatef(-0.6f, -0.59f, 0.01f);
glScalef(0.1f, 0.1f, 0.1f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0.6f, -0.59f, 0.01f);
glScalef(0.1f, 0.1f, 0.1f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

glPopMatrix();
glPopMatrix();

// Body with jacket design
glPushMatrix();
glTranslatef(0, 5, 0);

glPushMatrix();
glColor3f(0.9f, 0.2f, 0.1f);
glScalef(2, 3, 1);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glColor3f(0.95f, 0.75f, 0.65f);
glTranslatef(0, 0, 0.51f);
glScalef(0.8f, 3.0f, 0.02f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glColor3f(1.0f, 1.0f, 0.0f);
glTranslatef(-0.8f, 0, 0.51f);
glScalef(0.4f, 3.0f, 0.02f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glColor3f(1.0f, 1.0f, 0.0f);
glTranslatef(0.8f, 0, 0.51f);
glScalef(0.4f, 3.0f, 0.02f);
drawCube(1.0f);
glPopMatrix();

glColor3f(0.95f, 0.65f, 0.2f);
glPushMatrix();
glTranslatef(0, -1.45f, 0.52f);
glScalef(2.0f, 0.1f, 0.01f);
drawCube(1.0f);
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
  drawCube(1.0f);
  glPopMatrix();

  if (i == 0) // left arm spear
  {
     glPushMatrix();
     glTranslatef(0, 4.0, spearX); // Use spearX for position
     glTranslatef(-3.0f, 0.0f, 7.8f); // to hand
     glRotatef(25, -0.5, 1, 0);     // point right (same as Liang)
        glScalef(2.2f, 2.2f, 2.2f);   // overall spear size
        drawSpear();
        glPopMatrix();
    }
}

// Pants
glPushMatrix();
glColor3f(0.0f, 0.0f, 0.0f);
glTranslatef(0, 2.5f, 0);
glScalef(1, 1.5f, 1);
drawCube(1.0f);
glPopMatrix();

// Legs and feet

for (int i = 0; i < 2; i++) {
  float side = i == 0 ? -1.0f : 1.0f;

    glPushMatrix();
    glTranslatef(side * 0.5f, 1.5f, 0);

    glPushMatrix();
    glColor3f(0.95f, 0.75f, 0.65f);
    glScalef(0.5f, 1.5f, 0.5f);
    drawCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.95f, 0.75f, 0.65f);
    glTranslatef(0, -0.75f, 0.05f);
    glScalef(0.55f, 0.3f, 0.6f);
    drawCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}
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
  glTranslatef(guardianPositionX, guardianPositionY, 0); // Position on right side

    // Add rotation when knocked off
    if (guardianKnockedOff) {
       glRotatef(guardianRotation, 0, 0, 1); // Rotate around Z axis when knocked off
    }

    glRotatef(-70, 0, 1, 0); // Face towards left (Liang)
    glScalef(0.30f, 0.30f, 0.30f); // Make Evil Guardian 25% of original size

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
drawCube(1.0f);
glPopMatrix();

// Hair (darker purple/black)
glPushMatrix();
glTranslatef(0, 0.7f, 0);
glColor3f(0.2f, 0.0f, 0.3f);
glScalef(2.1f, 0.6f, 2.1f);
drawCube(1.0f);
glPopMatrix();

// Left devil horn (L-shaped)
glColor3f(0.8f, 0.1f, 0.1f);
glPushMatrix();
glTranslatef(-1.3f, 0.9f, 0);
glScalef(1.6f, 0.25f, 0.25f);
drawCube(1.0f);
glPopMatrix();
glPushMatrix();
glTranslatef(-2.0f, 1.3f, 0);
glScalef(0.25f, 0.8f, 0.25f);
drawCube(1.0f);
glPopMatrix();

// Right devil horn (L-shaped)
glPushMatrix();
glTranslatef(1.3f, 0.9f, 0);
glScalef(1.6f, 0.25f, 0.25f);
drawCube(1.0f);
glPopMatrix();
glPushMatrix();
glTranslatef(2.0f, 1.3f, 0);
glScalef(0.25f, 0.8f, 0.25f);
drawCube(1.0f);
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
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0, 0, 0.01f);
glScalef(0.4f, 0.15f, 0.1f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

glPopMatrix();

// Right Eye (demonic red eyes)
glPushMatrix();
glTranslatef(0.4f, -0.1f, 0);

glPushMatrix();
glScalef(0.6f, 0.3f, 0.1f);
glColor3f(1.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0, 0, 0.01f);
glScalef(0.4f, 0.15f, 0.1f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

glPopMatrix();

// Third eye on forehead
glPushMatrix();
glTranslatef(0, 0.25f, 0.01f);
glScalef(0.6f, 0.3f, 0.1f);
glColor3f(1.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glTranslatef(0, 0.25f, 0.03f);
glScalef(0.4f, 0.15f, 0.1f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

// Dark nose
glPushMatrix();
glTranslatef(0, -0.4f, 0.01f);
glScalef(0.15f, 0.15f, 0.1f);
glColor3f(0.3f, 0.1f, 0.4f);
drawCube(1.0f);
glPopMatrix();

// Demonic mouth (fanged appearance)
glPushMatrix();
glTranslatef(0, -0.7f, 0.01f);
glScalef(1.3f, 0.2f, 0.15f);

glPushMatrix();
glColor3f(0.5f, 0.0f, 0.0f);
drawCube(1.0f);
glPopMatrix();

// Left fang
glPushMatrix();
glTranslatef(-0.3f, -0.2f, 0.01f);
glScalef(0.15f, 0.4f, 1.0f);
glColor3f(1.0f, 1.0f, 1.0f);
drawCube(1.0f);
glPopMatrix();

// Right fang
glPushMatrix();
glTranslatef(0.3f, -0.2f, 0.01f);
glScalef(0.15f, 0.4f, 1.0f);
glColor3f(1.0f, 1.0f, 1.0f);
drawCube(1.0f);
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
drawCube(1.0f);
glPopMatrix();
glPushMatrix();
glColor3f(0.5f, 0.2f, 0.7f);
glTranslatef(0, 0, 0.51f);
glScalef(0.8f, 3.0f, 0.02f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glColor3f(0.8f, 0.1f, 0.1f);
glTranslatef(-0.8f, 0, 0.51f);
glScalef(0.4f, 3.0f, 0.02f);
drawCube(1.0f);
glPopMatrix();

glPushMatrix();
glColor3f(0.8f, 0.1f, 0.1f);
glTranslatef(0.8f, 0, 0.51f);
glScalef(0.4f, 3.0f, 0.02f);
drawCube(1.0f);
glPopMatrix();

glColor3f(0.1f, 0.0f, 0.2f);
glPushMatrix();
glTranslatef(0, -1.45f, 0.52f);
glScalef(2.0f, 0.25f, 0.01f);
drawCube(1.0f);
glPopMatrix();

glPopMatrix();

// Arms (dark purple skin)
for (int i = 0; i < 2; i++) {
   float side = i == 0 ? -1.0f : 1.0f;

  glPushMatrix();
  glTranslatef(side * 1.5f, 5, 0);
  glColor3f(0.4f, 0.2f, 0.6f);
    glScalef(0.5f, 1.5f, 0.5f);
    drawCube(1.0f);
    glPopMatrix();

    if (i == 1) // right arm trident
    {
       glPushMatrix();
       glTranslatef(0, 4.0, -tridentX); // Use tridentX for position
       glTranslatef(2.0f, 0.0f, 4.0f); // to hand
       glRotatef(-70, -30, 200, -700); // point left (towards Liang)
       glScalef(2.3f, 2.3f, 2.3f); // overall trident size
       if (tridentBroken) {
          drawTridentStick(); // Draw only the stick after it breaks
       }
       else {
          drawTrident(); // Draw full trident before it breaks
       }
       glPopMatrix();
    }
}

// Pants (black with red trim)
glPushMatrix();
glColor3f(0.1f, 0.0f, 0.1f);
glTranslatef(0, 2.5f, 0);
glScalef(1, 1.5f, 1);
drawCube(1.0f);
glPopMatrix();

// Legs and feet (dark purple skin)
for (int i = 0; i < 2; i++) {
   float side = i == 0 ? -1.0f : 1.0f;

    glPushMatrix();
    glTranslatef(side * 0.5f, 1.5f, 0);

    glPushMatrix();
    glColor3f(0.4f, 0.2f, 0.6f);
        glScalef(0.5f, 1.5f, 0.5f);
        drawCube(1.0f);
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.3f, 0.1f, 0.5f);
        glTranslatef(0, -0.75f, 0.05f);
        glScalef(0.55f, 0.3f, 0.8f);
        drawCube(1.0f);
        glPopMatrix();

        glPopMatrix();
    }

    glPopMatrix();
}

void drawSkyBox()
{
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    glBegin(GL_QUADS);
    // TOP edge - current sky top color
    glColor3f(skyTopR, skyTopG, skyTopB);
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
      // BOTTOM edge - current sky bottom color
      glColor3f(skyBottomR, skyBottomG, skyBottomB);
      glVertex2f(1.0f, -1.0f);
      glVertex2f(-1.0f, -1.0f);
      glEnd();

      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
      glPopAttrib();
  }

  void drawSunSphere()
  {
    glPushMatrix();
    glTranslatef(-20.0f, 30.0f, -60.0f); // up & far away

      // Yellow sun sphere
      glColor3f(1.0f, 0.95f, 0.0f); // Bright yellow color
      glutSolidSphere(5.0, 24, 24);

      // Add a subtle glow effect
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      glColor4f(1.0f, 0.8f, 0.0f, 0.3f); // Transparent yellow for glow
      glutSolidSphere(5.2, 24, 24); // Slightly larger sphere for glow
      glDisable(GL_BLEND);

      glPopMatrix();
  }



   void draw3DMountain(float x, float z, float width, float height, float depth, float r, float g,
float b) {
     glColor3f(r, g, b);

      // Calculate the points for the mountain
float halfWidth = width / 2.0f;
float halfDepth = depth / 2.0f;

// Draw the mountain as a pyramid with 4 triangular faces
glBegin(GL_TRIANGLES);

// Set normals for proper lighting

// Front face
glNormal3f(0.0f, 0.8f, 0.6f);
glVertex3f(x - halfWidth, 0.0f, z + halfDepth); // bottom left
glVertex3f(x + halfWidth, 0.0f, z + halfDepth); // bottom right
glVertex3f(x, height, z);            // peak

// Right face
glNormal3f(0.6f, 0.8f, 0.0f);
glVertex3f(x + halfWidth, 0.0f, z + halfDepth); // front bottom
glVertex3f(x + halfWidth, 0.0f, z - halfDepth); // back bottom
glVertex3f(x, height, z);            // peak

// Back face
glNormal3f(0.0f, 0.8f, -0.6f);
glVertex3f(x + halfWidth, 0.0f, z - halfDepth); // bottom right
glVertex3f(x - halfWidth, 0.0f, z - halfDepth); // bottom left
glVertex3f(x, height, z);            // peak

// Left face
glNormal3f(-0.6f, 0.8f, 0.0f);
glVertex3f(x - halfWidth, 0.0f, z - halfDepth); // back bottom
glVertex3f(x - halfWidth, 0.0f, z + halfDepth); // front bottom
glVertex3f(x, height, z);            // peak

glEnd();

// Draw the base (optional, for better grounding)
glBegin(GL_QUADS);
glNormal3f(0.0f, 1.0f, 0.0f);
glVertex3f(x - halfWidth, 0.0f, z - halfDepth);
    glVertex3f(x + halfWidth, 0.0f, z - halfDepth);
    glVertex3f(x + halfWidth, 0.0f, z + halfDepth);
    glVertex3f(x - halfWidth, 0.0f, z + halfDepth);
    glEnd();
}

void draw3DMountains() {
  // Far mountains (smaller, lighter)
  draw3DMountain(-10.0f, -25.0f, 6.0f, 2.5f, 4.0f, 0.5f, 0.5f, 0.7f);
  draw3DMountain(-2.0f, -24.0f, 8.0f, 3.0f, 5.0f, 0.5f, 0.5f, 0.7f);
  draw3DMountain(9.0f, -25.5f, 5.0f, 2.0f, 3.5f, 0.5f, 0.5f, 0.7f);

    // Medium distance mountains
    draw3DMountain(-7.0f, -18.0f, 8.0f, 4.0f, 6.0f, 0.4f, 0.4f, 0.6f);
    draw3DMountain(3.0f, -17.5f, 10.0f, 5.0f, 7.0f, 0.4f, 0.4f, 0.6f);
    draw3DMountain(14.0f, -19.0f, 7.0f, 5.0f, 8.5f, 0.4f, 0.4f, 0.6f);

    // Close mountains (larger, darker)
    draw3DMountain(-14.0f, -12.0f, 12.0f, 6.0f, 8.0f, 0.3f, 0.3f, 0.5f);
    draw3DMountain(18.0f, -11.0f, 14.0f, 7.0f, 9.0f, 0.3f, 0.3f, 0.5f);
}

void drawSimpleTree(float x, float z, float height, float trunkRadius, float leafRadius) {
  GLUquadricObj* quadric = gluNewQuadric();
  gluQuadricDrawStyle(quadric, GLU_FILL);
  gluQuadricNormals(quadric, GLU_SMOOTH);

    // Draw trunk (brown cylinder)
    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadric, trunkRadius, trunkRadius * 0.8f, height * 0.6f, 8, 1);
    glPopMatrix();

    // Draw leaves (green sphere)
    glColor3f(0.1f, 0.6f, 0.1f);
    glPushMatrix();
    glTranslatef(x, height * 0.6f + leafRadius * 0.5f, z);
    gluSphere(quadric, leafRadius, 12, 12);
    glPopMatrix();

    gluDeleteQuadric(quadric);
}

void draw3DTrees() {
  // Trees scattered around the scene
  drawSimpleTree(-2.0f, 2.0f, 1.0f, 0.05f, 0.2f);
  drawSimpleTree(2.0f, 1.5f, 2.5f, 0.12f, 0.8f);
  drawSimpleTree(-6.0f, -1.0f, 2.0f, 0.07f, 0.7f);
  drawSimpleTree(7.0f, -2.0f, 2.8f, 0.14f, 0.9f);
  drawSimpleTree(0.0f, -3.5f, 3.2f, 0.16f, 1.1f);
  drawSimpleTree(2.5f, -1.8f, 2.2f, 0.10f, 0.7f);
  drawSimpleTree(-7.0f, 3.0f, 4.0f, 0.20f, 1.3f);
  drawSimpleTree(9.0f, 2.5f, 3.8f, 0.19f, 1.25f);

    // Some trees closer to mountains
    drawSimpleTree(-10.0f, -5.0f, 2.0f, 0.12f, 0.6f);
    drawSimpleTree(12.0f, -4.0f, 2.5f, 0.14f, 0.8f);
    drawSimpleTree(-5.0f, -6.0f, 1.8f, 0.10f, 0.5f);
    drawSimpleTree(6.0f, -5.5f, 2.2f, 0.12f, 0.7f);
}

void drawClouds() {
  glDisable(GL_LIGHTING);
  glColor3f(1.0f, 0.9f, 1.0f);

    // Update cloud animation
    cloudSwayAngle += 0.03f; // Increased speed for cloud movement
    cloudOffsetX = sin(cloudSwayAngle) * 0.15f; // Slightly larger movement range

    GLUquadricObj* cloud = gluNewQuadric();
    gluQuadricDrawStyle(cloud, GLU_FILL);

    // Cloud 1
    glPushMatrix();
    glTranslatef(-8.0f + cloudOffsetX, 9.0f + sin(cloudSwayAngle) * 0.2f, -15.0f);
    glScalef(1.5f, 0.8f, 1.0f);
    gluSphere(cloud, 1.0, 16, 16);
    glTranslatef(1.2f, 0.0f, 0.0f);
    gluSphere(cloud, 0.8, 16, 16);
    glTranslatef(-2.0f, 0.2f, 0.0f);
    gluSphere(cloud, 0.6, 16, 16);
    glPopMatrix();

    // Cloud 2
    glPushMatrix();
    glTranslatef(5.0f - cloudOffsetX, 12.0f + sin(cloudSwayAngle + 2.0f) * 0.2f, -16.0f);
    glScalef(2.0f, 1.0f, 1.2f);
    gluSphere(cloud, 1.0, 16, 16);
    glTranslatef(0.8f, -0.2f, 0.0f);
    gluSphere(cloud, 0.9, 16, 16);
    glTranslatef(-1.5f, 0.3f, 0.0f);
    gluSphere(cloud, 0.7, 16, 16);
    glPopMatrix();

    // Cloud 3
    glPushMatrix();
    glTranslatef(12.0f + cloudOffsetX, 9.0f + sin(cloudSwayAngle + 4.0f) * 0.2f, -14.0f);
    glScalef(1.2f, 0.9f, 1.0f);
    gluSphere(cloud, 0.8, 16, 16);
    glTranslatef(1.0f, 0.1f, 0.0f);
    gluSphere(cloud, 0.6, 16, 16);
    glPopMatrix();

    gluDeleteQuadric(cloud);
    glEnable(GL_LIGHTING);
}

void drawCollisionEffect() {
  if (!collisionEffect.active) return;

    // Save all relevant states
    glPushAttrib(GL_ALL_ATTRIB_BITS);
// Setup for explosion effect
glDisable(GL_LIGHTING);
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE);

glPushMatrix();
glTranslatef(collisionEffect.x, collisionEffect.y, collisionEffect.z);

// Draw the star-burst effect
float innerRadius = collisionEffect.size * 0.15f;
float outerRadius = collisionEffect.size * 0.8f;
int numSpikes = 12;

// Draw the outer spikes
glBegin(GL_TRIANGLES);
for (int i = 0; i < numSpikes; i++) {
   float angle1 = (i * 2 * M_PI) / numSpikes;
   float angle2 = ((i + 0.5f) * 2 * M_PI) / numSpikes;
   float angle3 = ((i + 1) * 2 * M_PI) / numSpikes;

  glColor4f(1.0f, 0.8f, 0.0f, collisionEffect.alpha);
  glVertex3f(0, 0, 0);

  glColor4f(1.0f, 0.4f, 0.0f, 0);
  glVertex3f(outerRadius * cos(angle1), outerRadius * sin(angle1), 0);
  glVertex3f(outerRadius * cos(angle3), outerRadius * sin(angle3), 0);
}
glEnd();

// Draw the inner bright core
glBegin(GL_TRIANGLE_FAN);
glColor4f(1.0f, 1.0f, 0.8f, collisionEffect.alpha);
glVertex3f(0, 0, 0);

for (int i = 0; i <= numSpikes; i++) {
  float angle = (i * 2 * M_PI) / numSpikes;
  glColor4f(1.0f, 0.8f, 0.0f, collisionEffect.alpha * 0.5f);
      glVertex3f(innerRadius * cos(angle), innerRadius * sin(angle), 0);
    }
    glEnd();

    glPopMatrix();

    // Restore all states
    glPopAttrib();

    // Explicitly reset blend function to original state
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void drawWoodenHouse() {
  // Save current matrix state
  glPushMatrix();

    // Position the house near the mountains
    glRotatef(30, 0, 1, 0);
    glTranslatef(-1.0f, 0.0f, -8.0f);
    glScalef(1.5f, 1.5f, 1.5f);

    // Wooden color for walls - lighter wood
    glColor3f(0.8f, 0.6f, 0.4f); // Much lighter wooden color

    // Main house body (cube)
    glPushMatrix();
    glScalef(2.0f, 1.5f, 1.5f); // Adjust dimensions to make the house more proportional
    drawCube(1.0f);
    glPopMatrix();

    // Roof (triangular prism with enhanced geometry)
    glColor3f(1.0f, 0.5f, 0.5f); // Correct color for the roof
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f); // Adjust positioning for the roof
    glBegin(GL_TRIANGLES);
    // Front triangle
    glVertex3f(-1.5f, -0.25f, 0.75f); // Increased the width for a wider roof
glVertex3f(1.5f, -0.25f, 0.75f);
glVertex3f(0.0f, 1.25f, 0.0f); // Higher peak for a more angled roof

// Back triangle
glVertex3f(-1.5f, -0.25f, -0.75f);
glVertex3f(1.5f, -0.25f, -0.75f);
glVertex3f(0.0f, 1.25f, 0.0f);
glEnd();

// Roof sides
glBegin(GL_QUADS);
glVertex3f(-1.5f, -0.25f, 0.75f);
glVertex3f(-1.5f, -0.25f, -0.75f);
glVertex3f(0.0f, 1.25f, 0.0f);
glVertex3f(0.0f, 1.25f, 0.0f);

glVertex3f(1.5f, -0.25f, 0.75f);
glVertex3f(1.5f, -0.25f, -0.75f);
glVertex3f(0.0f, 1.25f, 0.0f);
glVertex3f(0.0f, 1.25f, 0.0f);
glEnd();
glPopMatrix();

// Door (made thicker and more proportionate)
glColor3f(0.6f, 0.4f, 0.2f); // Darker than walls but still relatively light
glPushMatrix();
glTranslatef(0.0f, -0.25f, 0.76f);
glScalef(0.4f, 1.3f, 0.2f); // Increased height and thickness
drawCube(1.0f);
glPopMatrix();

// Windows (adjusted position and scale)
glColor3f(0.7f, 0.9f, 1.0f); // Light blue for windows
// Left window
glPushMatrix();
glTranslatef(-0.7f, 0.4f, 0.76f); // Adjusted height for better positioning
glScalef(0.3f, 0.3f, 0.1f); // Increased window size
drawCube(1.0f);
    glPopMatrix();

    // Right window
    glPushMatrix();
    glTranslatef(0.7f, 0.4f, 0.76f);
    glScalef(0.3f, 0.3f, 0.1f); // Increased window size
    drawCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

void drawFloor() {
  glColor3f(0.3f, 0.7f, 0.3f);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);

    float size = 30.0f;
    glVertex3f(-size, 0.0f, -size);
    glVertex3f(size, 0.0f, -size);
    glVertex3f(size, 0.0f, size);
    glVertex3f(-size, 0.0f, size);

    glEnd();
}

void init()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Enable blending for sparks
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLfloat lightPos[] = { 5.0f, 10.0f, 5.0f, 1.0f };
    GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

    // Set initial dark red background color
    glClearColor(bgColorR, bgColorG, bgColorB, 1.0f);
}

void display()
{

    if (phase2Started) {
       P2::display();    // draw guardian close-up instead
       return;
    }

    if (phase3Started) {
       P3::display();    // draw guardian close-up instead
       return;
    }

    // Update clear color each frame
    glClearColor(bgColorR, bgColorG, bgColorB, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0, 4, 0, 0, 1, 0);

    drawSkyBox();
    draw3DMountains();
    drawClouds();
    drawCollisionEffect();
    drawWoodenHouse();
    drawFloor();
    draw3DTrees();
    drawSunSphere();
drawLiang();
drawEvilGuardian();

// Draw the broken trident head if it exists
if (tridentBroken && !tridentHitHouse) {
   glPushMatrix();
   glTranslatef(brokenHeadX, brokenHeadY, brokenHeadZ);
   glRotatef(brokenHeadRotation, 0, 0, 1); // Spin around Z axis
   glScalef(1.8f, 1.8f, 1.8f); // Match the original trident scale

  // Draw only the prongs part of the trident
  GLUquadric* q = gluNewQuadric();

  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 3.6f); // Start at where prongs begin

  /* base block that ties the prongs together */
  glPushMatrix();
  glColor3f(0.60f, 0.10f, 0.80f);
  glScalef(0.70f, 0.15f, 0.28f);
  drawCube(1.0f);
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

    gluDeleteQuadric(q);
    glPopMatrix();
}

// Render dialogue
if (!animationStarted || showGuardianFinalDialogue || guardianKnockedOff) {
   // Get screen coordinates for Liang's position (left side)
   ScreenCoords liangPos = worldToScreen(-4.5f, 4.7f, 0.0f); // Above Liang's head

    // Get screen coordinates for Evil Guardian's position or flying dialogue
    ScreenCoords guardianPos;
    if (guardianKnockedOff) {
       guardianPos = worldToScreen(dialogueX, dialogueY, dialogueZ);
    }
    else {
       guardianPos = worldToScreen(3.0f, 4.9f, 0.0f); // Above Evil Guardian's head
    }

    if (showLiangDialogue) {
       // Position text above Liang, centered
       int textWidth = strlen(liangText) * 9; // Approximate width of text
       renderText(liangPos.x - textWidth / 1.3, liangPos.y, liangText);
    }
    if (showGuardianDialogue) {
       // Position text above Evil Guardian, centered
       int textWidth = strlen(guardianText) * 9; // Approximate width of text
       renderText(guardianPos.x - textWidth / 2, guardianPos.y, guardianText);
    }
    if (showGuardianFinalDialogue || guardianKnockedOff) {
       // Position final text above Evil Guardian, centered
       int textWidth = strlen(guardianFinalText) * 9; // Approximate width of text
       renderText(guardianPos.x - textWidth / 2, guardianPos.y, guardianFinalText);
        }
    }

    glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)w / h, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

// Minimal keyboard: ESC quits
void keyboard(unsigned char key, int, int)
{
   if (key == 27) exit(0);
}

void startPhase2()
{
  if (phase2Started) return;
  phase2Started = true;

    /* give Phase-2 the GL state and the callbacks it expects */
    P2::init();            // its own clear-colour, light, etc.
    glutDisplayFunc(P2::display);
    glutReshapeFunc(P2::reshape);
    glutKeyboardFunc(P2::keyboard);
    glutIdleFunc(P2::updateAnimation); // Phase-2 drives itself in idle
}

void startPhase3()
{
  if (phase3Started) return;
  phase3Started = true;
         /* give Phase-3 the GL state and the callbacks it expects */
         P3::init();            // its own clear-colour, light, etc.
         glutDisplayFunc(P3::display);
         glutReshapeFunc(P3::reshape);
         glutKeyboardFunc(P3::keyboard);
         glutIdleFunc(P3::updateAnimation); // Phase-3 drives itself in idle
     }



     void updateAnimations(int)
     {
       /* ══ 1. advance the phase-1 clock every tick ─────────────── */
         const float dt = 0.016f;       // 16 ms timer
         phase1Elapsed += dt;            // <-- make phase1Elapsed global (you already declared it)

         /* ══ 2. switch when the clock hits 22 s ────────────────────
*/
         if (!phase2Started && phase1Elapsed >= 22.0f) {
            startPhase2();
            return;          // nothing else from phase-1 this frame
         }
         if (phase2Started) return;     // safety guard (after the switch)



         /*----------------------------------------------------*/
         if (!phase3Started && phase1Elapsed >= 30.0f) {
            startPhase3();
            return;
         }
         if (phase3Started) return;

         if (!animationStarted) {
            startDelay++;

           // Show Liang's dialogue starting at the beginning
           if (startDelay == 1) { // First frame
              showLiangDialogue = true;
     showGuardianDialogue = false;
     showGuardianFinalDialogue = false; // Make sure it's off initially
    }
    // Show Guardian's dialogue after 2 seconds
    else if (startDelay == 125) { // 2 seconds (125 frames)
       showLiangDialogue = true; // Keep Liang's dialogue
       showGuardianDialogue = true;
    }

    // 5 seconds = 5000ms, with 16ms per frame ≈ 313 frames
    if (startDelay >= 313) {
       animationStarted = true;
       showLiangDialogue = false;
       showGuardianDialogue = false;
    }

    // Update wheel and hover animations even during dialogue
    wheelAngle += 3.0f;
    if (wheelAngle > 360.0f) wheelAngle -= 360.0f;

    hoverAngle += 2.0f;
    if (hoverAngle > 360.0f) hoverAngle -= 360.0f;

    // Update vertical hover movement during dialogue too
    hoverOffset = sin(hoverAngle * M_PI / 180.0f) * 0.6f;



    glutPostRedisplay();
    glutTimerFunc(16, updateAnimations, 0);
    return;
}

if (!animationComplete) {
   // Initial weapon clash phase
   if (!weaponsCollided) {
      spearX += weaponSpeed;
      tridentX -= weaponSpeed;
 // Calculate actual world positions of weapon tips
 float spearTipWorldX = -5.0f + spearX + 2.0f;
 float tridentTipWorldX = 5.0f + tridentX - 4.0f;
 float distance = abs(spearTipWorldX - tridentTipWorldX);

 if (distance <= 0.2f) {
    weaponsCollided = true;
    // Use fixed position for weapons
    spearX = 2.0f; // Fixed spear position
    tridentX = -2.0f; // Fixed trident position

     // Start collision effect at a specific position
     collisionEffect.start(0.0f, 4.0f, 0.0f); // Centered at (0,4,0)

     // Play collision sound
     PlaySound(TEXT("sound effect collision"), NULL, SND_ALIAS | SND_ASYNC);
 }
}
// Collision effect and pause phase
else if (!weaponsRetreating) {
   static int collisionFrames = 0;
   collisionFrames++;

 if (collisionFrames > 60) {
    weaponsRetreating = true;
    tridentBroken = true; // Break the trident head
    brokenHeadX = 0.0f; // Keep at collision point
    brokenHeadY = 4.0f; // Start at collision height
    brokenHeadZ = -8.0f; // Keep at collision point
    brokenHeadVelocityY = 0.15f; // Reduced initial upward velocity for slower motion
    collisionFrames = 0;
 }
}
// Handle broken trident head and weapons retreat
else if (weaponsRetreating && !weaponsReturned) {
   // Move spear back to starting position
   if (spearX > -10.0f) {
      spearX -= weaponSpeed * 0.8f;
}

// Move trident base back to starting position
if (tridentX < 7.0f) {
   tridentX += weaponSpeed * 0.8f;
}

// Check if both weapons have returned to their original positions
if (spearX <= -10.0f && tridentX >= 7.0f) {
   spearX = -10.0f; // Ensure exact position
   tridentX = 7.0f; // Ensure exact position
   weaponsReturned = true;
}

// Animate broken trident head falling
if (!brokenHeadLanded) {
   // Update vertical position only
   brokenHeadY += brokenHeadVelocityY;
   brokenHeadVelocityY += brokenHeadGravity; // Apply reduced gravity
   brokenHeadRotation += 5.0f; // Even slower rotation for better visibility

    // Check if the head has hit the ground
    if (brokenHeadY <= 0.5f) { // Ground level with small offset
       brokenHeadY = 0.5f; // Set to ground level
       brokenHeadVelocityY = -brokenHeadVelocityY * 0.4f; // Slightly more bouncy

        // If the bounce is very small, stop the animation
        if (abs(brokenHeadVelocityY) < 0.01f) { // Reduced threshold for more bounces
           brokenHeadLanded = true;
           // Removed collision effect here
        }
    }
}

// Camera zooms back out
cameraZ += (originalCameraZ - targetCameraZ) * 0.02f;
if (cameraZ >= originalCameraZ) {
   cameraZ = originalCameraZ;
        }

        // Set animation complete when everything is done
        if (weaponsReturned && brokenHeadLanded) {
           animationComplete = true;
           finalDialogueDelay = 0; // Start counting for final dialogue
        }
    }

    // Update camera zoom during initial clash
    if (!weaponsRetreating && cameraZ > targetCameraZ) {
       cameraZ -= 0.05f;
       if (cameraZ < targetCameraZ) {
          cameraZ = targetCameraZ;
       }
    }
}

// Update collision effect
if (collisionEffect.active) {
   collisionEffect.update();
}

// Handle final dialogue delay after animation completes
if (animationComplete && !showGuardianFinalDialogue) {
   finalDialogueDelay++;
   // Show final dialogue after 1 second (≈ 62 frames at 16ms per frame)
    if (finalDialogueDelay >= 62) {
       showGuardianFinalDialogue = true;
    }
}

// Start spear throw animation after showing final dialogue for a moment
if (showGuardianFinalDialogue && !spearThrowStarted) {
   static int dialogueDisplayTime = 0;
   dialogueDisplayTime++;

    // Start throw after showing dialogue for 2 seconds
    if (dialogueDisplayTime >= 125) { // 2 seconds at 16ms per frame
       spearThrowStarted = true;
       liangMoving = true; // Start Liang's movement
       showGuardianFinalDialogue = false; // Hide dialogue during action
    }
}

// Handle spear throw and Liang's movement animation
if (spearThrowStarted && !guardianKnockedOff) {
   if (liangMoving) {
      // Move Liang towards guardian
      liangPositionX += liangMoveSpeed;

        // Calculate distance to guardian
        float distanceToGuardian = abs(liangPositionX - guardianPositionX);

        // When Liang gets close enough to guardian
        if (distanceToGuardian <= 2.0f) {
           finalImpact = true;
           guardianKnockedOff = true;
           // Initialize knockoff velocities with lower values
           guardianXVelocity = 0.05f; // Reduced initial X velocity (was 0.3f)
           guardianYVelocity = 0.05f; // Reduced initial Y velocity (was 0.4f)
           // Play sound effect for guardian being hit and flying away
           PlaySound(TEXT("fly away"), NULL, SND_ALIAS | SND_ASYNC);
        }
    }
}

// Handle guardian being knocked off screen
if (guardianKnockedOff) {
   // Apply acceleration to velocities
   guardianXVelocity += guardianXAccel;
   guardianYVelocity += guardianYAccel;

    // Update guardian's position with current velocities
    guardianPositionX += guardianXVelocity;
    guardianPositionY += guardianYVelocity;
     // Update dialogue position to follow guardian
     dialogueX = guardianPositionX;
     dialogueY = guardianPositionY + 1.5f; // Keep it above the guardian
     dialogueZ = 0.0f;

     // Increase rotation for spinning effect
     guardianRotation += 5.0f; // Increased rotation speed

     // Check if guardian is off screen (you can adjust these values)
     if (guardianPositionX > 15.0f || guardianPositionY > 15.0f) {
        if (!startColorTransition) {
           transitionDelay++;
           // Start transition after about 1 second (60 frames at 16ms per frame)
           if (transitionDelay >= 60) {
              startColorTransition = true;
           }
        }
     }

     // If transition should start
     if (startColorTransition) {
        // Calculate transition over 3 seconds (about 180 frames at 16ms per frame)
        float transitionSpeed = 1.0f / 180.0f;
        transitionProgress = fmin(transitionProgress + transitionSpeed, 1.0f);

       // Interpolate colors using transitionProgress
       // Background color
       bgColorR = (1.0f - transitionProgress) * 0.5f + transitionProgress * TARGET_R;
       bgColorG = (1.0f - transitionProgress) * 0.1f + transitionProgress * TARGET_G;
       bgColorB = (1.0f - transitionProgress) * 0.1f + transitionProgress * TARGET_B;

      // Sky top color
      skyTopR = (1.0f - transitionProgress) * 0.5f + transitionProgress * TARGET_SKY_TOP_R;
      skyTopG = (1.0f - transitionProgress) * 0.1f + transitionProgress *
TARGET_SKY_TOP_G;
      skyTopB = (1.0f - transitionProgress) * 0.1f + transitionProgress * TARGET_SKY_TOP_B;
      // Sky bottom color
      skyBottomR = (1.0f - transitionProgress) * 0.3f + transitionProgress *
TARGET_SKY_BOTTOM_R;
      skyBottomG = (1.0f - transitionProgress) * 0.05f + transitionProgress *
TARGET_SKY_BOTTOM_G;
      skyBottomB = (1.0f - transitionProgress) * 0.05f + transitionProgress *
TARGET_SKY_BOTTOM_B;
    }

         if (finalImpact) {
            if (liangPositionX > -5.0f) {
               liangPositionX -= liangMoveSpeed * 0.5f;
            }
         }
     }

     // Update wheel rotation and hover animations
     wheelAngle += 3.0f;
     if (wheelAngle > 360.0f) wheelAngle -= 360.0f;

     hoverAngle += 2.0f;
     if (hoverAngle > 360.0f) hoverAngle -= 360.0f;
     hoverOffset = sin(hoverAngle * M_PI / 180.0f) * 0.3f;

     glutPostRedisplay();
     if (!phase2Started)          // ← wrap the re-arm
         glutTimerFunc(16, updateAnimations, 0);

 }

 // Function to draw just the trident base stick (without prongs)
 void drawTridentStick()
 {
    glPushMatrix();
    GLUquadric* q = gluNewQuadric();
     /*                         ──                              shaft   only
  // ───────────────────────────────────────
──── */
     glColor3f(0.30f, 0.10f, 0.40f);     // dark-purple shaft
     gluCylinder(q, 0.12, 0.12, 3.5, 20, 4);

     /* decorative collar where prongs would have been */
     glPushMatrix();
     glColor3f(0.60f, 0.10f, 0.80f); // brighter purple ring
     glTranslatef(0.0f, 0.0f, 3.45f);
     gluCylinder(q, 0.14, 0.14, 0.15, 20, 2);
     glPopMatrix();

     gluDeleteQuadric(q);
     glPopMatrix();
 }

} //namespace P1

//
  // ───────────────────────────────────────
  // ─────────────────────────────────────
// MAIN
//
  // ───────────────────────────────────────
  // ─────────────────────────────────────
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1280, 760);
  glutCreateWindow("Liang vs Evil Guardian");

 P1::init();
 glutDisplayFunc(P1::display);
 glutReshapeFunc(P1::reshape);
 glutTimerFunc(0, P1::updateAnimations, 0);
    glutKeyboardFunc(P1::keyboard);




    glutMainLoop();
    return 0;
}
