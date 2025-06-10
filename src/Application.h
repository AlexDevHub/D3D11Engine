//
// Created by xande on 6/9/2025.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include "InputSystem.h"
#include "System.h"
#include "Window.h"

namespace D3D11Engine {
class Application : public  System{
public:
    Application() = default;
    ~Application() override = default;

    HRESULT Init() override;
    HRESULT Update() override;
    HRESULT Shutdown() override;

private:
    Window *m_window_handle = nullptr;
    InputSystem *m_input_handle = nullptr;

};
} // namespace D3D11Engine


#endif //APPLICATION_H
