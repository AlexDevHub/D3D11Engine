#include "pch.hpp"
#include "Application.h"
#include "Error.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
    auto *app = new D3D11Engine::Application();

    RETURN_IF_FAILED(app->Init())

    RETURN_IF_FAILED(app->Update())

    RETURN_IF_FAILED(app->Shutdown())
    delete app;

    return 0;
}
