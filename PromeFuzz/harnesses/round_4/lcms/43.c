// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

static void fill_matrix_from_data(cmsMAT3* mat, const uint8_t* data, size_t size, size_t* offset) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double value = 0.0;
            if (*offset + sizeof(double) <= size) {
                memcpy(&value, data + *offset, sizeof(double));
                *offset += sizeof(double);
            }
            mat->v[i].n[j] = value;
        }
    }
}

static void fill_vector_from_data(cmsVEC3* vec, const uint8_t* data, size_t size, size_t* offset) {
    for (int i = 0; i < 3; i++) {
        double value = 0.0;
        if (*offset + sizeof(double) <= size) {
            memcpy(&value, data + *offset, sizeof(double));
            *offset += sizeof(double);
        }
        vec->n[i] = value;
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3) {
        return 0;
    }

    size_t offset = 0;
    
    // Initialize matrices and vectors from fuzzer data
    cmsMAT3 mat1, mat2, mat3, mat_inv;
    cmsVEC3 vec1, vec2, vec_result;
    
    fill_matrix_from_data(&mat1, Data, Size, &offset);
    fill_matrix_from_data(&mat2, Data, Size, &offset);
    fill_vector_from_data(&vec1, Data, Size, &offset);
    fill_vector_from_data(&vec2, Data, Size, &offset);
    
    // Test _cmsMAT3isIdentity
    cmsBool is_identity = _cmsMAT3isIdentity(&mat1);
    (void)is_identity;
    
    // Test _cmsMAT3identity
    _cmsMAT3identity(&mat3);
    
    // Test _cmsMAT3per
    _cmsMAT3per(&mat3, &mat1, &mat2);
    
    // Test _cmsMAT3inverse
    cmsBool has_inverse = _cmsMAT3inverse(&mat1, &mat_inv);
    (void)has_inverse;
    
    // Test _cmsMAT3eval
    _cmsMAT3eval(&vec_result, &mat1, &vec1);
    
    // Test _cmsMAT3solve
    cmsBool solved = _cmsMAT3solve(&vec_result, &mat1, &vec1);
    (void)solved;
    
    return 0;
}