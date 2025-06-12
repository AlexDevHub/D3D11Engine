//
// Created by xande on 6/9/2025.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include "pch.hpp"
#include "D3D11API.h"
#include "InputSystem.h"
#include "System.h"
#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
#include "TextureShader.h"

namespace D3D11Engine {
class Application : public  System{
public:
    Application() = default;
    ~Application() override = default;

    HRESULT Init() override;
    void Update() override;
    HRESULT Shutdown() override;

    HRESULT Render();

    friend class InputSystem;

private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<InputSystem> m_input;
    std::unique_ptr<D3D11API> m_d3d11api;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Model> m_model;
    std::unique_ptr<ColorShader> m_color_shader;
    std::unique_ptr<TextureShader> m_texture_shader;
};
} // namespace D3D11Engine


#endif //APPLICATION_H
