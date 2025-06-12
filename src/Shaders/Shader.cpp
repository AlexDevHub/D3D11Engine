﻿//
// Created by xande on 6/11/2025.
//

#include "Shader.h"
#include <fstream>

namespace D3D11Engine {

HRESULT Shader::InitializeShader(ID3D11Device *device, HWND hwnd, std::wstring& shader_filename) {
    // Compile the vertex shader code.
    ComPtr<ID3D10Blob> errorMessage;
    ComPtr<ID3D10Blob> vertexShaderBuffer;
    HRESULT result = D3DCompileFromFile(shader_filename.c_str(), nullptr, nullptr, "vsMain", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
                    &vertexShaderBuffer, &errorMessage);
    if(FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if(errorMessage)
        {
            OutputShaderErrorMessage(errorMessage.Get(), hwnd, shader_filename);
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBoxW(hwnd, shader_filename.c_str(), L"Missing Shader File", MB_OK);
        }

        return E_FAIL;
    }

    // Compile the pixel shader code.
    ComPtr<ID3D10Blob> pixelShaderBuffer;
    result = D3DCompileFromFile(shader_filename.c_str(), nullptr, nullptr, "psMain", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
                    &pixelShaderBuffer, &errorMessage);
    if(FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if(errorMessage)
        {
            OutputShaderErrorMessage(errorMessage.Get(), hwnd, shader_filename);
        }
        // If there was nothing in the error message then it simply could not find the file itself.
        else
        {
            MessageBoxW(hwnd, shader_filename.c_str(), L"Missing Shader File", MB_OK);
        }

        return E_FAIL;
    }

    // Create the vertex shader from the buffer.
    RETURN_FAIL_IF_FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf()))

    // Create the pixel shader from the buffer.
    RETURN_FAIL_IF_FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf()))

    // Create the vertex input layout description.
    // This setup needs to match the VertexType stucture in the ModelClass and in the shader.
    D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "COLOR";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    polygonLayout[2].SemanticName = "TEXCOORD";
    polygonLayout[2].SemanticIndex = 0;
    polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[2].InstanceDataStepRate = 0;

    polygonLayout[3].SemanticName = "NORMAL";
    polygonLayout[3].SemanticIndex = 0;
    polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[3].InputSlot = 0;
    polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[3].InstanceDataStepRate = 0;

    // Get a count of the elements in the layout.
    unsigned int numElements = std::size(polygonLayout);

    // Create the vertex input layout.
    RETURN_FAIL_IF_FAILED(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
                       vertexShaderBuffer->GetBufferSize(), m_layout.GetAddressOf()))

    // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    D3D11_BUFFER_DESC matrixBufferDesc;
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    RETURN_FAIL_IF_FAILED(device->CreateBuffer(&matrixBufferDesc, nullptr, m_matrixBuffer.GetAddressOf()))

    // Create a texture sampler state description.
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the texture sampler state.
    RETURN_FAIL_IF_FAILED(device->CreateSamplerState(&samplerDesc, &m_sampleState))

    // Setup the description of the light dynamic constant buffer that is in the pixel shader.
    // Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
    D3D11_BUFFER_DESC lightBufferDesc;
    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(LightBufferType);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    RETURN_FAIL_IF_FAILED(device->CreateBuffer(&lightBufferDesc, nullptr, &m_lightBuffer))

    return S_OK;
}

void Shader::OutputShaderErrorMessage(ID3D10Blob *error_message, HWND hwnd, std::wstring &shader_filename) {
    // Get a pointer to the error message text buffer.
    char *compileErrors = (char*)(error_message->GetBufferPointer());

    // Get the length of the message.
    size_t bufferSize = error_message->GetBufferSize();

    // Open a file to write the error message to.
    std::ofstream fout;
    fout.open("shader-error.txt");

    // Write out the error message.
    for(size_t i = 0; i < bufferSize; ++i)
    {
        fout << compileErrors[i];
    }

    // Close the file.
    fout.close();

    // Pop a message up on the screen to notify the user to check the text file for compile errors.
    MessageBoxW(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shader_filename.c_str(), MB_OK);
}


HRESULT Shader::SetShaderParameters(ID3D11DeviceContext *device_context, XMMATRIX world_matrix,
    XMMATRIX view_matrix, XMMATRIX projection_matrix) {
    // Transpose the matrices to prepare them for the shader.
    world_matrix = XMMatrixTranspose(world_matrix);
    view_matrix = XMMatrixTranspose(view_matrix);
    projection_matrix = XMMatrixTranspose(projection_matrix);

    // Lock the constant buffer so it can be written to.
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    RETURN_FAIL_IF_FAILED(device_context->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))

    // Get a pointer to the data in the constant buffer.
    MatrixBufferType* dataPtr = static_cast<MatrixBufferType *>(mappedResource.pData);

    // Copy the matrices into the constant buffer.
    dataPtr->world = world_matrix;
    dataPtr->view = view_matrix;
    dataPtr->projection = projection_matrix;

    // Unlock the constant buffer.
    device_context->Unmap(m_matrixBuffer.Get(), 0);

    // Set the position of the constant buffer in the vertex shader.
    unsigned int bufferNumber = 0;

    // Finanly set the constant buffer in the vertex shader with the updated values.
    device_context->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer.GetAddressOf());

    return S_OK;
}

void Shader::RenderShader(ID3D11DeviceContext *device_context, int index_count) {
    // Set the vertex input layout.
    device_context->IASetInputLayout(m_layout.Get());

    // Set the vertex and pixel shaders that will be used to render this triangle.
    device_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    device_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // Set the sampler state in the pixel shader.
    device_context->PSSetSamplers(0, 1, m_sampleState.GetAddressOf());

    // Render the triangle.
    device_context->DrawIndexed(index_count, 0, 0);
}

} // D3D11Engine
