// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsHPROFILE create_test_profile(cmsContext ctx) {
    // Create a simple RGB profile for testing
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;

    cmsCIExyY primaries[3];
    primaries[0].x = 0.64; primaries[0].y = 0.33; primaries[0].Y = 0.0; // Red
    primaries[1].x = 0.30; primaries[1].y = 0.60; primaries[1].Y = 0.0; // Green
    primaries[2].x = 0.15; primaries[2].y = 0.06; primaries[2].Y = 0.0; // Blue

    // Use cmsBuildGamma instead of cmsBuildToneCurve for compatibility
    cmsToneCurve *gamma = cmsBuildGamma(ctx, 1.0);
    if (gamma == NULL) return NULL;

    cmsToneCurve *curves[3] = {gamma, gamma, gamma};
    cmsHPROFILE h = cmsCreateRGBProfile(&whitePoint, primaries, curves);
    
    cmsFreeToneCurve(gamma);
    return h;
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = create_test_profile(ctx);
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsProfileID profileID;
    cmsUInt8Number buffer[16];
    cmsUInt32Number bytesNeeded = 0;
    void *memPtr = NULL;
    cmsBool result;

    // Fuzz cmsSetDeviceClass
    cmsProfileClassSignature sig = (cmsProfileClassSignature)(Data[0] & 0xFF);
    cmsSetDeviceClass(hProfile, sig);

    // Fuzz cmsMD5computeID
    result = cmsMD5computeID(hProfile);

    // Fuzz cmsGetHeaderProfileID
    if (Size >= 16) {
        memcpy(buffer, Data, 16);
        cmsGetHeaderProfileID(hProfile, buffer);
    } else {
        // Ensure buffer is zeroed if we don't have enough data to copy
        memset(buffer, 0, 16);
        cmsGetHeaderProfileID(hProfile, buffer);
    }

    // Fuzz cmsSetHeaderProfileID
    if (Size >= 16) {
        cmsSetHeaderProfileID(hProfile, (cmsUInt8Number*)Data);
    } else if (Size > 0) {
        // Partial data case
        memset(buffer, 0, 16);
        memcpy(buffer, Data, Size);
        cmsSetHeaderProfileID(hProfile, buffer);
    } else {
        // Zero size case, set to zero ID
        memset(buffer, 0, 16);
        cmsSetHeaderProfileID(hProfile, buffer);
    }

    // Fuzz cmsSaveProfileToMem
    // First call to get size
    cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    
    if (bytesNeeded > 0) {
        memPtr = malloc(bytesNeeded);
        if (memPtr != NULL) {
            result = cmsSaveProfileToMem(hProfile, memPtr, &bytesNeeded);
            // Optionally verify result or use memPtr
            free(memPtr);
        }
    }

    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}