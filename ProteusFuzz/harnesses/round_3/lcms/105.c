// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3dot at cmsmtrx.c:58:28 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsVEC3minus at cmsmtrx.c:42:16 in lcms2_plugin.h
// _cmsVEC3cross at cmsmtrx.c:50:16 in lcms2_plugin.h
// _cmsVEC3init at cmsmtrx.c:34:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lcms2_plugin.h"

/*
 * Helper to extract a cmsFloat64Number from the fuzz input buffer.
 * Returns 0.0 if not enough data or if the value is NaN/Inf.
 */
static cmsFloat64Number extract_double(const uint8_t **data, size_t *size) {
    if (*size < sizeof(double)) {
        return 0.0;
    }
    double val;
    memcpy(&val, *data, sizeof(double));
    *data += sizeof(double);
    *size -= sizeof(double);

    if (!isfinite(val)) {
        return 0.0;
    }
    return (cmsFloat64Number)val;
}

/*
 * Helper to extract a cmsVEC3 from the fuzz input buffer.
 * Returns 0 if not enough data.
 */
static int extract_vec3(const uint8_t **data, size_t *size, cmsVEC3 *v) {
    if (*size < 3 * sizeof(double)) {
        return 0;
    }
    double x = extract_double(data, size);
    double y = extract_double(data, size);
    double z = extract_double(data, size);
    _cmsVEC3init(v, x, y, z);
    return 1;
}

/*
 * Helper to extract a cmsMAT3 from the fuzz input buffer.
 * A 3x3 matrix has 9 doubles.
 * Returns 0 if not enough data.
 * Note: cmsMAT3 is an opaque struct in public headers, but lcms2_plugin.h
 * usually exposes the internal layout for plugin development or we must
 * rely on the fact that _cmsMAT3solve/_cmsMAT3eval expect a specific layout.
 * Since we cannot access 'n' directly if it's hidden, we rely on the fact
 * that in the context of lcms2_plugin.h (internal build), the struct is often:
 * typedef struct { cmsFloat64Number n[3][3]; } cmsMAT3;
 * If compilation fails on 'n', it implies the struct is opaque in this specific
 * build configuration. However, the error message explicitly said 'no member named n'.
 * This suggests the struct definition in the included header might be different or opaque.
 * 
 * WORKAROUND: Since we cannot access members directly if opaque, we must assume
 * the user has a specific internal header or we must use a union/cast if we know
 * the layout. Given the error, the struct is likely defined as:
 * struct cmsMAT3 { cmsFloat64Number n[3][3]; };
 * But the compiler says 'n' doesn't exist.
 * 
 * Let's check standard lcms2 internal definitions. Usually, it is:
 * typedef struct { cmsFloat64Number n[3][3]; } cmsMAT3;
 * 
 * If the error persists, it might be that the header provided defines it differently.
 * However, without the ability to modify the library, we must assume the standard
 * internal layout is expected for these internal APIs (_cms...).
 * 
 * Re-evaluating the error: "no member named 'n' in 'cmsMAT3'".
 * This implies the struct definition visible to the compiler does not have 'n'.
 * Perhaps it is defined as an array directly? Or the header is different.
 * 
 * Let's try to initialize it by casting to a double array if the struct is opaque
 * but we know the size, OR assume the struct is actually:
 * typedef struct { cmsFloat64Number n[3][3]; } cmsMAT3;
 * 
 * Wait, if the error is real, maybe the struct is defined as:
 * struct cmsMAT3 { cmsFloat64Number n[9]; }; // flattened?
 * Or maybe the header included is not the internal one.
 * 
 * However, the prompt says "You need to include lcms2_plugin.h".
 * In standard lcms2 source, cmsMAT3 is:
 * typedef struct { cmsFloat64Number n[3][3]; } cmsMAT3;
 * 
 * If the compiler complains, it's possible the environment has a different definition.
 * But since I must fix the code to compile, and I cannot see the actual header,
 * I will assume the struct is opaque and I cannot access members.
 * 
 * BUT, the functions _cmsMAT3solve and _cmsMAT3eval require the matrix to be populated.
 * If I cannot populate it, I cannot fuzz the matrix logic.
 * 
 * Hypothesis: The struct might be defined as `cmsFloat64Number n[3][3]` inside a struct
 * but the name of the member is different? No, standard is 'n'.
 * 
 * Alternative: Maybe the struct is defined as `cmsFloat64Number n[9]`?
 * 
 * Let's try a different approach. If `cmsMAT3` is opaque, we can't initialize it.
 * But the task requires fuzzing.
 * 
 * Let's assume the standard lcms2 internal definition is correct and the error
 * might be due to a specific build flag or version.
 * However, to strictly fix the compilation error "no member named 'n'",
 * we must stop accessing `.n`.
 * 
 * Is it possible the struct is just `typedef cmsFloat64Number cmsMAT3[3][3];`?
 * No, it's a struct.
 * 
 * Let's look at the lcms2 source code (common knowledge):
 * typedef struct { cmsFloat64Number n[3][3]; } cmsMAT3;
 * 
 * If the error is real, maybe the header `lcms2_plugin.h` in this specific environment
 * defines it differently.
 * 
 * Let's try to assume the struct is `cmsFloat64Number n[3][3]` but maybe the member name is not `n`?
 * Or maybe it's `m`?
 * 
 * Actually, in some versions or configurations, it might be:
 * typedef struct { cmsFloat64Number n[3][3]; } cmsMAT3;
 * 
 * If I cannot access `n`, I cannot set the values.
 * 
 * WAIT! Maybe the struct is defined as:
 * typedef struct { cmsFloat64Number n[3][3]; } cmsMAT3;
 * But the error says "no member named 'n'".
 * 
 * Could it be that the struct is defined as:
 * typedef struct { cmsFloat64Number n[9]; } cmsMAT3;
 * And I am trying to access `n[i][j]` which is invalid if it's 1D?
 * 
 * Let's try to use a union or cast to `double*` to fill the memory directly,
 * assuming the struct is just a container for 9 doubles.
 * This is a common technique when struct layout is known but member names are hidden.
 * 
 * We will cast the pointer to `double*` and write 9 doubles.
 */
static int extract_mat3(const uint8_t **data, size_t *size, cmsMAT3 *m) {
    if (*size < 9 * sizeof(double)) {
        return 0;
    }
    
    // Cast to double pointer to bypass struct member access issues
    // Assuming cmsMAT3 contains 9 doubles in row-major order
    double *mat_data = (double *)m;
    
    for (int i = 0; i < 9; i++) {
        double val = extract_double(data, size);
        mat_data[i] = val;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    cmsVEC3 v1, v2, v3, v4, v5, res_vec;
    cmsMAT3 mat;
    cmsBool solve_result;

    /* Extract vectors */
    if (!extract_vec3(&ptr, &remaining, &v1)) return 0;
    if (!extract_vec3(&ptr, &remaining, &v2)) return 0;
    if (!extract_vec3(&ptr, &remaining, &v3)) return 0;

    /* Extract matrix */
    if (!extract_mat3(&ptr, &remaining, &mat)) return 0;
    
    /* Extract another vector for matrix operations */
    if (!extract_vec3(&ptr, &remaining, &v4)) return 0;

    /* 1. _cmsVEC3minus: r = a - b */
    _cmsVEC3minus(&res_vec, &v1, &v2);
    _cmsVEC3minus(&res_vec, &v2, &v3);
    _cmsVEC3minus(&res_vec, &v1, &v1);

    /* 2. _cmsVEC3dot: dot product */
    cmsFloat64Number dot1 = _cmsVEC3dot(&v1, &v2);
    cmsFloat64Number dot2 = _cmsVEC3dot(&v2, &v3);
    cmsFloat64Number dot3 = _cmsVEC3dot(&v1, &v1);
    (void)dot1; (void)dot2; (void)dot3;

    /* 3. _cmsVEC3init: Re-initialize if data available */
    if (remaining >= 3 * sizeof(double)) {
        double x = extract_double(&ptr, &remaining);
        double y = extract_double(&ptr, &remaining);
        double z = extract_double(&ptr, &remaining);
        _cmsVEC3init(&v5, x, y, z);
        _cmsVEC3minus(&res_vec, &v5, &v1);
        _cmsVEC3dot(&v5, &v2);
    }

    /* 4. _cmsMAT3solve: Ax = b */
    solve_result = _cmsMAT3solve(&res_vec, &mat, &v4);
    (void)solve_result;

    /* 5. _cmsVEC3cross: r = u x v */
    _cmsVEC3cross(&res_vec, &v1, &v2);
    _cmsVEC3cross(&res_vec, &v2, &v3);
    _cmsVEC3cross(&res_vec, &v1, &v1);

    /* 6. _cmsMAT3eval: r = A * v */
    _cmsMAT3eval(&res_vec, &mat, &v4);
    _cmsMAT3eval(&res_vec, &mat, &v1);
    _cmsMAT3eval(&res_vec, &mat, &v2);

    /* Additional diversity */
    _cmsVEC3init(&v3, 1.0, 1.0, 1.0);
    _cmsMAT3eval(&v3, &mat, &v3);
    _cmsVEC3minus(&v3, &v3, &v1);
    _cmsVEC3cross(&res_vec, &v3, &v2);

    return 0;
}