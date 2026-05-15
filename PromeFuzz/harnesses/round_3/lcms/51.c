// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

static void init_vec_from_data(cmsVEC3* vec, const double* data) {
    for (int i = 0; i < 3; ++i) {
        vec->n[i] = data[i];
    }
}

static void init_mat_from_data(cmsMAT3* mat, const double* data) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            mat->v[i].n[j] = data[i * 3 + j];
        }
    }
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 15 * sizeof(double)) {
        return 0;
    }

    const double* double_data = (const double*)Data;
    
    cmsVEC3 u, v, a, b, r, x;
    cmsMAT3 mat;
    
    init_vec_from_data(&u, &double_data[0]);
    init_vec_from_data(&v, &double_data[3]);
    init_vec_from_data(&a, &double_data[6]);
    init_vec_from_data(&b, &double_data[9]);
    init_mat_from_data(&mat, &double_data[12]);
    
    _cmsVEC3cross(&r, &u, &v);
    
    double distance = _cmsVEC3distance(&a, &b);
    (void)distance;
    
    _cmsMAT3eval(&r, &mat, &u);
    
    _cmsVEC3minus(&r, &a, &b);
    
    double dot = _cmsVEC3dot(&u, &v);
    (void)dot;
    
    cmsBool solved = _cmsMAT3solve(&x, &mat, &b);
    (void)solved;
    
    return 0;
}