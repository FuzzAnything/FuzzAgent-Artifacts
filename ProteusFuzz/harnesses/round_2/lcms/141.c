// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void dummy_free_fn(cmsContext ctx, void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Initialize default white point and primaries for RGB profile
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127f;
    whitePoint.y = 0.3290f;
    whitePoint.Y = 1.0f;

    cmsCIExyYTRIPLE primaries;
    // Red
    primaries.Red.x = 0.6400f;
    primaries.Red.y = 0.3300f;
    primaries.Red.Y = 1.0f;
    // Green
    primaries.Green.x = 0.3000f;
    primaries.Green.y = 0.6000f;
    primaries.Green.Y = 1.0f;
    // Blue
    primaries.Blue.x = 0.1500f;
    primaries.Blue.y = 0.0600f;
    primaries.Blue.Y = 1.0f;

    cmsToneCurve *transfer[3] = {NULL, NULL, NULL};

    cmsHPROFILE hInput = cmsCreateRGBProfile(&whitePoint, &primaries, transfer);
    cmsHPROFILE hOutput = cmsCreateRGBProfile(&whitePoint, &primaries, transfer);

    if (!hInput || !hOutput) {
        if (hInput) cmsCloseProfile(hInput);
        if (hOutput) cmsCloseProfile(hOutput);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Use standard pixel types instead of cmsGetPixelType
    cmsUInt32Number inputFormat = TYPE_RGB_8;
    cmsUInt32Number outputFormat = TYPE_RGB_8;

    cmsUInt32Number flags = 0;
    // Use defined constant for intent
    cmsUInt32Number intent = INTENT_PERCEPTUAL;

    cmsHTRANSFORM trans = cmsCreateTransform(hInput, inputFormat, hOutput, outputFormat, intent, flags);

    if (!trans) {
        cmsCloseProfile(hInput);
        cmsCloseProfile(hOutput);
        cmsDeleteContext(ctx);
        return 0;
    }

    struct _cmstransform_struct *CMMcargo = (struct _cmstransform_struct *)trans;

    // Fuzz _cmsGetTransformFormatters16
    cmsFormatter16 fromInput = NULL;
    cmsFormatter16 toOutput = NULL;
    _cmsGetTransformFormatters16(CMMcargo, &fromInput, &toOutput);
    _cmsGetTransformFormatters16(CMMcargo, NULL, NULL);
    _cmsGetTransformFormatters16(CMMcargo, &fromInput, NULL);
    _cmsGetTransformFormatters16(CMMcargo, NULL, &toOutput);

    // Fuzz _cmsGetTransformWorkerFlags
    cmsUInt32Number workerFlags = _cmsGetTransformWorkerFlags(CMMcargo);
    (void)workerFlags;

    // Fuzz _cmsGetTransformFlags
    cmsUInt32Number originalFlags = _cmsGetTransformFlags(CMMcargo);
    (void)originalFlags;

    // Fuzz _cmsSetTransformUserData and _cmsGetTransformUserData
    void *userData = malloc(Size > 0 ? Size : 1);
    if (userData) {
        memcpy(userData, Data, Size > 0 ? Size : 1);
        _cmsSetTransformUserData(CMMcargo, userData, dummy_free_fn);
        _cmsSetTransformUserData(CMMcargo, userData, NULL);
        void *retrieved = _cmsGetTransformUserData(CMMcargo);
        (void)retrieved;
    }

    // Fuzz _cmsGetTransformWorker
    _cmsTransform2Fn worker = _cmsGetTransformWorker(CMMcargo);
    (void)worker;

    cmsDeleteTransform(trans);
    cmsCloseProfile(hInput);
    cmsCloseProfile(hOutput);
    cmsDeleteContext(ctx);

    return 0;
}