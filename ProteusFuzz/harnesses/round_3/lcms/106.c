// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2_plugin.h>

// Do not redefine cmsMAT3 or cmsVEC3 as they are in lcms2_plugin.h
// Access members via the actual definition in the library.
// cmsMAT3 typically has a 'Row' member or direct array access depending on version.
// cmsVEC3 typically has 'x', 'y', 'z' or an array.
// We will use the public API or cast to access internals if necessary, 
// but since these are internal functions, we assume the standard lcms2 structure layout.
// In lcms2, cmsMAT3 is usually:
// typedef struct { cmsFloat64Number Row[3][3]; } cmsMAT3;
// And cmsVEC3 is:
// typedef struct { cmsFloat64Number x, y, z; } cmsVEC3;
// The error suggests the header defines them differently or we are redefining them.
// We will remove our typedefs and rely on the header.
// If the header defines them as opaque or differently, we might need to cast.
// However, the error "no member named 'Row'" suggests the header definition might be different.
// Let's inspect the likely structure from lcms2 source if not exposed.
// Actually, lcms2_plugin.h usually exposes the full structs for internal use.
// The error "typedef redefinition" means we removed the typedefs but the code still tries to use them?
// No, the error says we redefined them. So we must remove the typedefs in the source.
// The "no member named 'Row'" error implies the header defines cmsMAT3 differently.
// In some lcms2 versions, cmsMAT3 might be defined as:
// typedef struct { cmsFloat64Number m[3][3]; } cmsMAT3;
// Or it might be a union.
// Let's try to access via a generic array if the member names are unknown, 
// but the safest bet for lcms2 internal structures is that they are arrays.
// Let's assume the header defines them correctly and we just need to stop redefining.
// If the member names are different, we might need to check the specific lcms2 version.
// Common lcms2 internal structure:
// struct cmsMAT3 { cmsFloat64Number Row[3][3]; };
// struct cmsVEC3 { cmsFloat64Number x, y, z; };
// If the error persists about members, we might need to use a union or cast to a known layout.
// But first, let's just remove the typedefs and see if the member names match the header.
// If the header uses 'm' instead of 'Row', we need to adapt.
// Let's assume the standard lcms2 layout and if it fails, we'll adjust.
// Wait, the error says "no member named 'Row'". This means the header defines it differently.
// In lcms2, the internal structure for cmsMAT3 is often:
// typedef struct { cmsFloat64Number Row[3][3]; } cmsMAT3;
// But maybe in this specific build it's different.
// Let's try to use a union to access the raw memory as a 3x3 array of doubles.
// Or, we can just use the raw memory if we know the size.
// Size of cmsMAT3 is 3*3*8 = 72 bytes.
// Size of cmsVEC3 is 3*8 = 24 bytes.
// We can cast to a generic array to set values.

static void fuzz_matrix_from_data(cmsMAT3 *m, const uint8_t *data, size_t size) {
    // Initialize to zero
    memset(m, 0, sizeof(cmsMAT3));
    
    // We need to set the 9 doubles.
    // Since we don't know the exact member name (Row vs m), we cast to double array.
    // This is safe if the struct is just an array of doubles.
    double *vals = (double *)m;
    
    size_t offset = 0;
    for (int i = 0; i < 9; i++) {
        if (offset + 8 <= size) {
            double val;
            memcpy(&val, data + offset, 8);
            vals[i] = val;
            offset += 8;
        } else {
            vals[i] = (double)(i * 17) / 100.0;
        }
    }
}

static void fuzz_vector_from_data(cmsVEC3 *v, const uint8_t *data, size_t size, int seed) {
    memset(v, 0, sizeof(cmsVEC3));
    
    double *vals = (double *)v;
    size_t offset = 0;
    for (int i = 0; i < 3; i++) {
        if (offset + 8 <= size) {
            double val;
            memcpy(&val, data + offset, 8);
            vals[i] = val;
            offset += 8;
        } else {
            vals[i] = (double)(seed + i) / 10.0;
        }
    }
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsMAT3 m1, m2, m3, m_inv;
    cmsVEC3 v1, v2, v_result;
    cmsBool result;

    // Initialize matrices and vectors with fuzz data
    fuzz_matrix_from_data(&m1, Data, Size);
    fuzz_matrix_from_data(&m2, Data, Size);
    fuzz_matrix_from_data(&m3, Data, Size);
    fuzz_vector_from_data(&v1, Data, Size, 1);
    fuzz_vector_from_data(&v2, Data, Size, 2);

    // 1. Test _cmsMAT3identity
    _cmsMAT3identity(&m1);
    result = _cmsMAT3isIdentity(&m1);
    (void)result;

    // 2. Test _cmsMAT3isIdentity on various matrices
    _cmsMAT3identity(&m2);
    result = _cmsMAT3isIdentity(&m2);
    (void)result;
    
    // Modify m2 slightly to test non-identity
    // Accessing via cast again to avoid member name issues
    double *m2_vals = (double *)&m2;
    m2_vals[0] += 0.0001;
    result = _cmsMAT3isIdentity(&m2);
    (void)result;

    // 3. Test _cmsMAT3per (matrix multiplication)
    _cmsMAT3per(&m3, &m1, &m2);
    _cmsMAT3per(&m3, &m2, &m1);
    _cmsMAT3per(&m3, &m1, &m1);

    // 4. Test _cmsMAT3inverse
    _cmsMAT3identity(&m1);
    result = _cmsMAT3inverse(&m1, &m_inv);
    (void)result;
    
    result = _cmsMAT3inverse(&m2, &m_inv);
    (void)result;

    // 5. Test _cmsMAT3solve
    _cmsMAT3identity(&m1);
    result = _cmsMAT3solve(&v_result, &m1, &v1);
    (void)result;
    
    result = _cmsMAT3solve(&v_result, &m2, &v1);
    (void)result;

    // 6. Test _cmsMAT3eval
    _cmsMAT3eval(&v_result, &m1, &v1);
    _cmsMAT3eval(&v_result, &m2, &v1);
    _cmsMAT3eval(&v_result, &m3, &v2);

    // Edge cases
    memset(&m1, 0, sizeof(cmsMAT3));
    result = _cmsMAT3inverse(&m1, &m_inv);
    (void)result;
    result = _cmsMAT3solve(&v_result, &m1, &v1);
    (void)result;
    result = _cmsMAT3isIdentity(&m1);
    (void)result;

    return 0;
}