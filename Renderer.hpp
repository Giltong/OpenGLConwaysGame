//
// Created by GPowers on 11/3/2022.
//

#ifndef CONWAYSGAMEOFLIFE_RENDERER_HPP
#define CONWAYSGAMEOFLIFE_RENDERER_HPP

#include "functional"
#include "Shader.hpp"
#include "Conway.hpp"
class Renderer {
public:
    Renderer();
    void drawTile(int x, int y);
    void changeScale(float minX, float maxX, float minY, float maxY);
private:
    Shader b_shader{};
    Conway c{100};
    double lt;
    void draw();
    void update();
};


#endif //CONWAYSGAMEOFLIFE_RENDERER_HPP
