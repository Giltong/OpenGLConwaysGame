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
class Renderer {
public:
    GLFWwindow* window;
    Renderer();
    void drawTile(int x, int y);
    void changeScale(float minX, float maxX, float minY, float maxY);
private:
    Shader b_shader{};
    Conway c{100};
    input in{window};
    bool enabled = false;
    bool tutorialWindow = true;
    bool menuBar = false;
    double lt = 0;
    void draw();
    void update();

    void draw_gui();
};


#endif //CONWAYSGAMEOFLIFE_RENDERER_HPP
