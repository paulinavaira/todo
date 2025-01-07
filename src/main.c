#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_BOXES 10
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    float xPercent, yPercent;
    float widthPercent, heightPercent;
    float x, y;
    float width, height;
    bool state;
    bool selected;
    char text[100];
} Box;

Box boxes[MAX_BOXES];
int boxCount = 0;
int selectedBox = -1;
GLFWwindow* window;

void updateBoxDimensions(Box* box, int windowWidth, int windowHeight) {
    box->x = (box->xPercent / 100.0f) * windowWidth;
    box->y = (box->yPercent / 100.0f) * windowHeight;
    box->width = (box->widthPercent / 100.0f) * windowWidth;
    box->height = (box->heightPercent / 100.0f) * windowHeight;
}

void createBox(float xPercent, float yPercent, float widthPercent, float heightPercent, const char* text) {
    if (boxCount >= MAX_BOXES) return;

    Box* box = &boxes[boxCount];
    
    // initial values
    box->xPercent = xPercent;
    box->yPercent = yPercent;
    box->widthPercent = widthPercent;
    box->heightPercent = heightPercent;
    
    box->state = false;
    box->selected = false;
    snprintf(box->text, sizeof(box->text), "%s", text);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    updateBoxDimensions(box, windowWidth, windowHeight);

    boxCount++;
}

void drawBox(const Box* box) {

    glColor3f(0.1f, 0.1f, 0.1f);

    // box
    glBegin(GL_QUADS);
    glVertex2f(box->x, box->y);
    glVertex2f(box->x + box->width, box->y);
    glVertex2f(box->x + box->width, box->y + box->height);
    glVertex2f(box->x, box->y + box->height);
    glEnd();

    // border
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(box->x, box->y);
    glVertex2f(box->x + box->width, box->y);
    glVertex2f(box->x + box->width, box->y + box->height);
    glVertex2f(box->x, box->y + box->height);
    glEnd();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    
    for (int i = 0; i < boxCount; i++) {
        updateBoxDimensions(&boxes[i], width, height);
    }
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Todo List", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    createBox(0, 0, 100.0f, 5.0f, "Todo List");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < boxCount; i++) {
            drawBox(&boxes[i]);
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
