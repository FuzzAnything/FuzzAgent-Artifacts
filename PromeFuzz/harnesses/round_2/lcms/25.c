// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "lcms2_plugin.h"

static void initialize_vector(cmsVEC3* vec, const uint8_t* data, size_t* offset, size_t size) {
    if (*offset + sizeof(double) * 3 <= size) {
        memcpy(&vec->n[0], data + *offset, sizeof(double) * 3);
        *offset += sizeof(double) * 3;
    } else {
        vec->n[0] = vec->n[1] = vec->n[2] = 0.0;
    }
}

static void initialize_matrix(cmsMAT3* mat, const uint8_t* data, size_t* offset, size_t size) {
    for (int i = 0; i < 3; i++) {
        if (*offset + sizeof(double) * 3 <= size) {
            memcpy(&mat->v[i].n[0], data + *offset, sizeof(double) * 3);
            *offset += sizeof(double) * 3;
        } else {
            mat->v[i].n[0] = mat->v[i].n[1] = mat->v[i].n[2] = 0.0;
        }
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3) {
        return 0;
    }

    size_t offset = 0;
    
    cmsVEC3 r1, u, v;
    cmsVEC3 r2, a, b;
    cmsVEC3 r3, v3;
    cmsMAT3 mat;
    cmsVEC3 x;
    cmsVEC3 b2;
    cmsMAT3 mat2;
    cmsVEC3 a_dist, b_dist;
    cmsVEC3 u_dot, v_dot;
    
    initialize_vector(&u, Data, &offset, Size);
    initialize_vector(&v, Data, &offset, Size);
    _cmsVEC3cross(&r1, &u, &v);
    
    initialize_vector(&a, Data, &offset, Size);
    initialize_vector(&b, Data, &offset, Size);
    _cmsVEC3minus(&r2, &a, &b);
    
    initialize_matrix(&mat, Data, &offset, Size);
    initialize_vector(&v3, Data, &offset, Size);
    _cmsMAT3eval(&r3, &mat, &v3);
    
    initialize_matrix(&mat2, Data, &offset, Size);
    initialize_vector(&b2, Data, &offset, Size);
    _cmsMAT3solve(&x, &mat2, &b2);
    
    initialize_vector(&a_dist, Data, &offset, Size);
    initialize_vector(&b_dist, Data, &offset, Size);
    _cmsVEC3distance(&a_dist, &b_dist);
    
    initialize_vector(&u_dot, Data, &offset, Size);
    initialize_vector(&v_dot, Data, &offset, Size);
    _cmsVEC3dot(&u_dot, &v_dot);
    
    return 0;
}