// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

static void safe_fuzz_float(float *val, const uint8_t *data, size_t offset) {
    if (offset + sizeof(float) <= data[0] && data[0] < SIZE_MAX) {
        memcpy(val, data + offset, sizeof(float));
    } else {
        *val = 0.0f;
    }
}

static void safe_fuzz_uint16(cmsUInt16Number *val, const uint8_t *data, size_t offset) {
    if (offset + sizeof(cmsUInt16Number) <= data[0] && data[0] < SIZE_MAX) {
        memcpy(val, data + offset, sizeof(cmsUInt16Number));
    } else {
        *val = 0;
    }
}

static void fuzz_cmsCIELab(cmsCIELab *lab, const uint8_t *data, size_t offset) {
    safe_fuzz_float(&lab->L, data, offset);
    safe_fuzz_float(&lab->a, data, offset + sizeof(float));
    safe_fuzz_float(&lab->b, data, offset + 2 * sizeof(float));
}

static void fuzz_cmsCIEXYZ(cmsCIEXYZ *xyz, const uint8_t *data, size_t offset) {
    safe_fuzz_float(&xyz->X, data, offset);
    safe_fuzz_float(&xyz->Y, data, offset + sizeof(float));
    safe_fuzz_float(&xyz->Z, data, offset + 2 * sizeof(float));
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsCIELab lab;
    cmsCIEXYZ xyz;
    cmsCIEXYZ whitePoint;
    cmsUInt16Number wLab[3];
    cmsUInt16Number wXYZ[3];
    cmsUInt16Number wLabOut[3];

    // Initialize structures to zero to avoid undefined behavior
    memset(&lab, 0, sizeof(lab));
    memset(&xyz, 0, sizeof(xyz));
    memset(&whitePoint, 0, sizeof(whitePoint));
    memset(wLab, 0, sizeof(wLab));
    memset(wXYZ, 0, sizeof(wXYZ));
    memset(wLabOut, 0, sizeof(wLabOut));

    // Fuzz input data based on available size
    if (Size >= 1 + 3 * sizeof(cmsUInt16Number)) {
        for (int i = 0; i < 3; i++) {
            safe_fuzz_uint16(&wLab[i], Data, 1 + i * sizeof(cmsUInt16Number));
            safe_fuzz_uint16(&wXYZ[i], Data, 1 + 3 * sizeof(cmsUInt16Number) + i * sizeof(cmsUInt16Number));
        }
    }

    if (Size >= 1 + 3 * sizeof(float)) {
        fuzz_cmsCIELab(&lab, Data, 1);
        fuzz_cmsCIEXYZ(&whitePoint, Data, 1 + 3 * sizeof(float));
    }

    // Test cmsLabEncoded2FloatV2
    cmsLabEncoded2FloatV2(&lab, wLab);

    // Test cmsLabEncoded2Float
    cmsLabEncoded2Float(&lab, wLab);

    // Test cmsXYZEncoded2Float
    cmsXYZEncoded2Float(&xyz, wXYZ);

    // Test cmsFloat2LabEncodedV2
    cmsFloat2LabEncodedV2(wLabOut, &lab);

    // Test cmsFloat2LabEncoded
    cmsFloat2LabEncoded(wLabOut, &lab);

    // Test cmsLab2XYZ with valid white point
    cmsLab2XYZ(&whitePoint, &xyz, &lab);

    // Test cmsLab2XYZ with NULL white point (uses D50)
    cmsLab2XYZ(NULL, &xyz, &lab);

    return 0;
}