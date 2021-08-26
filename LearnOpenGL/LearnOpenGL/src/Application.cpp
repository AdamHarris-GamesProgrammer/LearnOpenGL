#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"

GLFWwindow* _pWindow = nullptr;

void Initialize()
{
    glewInit();

    /* Initialize the library */
    if (!glfwInit())
        return;

    /* Create a windowed mode window and its OpenGL context */
    _pWindow = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!_pWindow)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(_pWindow);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "[ERROR]: With setting up GLEW" << std::endl;
    }
}


int main(void)
{
    Initialize();


    float position[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(_pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);


        /* Swap front and back buffers */
        glfwSwapBuffers(_pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


