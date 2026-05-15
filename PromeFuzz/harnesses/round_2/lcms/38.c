// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"

static void fill_matrix_from_data(cmsMAT3* mat, const uint8_t* data, size_t size, size_t* offset) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (*offset + sizeof(double) <= size) {
                double value;
                memcpy(&value, data + *offset, sizeof(double));
                mat->v[i].n[j] = value;
                *offset += sizeof(double);
            } else {
                mat->v[i].n[j] = 0.0;
            }
        }
    }
}

static void fill_vector_from_data(cmsVEC3* vec, const uint8_t* data, size_t size, size_t* offset) {
    for (int i = 0; i < 3; i++) {
        if (*offset + sizeof(double) <= size) {
            double value;
            memcpy(&value, data + *offset, sizeof(double));
            vec->n[i] = value;
            *offset += sizeof(double);
        } else {
            vec->n[i] = 0.0;
        }
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    cmsMAT3 mat1, mat2, mat_result, mat_inverse;
    cmsVEC3 vec1, vec2, vec_result;
    
    fill_matrix_from_data(&mat1, Data, Size, &offset);
    fill_matrix_from_data(&mat2, Data, Size, &offset);
    fill_vector_from_data(&vec1, Data, Size, &offset);
    fill_vector_from_data(&vec2, Data, Size, &offset);
    
    cmsBool bool_result;
    
    bool_result = _cmsMAT3isIdentity(&mat1);
    
    _cmsMAT3eval(&vec_result, &mat1, &vec1);
    
    cmsMAT3 mat_solve = mat1;
    cmsVEC3 vec_solve = vec1;
    bool_result = _cmsMAT3solve(&vec_result, &mat_solve, &vec_solve);
    
    _cmsMAT3per(&mat_result, &mat1, &mat2);
    
    _cmsMAT3identity(&mat_result);
    
    bool_result = _cmsMAT3inverse(&mat1, &mat_inverse);
    
    return 0;
}