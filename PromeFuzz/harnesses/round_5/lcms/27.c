// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

static void init_vector_from_data(cmsVEC3* vec, const uint8_t* data, size_t size, size_t* offset) {
    for (int i = 0; i < 3; i++) {
        if (*offset + sizeof(double) <= size) {
            memcpy(&vec->n[i], data + *offset, sizeof(double));
            *offset += sizeof(double);
        } else {
            vec->n[i] = 0.0;
        }
    }
}

static void init_matrix_from_data(cmsMAT3* mat, const uint8_t* data, size_t size, size_t* offset) {
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

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 3 * sizeof(double)) {
        return 0;
    }

    size_t offset = 0;
    
    cmsVEC3 u, v, a, b, r, x;
    cmsMAT3 mat;
    
    init_vector_from_data(&u, Data, Size, &offset);
    init_vector_from_data(&v, Data, Size, &offset);
    init_vector_from_data(&a, Data, Size, &offset);
    init_vector_from_data(&b, Data, Size, &offset);
    init_matrix_from_data(&mat, Data, Size, &offset);
    
    cmsFloat64Number dot_result = _cmsVEC3dot(&u, &v);
    (void)dot_result;
    
    cmsFloat64Number dist_result = _cmsVEC3distance(&a, &b);
    (void)dist_result;
    
    _cmsVEC3cross(&r, &u, &v);
    
    _cmsMAT3eval(&x, &mat, &v);
    
    _cmsVEC3minus(&r, &a, &b);
    
    cmsBool solve_result = _cmsMAT3solve(&x, &mat, &b);
    (void)solve_result;
    
    return 0;
}