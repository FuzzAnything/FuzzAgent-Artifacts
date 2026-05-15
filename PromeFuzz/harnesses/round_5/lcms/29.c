// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"

static void initialize_matrix_from_data(cmsMAT3* mat, const uint8_t* data, size_t size, size_t* offset) {
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

static void initialize_vector_from_data(cmsVEC3* vec, const uint8_t* data, size_t size, size_t* offset) {
    for (int i = 0; i < 3; i++) {
        if (*offset + sizeof(double) <= size) {
            memcpy(&vec->n[i], data + *offset, sizeof(double));
            *offset += sizeof(double);
        } else {
            vec->n[i] = 0.0;
        }
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    cmsMAT3 mat_a, mat_b, mat_result, mat_inverse;
    cmsVEC3 vec_v, vec_r, vec_solution;
    
    initialize_matrix_from_data(&mat_a, Data, Size, &offset);
    initialize_matrix_from_data(&mat_b, Data, Size, &offset);
    initialize_vector_from_data(&vec_v, Data, Size, &offset);
    
    _cmsMAT3identity(&mat_a);
    
    cmsBool is_identity = _cmsMAT3isIdentity(&mat_a);
    
    _cmsMAT3eval(&vec_r, &mat_a, &vec_v);
    
    _cmsMAT3per(&mat_result, &mat_a, &mat_b);
    
    cmsBool invertible = _cmsMAT3inverse(&mat_a, &mat_inverse);
    
    cmsBool solvable = _cmsMAT3solve(&vec_solution, &mat_a, &vec_v);
    
    return 0;
}