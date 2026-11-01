// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
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

static void safe_init_vec3(cmsVEC3 *v, const uint8_t *data, size_t offset, size_t size) {
    cmsFloat64Number x = 0.0, y = 0.0, z = 0.0;
    if (size >= offset + 8) {
        memcpy(&x, data + offset, 8);
        offset += 8;
    }
    if (size >= offset + 8) {
        memcpy(&y, data + offset, 8);
        offset += 8;
    }
    if (size >= offset + 8) {
        memcpy(&z, data + offset, 8);
    }
    _cmsVEC3init(v, x, y, z);
}

static void safe_init_vec3_random(cmsVEC3 *v, const uint8_t *data, size_t offset, size_t size) {
    cmsFloat64Number x = 0.0, y = 0.0, z = 0.0;
    if (size >= offset + 4) {
        uint32_t val;
        memcpy(&val, data + offset, 4);
        x = (cmsFloat64Number)val / 4294967295.0;
        offset += 4;
    }
    if (size >= offset + 4) {
        uint32_t val;
        memcpy(&val, data + offset, 4);
        y = (cmsFloat64Number)val / 4294967295.0;
        offset += 4;
    }
    if (size >= offset + 4) {
        uint32_t val;
        memcpy(&val, data + offset, 4);
        z = (cmsFloat64Number)val / 4294967295.0;
    }
    _cmsVEC3init(v, x, y, z);
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 24) {
        return 0;
    }

    cmsVEC3 v1, v2, v3, v4, v5;
    cmsFloat64Number res1, res2, res3;

    // Initialize vectors from input data
    safe_init_vec3(&v1, Data, 0, Size);
    safe_init_vec3(&v2, Data, 24, Size);
    safe_init_vec3_random(&v3, Data, 48, Size);
    safe_init_vec3_random(&v4, Data, 60, Size);
    safe_init_vec3(&v5, Data, 72, Size);

    // Test _cmsVEC3distance
    res1 = _cmsVEC3distance(&v1, &v2);
    (void)res1; // Suppress unused variable warning

    // Test _cmsVEC3dot
    res2 = _cmsVEC3dot(&v3, &v4);
    (void)res2;

    // Test _cmsVEC3length
    res3 = _cmsVEC3length(&v5);
    (void)res3;

    // Test _cmsVEC3minus
    _cmsVEC3minus(&v1, &v2, &v3);

    // Test _cmsVEC3cross
    _cmsVEC3cross(&v4, &v5, &v1);

    // Additional diverse calls to explore more states
    _cmsVEC3distance(&v3, &v4);
    _cmsVEC3dot(&v1, &v2);
    _cmsVEC3length(&v3);
    _cmsVEC3minus(&v2, &v1, &v5);
    _cmsVEC3cross(&v5, &v2, &v3);

    // Edge case: zero vector
    cmsVEC3 zero_vec;
    _cmsVEC3init(&zero_vec, 0.0, 0.0, 0.0);
    _cmsVEC3distance(&zero_vec, &zero_vec);
    _cmsVEC3dot(&zero_vec, &zero_vec);
    _cmsVEC3length(&zero_vec);
    _cmsVEC3minus(&zero_vec, &v1, &v1);
    _cmsVEC3cross(&zero_vec, &v1, &v1);

    return 0;
}