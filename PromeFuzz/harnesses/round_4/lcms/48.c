// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "lcms2_plugin.h"

static void fill_matrix_from_data(cmsMAT3* mat, const uint8_t* data, size_t size, size_t* offset) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (*offset + sizeof(double) <= size) {
                memcpy(&mat->v[i].n[j], data + *offset, sizeof(double));
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
            memcpy(&vec->n[i], data + *offset, sizeof(double));
            *offset += sizeof(double);
        } else {
            vec->n[i] = 0.0;
        }
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t* Data, size_t Size) {
    if (Size < sizeof(double) * 3) {
        return 0;
    }

    size_t offset = 0;
    
    cmsMAT3 mat1, mat2, mat3, result_mat;
    cmsVEC3 vec1, vec2, result_vec;
    
    fill_matrix_from_data(&mat1, Data, Size, &offset);
    fill_matrix_from_data(&mat2, Data, Size, &offset);
    fill_vector_from_data(&vec1, Data, Size, &offset);
    fill_vector_from_data(&vec2, Data, Size, &offset);
    
    cmsBool bool_result;
    
    bool_result = _cmsMAT3isIdentity(&mat1);
    
    _cmsMAT3identity(&mat3);
    
    _cmsMAT3per(&result_mat, &mat1, &mat2);
    
    bool_result = _cmsMAT3inverse(&mat1, &result_mat);
    
    bool_result = _cmsMAT3solve(&result_vec, &mat1, &vec1);
    
    _cmsMAT3eval(&result_vec, &mat1, &vec1);
    
    return 0;
}