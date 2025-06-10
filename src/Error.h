//
// Created by xande on 6/9/2025.
//

#ifndef ERROR_H
#define ERROR_H

#include <Winerror.h>

#define RETURN_IF_FAILED(x)\
    {\
    auto res = (x);\
    if (FAILED(res)) return res;\
    }

#define RETURN_RESULT_IF_FAILED(x, return_value)\
    if (FAILED(res)) return return_value;

#endif //ERROR_H
