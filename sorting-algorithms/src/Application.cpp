#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"

static void DrawRectangles(unsigned int indices[], float height[], float positions[], Renderer& renderer, Shader& shader)
{

    //Setting up the color of the rectangles
    shader.Bind();
    //shader.SetUniform4f("u_Color", r, g, b, 1.0f);
    shader.Unbind();

    //MVP matrix
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

}

static void CreateHeightArray(float heightArray[])
{
    for (int x = 0; x < sizeof(heightArray); x++)
    {
        heightArray[x] = x;
    }
}

int main(void)
{
    float heightArray[30];
    CreateHeightArray(heightArray);
    unsigned int test = 40;


    std::cout << test * 1.0f << std::endl;

    //std::random_shuffle(height, height + 30);


    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Bubble sort visualization", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //This syncronizes the fps with your monitor refreshrate
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error when initializing glewInit()" << std::endl;

    //This prints the OpenGL version on the terminal
    std::cout << glGetString(GL_VERSION) << std::endl;

    //Positions
    float positions[] = {
        0.0f,   0.0f,
        10.0f,  0.0f,
        10.0f,  0.0f,
        0.0f,   0.0f,
    };

    //Starting initialization
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };



    Shader shader("res/shaders/Basic.shader");

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        //DrawRectangles(indices, renderer, shader);

        for (unsigned int i = 0; i < 30; i++)
        {

            //Changing position --------------------------------

            VertexArray va;
            VertexBuffer vb(positions, 4 * 2 * sizeof(float));

            VertexBufferLayout layout;
            layout.Push<float>(2);
            va.AddBuffer(vb, layout);

            IndexBuffer ib(indices, 6);

            va.Unbind();
            vb.Unbind();
            ib.Unbind();
            //_----------------------------------------

            //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xChange, 200, 0));
            //glm::mat4 mvp = proj * view * model;
            //shader.SetUniformMat4f("u_MVP", mvp);
            //renderer.Draw(va, ib, shader);
            //xChange += 15;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

