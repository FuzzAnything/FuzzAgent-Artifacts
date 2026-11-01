// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Correct signature for _cmsFreeUserDataFn: void (*)(struct _cmsContext_struct *, void *)
static void static_free_user_data(struct _cmsContext_struct *ctx, void *ptr) {
    (void)ctx; // Unused context parameter
    if (ptr) {
        free(ptr);
    }
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB-like profile using default parameters
    cmsHPROFILE hProfile = cmsCreateRGBProfile(NULL, NULL, NULL);
    if (!hProfile) {
        return NULL;
    }
    return hProfile;
}

static cmsHTRANSFORM create_dummy_transform(cmsContext ctx) {
    cmsHPROFILE hProfileIn = create_dummy_profile(ctx);
    cmsHPROFILE hProfileOut = create_dummy_profile(ctx);

    if (!hProfileIn || !hProfileOut) {
        if (hProfileIn) cmsCloseProfile(hProfileIn);
        if (hProfileOut) cmsCloseProfile(hProfileOut);
        return NULL;
    }

    cmsUInt32Number intent = 0; // cmsINTENT_PERCEPTUAL is 0
    cmsUInt32Number flags = 0;  // cmsFLAGS_DEFAULTINTENT is 0

    cmsHTRANSFORM tr = cmsCreateTransform(hProfileIn, cmsSigRgbData,
                                          hProfileOut, cmsSigRgbData,
                                          intent, flags);
    
    cmsCloseProfile(hProfileIn);
    cmsCloseProfile(hProfileOut);
    return tr;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsHTRANSFORM transform = create_dummy_transform(ctx);
    if (!transform) {
        cmsDeleteContext(ctx);
        return 0;
    }

    struct _cmstransform_struct* CMMcargo = (struct _cmstransform_struct*)transform;

    // Fuzz _cmsSetTransformUserData
    if (Size >= 8) {
        void* user_data = malloc(Size);
        if (user_data) {
            memcpy(user_data, Data, Size);
            _cmsSetTransformUserData(CMMcargo, user_data, static_free_user_data);
        }
    }

    // Fuzz _cmsGetTransformUserData
    void* retrieved_data = _cmsGetTransformUserData(CMMcargo);
    (void)retrieved_data;

    // Fuzz _cmsGetTransformFormatters16
    cmsFormatter16 from_input_16 = NULL;
    cmsFormatter16 to_output_16 = NULL;
    _cmsGetTransformFormatters16(CMMcargo, &from_input_16, &to_output_16);
    // Also test with NULL pointers
    _cmsGetTransformFormatters16(CMMcargo, NULL, NULL);

    // Fuzz _cmsGetTransformWorker
    _cmsTransform2Fn worker = _cmsGetTransformWorker(CMMcargo);
    (void)worker;

    // Fuzz _cmsGetTransformMaxWorkers
    cmsInt32Number max_workers = _cmsGetTransformMaxWorkers(CMMcargo);
    (void)max_workers;

    // Fuzz _cmsGetTransformFormattersFloat
    cmsFormatterFloat from_input_float = NULL;
    cmsFormatterFloat to_output_float = NULL;
    _cmsGetTransformFormattersFloat(CMMcargo, &from_input_float, &to_output_float);
    // Also test with NULL pointers
    _cmsGetTransformFormattersFloat(CMMcargo, NULL, NULL);

    // Cleanup
    cmsDeleteTransform(transform);
    cmsDeleteContext(ctx);

    return 0;
}