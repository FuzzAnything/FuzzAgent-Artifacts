// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Correct signature for _cmsFreeUserDataFn as per lcms2_plugin.h
// It expects (cmsContext, void*)
static void static_free_user_data(cmsContext context, void *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Create a dummy profile for transform creation
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Use standard pixel formats defined in lcms2.h
    cmsUInt32Number inputFormat = TYPE_RGB_8;
    cmsUInt32Number outputFormat = TYPE_RGB_8;

    // Use INTENT_PERCEPTUAL from lcms2.h
    // Fixed: Removed the context argument as cmsCreateTransform does not take it as the first argument
    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, inputFormat, hProfile, outputFormat, INTENT_PERCEPTUAL, 0);
    if (hTransform == NULL) {
        cmsCloseProfile(hProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Cast to internal struct for testing internal APIs
    struct _cmstransform_struct *transform = (struct _cmstransform_struct *)hTransform;

    // Fuzz _cmsGetTransformFormattersFloat
    cmsFormatterFloat fromInputFloat = NULL;
    cmsFormatterFloat toOutputFloat = NULL;
    _cmsGetTransformFormattersFloat(transform, &fromInputFloat, &toOutputFloat);
    _cmsGetTransformFormattersFloat(transform, NULL, NULL);

    // Fuzz _cmsGetTransformUserData
    void *userData = _cmsGetTransformUserData(transform);
    (void)userData;

    // Fuzz _cmsGetTransformMaxWorkers
    cmsInt32Number maxWorkers = _cmsGetTransformMaxWorkers(transform);
    (void)maxWorkers;

    // Fuzz _cmsGetTransformFormatters16
    cmsFormatter16 fromInput16 = NULL;
    cmsFormatter16 toOutput16 = NULL;
    _cmsGetTransformFormatters16(transform, &fromInput16, &toOutput16);
    _cmsGetTransformFormatters16(transform, NULL, NULL);

    // Fuzz _cmsGetTransformWorker
    _cmsTransform2Fn worker = _cmsGetTransformWorker(transform);
    (void)worker;

    // Fuzz _cmsSetTransformUserData
    // Allocate some dummy data from the fuzz input if available
    void *customData = NULL;
    if (Size > 0) {
        customData = malloc(Size);
        if (customData != NULL) {
            memcpy(customData, Data, Size);
            // Pass the context and the custom free function
            _cmsSetTransformUserData(transform, customData, static_free_user_data);
        }
    } else {
        // Set with NULL data and NULL free function
        _cmsSetTransformUserData(transform, NULL, NULL);
    }

    // Cleanup
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}