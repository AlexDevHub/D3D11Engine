//
// Created by xande on 6/9/2025.
//

#include "Application.h"

constexpr bool FULLSCREEN = false;
constexpr bool VSYNC_ENABLED = false;
constexpr float SCREEN_DEPTH = 1000.0f;
constexpr float SCREEN_NEAR = 0.3f;

HRESULT D3D11Engine::Application::Init() {
    if (!glfwInit()) {
        return E_FAIL;
    }

    m_window_handle = new Window(640, 480, "D3D11Engine");
    RETURN_FAIL_IF_FAILED(m_window_handle->Init())

    m_input_handle = new InputSystem();

    m_d3d11api = new D3D11API();
    RETURN_FAIL_IF_FAILED(m_d3d11api->Init(640, 480, VSYNC_ENABLED, m_window_handle->getWindowHandle(), false, SCREEN_DEPTH, SCREEN_NEAR))

    return S_OK;
}

void D3D11Engine::Application::Update() {
    while (!m_window_handle->ShouldWindowClose()) {
        m_input_handle->Update();
    }
}

HRESULT D3D11Engine::Application::Shutdown() {
    glfwTerminate();

    return S_OK;
}

