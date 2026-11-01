// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsContext CreateContext(void) {
    return cmsCreateContext(NULL, NULL);
}

static void CleanupContext(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static cmsHPROFILE CreateDummyProfile(cmsContext ctx) {
    // Define default white point (D65)
    cmsCIExyY WhitePoint = {0.3127f, 0.3290f, 1.0f};
    
    // Define default primaries (sRGB)
    cmsCIExyYTRIPLE Primaries = {
        {0.640f, 0.330f, 1.0f}, // Red
        {0.300f, 0.600f, 1.0f}, // Green
        {0.150f, 0.060f, 1.0f}  // Blue
    };

    // Create identity tone curves using cmsBuildGamma
    cmsToneCurve *TransferFunction[3] = {
        cmsBuildGamma(ctx, 1.0),
        cmsBuildGamma(ctx, 1.0),
        cmsBuildGamma(ctx, 1.0)
    };

    if (TransferFunction[0] == NULL || TransferFunction[1] == NULL || TransferFunction[2] == NULL) {
        if (TransferFunction[0]) cmsFreeToneCurve(TransferFunction[0]);
        if (TransferFunction[1]) cmsFreeToneCurve(TransferFunction[1]);
        if (TransferFunction[2]) cmsFreeToneCurve(TransferFunction[2]);
        return NULL;
    }

    cmsHPROFILE hProfile = cmsCreateRGBProfile(&WhitePoint, &Primaries, TransferFunction);

    // Clean up tone curves regardless of profile creation success
    if (TransferFunction[0]) cmsFreeToneCurve(TransferFunction[0]);
    if (TransferFunction[1]) cmsFreeToneCurve(TransferFunction[1]);
    if (TransferFunction[2]) cmsFreeToneCurve(TransferFunction[2]);

    return hProfile;
}

static void CleanupProfile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = CreateContext();
    if (ctx == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = CreateDummyProfile(ctx);
    if (hProfile == NULL) {
        CleanupContext(ctx);
        return 0;
    }

    // Step 1: First call to cmsGetPostScriptCSA to get required size
    cmsUInt32Number Intent = (Size > 0) ? Data[0] % 4 : 0;
    cmsUInt32Number dwFlags = (Size > 1) ? Data[1] % 256 : 0;
    
    cmsUInt32Number requiredSize = cmsGetPostScriptCSA(ctx, hProfile, Intent, dwFlags, NULL, 0);
    
    if (requiredSize == 0) {
        CleanupProfile(hProfile);
        CleanupContext(ctx);
        return 0;
    }

    // Step 2: Call _cmsMalloc to allocate memory
    void *ptr = _cmsMalloc(ctx, requiredSize);
    if (ptr == NULL) {
        CleanupProfile(hProfile);
        CleanupContext(ctx);
        return 0;
    }

    // Step 3: Second call to cmsGetPostScriptCSA to fill the buffer
    cmsUInt32Number written = cmsGetPostScriptCSA(ctx, hProfile, Intent, dwFlags, ptr, requiredSize);
    (void)written; // Use the return value to ensure it's computed

    // Step 4: Call _cmsFree to deallocate memory
    _cmsFree(ctx, ptr);

    CleanupProfile(hProfile);
    CleanupContext(ctx);

    return 0;
}