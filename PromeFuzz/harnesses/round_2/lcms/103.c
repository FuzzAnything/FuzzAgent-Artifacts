// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"

static void init_vectors_from_data(cmsVEC3* vec1, cmsVEC3* vec2, const uint8_t* data, size_t size) {
    // Initialize vectors with data from fuzzer input
    // We need 6 doubles (48 bytes) for two full vectors
    // If we don't have enough data, use what we have and set rest to 0
    size_t bytes_available = size;
    size_t offset = 0;
    
    // Read first vector
    if (offset + sizeof(cmsFloat64Number) <= bytes_available) {
        memcpy(&vec1->n[0], data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    } else {
        vec1->n[0] = 0.0;
    }
    
    if (offset + sizeof(cmsFloat64Number) <= bytes_available) {
        memcpy(&vec1->n[1], data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    } else {
        vec1->n[1] = 0.0;
    }
    
    if (offset + sizeof(cmsFloat64Number) <= bytes_available) {
        memcpy(&vec1->n[2], data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    } else {
        vec1->n[2] = 0.0;
    }
    
    // Read second vector
    if (offset + sizeof(cmsFloat64Number) <= bytes_available) {
        memcpy(&vec2->n[0], data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    } else {
        vec2->n[0] = 0.0;
    }
    
    if (offset + sizeof(cmsFloat64Number) <= bytes_available) {
        memcpy(&vec2->n[1], data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    } else {
        vec2->n[1] = 0.0;
    }
    
    if (offset + sizeof(cmsFloat64Number) <= bytes_available) {
        memcpy(&vec2->n[2], data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    } else {
        vec2->n[2] = 0.0;
    }
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsVEC3 vec1, vec2, result;
    
    // Initialize vectors from fuzzer data
    init_vectors_from_data(&vec1, &vec2, Data, Size);
    
    // Test _cmsVEC3cross
    _cmsVEC3cross(&result, &vec1, &vec2);
    
    // Test _cmsVEC3minus
    _cmsVEC3minus(&result, &vec1, &vec2);
    
    // Test _cmsVEC3length
    cmsFloat64Number len1 = _cmsVEC3length(&vec1);
    cmsFloat64Number len2 = _cmsVEC3length(&vec2);
    
    // Test _cmsVEC3distance
    cmsFloat64Number dist = _cmsVEC3distance(&vec1, &vec2);
    
    // Test _cmsVEC3dot
    cmsFloat64Number dot = _cmsVEC3dot(&vec1, &vec2);
    
    // Test _cmsVEC3init - use some values from the input
    cmsFloat64Number x = 0.0, y = 0.0, z = 0.0;
    if (Size >= sizeof(cmsFloat64Number)) {
        memcpy(&x, Data, sizeof(cmsFloat64Number));
    }
    if (Size >= 2 * sizeof(cmsFloat64Number)) {
        memcpy(&y, Data + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    }
    if (Size >= 3 * sizeof(cmsFloat64Number)) {
        memcpy(&z, Data + 2 * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    }
    _cmsVEC3init(&result, x, y, z);
    
    // Test with swapped arguments to explore more states
    _cmsVEC3cross(&result, &vec2, &vec1);
    _cmsVEC3minus(&result, &vec2, &vec1);
    
    // Test with same vector
    _cmsVEC3cross(&result, &vec1, &vec1);
    _cmsVEC3minus(&result, &vec1, &vec1);
    cmsFloat64Number dist_same = _cmsVEC3distance(&vec1, &vec1);
    cmsFloat64Number dot_same = _cmsVEC3dot(&vec1, &vec1);
    
    // Test with zero vector
    cmsVEC3 zero_vec;
    _cmsVEC3init(&zero_vec, 0.0, 0.0, 0.0);
    _cmsVEC3cross(&result, &vec1, &zero_vec);
    _cmsVEC3minus(&result, &vec1, &zero_vec);
    cmsFloat64Number len_zero = _cmsVEC3length(&zero_vec);
    cmsFloat64Number dist_zero = _cmsVEC3distance(&vec1, &zero_vec);
    cmsFloat64Number dot_zero = _cmsVEC3dot(&vec1, &zero_vec);
    
    return 0;
}