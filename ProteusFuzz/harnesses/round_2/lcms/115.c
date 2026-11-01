// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract parameters from fuzz input
    cmsUInt32Number nLUTPoints = 0;
    cmsFloat64Number Bright = 0.0;
    cmsFloat64Number Contrast = 0.0;
    cmsFloat64Number Hue = 0.0;
    cmsFloat64Number Saturation = 0.0;
    cmsUInt32Number TempSrc = 0;
    cmsUInt32Number TempDest = 0;
    cmsFloat64Number Version = 0.0;
    cmsFloat64Number Limit = 0.0;
    cmsUInt32Number Flags = 0;
    cmsColorSpaceSignature ColorSpace = cmsSigCmykData;
    cmsUInt8Number ProfileID[16] = {0};

    // Safe extraction of values from input data
    if (Size >= 4) {
        nLUTPoints = *(const cmsUInt32Number *)(Data);
        Data += 4;
        Size -= 4;
    }
    if (Size >= 8) {
        Bright = *(const cmsFloat64Number *)(Data);
        Data += 8;
        Size -= 8;
    }
    if (Size >= 8) {
        Contrast = *(const cmsFloat64Number *)(Data);
        Data += 8;
        Size -= 8;
    }
    if (Size >= 8) {
        Hue = *(const cmsFloat64Number *)(Data);
        Data += 8;
        Size -= 8;
    }
    if (Size >= 8) {
        Saturation = *(const cmsFloat64Number *)(Data);
        Data += 8;
        Size -= 8;
    }
    if (Size >= 4) {
        TempSrc = *(const cmsUInt32Number *)(Data);
        Data += 4;
        Size -= 4;
    }
    if (Size >= 4) {
        TempDest = *(const cmsUInt32Number *)(Data);
        Data += 4;
        Size -= 4;
    }
    if (Size >= 8) {
        Version = *(const cmsFloat64Number *)(Data);
        Data += 8;
        Size -= 8;
    }
    if (Size >= 8) {
        Limit = *(const cmsFloat64Number *)(Data);
        Data += 8;
        Size -= 8;
    }
    if (Size >= 4) {
        Flags = *(const cmsUInt32Number *)(Data);
        Data += 4;
        Size -= 4;
    }
    if (Size >= 4) {
        ColorSpace = *(const cmsColorSpaceSignature *)(Data);
        Data += 4;
        Size -= 4;
    }
    if (Size >= 16) {
        memcpy(ProfileID, Data, 16);
        Data += 16;
        Size -= 16;
    }

    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Test cmsCreateBCHSWabstractProfile
    cmsHPROFILE hProfile1 = cmsCreateBCHSWabstractProfile(
        nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest
    );
    if (hProfile1 != NULL) {
        cmsSetHeaderFlags(hProfile1, Flags);
        cmsSetProfileVersion(hProfile1, Version);
        cmsSetHeaderProfileID(hProfile1, ProfileID);
        cmsGetHeaderProfileID(hProfile1, ProfileID);
        cleanup_profile(hProfile1);
    }

    // Test cmsCreateInkLimitingDeviceLinkTHR
    cmsHPROFILE hProfile2 = cmsCreateInkLimitingDeviceLinkTHR(
        ctx, ColorSpace, Limit
    );
    if (hProfile2 != NULL) {
        cmsSetHeaderFlags(hProfile2, Flags);
        cmsSetProfileVersion(hProfile2, Version);
        cmsSetHeaderProfileID(hProfile2, ProfileID);
        cmsGetHeaderProfileID(hProfile2, ProfileID);
        cleanup_profile(hProfile2);
    }

    // Cleanup context
    cleanup_context(ctx);

    return 0;
}