#pragma once
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include <GLFW/glfw3.h>


class Window{

    GLFWwindow* window;
    GLuint vertex_id;
    ShaderProgram m_shader;
    int h, w;
    GLuint texId;

public:
    Window(int h, int w);
    ~Window(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void draw();


};
