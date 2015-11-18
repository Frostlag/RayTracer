#pragma once
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include <GLFW/glfw3.h>
#include "Image.hpp"

class Window{

    GLFWwindow* window;
    GLuint vertex_id;
    ShaderProgram m_shader;
    int h, w;
    GLuint texId, texCordsId;
    unsigned char* data;
    char test;
    bool closed;
    Image& image;

    void changeTexture();
public:
    Window(int h, int w, Image& image);
    ~Window(){
        glfwTerminate();
        delete [] data;
    }
    void tick();
    void draw();
    bool isClosed(){return closed;}


};
