//
// Created by xande on 6/9/2025.
//

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wrl.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Error.h"

using namespace Microsoft::WRL;

#endif //PCH_H
