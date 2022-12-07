#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "Shader.hpp"
#include "Application.hpp"
#include "Conway.hpp"
Application* r;

int WinMain() {
    r = new Application();
    delete r;
    return 0;
}


