// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2_plugin.h"

static void safe_parse_doubles(const uint8_t* data, size_t size, double* dest, size_t count) {
    size_t bytes_needed = count * sizeof(double);
    if (size >= bytes_needed) {
        memcpy(dest, data, bytes_needed);
    } else {
        memset(dest, 0, bytes_needed);
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsMAT3 mat1, mat2, mat3, mat4;
    cmsVEC3 vec1, vec2, vec3;
    
    size_t max_doubles = Size / sizeof(double);
    
    if (max_doubles >= 9) {
        safe_parse_doubles(Data, Size, (double*)&mat1, 9);
    } else {
        memset(&mat1, 0, sizeof(mat1));
    }
    
    if (max_doubles >= 18) {
        safe_parse_doubles(Data + 9 * sizeof(double), Size - 9 * sizeof(double), (double*)&mat2, 9);
    } else {
        memset(&mat2, 0, sizeof(mat2));
    }
    
    if (max_doubles >= 21) {
        safe_parse_doubles(Data + 18 * sizeof(double), Size - 18 * sizeof(double), (double*)&vec1, 3);
    } else {
        memset(&vec1, 0, sizeof(vec1));
    }
    
    if (max_doubles >= 24) {
        safe_parse_doubles(Data + 21 * sizeof(double), Size - 21 * sizeof(double), (double*)&vec2, 3);
    } else {
        memset(&vec2, 0, sizeof(vec2));
    }
    
    _cmsMAT3per(&mat3, &mat1, &mat2);
    
    _cmsMAT3eval(&vec3, &mat1, &vec1);
    
    cmsVEC3 x;
    cmsMAT3 a_copy = mat1;
    cmsVEC3 b_copy = vec2;
    _cmsMAT3solve(&x, &a_copy, &b_copy);
    
    _cmsMAT3identity(&mat4);
    
    _cmsMAT3isIdentity(&mat1);
    
    cmsMAT3 inverse;
    _cmsMAT3inverse(&mat1, &inverse);
    
    return 0;
}