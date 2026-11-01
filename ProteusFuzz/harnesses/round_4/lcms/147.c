// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Correct signature for _cmsFreeUserDataFn based on error message: void (*)(struct _cmsContext_struct *, void *)
static void dummy_free_fn(cmsContext ctx, void *ptr) {
    (void)ctx;
    if (ptr) {
        free(ptr);
    }
}

static cmsHTRANSFORM create_dummy_transform(cmsContext ctx) {
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hDestProfile = NULL;
    cmsHTRANSFORM t = NULL;

    // Create placeholder profiles
    hProfile = cmsCreateProfilePlaceholder(ctx);
    hDestProfile = cmsCreateProfilePlaceholder(ctx);

    if (hProfile && hDestProfile) {
        // Use defined constants from lcms2.h
        // Fixed: Removed the 7th argument (ctx) which was not expected by the function signature
        t = cmsCreateTransform(hProfile, TYPE_RGB_8, hDestProfile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    }

    // Use cmsCloseProfile instead of cmsDeleteProfile
    if (hProfile) cmsCloseProfile(hProfile);
    if (hDestProfile) cmsCloseProfile(hDestProfile);

    return t;
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHTRANSFORM transform = NULL;
    
    if (ctx) {
        transform = create_dummy_transform(ctx);
    } else {
        transform = create_dummy_transform(NULL);
    }

    if (!transform) {
        if (ctx) cmsDeleteContext(ctx);
        return 0;
    }

    struct _cmstransform_struct* internal_transform = (struct _cmstransform_struct*)transform;

    // Fuzz _cmsGetTransformUserData
    void* userData = _cmsGetTransformUserData(internal_transform);
    (void)userData;

    // Fuzz _cmsGetTransformFlags
    cmsUInt32Number flags = _cmsGetTransformFlags(internal_transform);
    (void)flags;

    // Fuzz _cmsGetTransformFormattersFloat
    cmsFormatterFloat fromInput = NULL;
    cmsFormatterFloat toOutput = NULL;
    _cmsGetTransformFormattersFloat(internal_transform, &fromInput, &toOutput);
    (void)fromInput;
    (void)toOutput;

    // Fuzz _cmsSetTransformUserData
    void* dummyData = NULL;
    if (Size > 0) {
        dummyData = malloc(Size);
        if (dummyData) {
            memcpy(dummyData, Data, Size);
            _cmsSetTransformUserData(internal_transform, dummyData, dummy_free_fn);
        }
    } else {
        dummyData = malloc(1);
        if (dummyData) {
            memset(dummyData, 0, 1);
            _cmsSetTransformUserData(internal_transform, dummyData, dummy_free_fn);
        }
    }

    // Verify userData was set
    void* retrievedData = _cmsGetTransformUserData(internal_transform);
    (void)retrievedData;

    // Fuzz _cmsGetTransformWorker
    _cmsTransform2Fn worker = _cmsGetTransformWorker(internal_transform);
    (void)worker;

    // Fuzz _cmsGetTransformWorkerFlags
    cmsUInt32Number workerFlags = _cmsGetTransformWorkerFlags(internal_transform);
    (void)workerFlags;

    // Cleanup
    cmsDeleteTransform(transform);
    if (ctx) {
        cmsDeleteContext(ctx);
    }

    return 0;
}