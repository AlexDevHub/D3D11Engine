//
// Created by xande on 6/10/2025.
//

#include "ColorShader.h"

namespace D3D11Engine {

ColorShader::ColorShader() {

}

ColorShader::ColorShader(const ColorShader &) {
}

ColorShader::~ColorShader() {
}

HRESULT ColorShader::Initialize(ID3D11Device *device, HWND hwnd) {

    std::wstring vsFilename (L"Assets/Shaders/color.vs.hlsl");
    std::wstring psFilename (L"Assets/Shaders/color.ps.hlsl");

    InitializeShader(device, hwnd, vsFilename, psFilename);

    return S_OK;
}

void ColorShader::Shutdown() {
    ShutdownShader();
}

HRESULT ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
                              XMMATRIX projectionMatrix) {

    RETURN_FAIL_IF_FAILED(SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix))

    RenderShader(deviceContext, indexCount);

    return S_OK;
}

HRESULT ColorShader::InitializeShader(ID3D11Device *device, HWND hwnd, std::wstring& vs_filename, std::wstring& ps_filename) {
    // Compile the vertex shader code.
    ComPtr<ID3D10Blob> errorMessage;
    ComPtr<ID3D10Blob> vertexShaderBuffer;
    HRESULT result = D3DCompileFromFile(vs_filename.c_str(), nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
                    &vertexShaderBuffer, &errorMessage);
    if(FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if(errorMessage)
        {
            OutputShaderErrorMessage(errorMessage.Get(), hwnd, vs_filename);
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBoxW(hwnd, vs_filename.c_str(), L"Missing Shader File", MB_OK);
        }

        return E_FAIL;
    }

    // Compile the pixel shader code.
    ComPtr<ID3D10Blob> pixelShaderBuffer;
    result = D3DCompileFromFile(ps_filename.c_str(), nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
                    &pixelShaderBuffer, &errorMessage);
    if(FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if(errorMessage)
        {
            OutputShaderErrorMessage(errorMessage.Get(), hwnd, ps_filename);
        }
        // If there was nothing in the error message then it simply could not find the file itself.
        else
        {
            MessageBoxW(hwnd, ps_filename.c_str(), L"Missing Shader File", MB_OK);
        }

        return E_FAIL;
    }

    // Create the vertex shader from the buffer.
    RETURN_FAIL_IF_FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader))

    // Create the pixel shader from the buffer.
    RETURN_FAIL_IF_FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader))

    // Create the vertex input layout description.
    // This setup needs to match the VertexType stucture in the ModelClass and in the shader.
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
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

    // Get a count of the elements in the layout.
    unsigned int numElements = std::size(polygonLayout);

    // Create the vertex input layout.
    RETURN_FAIL_IF_FAILED(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
                       vertexShaderBuffer->GetBufferSize(), &m_layout))

    // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    D3D11_BUFFER_DESC matrixBufferDesc;
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    RETURN_FAIL_IF_FAILED(device->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer))

    return S_OK;
}

void ColorShader::ShutdownShader() {
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob *error_message, HWND hwnd, std::wstring &shader_filename) {
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

HRESULT ColorShader::SetShaderParameters(ID3D11DeviceContext *device_context, XMMATRIX world_matrix,
    XMMATRIX view_matrix, XMMATRIX projection_matrix) {
    // Transpose the matrices to prepare them for the shader.
    world_matrix = XMMatrixTranspose(world_matrix);
    view_matrix = XMMatrixTranspose(view_matrix);
    projection_matrix = XMMatrixTranspose(projection_matrix);

    // Lock the constant buffer so it can be written to.
    D3D11_MAPPED_SUBRESOURCE mappedResource;
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
    device_context->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

    return S_OK;
}

void ColorShader::RenderShader(ID3D11DeviceContext *device_context, int index_count) {
    // Set the vertex input layout.
    device_context->IASetInputLayout(m_layout.Get());

    // Set the vertex and pixel shaders that will be used to render this triangle.
    device_context->VSSetShader(m_vertexShader.Get(), NULL, 0);
    device_context->PSSetShader(m_pixelShader.Get(), NULL, 0);

    // Render the triangle.
    device_context->DrawIndexed(index_count, 0, 0);
}
} // D3D11Engine