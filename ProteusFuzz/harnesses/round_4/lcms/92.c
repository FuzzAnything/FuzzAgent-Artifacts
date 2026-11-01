// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2_plugin.h"

static void init_mat3_from_fuzz(cmsMAT3 *m, const uint8_t *data, size_t offset, size_t size) {
    if (m == NULL) return;
    
    // Start with identity to ensure a valid state
    _cmsMAT3identity(m);
    
    // Access raw memory to fill floats, assuming contiguous layout
    size_t max_idx = (size - offset) / sizeof(float);
    int count = 0;
    float *f_ptr = (float *)m;
    
    for (int i = 0; i < 9 && count < max_idx; i++) {
        uint32_t val = 0;
        if (offset + count * 4 + 4 <= size) {
            memcpy(&val, data + offset + count * 4, sizeof(uint32_t));
            float f = *(float*)&val;
            if (isnan(f)) f = 0.0f;
            if (isinf(f)) f = (f > 0) ? 100000.0f : -100000.0f;
            f_ptr[i] = f;
        } else {
            f_ptr[i] = 0.0f;
        }
        count++;
    }
}

static void init_vec3_from_fuzz(cmsVEC3 *v, const uint8_t *data, size_t offset, size_t size) {
    if (v == NULL) return;
    
    size_t max_idx = (size - offset) / sizeof(float);
    int count = 0;
    float *f_ptr = (float *)v;
    
    for (int i = 0; i < 3 && count < max_idx; i++) {
        uint32_t val = 0;
        if (offset + count * 4 + 4 <= size) {
            memcpy(&val, data + offset + count * 4, sizeof(uint32_t));
            float f = *(float*)&val;
            if (isnan(f)) f = 0.0f;
            if (isinf(f)) f = (f > 0) ? 100000.0f : -100000.0f;
            f_ptr[i] = f;
        } else {
            f_ptr[i] = 0.0f;
        }
        count++;
    }
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsMAT3 m1, m2, m3, m4;
    cmsVEC3 v1, v2, v3;
    cmsBool result;
    
    // Initialize matrices and vectors from fuzz data using raw memory access
    init_mat3_from_fuzz(&m1, Data, 0, Size);
    init_mat3_from_fuzz(&m2, Data, 36, Size); 
    init_mat3_from_fuzz(&m3, Data, 72, Size);
    init_mat3_from_fuzz(&m4, Data, 108, Size);
    init_vec3_from_fuzz(&v1, Data, 144, Size);
    init_vec3_from_fuzz(&v2, Data, 156, Size);
    init_vec3_from_fuzz(&v3, Data, 168, Size);
    
    // Test _cmsMAT3per (matrix multiplication)
    _cmsMAT3per(&m4, &m1, &m2);
    
    // Test _cmsMAT3identity
    _cmsMAT3identity(&m3);
    
    // Test _cmsMAT3isIdentity
    result = _cmsMAT3isIdentity(&m3);
    (void)result;
    
    // Test _cmsMAT3eval (matrix-vector multiplication)
    _cmsMAT3eval(&v2, &m1, &v1);
    
    // Test _cmsMAT3inverse
    result = _cmsMAT3inverse(&m1, &m4);
    (void)result;
    
    // Test _cmsMAT3solve
    // Prepare a potentially invertible matrix for testing
    _cmsMAT3identity(&m2);
    // Modify slightly to make it non-trivial but likely invertible
    if (Size > 200) {
        uint32_t val;
        memcpy(&val, Data + 200, sizeof(uint32_t));
        float f = *(float*)&val;
        if (!isnan(f) && !isinf(f)) {
            // Access via raw pointer to set element [0][0]
            float *m2_ptr = (float *)&m2;
            m2_ptr[0] = f; 
        }
    }
    result = _cmsMAT3solve(&v3, &m2, &v1);
    (void)result;
    
    // Additional diverse tests
    // Test with zero matrix
    memset(&m1, 0, sizeof(cmsMAT3));
    _cmsMAT3per(&m4, &m1, &m2);
    _cmsMAT3eval(&v2, &m1, &v1);
    result = _cmsMAT3inverse(&m1, &m4);
    (void)result;
    
    // Test with large values
    memset(&m1, 0xFF, sizeof(cmsMAT3));
    _cmsMAT3per(&m4, &m1, &m2);
    _cmsMAT3eval(&v2, &m1, &v1);
    result = _cmsMAT3inverse(&m1, &m4);
    (void)result;
    
    return 0;
}