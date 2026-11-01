// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "lcms2_plugin.h"

static cmsFloat64Number extract_double(const uint8_t *data, size_t offset, size_t size) {
    if (offset + sizeof(cmsFloat64Number) > size) {
        return 0.0;
    }
    cmsFloat64Number val;
    memcpy(&val, data + offset, sizeof(cmsFloat64Number));
    return val;
}

static cmsVEC3 extract_vec3(const uint8_t *data, size_t offset, size_t size) {
    cmsVEC3 v;
    v.n[VX] = extract_double(data, offset, size);
    v.n[VY] = extract_double(data, offset + sizeof(cmsFloat64Number), size);
    v.n[VZ] = extract_double(data, offset + 2 * sizeof(cmsFloat64Number), size);
    return v;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 3 * sizeof(cmsFloat64Number)) {
        return 0;
    }

    cmsVEC3 v1, v2, v3, v4, v5, v6, v7, v8, v9, v10;
    cmsVEC3 r1, r2, r3;

    // Initialize vectors from input data
    v1 = extract_vec3(Data, 0, Size);
    v2 = extract_vec3(Data, 3 * sizeof(cmsFloat64Number), Size);
    v3 = extract_vec3(Data, 6 * sizeof(cmsFloat64Number), Size);
    v4 = extract_vec3(Data, 9 * sizeof(cmsFloat64Number), Size);
    v5 = extract_vec3(Data, 12 * sizeof(cmsFloat64Number), Size);
    v6 = extract_vec3(Data, 15 * sizeof(cmsFloat64Number), Size);
    v7 = extract_vec3(Data, 18 * sizeof(cmsFloat64Number), Size);
    v8 = extract_vec3(Data, 21 * sizeof(cmsFloat64Number), Size);
    v9 = extract_vec3(Data, 24 * sizeof(cmsFloat64Number), Size);
    v10 = extract_vec3(Data, 27 * sizeof(cmsFloat64Number), Size);

    // Test _cmsVEC3init
    cmsFloat64Number x = extract_double(Data, 30 * sizeof(cmsFloat64Number), Size);
    cmsFloat64Number y = extract_double(Data, 31 * sizeof(cmsFloat64Number), Size);
    cmsFloat64Number z = extract_double(Data, 32 * sizeof(cmsFloat64Number), Size);
    _cmsVEC3init(&v1, x, y, z);

    // Test _cmsVEC3minus
    _cmsVEC3minus(&r1, &v1, &v2);
    _cmsVEC3minus(&r2, &v3, &v4);
    _cmsVEC3minus(&r3, &v5, &v6);

    // Test _cmsVEC3length
    cmsFloat64Number len1 = _cmsVEC3length(&v1);
    cmsFloat64Number len2 = _cmsVEC3length(&v2);
    cmsFloat64Number len3 = _cmsVEC3length(&v3);
    (void)len1; (void)len2; (void)len3;

    // Test _cmsVEC3dot
    cmsFloat64Number dot1 = _cmsVEC3dot(&v1, &v2);
    cmsFloat64Number dot2 = _cmsVEC3dot(&v3, &v4);
    cmsFloat64Number dot3 = _cmsVEC3dot(&v5, &v6);
    (void)dot1; (void)dot2; (void)dot3;

    // Test _cmsVEC3distance
    cmsFloat64Number dist1 = _cmsVEC3distance(&v1, &v2);
    cmsFloat64Number dist2 = _cmsVEC3distance(&v3, &v4);
    cmsFloat64Number dist3 = _cmsVEC3distance(&v5, &v6);
    (void)dist1; (void)dist2; (void)dist3;

    // Test _cmsVEC3cross
    _cmsVEC3cross(&r1, &v1, &v2);
    _cmsVEC3cross(&r2, &v3, &v4);
    _cmsVEC3cross(&r3, &v5, &v6);

    // Additional diverse invocations
    _cmsVEC3minus(&v1, &v1, &v1); // Self subtraction
    _cmsVEC3cross(&v2, &v2, &v2); // Self cross product (should be zero)
    _cmsVEC3init(&v3, 0.0, 0.0, 0.0); // Zero vector
    _cmsVEC3length(&v3); // Length of zero vector

    return 0;
}