//
// Created by xande on 6/9/2025.
//

#ifndef D3D11API_H
#define D3D11API_H

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

namespace D3D11Engine {
class D3D11API {
public:

private:
    bool m_vsync_enabled;
    int m_videoCardMemory;
    char m_videoCardDescription[128];
    IDXGISwapChain *m_swapChain = nullptr;
    ID3D11Device *m_device = nullptr;
    ID3D11DeviceContext *m_deviceContext = nullptr;
    ID3D11RenderTargetView *m_renderTargetView = nullptr;
    ID3D11Texture2D *m_depthStencilBuffer = nullptr;
    ID3D11DepthStencilState *m_depthStencilState = nullptr;
    ID3D11DepthStencilView *m_depthStencilView = nullptr;
    ID3D11RasterizerState *m_rasterState = nullptr;
    XMMATRIX m_projectionMatrix;
    XMMATRIX m_worldMatrix;
    XMMATRIX m_orthoMatrix;
    D3D11_VIEWPORT m_viewport;
};
}


#endif //D3D11API_H
