//
// Created by xande on 6/9/2025.
//

#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H
#include "System.h"

namespace D3D11Engine {

class InputSystem : System {
public:
    HRESULT Init() override;
    void Update() override;
    HRESULT Shutdown() override;
};

} // D3D11Engine

#endif //INPUTSYSTEM_H
