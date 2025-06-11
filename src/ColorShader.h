//
// Created by xande on 6/10/2025.
//

#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "pch.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

using namespace DirectX;

namespace D3D11Engine {
class ColorShader {
private:
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

public:
    ColorShader();
    ColorShader(const ColorShader&);
    ~ColorShader();

    HRESULT Initialize(ID3D11Device*, HWND);
    void Shutdown();
    HRESULT Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
    HRESULT InitializeShader(ID3D11Device *device, HWND hwnd, std::wstring& shader_filename);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob* error_message, HWND hwnd, std::wstring &shader_filename);

    HRESULT SetShaderParameters(ID3D11DeviceContext *device_context, XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix);
    void RenderShader(ID3D11DeviceContext *device_context, int index_count);

private:
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11InputLayout> m_layout;
    ComPtr<ID3D11Buffer> m_matrixBuffer;
};
} // D3D11Engine

#endif //COLORSHADER_H
