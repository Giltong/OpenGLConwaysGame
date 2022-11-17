//
// Created by GPowers on 11/15/2022.
//

#ifndef CONWAYSGAMEOFLIFE_INPUT_HPP
#define CONWAYSGAMEOFLIFE_INPUT_HPP

#include <map>
#include "GLFW/glfw3.h"

class input {
public:
    input(GLFWwindow* window);
    void setWindow(GLFWwindow* window);
    bool getKey(int keyCode);
    bool getKeyDown(int keyCode);
    bool getKeyUp(int keyCode);
    void updateKey(int keyCode);

    bool getMouseButton(int keyCode);
    bool getMouseButtonDown(int keyCode);
    bool getMouseButtonUp(int keyCode);
    void updateMouseButton(int keyCode);
private:
    std::map<int, bool> heldKeys;
    std::map<int, bool> releasedKeys;
    std::map<int, bool> pressedKeys;

    std::map<int, bool> heldMouseButtons;
    std::map<int, bool> releasedMouseButtons;
    std::map<int, bool> pressedMouseButtons;
    GLFWwindow* window;
};


#endif //CONWAYSGAMEOFLIFE_INPUT_HPP
