// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

static void set_vector_from_data(cmsVEC3* vec, const uint8_t* data, size_t size, size_t* offset) {
    if (vec == NULL) return;
    
    for (int i = 0; i < 3; i++) {
        if (*offset + sizeof(cmsFloat64Number) <= size) {
            memcpy(&vec->n[i], data + *offset, sizeof(cmsFloat64Number));
            *offset += sizeof(cmsFloat64Number);
        } else {
            vec->n[i] = 0.0;
        }
    }
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    cmsVEC3 a, b, c, result;
    size_t offset = 0;
    
    // Initialize vectors from fuzzer data
    set_vector_from_data(&a, Data, Size, &offset);
    set_vector_from_data(&b, Data, Size, &offset);
    set_vector_from_data(&c, Data, Size, &offset);
    
    // Test _cmsVEC3cross
    _cmsVEC3cross(&result, &a, &b);
    
    // Test _cmsVEC3minus
    _cmsVEC3minus(&result, &a, &b);
    
    // Test _cmsVEC3length
    cmsFloat64Number length_a = _cmsVEC3length(&a);
    (void)length_a; // Use result to avoid unused variable warning
    
    // Test _cmsVEC3distance
    cmsFloat64Number distance_ab = _cmsVEC3distance(&a, &b);
    (void)distance_ab;
    
    // Test _cmsVEC3dot
    cmsFloat64Number dot_ab = _cmsVEC3dot(&a, &b);
    (void)dot_ab;
    
    // Test _cmsVEC3init with values from fuzzer data
    cmsFloat64Number x = 0.0, y = 0.0, z = 0.0;
    
    if (offset + sizeof(cmsFloat64Number) <= Size) {
        memcpy(&x, Data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    }
    if (offset + sizeof(cmsFloat64Number) <= Size) {
        memcpy(&y, Data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    }
    if (offset + sizeof(cmsFloat64Number) <= Size) {
        memcpy(&z, Data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    }
    
    _cmsVEC3init(&result, x, y, z);
    
    return 0;
}