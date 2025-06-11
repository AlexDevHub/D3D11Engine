//
// Created by xande on 6/11/2025.
//

#ifndef MODEL_H
#define MODEL_H

#include "pch.hpp"
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

namespace D3D11Engine {
class Model {
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

public:
    Model() = default;
    Model(const Model&) = default;
    ~Model() = default;

    HRESULT Initialize(ID3D11Device *device);
    void Shutdown();
    void Render(ID3D11DeviceContext* device_context);

    int GetIndexCount() const;

private:
    HRESULT InitializeBuffers(ID3D11Device *device);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext *device_context);

private:
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    int m_vertexCount, m_indexCount;
};

} // D3D11Engine

#endif //MODEL_H
