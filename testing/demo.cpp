#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

// Window dimensions
const int WIDTH = 1200;
const int HEIGHT = 800;

// Camera and rotation variables
float cameraAngleX = 20.0f;
float cameraAngleY = 0.0f;
float cameraDistance = 50.0f;

// Animation variables
float sunRotation = 0.0f;
float mercuryOrbit = 0.0f;
float venusOrbit = 0.0f;
float earthOrbit = 0.0f;
float moonOrbit = 0.0f;
float marsOrbit = 0.0f;
float jupiterOrbit = 0.0f;
float saturnOrbit = 0.0f;
float saturnRingRotation = 0.0f;

// Stars for background
struct Star {
    float x, y, z;
    float brightness;
};
Star stars[1000];

// Mouse control
double lastMouseX = WIDTH / 2.0;
double lastMouseY = HEIGHT / 2.0;
bool firstMouse = true;
bool mousePressed = false;

// FPS tracking
double lastTime = 0.0;
int frameCount = 0;

void initStars() {
    srand(time(NULL));
    for (int i = 0; i < 1000; i++) {
        stars[i].x = (rand() % 2000 - 1000) / 10.0f;
        stars[i].y = (rand() % 2000 - 1000) / 10.0f;
        stars[i].z = (rand() % 2000 - 1000) / 10.0f;
        stars[i].brightness = (rand() % 100) / 100.0f;
    }
}

void drawStars() {
    glDisable(GL_LIGHTING);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 1000; i++) {
        glColor3f(stars[i].brightness, stars[i].brightness, stars[i].brightness);
        glVertex3f(stars[i].x, stars[i].y, stars[i].z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawSphere(float radius, float r, float g, float b, int slices, int stacks) {
    GLfloat mat_ambient[] = { r * 0.3f, g * 0.3f, b * 0.3f, 1.0f };
    GLfloat mat_diffuse[] = { r, g, b, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, radius, slices, stacks);
    gluDeleteQuadric(quad);
}

void drawRing(float innerRadius, float outerRadius, float r, float g, float b) {
    glDisable(GL_LIGHTING);
    glColor4f(r, g, b, 0.6f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * M_PI * i / 100.0f;
        float x = cos(angle);
        float z = sin(angle);
        
        glVertex3f(x * innerRadius, 0, z * innerRadius);
        glVertex3f(x * outerRadius, 0, z * outerRadius);
    }
    glEnd();
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void drawOrbit(float radius) {
    glDisable(GL_LIGHTING);
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        float angle = 2.0f * M_PI * i / 100.0f;
        glVertex3f(radius * cos(angle), 0, radius * sin(angle));
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawSun() {
    GLfloat mat_emission[] = { 1.0f, 0.8f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    
    glPushMatrix();
    glRotatef(sunRotation, 0, 1, 0);
    drawSphere(3.0f, 1.0f, 0.9f, 0.0f, 40, 40);
    glPopMatrix();
    
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
}

void drawMercury() {
    drawOrbit(8.0f);
    glPushMatrix();
    glRotatef(mercuryOrbit, 0, 1, 0);
    glTranslatef(8.0f, 0, 0);
    drawSphere(0.4f, 0.7f, 0.7f, 0.7f, 20, 20);
    glPopMatrix();
}

void drawVenus() {
    drawOrbit(11.0f);
    glPushMatrix();
    glRotatef(venusOrbit, 0, 1, 0);
    glTranslatef(11.0f, 0, 0);
    drawSphere(0.9f, 0.9f, 0.7f, 0.3f, 30, 30);
    glPopMatrix();
}

void drawEarth() {
    drawOrbit(15.0f);
    glPushMatrix();
    glRotatef(earthOrbit, 0, 1, 0);
    glTranslatef(15.0f, 0, 0);
    
    glPushMatrix();
    glRotatef(earthOrbit * 365.0f, 0, 1, 0);
    drawSphere(1.0f, 0.2f, 0.4f, 0.8f, 30, 30);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(moonOrbit, 0, 1, 0);
    glTranslatef(2.0f, 0, 0);
    drawSphere(0.27f, 0.8f, 0.8f, 0.8f, 15, 15);
    glPopMatrix();
    
    glPopMatrix();
}

void drawMars() {
    drawOrbit(20.0f);
    glPushMatrix();
    glRotatef(marsOrbit, 0, 1, 0);
    glTranslatef(20.0f, 0, 0);
    drawSphere(0.5f, 0.9f, 0.3f, 0.1f, 20, 20);
    glPopMatrix();
}

void drawJupiter() {
    drawOrbit(28.0f);
    glPushMatrix();
    glRotatef(jupiterOrbit, 0, 1, 0);
    glTranslatef(28.0f, 0, 0);
    
    glPushMatrix();
    glRotatef(jupiterOrbit * 2.4f, 0, 1, 0);
    drawSphere(2.2f, 0.8f, 0.6f, 0.4f, 40, 40);
    glPopMatrix();
    
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(moonOrbit * (i + 1) * 30.0f + i * 90.0f, 0, 1, 0);
        glTranslatef(3.0f + i * 0.5f, 0, 0);
        drawSphere(0.15f, 0.9f, 0.9f, 0.7f, 10, 10);
        glPopMatrix();
    }
    
    glPopMatrix();
}

void drawSaturn() {
    drawOrbit(37.0f);
    glPushMatrix();
    glRotatef(saturnOrbit, 0, 1, 0);
    glTranslatef(37.0f, 0, 0);
    
    glPushMatrix();
    glRotatef(saturnRingRotation, 0, 1, 0);
    drawSphere(1.8f, 0.9f, 0.8f, 0.5f, 40, 40);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(20, 1, 0, 0);
    glRotatef(saturnRingRotation * 0.5f, 0, 1, 0);
    drawRing(2.2f, 4.0f, 0.8f, 0.7f, 0.5f);
    drawRing(4.2f, 5.0f, 0.7f, 0.6f, 0.4f);
    glPopMatrix();
    
    glPopMatrix();
}

void updateAnimation(double deltaTime) {
    float speed = deltaTime * 60.0f; // Normalize to 60 FPS equivalent
    
    sunRotation += 0.5f * speed;
    mercuryOrbit += 4.09f * speed;
    venusOrbit += 1.62f * speed;
    earthOrbit += 1.0f * speed;
    moonOrbit += 13.0f * speed;
    marsOrbit += 0.53f * speed;
    jupiterOrbit += 0.08f * speed;
    saturnOrbit += 0.03f * speed;
    saturnRingRotation += 0.5f * speed;
    
    if (sunRotation > 360) sunRotation -= 360;
    if (mercuryOrbit > 360) mercuryOrbit -= 360;
    if (venusOrbit > 360) venusOrbit -= 360;
    if (earthOrbit > 360) earthOrbit -= 360;
    if (moonOrbit > 360) moonOrbit -= 360;
    if (marsOrbit > 360) marsOrbit -= 360;
    if (jupiterOrbit > 360) jupiterOrbit -= 360;
    if (saturnOrbit > 360) saturnOrbit -= 360;
    if (saturnRingRotation > 360) saturnRingRotation -= 360;
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glTranslatef(0.0f, 0.0f, -cameraDistance);
    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);
    
    drawStars();
    drawSun();
    drawMercury();
    drawVenus();
    drawEarth();
    drawMars();
    drawJupiter();
    drawSaturn();
}

void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 0.9f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    
    initStars();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else if (action == GLFW_RELEASE) {
            mousePressed = false;
            firstMouse = true;
        }
    }
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        if (firstMouse) {
            lastMouseX = xpos;
            lastMouseY = ypos;
            firstMouse = false;
        }
        
        float xoffset = xpos - lastMouseX;
        float yoffset = lastMouseY - ypos;
        
        lastMouseX = xpos;
        lastMouseY = ypos;
        
        cameraAngleY += xoffset * 0.5f;
        cameraAngleX += yoffset * 0.5f;
        
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraDistance -= yoffset * 2.0f;
    if (cameraDistance < 10.0f) cameraDistance = 10.0f;
    if (cameraDistance > 100.0f) cameraDistance = 100.0f;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch(key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            case GLFW_KEY_EQUAL: // + key
            case GLFW_KEY_KP_ADD:
                cameraDistance -= 2.0f;
                if (cameraDistance < 10.0f) cameraDistance = 10.0f;
                break;
            case GLFW_KEY_MINUS:
            case GLFW_KEY_KP_SUBTRACT:
                cameraDistance += 2.0f;
                if (cameraDistance > 100.0f) cameraDistance = 100.0f;
                break;
            case GLFW_KEY_R:
                cameraAngleX = 20.0f;
                cameraAngleY = 0.0f;
                cameraDistance = 50.0f;
                break;
        }
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    if (height == 0) height = 1;
    float ratio = (float)width / (float)height;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ratio, 0.1f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
}

void showFPS(GLFWwindow* window) {
    double currentTime = glfwGetTime();
    frameCount++;
    
    if (currentTime - lastTime >= 1.0) {
        double fps = frameCount / (currentTime - lastTime);
        std::cout << "FPS: " << fps << std::endl;
        
        frameCount = 0;
        lastTime = currentTime;
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, 
        "🌌 GLFW Solar System - Drag to Rotate | Scroll to Zoom | R to Reset | ESC to Exit", 
        NULL, NULL);
    
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    // Initialize OpenGL
    initOpenGL();
    
    // Set initial viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    framebufferSizeCallback(window, width, height);
    
    std::cout << "Controls:\n"
              << "- Drag mouse to rotate camera\n"
              << "- Scroll to zoom in/out\n"
              << "- +/- keys to zoom\n"
              << "- R to reset camera\n"
              << "- ESC to exit\n\n";
    
    lastTime = glfwGetTime();
    double lastFrameTime = glfwGetTime();
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        
        updateAnimation(deltaTime);
        render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        showFPS(window);
    }
    
    glfwTerminate();
    return 0;
}