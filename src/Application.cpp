﻿//
// Created by xande on 6/9/2025.
//

#include "Application.h"

constexpr bool FULLSCREEN = false;
constexpr bool VSYNC_ENABLED = true;
constexpr float SCREEN_DEPTH = 1000.0f;
constexpr float SCREEN_NEAR = 0.3f;

HRESULT D3D11Engine::Application::Init() {
    if (!glfwInit()) {
        return E_FAIL;
    }

    m_window = std::make_unique<Window>(640, 480, "D3D11Engine");
    RETURN_FAIL_IF_FAILED(m_window->Init())
    HWND window_hwnd = m_window->getWindowHandleHWND();

    m_d3d11api = std::make_unique<D3D11API>();
    RETURN_FAIL_IF_FAILED(m_d3d11api->Init(640, 480, VSYNC_ENABLED, window_hwnd, false, SCREEN_DEPTH, SCREEN_NEAR))

    m_camera = std::make_unique<Camera>();
    m_camera->SetPosition(0.0f, 0.0f, -5.0f);

    m_input = std::make_unique<InputSystem>();
    m_input->Init(m_window->getWindowHandle(), m_camera.get());

    std::string texture_filename("Assets/Textures/stone01.tga");
    m_model = std::make_unique<Model>();
    RETURN_FAIL_IF_FAILED(m_model->Initialize(m_d3d11api->GetDevice(),m_d3d11api->GetDeviceContext(), texture_filename))

    m_color_shader = std::make_unique<ColorShader>();
    RETURN_FAIL_IF_FAILED(m_color_shader->Initialize(m_d3d11api->GetDevice(), window_hwnd))

    // Create and initialize the texture shader object.
    m_texture_shader = std::make_unique<TextureShader>();

    if(FAILED(m_texture_shader->Initialize(m_d3d11api->GetDevice(), window_hwnd)))
    {
        MessageBoxW(window_hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
        return false;
    }

    // Create and initialize the light shader object.
    m_light_shader = std::make_unique<LightShader>();
    if(FAILED(m_light_shader->Initialize(m_d3d11api->GetDevice(), window_hwnd)))
    {
        MessageBoxW(window_hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
        return false;
    }

    // Create and initialize the light object.
    m_light = std::make_unique<Light>();

    m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_light->SetDirection(0.0f, 0.0f, 1.0f);

    return S_OK;
}

void D3D11Engine::Application::Update() {
    while (!m_window->ShouldWindowClose()) {
        m_input->Update();

        if (FAILED(Frame())) {
            m_window->SetWindowShouldClose(true);
        }
    }
}

HRESULT D3D11Engine::Application::Shutdown() {
    glfwTerminate();

    return S_OK;
}

HRESULT D3D11Engine::Application::Frame() {
    static float rotation = 0.0f;

    rotation -= 0.0174532925f * 0.1f;
    if(rotation < 0.0f) rotation += 360.0f;

    RETURN_FAIL_IF_FAILED(Render(rotation))

    return S_OK;
}

HRESULT D3D11Engine::Application::Render(float rotation) {
    // Clear the buffers to begin the scene.
    m_d3d11api->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // Generate the view matrix based on the camera's position.
    m_camera->Render();

    // Get the world, view, and projection matrices from the camera and d3d objects.
    XMMATRIX worldMatrix{}, viewMatrix{}, projectionMatrix{};
    m_d3d11api->GetWorldMatrix(worldMatrix);
    m_camera->GetViewMatrix(viewMatrix);
    m_d3d11api->GetProjectionMatrix(projectionMatrix);

    // Rotate the world matrix by the rotation value so that the triangle will spin.
    worldMatrix = XMMatrixRotationY(rotation);

    // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
    m_model->Render(m_d3d11api->GetDeviceContext());

    // Render the model using the light shader.
    RETURN_FAIL_IF_FAILED(m_light_shader->Render(m_d3d11api->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture(),
                               m_light->GetDirection(), m_light->GetDiffuseColor()))

    // Render the model using the texture shader.
    // RETURN_FAIL_IF_FAILED(m_texture_shader->Render(m_d3d11api->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture()))

    // Render the model using the color shader.
    // RETURN_FAIL_IF_FAILED(m_color_shader->Render(m_d3d11api->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))

    // Present the rendered scene to the screen.
    m_d3d11api->EndScene();

    return S_OK;
}

