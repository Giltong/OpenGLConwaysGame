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
#include "cmath"

void cursorToGrid(int &xmouse, int &ymouse);



int x_mouse{};
int y_mouse{};
float scale = 50;
int window_width = 800, window_height = 450;
float xRight = 80, xLeft = -80, yTop = 45, yBottom = -45;
float xOffset = 0.0f;
float yOffset = 0.0f;
float moveSpeed = 20.0f;
float scrollSpeed = 95.0f;
float aspectRatio = window_width/window_height;

void resize_window()
{
    float wScale = scale * (window_width / 200.0f);
    float hScale =  scale * (window_height / 200.0f);
    xRight = wScale/2;
    xLeft = -wScale/2;
    yBottom = -hScale/2;
    yTop = hScale/2;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    double w = width;
    double h = height;
    glViewport(0,0,width,height);
    window_width = width;
    window_height = height;
    aspectRatio = window_width/window_height;
    resize_window();
    static_cast<Application*>(glfwGetWindowUserPointer(window))->loop();
}

//void cursorToGrid(int &xmouse, int &ymouse) {
//
//}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
//    cursorToGrid(x_mouse, y_mouse);
    x_mouse = round(xpos/window_width * (xRight - xLeft) + (xLeft + xOffset) - 0.5);
    y_mouse = round(ypos/window_height * (yBottom - yTop) + (yTop + yOffset) - 0.5);
}

//static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    if(yoffset < 0)
//    {
//        scale = (1.0f/(scrollSpeed / 100) * scale);
//    }
//    else if(yoffset > 0)
//    {
//        scale = (scrollSpeed / 100) * scale;
//    }
//
//    if(scale < 1)
//    {
//        scale = 1;
//    }
//    resize_window();
//}

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
    window = glfwCreateWindow(window_width, window_height, "Conway's Game", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
//    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeLimits(window, 100, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glViewport(0,0,window_width,window_height);

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
    resize_window();
    glm::mat4 proj = glm::ortho<float>(xLeft, xRight, yBottom, yTop, -1, 1);
    b_shader.SetUniformMatrix4fv("proj", proj);

    while(!glfwWindowShouldClose(window)) {
        loop();

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
    glm::mat4 proj = glm::ortho<float>(minX + xOffset,maxX + xOffset,minY + yOffset,maxY + yOffset, -1, 1);
    b_shader.SetUniformMatrix4fv("proj", proj);
}

void Application::draw() {
    for (const auto& point : c.get_table()) {
        drawTile(point.first, point.second);
    }
    drawTile(x_mouse,y_mouse);

}

bool held = false;
void Application::update() {
    double ct = glfwGetTime()-lastTick;
    dt = glfwGetTime() - lt;


    if(ct > 1.0f/tps)
    {
        if (enabled)
        {
            c.tick();
        }
        lastTick = glfwGetTime();
    }
    lt = glfwGetTime();


}

void Application::draw_gui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if(tutorialWindow)
    {
        ImGui::SetNextWindowPos({window_width/2.0f,window_height/2.0f}, ImGuiCond_Once, {0.5,0.5});
        ImGui::SetNextWindowSize({window_width/2.0f,window_height/2.0f}, ImGuiCond_Once);
        if(ImGui::Begin("Tutorial", &tutorialWindow))
        {
            ImGui::TextWrapped("Conway's Game of Life is a simulation of cellular automata, which is a unpredictable simulation of a grid of cells. Each cell follows a certain set of rules to determine whether it lives or dies.");
            ImGui::NewLine();
            ImGui::TextWrapped("Use left click to place live cells.");
            ImGui::TextWrapped("Use right click to remove live cells.");
            ImGui::TextWrapped("Use scroll wheel to zoom in and out.");
            ImGui::TextWrapped("Use arrow keys to move viewport.");
            ImGui::TextWrapped("Once you have cells placed, press space to start and stop the simulation.");
            ImGui::TextWrapped("Pressing R will reset the simulation to have no live cells.");
            ImGui::NewLine();
            ImGui::TextWrapped("To open and close the settings menu, press ESC.");
            ImGui::TextWrapped("The rules tab allows you to change the properties of the game. Try adjusting them and see what changes!");



            ImGui::End();
        }
    }


    if(menuBar && ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("Controls"))
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

            ImGui::SliderFloat("Ticks per Second", &tps, 0.0f, 20.0f, "%.1f");


            if(ImGui::BeginMenu("Viewport Options"))
            {

                if(ImGui::SliderFloat("Scale", &scale, 1.0f, 100.0f))
                {
                    if(scale < 1)
                    {
                        scale = 1;
                    }
                    resize_window();
                }
                if(ImGui::SliderFloat("Viewport Move Speed", &moveSpeed, 10.0f, 100.0f))
                {
                    resize_window();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Rules"))
        {
            ImGui::InputInt("Minimum Neighbors", &c.minPop);
            ImGui::InputInt("Maximum Neighbors", &c.maxPop);
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

void Application::get_input() {
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        xOffset -= moveSpeed * dt;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        xOffset += moveSpeed * dt;
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        yOffset += moveSpeed * dt;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        yOffset -= moveSpeed * dt;
    }
    if(in.getMouseButton(GLFW_MOUSE_BUTTON_1))
    {
        c.get_table().insert({x_mouse,y_mouse});
    }

    if(in.getKeyDown(GLFW_KEY_ESCAPE))
    {
        menuBar = !menuBar;
    }

    if(in.getMouseButton(GLFW_MOUSE_BUTTON_2))
    {
        c.get_table().erase({x_mouse,y_mouse});
    }

    if(in.getKeyDown(GLFW_KEY_SPACE))
    {
        enabled = !enabled;
    }

    if(in.getKeyDown(GLFW_KEY_R))
    {
        c.reset();
    }
}

void Application::loop() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    changeScale(xLeft, xRight, yBottom, yTop);
    update();
    draw();
    draw_gui();
    get_input();
    glfwSwapBuffers(window);
    glfwPollEvents();
}
