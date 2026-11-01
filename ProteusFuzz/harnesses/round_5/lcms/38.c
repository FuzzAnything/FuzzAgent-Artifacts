// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2_plugin.h>

static void parse_double(const uint8_t **data, size_t *size, cmsFloat64Number *val) {
    if (*size >= sizeof(double)) {
        memcpy(val, *data, sizeof(double));
        *data += sizeof(double);
        *size -= sizeof(double);
    } else {
        *val = 0.0;
    }
}

static void parse_double_array(const uint8_t **data, size_t *size, cmsFloat64Number *arr, size_t count) {
    for (size_t i = 0; i < count; i++) {
        parse_double(data, size, &arr[i]);
    }
}

static cmsBool is_finite(cmsFloat64Number val) {
    return isfinite(val);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsVEC3 v1, v2, v3, r_vec;
    cmsMAT3 m;
    cmsBool res_bool;
    cmsFloat64Number dot_res;

    // Initialize vectors and matrix with zeros to avoid undefined behavior on partial reads
    _cmsVEC3init(&v1, 0, 0, 0);
    _cmsVEC3init(&v2, 0, 0, 0);
    _cmsVEC3init(&v3, 0, 0, 0);
    _cmsVEC3init(&r_vec, 0, 0, 0);
    memset(&m, 0, sizeof(m));

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // Read 3x3 matrix (9 doubles)
    cmsFloat64Number m_vals[9];
    parse_double_array(&ptr, &remaining, m_vals, 9);
    if (remaining > 0) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cmsFloat64Number val = is_finite(m_vals[i * 3 + j]) ? m_vals[i * 3 + j] : 0.0;
                // Access matrix elements directly using the internal array layout
                // cmsMAT3 typically contains a double[3][3] or double[9] array.
                // We cast the struct to a pointer to double array to access elements.
                ((double(*)[3])(&m))[i][j] = val;
            }
        }
    }

    // Read vector v1 (3 doubles)
    cmsFloat64Number v1_vals[3];
    parse_double_array(&ptr, &remaining, v1_vals, 3);
    _cmsVEC3init(&v1, 
        is_finite(v1_vals[0]) ? v1_vals[0] : 0.0,
        is_finite(v1_vals[1]) ? v1_vals[1] : 0.0,
        is_finite(v1_vals[2]) ? v1_vals[2] : 0.0
    );

    // Read vector v2 (3 doubles)
    cmsFloat64Number v2_vals[3];
    parse_double_array(&ptr, &remaining, v2_vals, 3);
    _cmsVEC3init(&v2,
        is_finite(v2_vals[0]) ? v2_vals[0] : 0.0,
        is_finite(v2_vals[1]) ? v2_vals[1] : 0.0,
        is_finite(v2_vals[2]) ? v2_vals[2] : 0.0
    );

    // Read vector v3 (3 doubles)
    cmsFloat64Number v3_vals[3];
    parse_double_array(&ptr, &remaining, v3_vals, 3);
    _cmsVEC3init(&v3,
        is_finite(v3_vals[0]) ? v3_vals[0] : 0.0,
        is_finite(v3_vals[1]) ? v3_vals[1] : 0.0,
        is_finite(v3_vals[2]) ? v3_vals[2] : 0.0
    );

    // Fuzz _cmsMAT3eval: r = a * v
    _cmsMAT3eval(&r_vec, &m, &v1);

    // Fuzz _cmsVEC3minus: r = a - b
    _cmsVEC3minus(&r_vec, &v1, &v2);

    // Fuzz _cmsVEC3dot: dot = u . v
    dot_res = _cmsVEC3dot(&v1, &v2);

    // Fuzz _cmsMAT3solve: solve Ax = b
    // We use a copy of m to avoid modifying original if it's singular
    cmsMAT3 m_copy = m;
    cmsVEC3 b_copy = v3;
    cmsVEC3 x_res;
    _cmsVEC3init(&x_res, 0, 0, 0);
    res_bool = _cmsMAT3solve(&x_res, &m_copy, &b_copy);
    // Result is ignored, just exercising the path

    // Fuzz _cmsVEC3cross: r = u x v
    _cmsVEC3cross(&r_vec, &v1, &v2);

    // Additional state exploration: reuse r_vec as input for next ops
    _cmsVEC3minus(&v3, &r_vec, &v1);
    _cmsMAT3eval(&v3, &m, &r_vec);

    return 0;
}