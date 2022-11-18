//
// Created by GPowers on 11/3/2022.
//

#ifndef CONWAYSGAMEOFLIFE_RENDERER_HPP
#define CONWAYSGAMEOFLIFE_RENDERER_HPP

#include "functional"
#include "Shader.hpp"
#include "Conway.hpp"
#include "input.hpp"
#include "GLFW/glfw3.h"
class Application {
public:
    GLFWwindow* window;
    Application();
    void drawTile(int x, int y);
    void changeScale(float minX, float maxX, float minY, float maxY);
private:
    void draw();
    void update();
    void draw_gui();
private:
    Shader b_shader{};
    Conway c{1000};
    input in{window};
    float tps = 10;
    bool enabled = false;
    bool tutorialWindow = true;
    bool menuBar = false;
    double lt = 0;
};


#endif //CONWAYSGAMEOFLIFE_RENDERER_HPP
