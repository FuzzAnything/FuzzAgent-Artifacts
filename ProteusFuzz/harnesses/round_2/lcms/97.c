// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2_plugin.h"

static cmsFloat64Number parse_double(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(cmsFloat64Number) > size) {
        return 0.0;
    }
    cmsFloat64Number val;
    memcpy(&val, data + *offset, sizeof(cmsFloat64Number));
    *offset += sizeof(cmsFloat64Number);
    return val;
}

static void init_vec3_from_data(cmsVEC3 *v, const uint8_t *data, size_t size, size_t *offset) {
    cmsFloat64Number x = parse_double(data, size, offset);
    cmsFloat64Number y = parse_double(data, size, offset);
    cmsFloat64Number z = parse_double(data, size, offset);
    _cmsVEC3init(v, x, y, z);
}

static void cleanup_vec3(cmsVEC3 *v) {
    // No explicit cleanup needed for cmsVEC3 as it's a simple struct
    (void)v;
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size < 18) {
        return 0;
    }

    size_t offset = 0;
    cmsVEC3 v1, v2, v3, v4;
    
    // Initialize vectors with fuzzed data
    init_vec3_from_data(&v1, Data, Size, &offset);
    init_vec3_from_data(&v2, Data, Size, &offset);
    init_vec3_from_data(&v3, Data, Size, &offset);
    init_vec3_from_data(&v4, Data, Size, &offset);

    // Test _cmsVEC3init with additional fuzzed values
    if (offset + 24 <= Size) {
        cmsVEC3 v5;
        cmsFloat64Number x = parse_double(Data, Size, &offset);
        cmsFloat64Number y = parse_double(Data, Size, &offset);
        cmsFloat64Number z = parse_double(Data, Size, &offset);
        _cmsVEC3init(&v5, x, y, z);
    }

    // Test _cmsVEC3dot
    cmsFloat64Number dot_result = _cmsVEC3dot(&v1, &v2);
    (void)dot_result;

    // Test _cmsVEC3minus
    _cmsVEC3minus(&v3, &v1, &v2);
    
    // Test _cmsVEC3distance
    cmsFloat64Number dist_result = _cmsVEC3distance(&v1, &v2);
    (void)dist_result;

    // Test _cmsVEC3length
    cmsFloat64Number len_result = _cmsVEC3length(&v1);
    (void)len_result;

    // Test _cmsVEC3cross
    _cmsVEC3cross(&v4, &v1, &v2);

    // Additional state exploration: reuse vectors
    _cmsVEC3minus(&v1, &v3, &v4);
    cmsFloat64Number dot2 = _cmsVEC3dot(&v1, &v3);
    (void)dot2;

    _cmsVEC3cross(&v2, &v1, &v3);
    cmsFloat64Number len2 = _cmsVEC3length(&v2);
    (void)len2;

    // Cleanup (not strictly necessary for these structs but good practice)
    cleanup_vec3(&v1);
    cleanup_vec3(&v2);
    cleanup_vec3(&v3);
    cleanup_vec3(&v4);

    return 0;
}