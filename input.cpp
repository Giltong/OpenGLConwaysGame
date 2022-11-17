//
// Created by GPowers on 11/15/2022.
//

#include "input.hpp"
#include "iostream"
#include "imgui.h"
void input::updateKey(int keyCode) {
    ImGuiIO& io = ImGui::GetIO();
    int status = glfwGetKey(window, keyCode);

    releasedKeys[keyCode] = false;
    pressedKeys[keyCode] = false;

    if(io.WantCaptureKeyboard)
    {
        heldKeys[keyCode] = false;
        return;
    }

    if(status == GLFW_PRESS)
    {
        if(!heldKeys[keyCode])
        {
            pressedKeys[keyCode] = true;
        }
        heldKeys[keyCode] = true;
    }
    if(status == GLFW_RELEASE && heldKeys[keyCode])
    {
        heldKeys[keyCode] = false;
        releasedKeys[keyCode] = true;
    }
}

input::input(GLFWwindow *window) {
this->window = window;
}

bool input::getKeyDown(int keyCode) {
    updateKey(keyCode);
    return pressedKeys[keyCode];
}

bool input::getKey(int keyCode) {
    updateKey(keyCode);
    return heldKeys[keyCode];
}

bool input::getKeyUp(int keyCode) {
    updateKey(keyCode);
    return releasedKeys[keyCode];
}

void input::setWindow(GLFWwindow* window) {
this->window = window;
}

void input::updateMouseButton(int keyCode) {
    ImGuiIO& io = ImGui::GetIO();
    int status = glfwGetMouseButton(window, keyCode);

    releasedMouseButtons[keyCode] = false;
    releasedMouseButtons[keyCode] = false;

    if(io.WantCaptureMouse)
    {
        heldMouseButtons[keyCode] = false;
        return;
    }

    if(status == GLFW_PRESS)
    {
        if(!heldMouseButtons[keyCode])
        {
            pressedMouseButtons[keyCode] = true;
        }
        heldMouseButtons[keyCode] = true;
    }
    if(status == GLFW_RELEASE && heldMouseButtons[keyCode])
    {
        heldMouseButtons[keyCode] = false;
        releasedMouseButtons[keyCode] = true;
    }
}

bool input::getMouseButtonUp(int keyCode) {
    updateMouseButton(keyCode);
    return releasedMouseButtons[keyCode];
}
bool input::getMouseButtonDown(int keyCode) {
    updateMouseButton(keyCode);
    return pressedMouseButtons[keyCode];
}
bool input::getMouseButton(int keyCode) {
    updateMouseButton(keyCode);
    return heldMouseButtons[keyCode];
}
