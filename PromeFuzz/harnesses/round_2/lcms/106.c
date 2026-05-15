// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"

static void initialize_vectors_from_data(const uint8_t *data, size_t size, 
                                         cmsVEC3 *v1, cmsVEC3 *v2, cmsVEC3 *v3) {
    // Use data to initialize vector components
    // We'll interpret the data as doubles if enough bytes are available
    // Otherwise use default values
    double default_vals[6] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    
    if (size >= 6 * sizeof(double)) {
        // Enough data for 6 doubles (2 vectors)
        const double *d = (const double *)data;
        v1->n[0] = d[0];
        v1->n[1] = d[1];
        v1->n[2] = d[2];
        v2->n[0] = d[3];
        v2->n[1] = d[4];
        v2->n[2] = d[5];
        // Initialize v3 with some values
        v3->n[0] = 7.0;
        v3->n[1] = 8.0;
        v3->n[2] = 9.0;
    } else {
        // Use default values
        v1->n[0] = default_vals[0];
        v1->n[1] = default_vals[1];
        v1->n[2] = default_vals[2];
        v2->n[0] = default_vals[3];
        v2->n[1] = default_vals[4];
        v2->n[2] = default_vals[5];
        v3->n[0] = 7.0;
        v3->n[1] = 8.0;
        v3->n[2] = 9.0;
    }
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    cmsVEC3 vec1, vec2, vec3, result;
    
    // Initialize vectors from fuzzer data
    initialize_vectors_from_data(Data, Size, &vec1, &vec2, &vec3);
    
    // Test _cmsVEC3cross
    _cmsVEC3cross(&result, &vec1, &vec2);
    
    // Test _cmsVEC3minus
    _cmsVEC3minus(&result, &vec1, &vec2);
    
    // Test _cmsVEC3length
    double length1 = _cmsVEC3length(&vec1);
    double length2 = _cmsVEC3length(&vec2);
    
    // Test _cmsVEC3distance
    double distance = _cmsVEC3distance(&vec1, &vec2);
    
    // Test _cmsVEC3dot
    double dot_product = _cmsVEC3dot(&vec1, &vec2);
    
    // Test _cmsVEC3init
    // Use some values from the input data if available
    double x = 1.0, y = 2.0, z = 3.0;
    if (Size >= 3 * sizeof(double)) {
        const double *d = (const double *)Data;
        x = d[0];
        if (Size >= 2 * sizeof(double)) {
            y = d[1];
        }
        if (Size >= 3 * sizeof(double)) {
            z = d[2];
        }
    }
    _cmsVEC3init(&result, x, y, z);
    
    // Additional test: chain operations
    cmsVEC3 temp;
    _cmsVEC3minus(&temp, &vec1, &vec2);
    double chain_length = _cmsVEC3length(&temp);
    
    // Test with swapped vectors
    _cmsVEC3cross(&result, &vec2, &vec1);
    _cmsVEC3minus(&result, &vec2, &vec1);
    
    // Test with third vector
    _cmsVEC3cross(&result, &vec1, &vec3);
    _cmsVEC3minus(&result, &vec3, &vec2);
    double length3 = _cmsVEC3length(&vec3);
    double distance2 = _cmsVEC3distance(&vec2, &vec3);
    double dot_product2 = _cmsVEC3dot(&vec1, &vec3);
    
    return 0;
}