#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib> 
#include <ctime>   

float leftDoorPos = 0.0f;
float rightDoorPos = 0.0f;
float doorSpeed = 0.05f;
float maxDoorOpen = 1.9f;

bool isLeftOpening = false;
bool isLeftClosing = false;
bool isRightOpening = false;
bool isRightClosing = false;

float rotateX = 0.0f;
float rotateY = 0.0f;
int lastMouseX = 0, lastMouseY = 0;
bool mousePressed = false;

GLfloat lightPosition[] = { 3.0f, 2.0f, 5.0f, 1.0f };
GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat globalDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };

float shelfY[5];
float shelfTopSurfaceY[5];

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_NORMALIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, globalDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, globalDiffuse);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    srand(time(0));
    
    float totalFrameHeight = 4.5f;
    float halfFrameHeight = totalFrameHeight / 2.0f;
    float shelfThickness = 0.1f;
    float interiorHeight = totalFrameHeight - 0.2f;
    float gapHeight = (interiorHeight - 5 * shelfThickness) / 4.0f;

    shelfY[0] = halfFrameHeight - 0.1f - (shelfThickness / 2.0f);
    for (int i = 1; i < 5; ++i) {
        shelfY[i] = shelfY[i - 1] - (shelfThickness + gapHeight);
    }
    for (int i = 0; i < 5; i++) {
        shelfTopSurfaceY[i] = shelfY[i] + (shelfThickness / 2.0f);
    }
}

void drawCube(float size) {
    glutSolidCube(size);
}

void drawShelfFrame() {
    glColor3f(0.96f, 0.96f, 0.86f);

    float totalFrameHeight = 4.5f;
    float halfFrameHeight = totalFrameHeight / 2.0f;

    glPushMatrix();
    glTranslatef(0.0f, -halfFrameHeight, 0.0f);
    glScalef(4.0f, 0.2f, 2.0f);
    drawCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, halfFrameHeight, 0.0f);
    glScalef(4.0f, 0.2f, 2.0f);
    drawCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.0f, 0.0f, 0.0f);
    glScalef(0.2f, totalFrameHeight, 2.0f);
    drawCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.0f, 0.0f, 0.0f);
    glScalef(0.2f, totalFrameHeight, 2.0f);
    drawCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.9f);
    glScalef(3.8f, totalFrameHeight, 0.2f);
    drawCube(1.0f);
    glPopMatrix();

    glColor3f(0.96f, 0.96f, 0.86f);
    float shelfThickness = 0.1f;
    
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glTranslatef(0.0f, shelfY[i], 0.0f);
        glScalef(3.8f, shelfThickness, 1.8f);
        drawCube(1.0f);
        glPopMatrix();
    }
}

void drawSingleDoorPanel(float xOffset, float zDepth, float doorHandleX, float doorHandleZ) {
    float doorWidth = 1.9f;
    float doorHeight = 4.1f;
    float doorDepth = 0.05f;
    float frameThickness = 0.2f;

    glColor3f(0.96f, 0.96f, 0.86f);

    glPushMatrix();
    glTranslatef(xOffset, 0.0f, zDepth);

    glPushMatrix();
    glTranslatef(0.0f, (doorHeight / 2.0f) - (frameThickness / 2.0f), 0.0f);
    glScalef(doorWidth, frameThickness, doorDepth);
    drawCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -(doorHeight / 2.0f) + (frameThickness / 2.0f), 0.0f);
    glScalef(doorWidth, frameThickness, doorDepth);
    drawCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(doorWidth / 2.0f) + (frameThickness / 2.0f), 0.0f, 0.0f);
    glScalef(frameThickness, doorHeight - (2 * frameThickness), doorDepth);
    drawCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((doorWidth / 2.0f) - (frameThickness / 2.0f), 0.0f, 0.0f);
    glScalef(frameThickness, doorHeight - (2 * frameThickness), doorDepth);
    drawCube(1.0f);
    glPopMatrix();

    glColor4f(0.8f, 0.9f, 1.0f, 0.3f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.01f);
    glScalef(doorWidth - (2 * frameThickness), doorHeight - (2 * frameThickness), doorDepth * 0.5f);
    drawCube(1.0f);
    glPopMatrix();

    float cornerSize = 0.2f;
    float frameHalfW = (doorWidth / 2.0f) - frameThickness;
    float frameHalfH = (doorHeight / 2.0f) - frameThickness;
    float triangleZ = -doorDepth / 2.0f - 0.01f;

    glDisable(GL_LIGHTING);
    glColor3f(0.96f, 0.96f, 0.86f);

    glBegin(GL_TRIANGLES);
    glVertex3f(-frameHalfW - cornerSize, frameHalfH, triangleZ);
    glVertex3f(-frameHalfW, frameHalfH, triangleZ);
    glVertex3f(-frameHalfW, frameHalfH + cornerSize, triangleZ);

    glVertex3f(frameHalfW, frameHalfH + cornerSize, triangleZ);
    glVertex3f(frameHalfW, frameHalfH, triangleZ);
    glVertex3f(frameHalfW + cornerSize, frameHalfH, triangleZ);

    glVertex3f(-frameHalfW, -frameHalfH - cornerSize, triangleZ);
    glVertex3f(-frameHalfW, -frameHalfH, triangleZ);
    glVertex3f(-frameHalfW - cornerSize, -frameHalfH, triangleZ);

    glVertex3f(frameHalfW + cornerSize, -frameHalfH, triangleZ);
    glVertex3f(frameHalfW, -frameHalfH, triangleZ);
    glVertex3f(frameHalfW, -frameHalfH - cornerSize, triangleZ);
    glEnd();

    glEnable(GL_LIGHTING);

    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(doorHandleX, 0.0f, doorHandleZ);
    glScalef(0.1f, 0.4f, 0.02f);
    drawCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

void drawSlidingDoors() {
    float doorDepth = 0.05f;
    float handleOffsetZ = doorDepth / 2.0f + 0.03f;

    drawSingleDoorPanel(
        -0.95f + leftDoorPos,
        0.9f,
        -0.8f,
        handleOffsetZ
    );

    drawSingleDoorPanel(
        0.95f - rightDoorPos,
        0.88f,
        0.8f,
        handleOffsetZ
    );
}

void drawItemsInside() {
    float bookHeight = 0.6f;
    float bookWidth = 0.075f;
    float bookDepth = 0.5f;
    float bookSpacing = 0.03f;
    float bookYTopSurface = shelfTopSurfaceY[2];
    float antiZFightOffset = 0.005f;
    float bookYCenter = bookYTopSurface + antiZFightOffset + (bookHeight / 2.0f);
    float startX = -1.8f + (bookWidth / 2.0f);

    glColor3f(0.9f, 0.1f, 0.1f);

    for (int i = 0; i < 10; i++) {
        glPushMatrix();
        glTranslatef(startX + i * (bookWidth + bookSpacing), bookYCenter, 0.0f);
        glScalef(bookWidth, bookHeight, bookDepth);
        drawCube(1.0f);
        glPopMatrix();
    }

    float paperYTopSurface = shelfTopSurfaceY[3];
    float paperStackXPosition = 1.0f;
    float basePaperThickness = 0.01f;
    int numPaperRects = 20;
    float paperZPosition = 0.3f;
    float paperGap = 0.01f;

    glColor3f(0.95f, 0.95f, 0.95f);

    static float paperRotations[20];
    static bool rotationsInitialized = false;
    
    if (!rotationsInitialized) {
        for (int i = 0; i < 20; ++i) {
            paperRotations[i] = (rand() % 21 - 10) / 2.0f;
        }
        rotationsInitialized = true;
    }

    for (int i = 0; i < numPaperRects; ++i) {
        glPushMatrix();
        float currentPaperYCenter = paperYTopSurface + (basePaperThickness / 2.0f) + (i * (basePaperThickness + paperGap));

        if (i == 0) {
            currentPaperYCenter += antiZFightOffset;
        }

        glTranslatef(paperStackXPosition, currentPaperYCenter, paperZPosition);
        glRotatef(paperRotations[i], 0.0f, 1.0f, 0.0f);
        glScalef(0.7f, basePaperThickness, 0.8f);
        drawCube(1.0f);
        glPopMatrix();
    }

    float boxYTopSurface = shelfTopSurfaceY[4];
    float boxHeight = 0.8f;
    float boxWidth = 0.15f;
    float boxDepth = 0.5f;
    float boxSpacing = 0.05f;
    float boxYCenter = boxYTopSurface + antiZFightOffset + (boxHeight / 2.0f);
    float boxStartX = -1.5f;

    glColor3f(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(boxStartX + i * (boxWidth + boxSpacing), boxYCenter, 0.0f);
        glScalef(boxWidth, boxHeight, boxDepth);
        drawCube(1.0f);
        glPopMatrix();
    }
}

void drawWall() {
    glDisable(GL_LIGHTING);
    glColor3f(0.8f, 0.7f, 0.6f);

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -1.1f);
    glScalef(20.0f, 20.0f, 0.1f);
    drawCube(1.0f);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void drawFloor() {
    float floorLevel = -2.25f;
    float floorSize = 20.0f;
    float tileSize = 1.0f;
    int numTiles = (int)(floorSize / tileSize);

    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, floorLevel, 0.0f);
    glScalef(floorSize, 0.02f, floorSize);
    drawCube(1.0f);
    glPopMatrix();

    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);

    for (int i = -numTiles / 2; i <= numTiles / 2; i++) {
        glVertex3f(i * tileSize, floorLevel + 0.02f, -floorSize / 2.0f);
        glVertex3f(i * tileSize, floorLevel + 0.02f, floorSize / 2.0f);
    }
    for (int i = -numTiles / 2; i <= numTiles / 2; i++) {
        glVertex3f(-floorSize / 2.0f, floorLevel + 0.02f, i * tileSize);
        glVertex3f(floorSize / 2.0f, floorLevel + 0.02f, i * tileSize);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

void drawLightSource() {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
    glutWireSphere(0.2, 10, 10);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 10.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    drawWall();
    drawFloor();
    drawLightSource();

    glPushMatrix();
    glTranslatef(0.0f, 0.121f, 0.0f);
    drawShelfFrame();
    drawItemsInside();
    drawSlidingDoors();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void updateAnimation(int value) {
    if (isLeftOpening) {
        leftDoorPos += doorSpeed;
        if (leftDoorPos >= maxDoorOpen) {
            leftDoorPos = maxDoorOpen;
            isLeftOpening = false;
        }
    }
    else if (isLeftClosing) {
        leftDoorPos -= doorSpeed;
        if (leftDoorPos <= 0.0f) {
            leftDoorPos = 0.0f;
            isLeftClosing = false;
        }
    }

    if (isRightOpening) {
        rightDoorPos += doorSpeed;
        if (rightDoorPos >= maxDoorOpen) {
            rightDoorPos = maxDoorOpen;
            isRightOpening = false;
        }
    }
    else if (isRightClosing) {
        rightDoorPos -= doorSpeed;
        if (rightDoorPos <= 0.0f) {
            rightDoorPos = 0.0f;
            isRightClosing = false;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateAnimation, 0);
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else {
            mousePressed = false;
        }
    }
}

void mouseMove(int x, int y) {
    if (!mousePressed) return;
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;
    rotateY += dx * 0.5f;
    rotateX += dy * 0.5f;
    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;

    case 'o':
    case 'O':
        if (leftDoorPos < 0.001f) { isLeftOpening = true; isLeftClosing = false; }
        else { isLeftOpening = false; isLeftClosing = true; }
        break;

    case 'p':
    case 'P':
        if (rightDoorPos < 0.001f) { isRightOpening = true; isRightClosing = false; }
        else { isRightOpening = false; isRightClosing = true; }
        break;

    case '1':
        globalAmbient[0] -= 0.05f; globalAmbient[1] -= 0.05f; globalAmbient[2] -= 0.05f;
        if (globalAmbient[0] < 0.0f) globalAmbient[0] = 0.0f;
        if (globalAmbient[1] < 0.0f) globalAmbient[1] = 0.0f;
        if (globalAmbient[2] < 0.0f) globalAmbient[2] = 0.0f;
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
        glutPostRedisplay();
        break;
    case '2':
        globalAmbient[0] += 0.05f; globalAmbient[1] += 0.05f; globalAmbient[2] += 0.05f;
        if (globalAmbient[0] > 1.0f) globalAmbient[0] = 1.0f;
        if (globalAmbient[1] > 1.0f) globalAmbient[1] = 1.0f;
        if (globalAmbient[2] > 1.0f) globalAmbient[2] = 1.0f;
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
        glutPostRedisplay();
        break;
    case '3':
        globalDiffuse[0] -= 0.05f; globalDiffuse[1] -= 0.05f; globalDiffuse[2] -= 0.05f;
        if (globalDiffuse[0] < 0.0f) globalDiffuse[0] = 0.0f;
        if (globalDiffuse[1] < 0.0f) globalDiffuse[1] = 0.0f;
        if (globalDiffuse[2] < 0.0f) globalDiffuse[2] = 0.0f;
        glLightfv(GL_LIGHT0, GL_DIFFUSE, globalDiffuse);
        glutPostRedisplay();
        break;
    case '4':
        globalDiffuse[0] += 0.05f; globalDiffuse[1] += 0.05f; globalDiffuse[2] += 0.05f;
        if (globalDiffuse[0] > 1.0f) globalDiffuse[0] = 1.0f;
        if (globalDiffuse[1] > 1.0f) globalDiffuse[1] = 1.0f;
        if (globalDiffuse[2] > 1.0f) globalDiffuse[2] = 1.0f;
        glLightfv(GL_LIGHT0, GL_DIFFUSE, globalDiffuse);
        glutPostRedisplay();
        break;

    case 'w':
        lightPosition[2] -= 0.2f;
        glutPostRedisplay();
        break;
    case 's':
        lightPosition[2] += 0.2f;
        glutPostRedisplay();
        break;
    case 'a':
        lightPosition[0] -= 0.2f;
        glutPostRedisplay();
        break;
    case 'd':
        lightPosition[0] += 0.2f;
        glutPostRedisplay();
        break;
    case 'q':
        lightPosition[1] += 0.2f;
        glutPostRedisplay();
        break;
    case 'e':
        lightPosition[1] -= 0.2f;
        glutPostRedisplay();
        break;

    default:
        break;
    }
}

void printInstructions() {
    printf("=== 3D Sliding Shelf Controls ===\n");
    printf("O: Toggle left door\n");
    printf("P: Toggle right door\n");
    printf("Mouse Drag: Rotate view\n");
    printf("--- Lighting Controls ---\n");
    printf("W/S: Move light Forward/Back\n");
    printf("A/D: Move light Left/Right\n");
    printf("Q/E: Move light Up/Down\n");
    printf("1/2: Decrease/Increase Ambient Light\n");
    printf("3/4: Decrease/Increase Diffuse Light\n");
    printf("ESC: Exit\n");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Shelf Demo - Real-time Lighting");

    printInstructions();
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(16, updateAnimation, 0);

    glutMainLoop();
    return 0;
}
