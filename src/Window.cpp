//
// Created by xande on 6/9/2025.
//

#include "Window.h"

D3D11Engine::Window::Window(const int width, const int height, std::string&& window_name) :
    m_width(width), m_height(height), m_window_name(std::move(window_name))
{
}

D3D11Engine::Window::~Window() {
}

HRESULT D3D11Engine::Window::Init() {
    m_window_handle = glfwCreateWindow(m_width, m_height,m_window_name.c_str(), nullptr, nullptr);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    return S_OK;
}

HRESULT D3D11Engine::Window::Update() {
    return S_OK;

}

HRESULT D3D11Engine::Window::Shutdown() {
    glfwDestroyWindow(m_window_handle);
    glfwTerminate();
    return S_OK;

}

bool D3D11Engine::Window::ShouldWindowClose() const {
    return glfwWindowShouldClose(m_window_handle);
}

