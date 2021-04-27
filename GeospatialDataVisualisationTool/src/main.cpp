#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Toolbar.h"
#include "Controls.h"
#include "DataLoader.h"
#include "RegionMappingLoader.h"


int main(void)
{
    GLFWwindow* window;
    State state;
    
    double sidebarW = Controls::sidebarW;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    
    /* Create a windowed mode window and its OpenGL context */
    int monitorX, monitorY, monitorWidth, monitorHeight;
    glfwGetMonitorWorkarea(primaryMonitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);
    float xScale, yScale;
    glfwGetMonitorContentScale(primaryMonitor, &xScale, &yScale);
    
    float aspectRatio = (state.max_y - state.min_y) / (state.max_x - state.min_x);
    int windowHeight = monitorHeight - 50;
    int windowWidth = windowHeight / aspectRatio;
    window = glfwCreateWindow(windowWidth+sidebarW, windowHeight, "Data Visualisation Tool", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;
    
    glfwSetWindowUserPointer(window, &state);
    glfwSetScrollCallback(window, Controls::scrollCallback);
    glfwSetMouseButtonCallback(window, Controls::mouseButtonCallback);
    glfwSetCursorPosCallback(window, Controls::cursorPositionCallback);
    
    DataLoader dataLoader;
    RegionMappingLoader mappingLoader("res/data/region_data.csv");
    Renderer renderer(state, mappingLoader);
    Toolbar toolbar(state, dataLoader, mappingLoader);
    
    glEnable(GL_SCISSOR_TEST);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui::StyleColorsLight();
    ImGui_ImplOpenGL3_Init("#version 150");
    
    ImFontConfig fontConfig;
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("res/fonts/roboto/RobotoMono-Regular.ttf", 14.0f, &fontConfig, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->Build();
    ImGuiStyle& imGuiStyle = ImGui::GetStyle();
    imGuiStyle.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        glViewport(0, 0, sidebarW*xScale, windowHeight*yScale);
        glScissor(0, 0, sidebarW*xScale, windowHeight*yScale);
        glClearColor(0.93f, 0.93f, 0.91f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        toolbar.draw();
        
        glViewport(sidebarW*xScale, 0, windowWidth*xScale, windowHeight*yScale);
        glScissor(sidebarW*xScale, 0, windowWidth*xScale, windowHeight*yScale);
        renderer.clear();
        
        renderer.draw();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
    return 0;
}
