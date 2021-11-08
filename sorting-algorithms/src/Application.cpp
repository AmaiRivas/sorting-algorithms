#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <array>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"


const unsigned int scale = 4;
const unsigned int elementCount = 200 / scale;
float heightArray[elementCount];
unsigned int counter = 0;
unsigned int limit = elementCount - 1;
unsigned int index = 0;
bool finished = false;


void CreateArray()
{
    for (unsigned int x = 0; x < elementCount; x++)
    {
        heightArray[x] = static_cast<float>(x);
    }
}

void swap(float* current, float* next)
{
    float temp = *current;
    *current = *next;
    *next = temp;
}

static void DrawRectangles(unsigned int indices[], float positions[], Renderer& renderer, Shader& shader, IndexBuffer& ib, VertexArray& va, VertexBufferLayout& layout)
{
    std::cout << "Counter: " << counter << std::endl;
    if (counter == limit)
    {
        counter = 0;
        limit--;
    }

    //MVP matrix
    glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    float xShift = 5.25f * scale;
    index = 0;

    for (float height : heightArray)
    {
        //Changing object's height
        positions[5] = height * 5.3f * scale;
        positions[7] = height * 5.3f * scale;

        //Setting up the object
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        va.AddBuffer(vb, layout);

        //Color for correct (from this palette: https://coolors.co/001219-005f73-0a9396-94d2bd-e9d8a6-ee9b00-ca6702-bb3e03-ae2012-9b2226 )
        
        shader.Bind();
        if (height == heightArray[counter+1] || height == heightArray[counter+2])
        {
            //Color of "evaluating"
            shader.SetUniform4f("u_Color", 0.733, 0.243, 0.011, 1.0f);
        }
        else if (index >= limit+1)
        {
            //Color of "correct"
            shader.SetUniform4f("u_Color", 0.2, 0.901, 0.337, 1.0f);
        }
        else
        {
            //Color of "not touched"
            shader.SetUniform4f("u_Color", 0.933f, 0.607f, 0.0f, 1.0f);
        }
          

        //Translation
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xShift, 10, 0));
        glm::mat4 mvp = proj * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.Unbind();

        renderer.Draw(va, ib, shader);


        xShift += 9.5 * scale;

        //Bubble sort
        if (!finished)
        {
            if (heightArray[counter] > heightArray[counter + 1])
            {
                if (limit == 0 && counter == elementCount - 1)
                {
                    finished = true;
                    return;
                }
                swap(&heightArray[counter], &heightArray[counter + 1]);
            }
        }

        index++;
    }
}

static void DrawBorders(unsigned int indices[], Renderer& renderer, Shader& shader)
{
    //MVP matrix
    glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    {
        //Border 1 and 3 (left and right)
        float positionsBorderLR[] = {
            0.0f,   0.0f,
            10.0f,  0.0f,
            10.0f,  1080.0f,
            0.0f,   1080.0f,
        };

        //Setting up the object
        VertexArray va;
        VertexBuffer vb(positionsBorderLR, 4 * 2 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        //Color
        shader.SetUniform4f("u_Color", 0, 0.070, 0.098, 1.0f);

        //Translation 1 
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 mvp = proj * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(va, ib, shader);

        //Translation 3
        model = glm::translate(glm::mat4(1.0f), glm::vec3(1910, 0, 0));
        mvp = proj * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(va, ib, shader);
    }

   //Border 2 and 4 (up and down)

    float positionsBorderUD[] = {
    0.0f,   0.0f,
    1920.0f,  0.0f,
    1920.0f,  10.0f,
    0.0f,   10.0f,
    };

    //Setting up the object
    VertexArray va;
    VertexBuffer vb(positionsBorderUD, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, 6);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();

    //Color
    shader.SetUniform4f("u_Color", 0, 0.070, 0.098, 1.0f);

    //Translation 1
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 mvp = proj * view * model;
    shader.SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(va, ib, shader);

    //Translation 2
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1070, 0));
    mvp = proj * view * model;
    shader.SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(va, ib, shader);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Bubble sort visualization", NULL, NULL);
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
        0.0f * scale,   0.0f * scale,
        9.5f * scale,  0.0f * scale,
        9.5f * scale,  0.0f * scale,
        0.0f * scale,   0.0f * scale,
    };

    //Starting initialization
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    //Shader
    Shader shader("res/shaders/Basic.shader");

    //Renderer
    Renderer renderer;

    //Array creation
    CreateArray();
    int calculation = sizeof(heightArray) / sizeof(heightArray[0]);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(heightArray, heightArray + calculation, std::default_random_engine(seed));

    while (glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS)
    {
        renderer.Clear();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        //Test
        VertexArray va;
        IndexBuffer ib(indices, 6);
        VertexBufferLayout layout;
        layout.Push<float>(2);

        DrawRectangles(indices, positions, renderer, shader, ib, va, layout);
        DrawBorders(indices, renderer, shader);
        counter++;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}

