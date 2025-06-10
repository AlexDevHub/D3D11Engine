//
// Created by xande on 6/9/2025.
//

#include "Application.h"

HRESULT D3D11Engine::Application::Init() {
    if (!glfwInit()) {
        return E_FAIL;
    }

    m_window_handle = new Window(640, 480, "D3D11Engine");
    m_window_handle->Init();

    m_input_handle = new InputSystem();

    return S_OK;
}


HRESULT D3D11Engine::Application::Update() {
    while (!m_window_handle->ShouldWindowClose()) {
        m_input_handle->Update();
    }

    return S_OK;
}


HRESULT D3D11Engine::Application::Shutdown() {
    glfwTerminate();

    return S_OK;
}

