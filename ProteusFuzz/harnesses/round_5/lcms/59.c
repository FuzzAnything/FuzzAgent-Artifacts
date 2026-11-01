// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lcms2_plugin.h"

static void parse_float64(const uint8_t *data, size_t size, cmsFloat64Number *val) {
    if (size >= sizeof(cmsFloat64Number)) {
        memcpy(val, data, sizeof(cmsFloat64Number));
    } else {
        *val = 0.0;
    }
}

static void parse_vec3(const uint8_t *data, size_t size, cmsVEC3 *vec) {
    cmsFloat64Number x = 0.0, y = 0.0, z = 0.0;
    
    if (size >= sizeof(cmsFloat64Number)) {
        parse_float64(data, size, &x);
    }
    if (size >= 2 * sizeof(cmsFloat64Number)) {
        parse_float64(data + sizeof(cmsFloat64Number), size - sizeof(cmsFloat64Number), &y);
    }
    if (size >= 3 * sizeof(cmsFloat64Number)) {
        parse_float64(data + 2 * sizeof(cmsFloat64Number), size - 2 * sizeof(cmsFloat64Number), &z);
    }
    
    _cmsVEC3init(vec, x, y, z);
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 9 * sizeof(cmsFloat64Number)) {
        return 0;
    }

    cmsVEC3 v1, v2, v3, result;
    cmsFloat64Number dist, len, dot;
    
    // Parse three vectors from input
    parse_vec3(Data, Size, &v1);
    parse_vec3(Data + 3 * sizeof(cmsFloat64Number), Size - 3 * sizeof(cmsFloat64Number), &v2);
    parse_vec3(Data + 6 * sizeof(cmsFloat64Number), Size - 6 * sizeof(cmsFloat64Number), &v3);
    
    // Initialize result vector
    _cmsVEC3init(&result, 0.0, 0.0, 0.0);
    
    // Test _cmsVEC3minus: v1 - v2
    _cmsVEC3minus(&result, &v1, &v2);
    
    // Test _cmsVEC3distance: distance between v1 and v2
    dist = _cmsVEC3distance(&v1, &v2);
    (void)dist; // Suppress unused variable warning
    
    // Test _cmsVEC3length: length of v3
    len = _cmsVEC3length(&v3);
    (void)len;
    
    // Test _cmsVEC3dot: dot product of v1 and v2
    dot = _cmsVEC3dot(&v1, &v2);
    (void)dot;
    
    // Test _cmsVEC3cross: cross product of v1 and v2
    _cmsVEC3cross(&result, &v1, &v2);
    
    // Additional diverse calls to explore more states
    _cmsVEC3minus(&v1, &v1, &v3); // Alias case: r = a
    _cmsVEC3minus(&v2, &v2, &v1); // Alias case: r = b
    _cmsVEC3cross(&v3, &v1, &v2); // Store cross product in v3
    
    // Re-initialize vectors with different values for further testing
    _cmsVEC3init(&v1, 1.0, 0.0, 0.0);
    _cmsVEC3init(&v2, 0.0, 1.0, 0.0);
    _cmsVEC3init(&v3, 0.0, 0.0, 1.0);
    
    _cmsVEC3minus(&result, &v1, &v2);
    dist = _cmsVEC3distance(&v1, &v2);
    len = _cmsVEC3length(&v3);
    dot = _cmsVEC3dot(&v1, &v2);
    _cmsVEC3cross(&result, &v1, &v2);
    
    return 0;
}