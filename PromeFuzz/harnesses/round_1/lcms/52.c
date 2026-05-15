// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
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

static void initialize_vectors_from_data(const uint8_t* data, size_t size, 
                                         cmsVEC3* vectors, int num_vectors) {
    int doubles_needed = num_vectors * 3;
    size_t bytes_needed = doubles_needed * sizeof(cmsFloat64Number);
    
    if (size >= bytes_needed) {
        cmsFloat64Number* vals = (cmsFloat64Number*)data;
        for (int i = 0; i < num_vectors; i++) {
            _cmsVEC3init(&vectors[i], vals[i*3], vals[i*3+1], vals[i*3+2]);
        }
    } else {
        // Not enough data, use default values
        for (int i = 0; i < num_vectors; i++) {
            _cmsVEC3init(&vectors[i], 0.0, 0.0, 0.0);
        }
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsFloat64Number) * 6) {
        // Need at least 6 doubles for basic testing
        return 0;
    }

    cmsVEC3 vectors[6];
    cmsVEC3 result;
    
    // Initialize vectors from fuzzer data
    initialize_vectors_from_data(Data, Size, vectors, 6);
    
    // Test _cmsVEC3cross
    _cmsVEC3cross(&result, &vectors[0], &vectors[1]);
    
    // Test _cmsVEC3minus
    _cmsVEC3minus(&result, &vectors[2], &vectors[3]);
    
    // Test _cmsVEC3distance
    cmsFloat64Number dist = _cmsVEC3distance(&vectors[0], &vectors[1]);
    (void)dist; // Suppress unused variable warning
    
    // Test _cmsVEC3length
    cmsFloat64Number len = _cmsVEC3length(&vectors[2]);
    (void)len; // Suppress unused variable warning
    
    // Test _cmsVEC3dot
    cmsFloat64Number dot = _cmsVEC3dot(&vectors[3], &vectors[4]);
    (void)dot; // Suppress unused variable warning
    
    // Test _cmsVEC3init with fuzzer data
    if (Size >= sizeof(cmsFloat64Number) * 3) {
        cmsFloat64Number* vals = (cmsFloat64Number*)Data;
        _cmsVEC3init(&vectors[5], vals[0], vals[1], vals[2]);
    }
    
    // Chain operations to explore more states
    _cmsVEC3cross(&result, &vectors[0], &vectors[1]);
    cmsFloat64Number cross_len = _cmsVEC3length(&result);
    (void)cross_len;
    
    _cmsVEC3minus(&vectors[5], &vectors[2], &vectors[3]);
    cmsFloat64Number minus_len = _cmsVEC3length(&vectors[5]);
    (void)minus_len;
    
    // Test with different combinations
    if (Size >= sizeof(cmsFloat64Number) * 9) {
        cmsFloat64Number* vals = (cmsFloat64Number*)Data;
        cmsVEC3 temp1, temp2;
        _cmsVEC3init(&temp1, vals[3], vals[4], vals[5]);
        _cmsVEC3init(&temp2, vals[6], vals[7], vals[8]);
        
        cmsFloat64Number temp_dist = _cmsVEC3distance(&temp1, &temp2);
        cmsFloat64Number temp_dot = _cmsVEC3dot(&temp1, &temp2);
        (void)temp_dist;
        (void)temp_dot;
    }

    return 0;
}