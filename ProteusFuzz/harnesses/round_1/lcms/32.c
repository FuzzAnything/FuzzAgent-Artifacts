// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2_plugin.h"

static cmsFloat64Number get_double(const uint8_t *buf, size_t offset, size_t size) {
    if (offset + sizeof(double) > size) {
        return 0.0;
    }
    double val;
    memcpy(&val, buf + offset, sizeof(double));
    return (cmsFloat64Number)val;
}

static void fuzz_vec3_functions(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;

    cmsVEC3 v1, v2, v3, v4, v5;
    cmsFloat64Number x, y, z;

    // Initialize v1
    x = get_double(Data, 0, Size);
    y = get_double(Data, 8, Size);
    z = get_double(Data, 16, Size);
    _cmsVEC3init(&v1, x, y, z);

    // Initialize v2
    x = get_double(Data, 24, Size);
    y = get_double(Data, 32, Size);
    z = get_double(Data, 40, Size);
    _cmsVEC3init(&v2, x, y, z);

    // Initialize v3 for result storage
    _cmsVEC3init(&v3, 0, 0, 0);

    // Initialize v4 and v5 for additional tests
    x = get_double(Data, 48, Size);
    y = get_double(Data, 56, Size);
    z = get_double(Data, 64, Size);
    _cmsVEC3init(&v4, x, y, z);

    x = get_double(Data, 72, Size);
    y = get_double(Data, 80, Size);
    z = get_double(Data, 88, Size);
    _cmsVEC3init(&v5, x, y, z);

    // 1. _cmsVEC3cross
    _cmsVEC3cross(&v3, &v1, &v2);

    // 2. _cmsVEC3distance
    cmsFloat64Number dist = _cmsVEC3distance(&v1, &v2);
    (void)dist; // Use return value to suppress warnings

    // 3. _cmsVEC3init (already used above, re-use with different params)
    x = get_double(Data, 96, Size);
    y = get_double(Data, 104, Size);
    z = get_double(Data, 112, Size);
    _cmsVEC3init(&v3, x, y, z);

    // 4. _cmsVEC3length
    cmsFloat64Number len = _cmsVEC3length(&v3);
    (void)len;

    // 5. _cmsVEC3dot
    cmsFloat64Number dot = _cmsVEC3dot(&v4, &v5);
    (void)dot;

    // 6. _cmsVEC3minus
    _cmsVEC3minus(&v3, &v4, &v5);

    // Additional diverse calls to explore states
    // Cross with self (should be zero)
    _cmsVEC3cross(&v3, &v1, &v1);
    
    // Distance to self (should be zero)
    dist = _cmsVEC3distance(&v1, &v1);
    (void)dist;

    // Dot with self (magnitude squared)
    dot = _cmsVEC3dot(&v1, &v1);
    (void)dot;

    // Minus self (should be zero)
    _cmsVEC3minus(&v3, &v1, &v1);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Call the fuzzing logic
    fuzz_vec3_functions(Data, Size);

    return 0;
}