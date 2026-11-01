// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2_plugin.h"

// Helper to read a float from the buffer
static int read_float(const uint8_t **data, size_t *size, float *out) {
    if (*size < sizeof(float)) return 0;
    memcpy(out, *data, sizeof(float));
    *data += sizeof(float);
    *size -= sizeof(float);
    return 1;
}

// Helper to read a cmsVEC3 from the buffer
// cmsVEC3 is typically an array of 3 floats: {x, y, z}
static int read_vec3(const uint8_t **data, size_t *size, cmsVEC3 *v) {
    if (*size < 3 * sizeof(float)) return 0;
    // Accessing as array since struct members might be named differently or it's an array
    // lcms2 defines cmsVEC3 as float[3] in many contexts or a struct with x,y,z
    // Given the error, it seems to be treated as an array or the members are not x,y,z
    // Let's try accessing as an array of floats
    float *f = (float*)v;
    if (!read_float(data, size, &f[0])) return 0;
    if (!read_float(data, size, &f[1])) return 0;
    if (!read_float(data, size, &f[2])) return 0;
    return 1;
}

// Helper to read a cmsMAT3 from the buffer
// cmsMAT3 is typically an array of 3 cmsVEC3: {r0, r1, r2}
// Based on lcms2 source, cmsMAT3 is often float[3][3] or struct with r[3] where r[i] is cmsVEC3
// If cmsVEC3 is float[3], then cmsMAT3 is float[3][3]
static int read_mat3(const uint8_t **data, size_t *size, cmsMAT3 *m) {
    if (*size < 9 * sizeof(float)) return 0;
    float *mat_data = (float*)m;
    for (int i = 0; i < 9; i++) {
        if (!read_float(data, size, &mat_data[i])) return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    uint8_t op = Data[0];
    const uint8_t *ptr = Data + 1;
    size_t remaining = Size - 1;

    cmsMAT3 matA, matB, matInv, matRes;
    cmsVEC3 vecV, vecRes, vecB;

    // Initialize with zeros to avoid uninitialized memory usage
    memset(&matA, 0, sizeof(matA));
    memset(&matB, 0, sizeof(matB));
    memset(&matInv, 0, sizeof(matInv));
    memset(&matRes, 0, sizeof(matRes));
    memset(&vecV, 0, sizeof(vecV));
    memset(&vecRes, 0, sizeof(vecRes));
    memset(&vecB, 0, sizeof(vecB));

    switch (op) {
        case 0: // _cmsMAT3eval
            if (read_mat3(&ptr, &remaining, &matA) && read_vec3(&ptr, &remaining, &vecV)) {
                _cmsMAT3eval(&vecRes, &matA, &vecV);
            }
            break;

        case 1: // _cmsMAT3inverse
            if (read_mat3(&ptr, &remaining, &matA)) {
                cmsBool res = _cmsMAT3inverse(&matA, &matInv);
                (void)res;
            }
            break;

        case 2: // _cmsMAT3identity
            _cmsMAT3identity(&matA);
            break;

        case 3: // _cmsMAT3per
            if (read_mat3(&ptr, &remaining, &matA) && read_mat3(&ptr, &remaining, &matB)) {
                _cmsMAT3per(&matRes, &matA, &matB);
            }
            break;

        case 4: // _cmsMAT3solve
            if (read_mat3(&ptr, &remaining, &matA) && read_vec3(&ptr, &remaining, &vecB)) {
                cmsBool res = _cmsMAT3solve(&vecRes, &matA, &vecB);
                (void)res;
            }
            break;

        case 5: // _cmsMAT3isIdentity
            if (read_mat3(&ptr, &remaining, &matA)) {
                cmsBool res = _cmsMAT3isIdentity(&matA);
                (void)res;
            }
            break;

        default:
            // Fallback: try to parse and call random functions if data is sufficient
            if (read_mat3(&ptr, &remaining, &matA)) {
                _cmsMAT3identity(&matB);
                _cmsMAT3per(&matRes, &matA, &matB);
                _cmsMAT3eval(&vecRes, &matA, &vecV);
                _cmsMAT3inverse(&matA, &matInv);
                _cmsMAT3solve(&vecRes, &matA, &vecB);
                _cmsMAT3isIdentity(&matA);
            }
            break;
    }

    return 0;
}