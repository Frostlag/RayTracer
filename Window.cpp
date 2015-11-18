#include <string>
#include "Window.hpp"
#include "cs488-framework/GlErrorCheck.hpp"


using namespace std;

Window::Window(int h, int w):h(h),w(w){
    if (glfwInit() == GL_FALSE)
        throw string("Not able to create window");

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(h, w,
        "A5", NULL, NULL);

    if (window == NULL)
        throw string("Not able to create window");

    glfwMakeContextCurrent(window);

    gl3wInit();

    glClearColor( 0, 0, 0, 1.0 );

    m_shader.generateProgramObject();
    m_shader.attachVertexShader("Assets/vertex.s");
    m_shader.attachFragmentShader("Assets/fragment.s");
    m_shader.link();

    GLfloat vertex_buffer_data[] = {
     -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     -1.0f,  1.0f, 0.0f,
     -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    };

    CHECK_GL_ERRORS;

    glGenVertexArrays(1, &vertex_id);
    glBindVertexArray(vertex_id);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    GLuint posAttrib = m_shader.getAttribLocation( "position" );
    glEnableVertexAttribArray( posAttrib );
    glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
    CHECK_GL_ERRORS;

    unsigned char* data = new unsigned char [h*w*4];
    for ( int i = 0; i < h * w * 4; i++){
        data[i] = 0xFF;
    }
    texId = m_shader.getUniformLocation("renderedTexture");
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    CHECK_GL_ERRORS;

    delete [] data;

}

void Window::draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,w,h);
    m_shader.enable();
        glBindVertexArray(vertex_id);
        glDrawArrays(GL_TRIANGLES, 0, 2*3);
    m_shader.disable();
    glfwSwapBuffers(window);
}
