//
// Created by xande on 6/9/2025.
//

#include "pch.hpp"
#include "InputSystem.h"
namespace D3D11Engine {
HRESULT InputSystem::Init() {
    return S_OK;

}

void InputSystem::Update() {

    glfwPollEvents();

}

HRESULT InputSystem::Shutdown() {
    return S_OK;

}
} // D3D11Engine