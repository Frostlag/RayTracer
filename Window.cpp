#include <string>
#include <iostream>
#include "Window.hpp"
#include "RenderThread.hpp"
#include "cs488-framework/GlErrorCheck.hpp"


using namespace std;

Window::Window(int h, int w, Image& image):h(h),w(w),image(image){
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
     -1.0f, -1.0f,
     1.0f, -1.0f,
     -1.0f,  1.0f,
     -1.0f,  1.0f,
     1.0f, -1.0f,
     1.0f,  1.0f,
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
    glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr );
    CHECK_GL_ERRORS;

    data = new unsigned char [h*w*3];
    for ( int i = 0; i < h * w * 3; i+=3){
        data[i] = 0x00;
        data[i + 1] = 0x00;
        data[i + 2] = 0x00;

    }
    texId = m_shader.getUniformLocation("renderedTexture");
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    CHECK_GL_ERRORS;

    test = 0x00;

    closed = false;
}

void Window::tick(){
    if (closed)
        return;
    glfwPollEvents();
    changeTexture();
    draw();
    if (glfwWindowShouldClose(window)){
        closed = true;
        glfwDestroyWindow(window);
    }
}

void Window::changeTexture(){

    list<Changed> changes;
    for (RenderThread *renderThread: RenderThread::Threads){
        changes.splice(changes.end(), renderThread->getChanges());
    }

    for (Changed change : changes){
        for (int i = 0; i < 3; i++){
            double newcomponent = image(change.x, change.y, i, 1);
            data[3 * (w * change.y + change.x) + i] = (unsigned char)(newcomponent * 255);
        }
        //cout << 3 * (w * change.y + change.x) + change.i << " " << (int)(newcomponent * 255) << endl;
    }

    texId = m_shader.getUniformLocation("renderedTexture");
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    CHECK_GL_ERRORS;
}

void Window::draw(){
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,w,h);
    CHECK_GL_ERRORS;

    m_shader.enable();
        glBindVertexArray(vertex_id);
        glDrawArrays(GL_TRIANGLES, 0, 2*3);
        CHECK_GL_ERRORS;
    m_shader.disable();

    glfwSwapBuffers(window);
}
