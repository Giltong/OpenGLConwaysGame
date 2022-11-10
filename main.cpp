#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Conway.hpp"
Renderer* r;

int main() {
    r = new Renderer();
    delete r;
}


