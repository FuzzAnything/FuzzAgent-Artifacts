// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <lcms2.h>

// Helper to extract a float from the fuzzer input buffer
static float extract_float(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + sizeof(float) > size) {
        return 0.0f;
    }
    float val;
    memcpy(&val, data + *offset, sizeof(float));
    *offset += sizeof(float);
    return val;
}

// Helper to extract a uint16 from the fuzzer input buffer
static cmsUInt16Number extract_uint16(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + sizeof(cmsUInt16Number) > size) {
        return 0;
    }
    cmsUInt16Number val;
    memcpy(&val, data + *offset, sizeof(cmsUInt16Number));
    *offset += sizeof(cmsUInt16Number);
    return val;
}

// Helper to generate a random-ish float based on input bytes to avoid NaN/Inf if possible
static float safe_float_from_bytes(uint32_t bytes) {
    float f;
    memcpy(&f, &bytes, sizeof(float));
    // Clamp to reasonable range to avoid immediate NaN/Inf in some math ops, 
    // though the API functions should handle them.
    if (isnan(f) || isinf(f)) {
        return 0.0f;
    }
    return f;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0; // Need at least 3 floats for a Lab struct

    size_t offset = 0;
    
    // Initialize structures
    cmsCIELab LabIn = {0};
    cmsCIELab LabOut = {0};
    cmsCIEXYZ XYZOut = {0};
    cmsCIEXYZ XYZIn = {0};
    cmsUInt16Number wLab[3] = {0};
    cmsUInt16Number wXYZ[3] = {0};
    cmsCIEXYZ WhitePoint = {0};

    // Extract Lab components (L, a, b) from input
    if (offset + 3 * sizeof(float) <= Size) {
        LabIn.L = extract_float(Data, &offset, Size);
        LabIn.a = extract_float(Data, &offset, Size);
        LabIn.b = extract_float(Data, &offset, Size);
    }

    // Extract WhitePoint components (X, Y, Z) if available
    if (offset + 3 * sizeof(float) <= Size) {
        WhitePoint.X = extract_float(Data, &offset, Size);
        WhitePoint.Y = extract_float(Data, &offset, Size);
        WhitePoint.Z = extract_float(Data, &offset, Size);
    }

    // Extract encoded Lab values if available
    if (offset + 3 * sizeof(cmsUInt16Number) <= Size) {
        wLab[0] = extract_uint16(Data, &offset, Size);
        wLab[1] = extract_uint16(Data, &offset, Size);
        wLab[2] = extract_uint16(Data, &offset, Size);
    }

    // Extract encoded XYZ values if available
    if (offset + 3 * sizeof(cmsUInt16Number) <= Size) {
        wXYZ[0] = extract_uint16(Data, &offset, Size);
        wXYZ[1] = extract_uint16(Data, &offset, Size);
        wXYZ[2] = extract_uint16(Data, &offset, Size);
    }

    // 1. cmsFloat2LabEncoded
    cmsFloat2LabEncoded(wLab, &LabIn);

    // 2. cmsFloat2LabEncodedV2
    cmsFloat2LabEncodedV2(wLab, &LabIn);

    // 3. cmsLabEncoded2FloatV2
    cmsLabEncoded2FloatV2(&LabOut, wLab);

    // 4. cmsLabEncoded2Float
    cmsLabEncoded2Float(&LabOut, wLab);

    // 5. cmsLab2XYZ
    // Test with provided white point
    cmsLab2XYZ(&WhitePoint, &XYZOut, &LabIn);
    // Test with NULL white point (uses D50)
    cmsLab2XYZ(NULL, &XYZOut, &LabIn);

    // 6. cmsXYZEncoded2Float
    cmsXYZEncoded2Float(&XYZOut, wXYZ);

    // Additional exploration: Mix and match to ensure state changes
    // Convert Lab -> Encoded -> Float -> XYZ
    cmsFloat2LabEncodedV2(wLab, &LabIn);
    cmsLabEncoded2Float(&LabOut, wLab);
    cmsLab2XYZ(&WhitePoint, &XYZOut, &LabOut);
    
    // Convert XYZ Encoded -> Float
    cmsXYZEncoded2Float(&XYZOut, wXYZ);

    return 0;
}