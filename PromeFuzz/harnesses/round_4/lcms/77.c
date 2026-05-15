// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

static void init_vec_from_data(cmsVEC3* vec, const uint8_t** data, size_t* size) {
    if (*size >= sizeof(double) * 3) {
        memcpy(vec, *data, sizeof(double) * 3);
        *data += sizeof(double) * 3;
        *size -= sizeof(double) * 3;
    } else {
        // If not enough data, initialize with zeros
        for (int i = 0; i < 3; i++) {
            ((double*)vec)[i] = 0.0;
        }
    }
}

static void init_mat_from_data(cmsMAT3* mat, const uint8_t** data, size_t* size) {
    if (*size >= sizeof(double) * 9) {
        memcpy(mat, *data, sizeof(double) * 9);
        *data += sizeof(double) * 9;
        *size -= sizeof(double) * 9;
    } else {
        // If not enough data, initialize with identity matrix
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                ((double*)mat)[i * 3 + j] = (i == j) ? 1.0 : 0.0;
            }
        }
    }
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3 * 2) {
        // Need at least 2 vectors worth of data
        return 0;
    }

    const uint8_t* data_ptr = Data;
    size_t data_size = Size;

    // Initialize vectors and matrix from fuzzing data
    cmsVEC3 vec_a, vec_b, vec_u, vec_v, vec_r, vec_x;
    cmsMAT3 mat_a;
    
    init_vec_from_data(&vec_a, &data_ptr, &data_size);
    init_vec_from_data(&vec_b, &data_ptr, &data_size);
    init_vec_from_data(&vec_u, &data_ptr, &data_size);
    init_vec_from_data(&vec_v, &data_ptr, &data_size);
    init_mat_from_data(&mat_a, &data_ptr, &data_size);

    // Test _cmsVEC3minus
    _cmsVEC3minus(&vec_r, &vec_a, &vec_b);

    // Test _cmsVEC3distance
    volatile cmsFloat64Number dist = _cmsVEC3distance(&vec_a, &vec_b);
    (void)dist; // Prevent unused variable warning

    // Test _cmsMAT3solve
    cmsBool solve_result = _cmsMAT3solve(&vec_x, &mat_a, &vec_a);
    (void)solve_result; // Prevent unused variable warning

    // Test _cmsVEC3cross
    _cmsVEC3cross(&vec_r, &vec_u, &vec_v);

    // Test _cmsVEC3dot
    volatile cmsFloat64Number dot = _cmsVEC3dot(&vec_u, &vec_v);
    (void)dot; // Prevent unused variable warning

    // Test _cmsMAT3eval
    _cmsMAT3eval(&vec_r, &mat_a, &vec_v);

    return 0;
}