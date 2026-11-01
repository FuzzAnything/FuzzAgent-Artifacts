// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2_plugin.h"

// Helper to read a float from the fuzz input
static void read_f32(const uint8_t *data, size_t *offset, size_t size, float *out) {
    if (*offset + sizeof(float) <= size) {
        memcpy(out, data + *offset, sizeof(float));
        *offset += sizeof(float);
    } else {
        *out = 0.0f;
    }
}

// Helper to populate a cmsMAT3 from fuzz data
// cmsMAT3 is typically defined as struct { cmsFloat32Number matrix[3][3]; }
// However, since the exact layout is opaque or different, we cast to float array
static void fuzz_mat3_from_data(const uint8_t *data, size_t *offset, size_t size, cmsMAT3 *m) {
    float val;
    // Access the internal data as a flat array of 9 floats
    float *mat_data = (float*)m;
    for (int i = 0; i < 9; i++) {
        read_f32(data, offset, size, &val);
        mat_data[i] = val;
    }
}

// Helper to populate a cmsVEC3 from fuzz data
// cmsVEC3 is typically defined as struct { cmsFloat32Number vector[3]; }
static void fuzz_vec3_from_data(const uint8_t *data, size_t *offset, size_t size, cmsVEC3 *v) {
    float val;
    float *vec_data = (float*)v;
    for (int i = 0; i < 3; i++) {
        read_f32(data, offset, size, &val);
        vec_data[i] = val;
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsMAT3 matA, matB, matR, matInv;
    cmsVEC3 vecV, vecR, vecX, vecB;
    cmsBool result;

    // Initialize with identity to ensure valid starting state
    _cmsMAT3identity(&matA);
    _cmsMAT3identity(&matB);
    _cmsMAT3identity(&matR);
    _cmsMAT3identity(&matInv);
    memset(&vecV, 0, sizeof(cmsVEC3));
    memset(&vecR, 0, sizeof(cmsVEC3));
    memset(&vecX, 0, sizeof(cmsVEC3));
    memset(&vecB, 0, sizeof(cmsVEC3));

    size_t offset = 0;

    // Parse input data into matrices and vectors
    fuzz_mat3_from_data(Data, &offset, Size, &matA);
    fuzz_mat3_from_data(Data, &offset, Size, &matB);
    fuzz_vec3_from_data(Data, &offset, Size, &vecV);
    fuzz_vec3_from_data(Data, &offset, Size, &vecB);

    // 1. Test _cmsMAT3identity
    _cmsMAT3identity(&matR);

    // 2. Test _cmsMAT3eval
    _cmsMAT3eval(&vecR, &matA, &vecV);

    // 3. Test _cmsMAT3inverse
    result = _cmsMAT3inverse(&matA, &matInv);

    // 4. Test _cmsMAT3per
    _cmsMAT3per(&matR, &matA, &matB);

    // 5. Test _cmsMAT3isIdentity
    result = _cmsMAT3isIdentity(&matR);
    result = _cmsMAT3isIdentity(&matA);

    // 6. Test _cmsMAT3solve
    result = _cmsMAT3solve(&vecX, &matA, &vecB);

    // Additional exploration with identity matrix
    _cmsMAT3identity(&matA);
    result = _cmsMAT3solve(&vecX, &matA, &vecB);

    _cmsMAT3identity(&matA);
    result = _cmsMAT3inverse(&matA, &matInv);

    _cmsMAT3identity(&matA);
    _cmsMAT3per(&matR, &matA, &matB);

    return 0;
}