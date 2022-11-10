//
// Created by GPowers on 11/3/2022.
//

#include "Renderer.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "Conway.hpp"


void cursorToGrid(double &x_mouse, double &y_mouse);

void cursorToGrid(double &x_mouse, double &y_mouse, int size) {
    x_mouse = (x_mouse + 1) * (size/2.0f);
    y_mouse = (y_mouse + 1) * (size/2.0f);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

double x_mouse, y_mouse;
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    x_mouse = xpos;
    y_mouse = ypos;
    cursorToGrid(x_mouse, y_mouse, 100);
}



const char* vertex_source = R"glsl(
    #version 150 core
    in vec2 position;
    uniform mat4 proj;
    uniform mat4 trans;
    void main()
    {
        gl_Position = proj * trans * vec4(position, 0.0, 1.0);
    }
	)glsl";

const char* fragment_source = R"glsl(
    #version 150 core
    uniform vec4 incolor;
	out vec4 color;
    void main()
    {
        color = incolor;
    }
	)glsl";


Renderer::Renderer() {
    if(!glfwInit())
    {
        std::cout << "GLFW failed to initialize" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(400, 400, "Conway's Game", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glViewport(0,0,400,400);


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);


    b_shader.Compile(vertex_source, fragment_source);
    b_shader.Use();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    float vertices[] =
            {
                    0,0,
                    1,0,
                    0,-1,
                    0,-1,
                    1,0,
                    1,-1
            };
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(b_shader.ID,"position"), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    b_shader.SetUniform4f("incolor", {1.0, 1.0, 1.0, 1.0});

    glm::mat4 proj = glm::ortho<float>(0,100,-1,99, -1, 1);
    b_shader.SetUniformMatrix4fv("proj", proj);
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        update();
        draw();
        glfwSwapBuffers(window);
    }
}

void Renderer::drawTile(int x, int y)
{
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3{x,y,0.0});
    b_shader.SetUniformMatrix4fv("trans", trans);
    glDrawArrays(GL_TRIANGLES, 0 , 6);
}

void Renderer::changeScale(float minX, float maxX, float minY, float maxY) {
    glm::mat4 proj = glm::ortho<float>(minX,maxY,minY,maxY, -1, 1);
    b_shader.SetUniformMatrix4fv("proj", proj);
}

void Renderer::draw() {
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            if(c.get_table()[i][j])
            {
                drawTile(i,j);
            }
        }
    }
}

void Renderer::update() {
    double ct = glfwGetTime()-lt;
    c.get_table()[(int)x_mouse][(int)y_mouse] = true;
    if(ct > 0.05)
    {
        c.tick();
        lt = glfwGetTime();
    }


}
