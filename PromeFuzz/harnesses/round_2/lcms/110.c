// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3length at cmsmtrx.c:64:28 in lcms2_plugin.h
// _cmsVEC3distance at cmsmtrx.c:72:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
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

static void read_double(const uint8_t *data, size_t size, size_t *offset, cmsFloat64Number *value) {
    if (*offset + sizeof(cmsFloat64Number) <= size) {
        memcpy(value, data + *offset, sizeof(cmsFloat64Number));
        *offset += sizeof(cmsFloat64Number);
    } else {
        *value = 0.0;
        *offset = size;
    }
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size < 12 * sizeof(cmsFloat64Number)) {
        return 0;
    }

    size_t offset = 0;
    cmsVEC3 a, b, c, d;
    cmsFloat64Number x1, y1, z1, x2, y2, z2, x3, y3, z3, x, y, z;
    cmsFloat64Number result;

    // Read input data for vectors
    read_double(Data, Size, &offset, &x1);
    read_double(Data, Size, &offset, &y1);
    read_double(Data, Size, &offset, &z1);
    read_double(Data, Size, &offset, &x2);
    read_double(Data, Size, &offset, &y2);
    read_double(Data, Size, &offset, &z2);
    read_double(Data, Size, &offset, &x3);
    read_double(Data, Size, &offset, &y3);
    read_double(Data, Size, &offset, &z3);
    read_double(Data, Size, &offset, &x);
    read_double(Data, Size, &offset, &y);
    read_double(Data, Size, &offset, &z);

    // Initialize vectors using _cmsVEC3init
    _cmsVEC3init(&a, x1, y1, z1);
    _cmsVEC3init(&b, x2, y2, z2);
    _cmsVEC3init(&c, x3, y3, z3);

    // Test _cmsVEC3cross
    _cmsVEC3cross(&d, &a, &b);

    // Test _cmsVEC3minus
    _cmsVEC3minus(&d, &a, &b);

    // Test _cmsVEC3length
    result = _cmsVEC3length(&a);
    (void)result;

    // Test _cmsVEC3distance
    result = _cmsVEC3distance(&a, &b);
    (void)result;

    // Test _cmsVEC3dot
    result = _cmsVEC3dot(&a, &b);
    (void)result;

    // Test _cmsVEC3init with new values
    _cmsVEC3init(&d, x, y, z);

    // Additional combinations to explore more states
    _cmsVEC3cross(&d, &b, &c);
    _cmsVEC3minus(&d, &b, &c);
    result = _cmsVEC3length(&b);
    result = _cmsVEC3distance(&b, &c);
    result = _cmsVEC3dot(&b, &c);
    _cmsVEC3init(&d, y, z, x);

    return 0;
}