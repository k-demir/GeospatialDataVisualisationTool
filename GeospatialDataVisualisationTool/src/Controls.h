#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Controls {
public:
    static constexpr double sidebarW = 500;
    
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
    static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};
