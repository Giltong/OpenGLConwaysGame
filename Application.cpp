//
// Created by GPowers on 11/3/2022.
//

#include "Application.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"


#include "Conway.hpp"


void cursorToGrid(int &xmouse, int &ymouse);



int x_mouse, y_mouse;
float w_width=400, w_height=400;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    w_width = width;
    w_height = height;
}

int size=200;
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
        x_mouse = xpos;
        y_mouse = ypos;
        cursorToGrid(x_mouse, y_mouse);
}

void cursorToGrid(int &xmouse, int &ymouse) {
    xmouse = xmouse/w_width * size;
    ymouse = size - ymouse/w_height * size;
    if(xmouse < 0) xmouse = 0;
    if(xmouse > size-1) xmouse = size-1;
    if(ymouse < 0) ymouse = 0;
    if(ymouse > size-1) ymouse = size-1;
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


Application::Application() {
    if(!glfwInit())
    {
        std::cout << "GLFW failed to initialize" << std::endl;
    }
    const char* glsl_version = "#version 150 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(400, 400, "Conway's Game", nullptr, nullptr);
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

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui::GetVersion();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    in.setWindow(window);

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
                    0,1,
                    0,1,
                    1,0,
                    1,1
            };
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(b_shader.ID,"position"), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    b_shader.SetUniform4f("incolor", {1.0, 1.0, 1.0, 1.0});

    glm::mat4 proj = glm::ortho<float>(0,c.get_size(),0,c.get_size(), -1, 1);
    b_shader.SetUniformMatrix4fv("proj", proj);
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        size = c.get_size();
        update();
        draw();
        draw_gui();
        glfwSwapBuffers(window);
    }
}

void Application::drawTile(int x, int y)
{
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3{x,y,0.0});
    b_shader.SetUniformMatrix4fv("trans", trans);
    glDrawArrays(GL_TRIANGLES, 0 , 6);
}

void Application::changeScale(float minX, float maxX, float minY, float maxY) {
    glm::mat4 proj = glm::ortho<float>(minX,maxY,minY,maxY, -1, 1);
    b_shader.SetUniformMatrix4fv("proj", proj);
}

void Application::draw() {
    for (int i = 0; i < c.get_size(); ++i) {
        for (int j = 0; j < c.get_size(); ++j) {
            if(c.get_table()[i][j])
            {
                drawTile(i,j);
            }
        }
    }

}

bool held = false;
void Application::update() {

    double ct = glfwGetTime()-lt;
    if(in.getMouseButton(GLFW_MOUSE_BUTTON_1))
    {
        c.get_table()[x_mouse][y_mouse] = true;
    }

    if(in.getKeyDown(GLFW_KEY_ESCAPE))
    {
        menuBar = !menuBar;
    }

    if(in.getMouseButton(GLFW_MOUSE_BUTTON_2))
    {
        c.get_table()[x_mouse][y_mouse] = false;
    }

    if(in.getKeyDown(GLFW_KEY_SPACE))
    {
        enabled = !enabled;
    }

    if(in.getKeyDown(GLFW_KEY_R))
    {
        c.reset();
    }

    if(ct > 1.0f/tps)
    {
        if (enabled)
        {
            c.tick();
        }
        lt = glfwGetTime();
    }



}

void Application::draw_gui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    if(tutorialWindow)
    {
        ImGui::SetNextWindowPos({w_width/2,w_height/2}, 0, {0.5,0.5});
        ImGui::SetNextWindowSize({w_width/2,w_height/2});

        ImGui::Begin("Tutorial", &tutorialWindow, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Press ESC to toggle menu");
        ImGui::End();
    }

    if(menuBar && ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("Settings"))
        {
            if(ImGui::MenuItem("Reset (R)"))
            {
                c.reset();
            }

            std::string curLabel;
            if(enabled)
            {
                curLabel = "Stop (Space)";
            }
            else
            {
                curLabel = "Start (Space)";
            }

            if(ImGui::MenuItem(curLabel.c_str()))
            {
                enabled = !enabled;
            }

            ImGui::SliderFloat("TPS", &tps, 0.0f, 20.0f, "%.1f");



            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Custom Rules"))
        {
            ImGui::InputInt("Minimum Population", &c.minPop);
            ImGui::InputInt("Maximum Population", &c.maxPop);
            ImGui::InputInt("Reproduction Population", &c.reproductionPop);

            if(ImGui::BeginMenu("Presets"))
            {
                if(ImGui::MenuItem("Game of Life (Default)"))
                {
                    c.minPop = 2;
                    c.maxPop = 3;
                    c.reproductionPop = 3;
                }
                if(ImGui::MenuItem("Creeping Vines"))
                {
                    c.minPop = 2;
                    c.maxPop = 5;
                    c.reproductionPop = 3;
                }
                if(ImGui::MenuItem("Fractal Squares"))
                {
                    c.minPop = 1;
                    c.maxPop = 8;
                    c.reproductionPop = 1;
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }



    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
