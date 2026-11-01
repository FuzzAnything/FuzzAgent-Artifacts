// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void fuzz_cmsxyY2XYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIExyY)) return;
    cmsCIExyY source;
    cmsCIEXYZ dest;
    memcpy(&source, Data, sizeof(cmsCIExyY));
    
    // Avoid division by zero if y is exactly 0
    if (source.y == 0.0f) {
        source.y = 0.0001f;
    }
    
    cmsxyY2XYZ(&dest, &source);
    (void)dest; // Suppress unused warning
}

static void fuzz_cmsD50_XYZ(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    if (d50) {
        // Just read to ensure no crash
        volatile float x = d50->X;
        volatile float y = d50->Y;
        volatile float z = d50->Z;
        (void)x; (void)y; (void)z;
    }
}

static void fuzz_cmsFloat2XYZEncoded(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIEXYZ)) return;
    cmsCIEXYZ fxyz;
    cmsUInt16Number encoded[3];
    memcpy(&fxyz, Data, sizeof(cmsCIEXYZ));
    
    cmsFloat2XYZEncoded(encoded, &fxyz);
    (void)encoded;
}

static void fuzz_cmsLab2XYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab)) return;
    cmsCIELab lab;
    cmsCIEXYZ xyz;
    const cmsCIEXYZ *whitePoint = NULL;
    
    memcpy(&lab, Data, sizeof(cmsCIELab));
    
    // Try with NULL white point (uses D50)
    cmsLab2XYZ(NULL, &xyz, &lab);
    
    // Try with D50 white point explicitly
    whitePoint = cmsD50_XYZ();
    if (whitePoint) {
        cmsLab2XYZ(whitePoint, &xyz, &lab);
    }
    (void)xyz;
}

static void fuzz_cmsDetectDestinationBlackPoint(const uint8_t *Data, size_t Size) {
    cmsCIEXYZ blackPoint;
    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    
    // The function cmsDetectDestinationBlackPoint expects a valid profile handle.
    // Passing NULL causes a segmentation fault inside the library as it tries to dereference it.
    // Since we cannot easily create a valid profile from raw fuzz data without complex parsing,
    // and the function behavior for NULL is undefined/crashing, we skip this specific call
    // to prevent the crash. Fuzzing this function effectively requires valid ICC profile data.
    // We return early to avoid the crash.
    (void)Data;
    (void)Size;
    (void)blackPoint;
    (void)hProfile;
    (void)intent;
    (void)flags;
}

static void fuzz_cmsXYZEncoded2Float(const uint8_t *Data, size_t Size) {
    if (Size < 3 * sizeof(cmsUInt16Number)) return;
    cmsUInt16Number encoded[3];
    cmsCIEXYZ fxyz;
    
    memcpy(encoded, Data, 3 * sizeof(cmsUInt16Number));
    
    cmsXYZEncoded2Float(&fxyz, encoded);
    (void)fxyz;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    // Initialize lcms context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Fuzz each target function
    fuzz_cmsxyY2XYZ(Data, Size);
    fuzz_cmsD50_XYZ(Data, Size);
    fuzz_cmsFloat2XYZEncoded(Data, Size);
    fuzz_cmsLab2XYZ(Data, Size);
    // fuzz_cmsDetectDestinationBlackPoint is skipped due to NULL profile crash
    fuzz_cmsXYZEncoded2Float(Data, Size);
    
    // Cleanup context
    cmsDeleteContext(ctx);
    
    return 0;
}