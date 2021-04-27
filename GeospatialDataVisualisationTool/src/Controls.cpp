#include "Controls.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "State.h"


void Controls::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    
    void* s = glfwGetWindowUserPointer(window);
    State* state = (State*)s;
    if (mouseX > sidebarW && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        state->mousePressed = true;
        glfwGetCursorPos(window, &(state->mouseDownX), &(state->mouseDownY));
        state->mouseDownTranslation.x = state->translation.x;
        state->mouseDownTranslation.y = state->translation.y;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        state->mousePressed = false;
    }
}


void Controls::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
    void* s = glfwGetWindowUserPointer(window);
    State* state = (State*)s;
    if (state->mousePressed) {
        int windowW, windowH;
        glfwGetWindowSize(window, &windowW, &windowH);
        
        state->translation.x = fmax(fmin(state->dx * (xPos - state->mouseDownX) / ((float)windowW - sidebarW) / state->zoom + state->mouseDownTranslation.x, state->maxTranslationX), state->minTranslationX);
        state->translation.y = fmax(fmin(state->dy * (-yPos + state->mouseDownY) / (float)windowH / state->zoom + state->mouseDownTranslation.y, state->maxTranslationY), state->minTranslationY);
    }
}


void Controls::scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    if (mouseX <= sidebarW)
        return;
    
    void* s = glfwGetWindowUserPointer(window);
    State* state = (State*)s;
    state->zoom = fmax(fmin(state->zoom + 0.1*state->zoom*yOffset, state->maxZoom), state->minZoom);
}
