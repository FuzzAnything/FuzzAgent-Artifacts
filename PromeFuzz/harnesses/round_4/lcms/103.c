// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

// Since we don't have the exact definition of cmsVEC3, we'll use the API functions
// to work with vectors. We need to allocate memory for the vectors and use the
// provided functions to initialize and manipulate them.

static void init_vectors_from_data(cmsVEC3* v1, cmsVEC3* v2, cmsVEC3* v3, const uint8_t* data, size_t size) {
    // Use data to initialize vector components
    // We'll interpret the data as doubles for vector initialization
    // We need at least 3 doubles (24 bytes) per vector, but we'll work with what we have
    
    // For v1
    double x1 = 0.0, y1 = 0.0, z1 = 0.0;
    if (size >= 24) {
        memcpy(&x1, data, 8);
        memcpy(&y1, data + 8, 8);
        memcpy(&z1, data + 16, 8);
    } else if (size >= 8) {
        memcpy(&x1, data, 8);
        if (size >= 16) {
            memcpy(&y1, data + 8, 8);
        }
    }
    _cmsVEC3init(v1, x1, y1, z1);
    
    // For v2
    double x2 = 0.0, y2 = 0.0, z2 = 0.0;
    if (size >= 48) {
        memcpy(&x2, data + 24, 8);
        memcpy(&y2, data + 32, 8);
        memcpy(&z2, data + 40, 8);
    } else if (size >= 32) {
        memcpy(&x2, data + 24, 8);
        if (size >= 40) {
            memcpy(&y2, data + 32, 8);
        }
    }
    _cmsVEC3init(v2, x2, y2, z2);
    
    // For v3
    double x3 = 0.0, y3 = 0.0, z3 = 0.0;
    if (size >= 72) {
        memcpy(&x3, data + 48, 8);
        memcpy(&y3, data + 56, 8);
        memcpy(&z3, data + 64, 8);
    } else if (size >= 56) {
        memcpy(&x3, data + 48, 8);
        if (size >= 64) {
            memcpy(&y3, data + 56, 8);
        }
    }
    _cmsVEC3init(v3, x3, y3, z3);
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine which function to test
    uint8_t function_selector = Data[0] % 6;
    
    // Allocate vectors
    cmsVEC3* vec1 = (cmsVEC3*)malloc(sizeof(cmsVEC3));
    cmsVEC3* vec2 = (cmsVEC3*)malloc(sizeof(cmsVEC3));
    cmsVEC3* vec3 = (cmsVEC3*)malloc(sizeof(cmsVEC3));
    cmsVEC3* result_vec = (cmsVEC3*)malloc(sizeof(cmsVEC3));
    
    if (!vec1 || !vec2 || !vec3 || !result_vec) {
        free(vec1);
        free(vec2);
        free(vec3);
        free(result_vec);
        return 0;
    }
    
    // Initialize vectors from remaining data
    init_vectors_from_data(vec1, vec2, vec3, Data + 1, Size - 1);
    
    cmsFloat64Number scalar_result;
    
    switch(function_selector) {
        case 0:
            // Test _cmsVEC3minus
            _cmsVEC3minus(result_vec, vec1, vec2);
            break;
            
        case 1:
            // Test _cmsVEC3distance
            scalar_result = _cmsVEC3distance(vec1, vec2);
            break;
            
        case 2:
            // Test _cmsVEC3cross
            _cmsVEC3cross(result_vec, vec1, vec2);
            break;
            
        case 3:
            // Test _cmsVEC3dot
            scalar_result = _cmsVEC3dot(vec1, vec2);
            break;
            
        case 4:
            // Test _cmsVEC3length
            scalar_result = _cmsVEC3length(vec1);
            break;
            
        case 5:
            // Test _cmsVEC3init
            // Use vector components as initialization parameters
            // We'll reinitialize result_vec with vec1's values
            _cmsVEC3init(result_vec, 0.0, 0.0, 0.0); // First initialize to zeros
            // Then call again with actual values (simulating the test)
            _cmsVEC3init(result_vec, 1.0, 2.0, 3.0);
            break;
    }
    
    free(vec1);
    free(vec2);
    free(vec3);
    free(result_vec);
    
    return 0;
}