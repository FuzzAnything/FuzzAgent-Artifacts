// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"

static void init_mat3_from_data(cmsMAT3* mat, const uint8_t* data, size_t size) {
    cmsFloat64Number val;
    int i = 0;
    // cmsMAT3 is typically defined as cmsFloat64Number row[3][3]
    // Accessing it as a flat array of 9 elements
    for (int k = 0; k < 9; k++) {
        if (i * sizeof(cmsFloat64Number) + sizeof(cmsFloat64Number) <= size) {
            memcpy(&val, data + i * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
            ((cmsFloat64Number*)mat)[k] = val;
            i++;
        } else {
            ((cmsFloat64Number*)mat)[k] = 0.0;
        }
    }
}

static void init_vec3_from_data(cmsVEC3* vec, const uint8_t* data, size_t size) {
    cmsFloat64Number val;
    int i = 0;
    // cmsVEC3 is typically defined as cmsFloat64Number xyz[3]
    for (int j = 0; j < 3; j++) {
        if (i * sizeof(cmsFloat64Number) + sizeof(cmsFloat64Number) <= size) {
            memcpy(&val, data + i * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
            ((cmsFloat64Number*)vec)[j] = val;
            i++;
        } else {
            ((cmsFloat64Number*)vec)[j] = 0.0;
        }
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsMAT3 matA, matB, matC, matInv;
    cmsVEC3 vecB, vecX, vecV, vecR;

    // Initialize matrices and vectors from input data
    init_mat3_from_data(&matA, Data, Size);
    init_mat3_from_data(&matB, Data, Size);
    init_mat3_from_data(&matC, Data, Size);
    init_vec3_from_data(&vecB, Data, Size);
    init_vec3_from_data(&vecV, Data, Size);

    // Test _cmsMAT3identity
    _cmsMAT3identity(&matC);

    // Test _cmsMAT3per (ensure r is distinct from a and b)
    cmsMAT3 matResult;
    _cmsMAT3per(&matResult, &matA, &matB);

    // Test _cmsMAT3inverse
    cmsBool invResult = _cmsMAT3inverse(&matA, &matInv);
    (void)invResult; // Suppress unused variable warning

    // Test _cmsMAT3solve
    cmsBool solveResult = _cmsMAT3solve(&vecX, &matA, &vecB);
    (void)solveResult; // Suppress unused variable warning

    // Test _cmsMAT3eval
    _cmsMAT3eval(&vecR, &matA, &vecV);

    // Test _cmsMAT3isIdentity
    cmsBool isIdResult = _cmsMAT3isIdentity(&matC);
    (void)isIdResult; // Suppress unused variable warning

    return 0;
}