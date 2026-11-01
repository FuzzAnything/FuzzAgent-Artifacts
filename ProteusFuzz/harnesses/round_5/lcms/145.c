// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void* dummy_user_data = (void*)0xDEADBEEF;

static void cleanup_dummy(cmsContext ContextID, void* ptr) {
    (void)ContextID;
    (void)ptr;
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsHPROFILE hProfileIn = cmsCreate_sRGBProfile();
    cmsHPROFILE hProfileOut = cmsCreate_sRGBProfile();
    
    if (!hProfileIn || !hProfileOut) {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsUInt32Number inputFmt = cmsSigRgbData | 8;
    cmsUInt32Number outputFmt = cmsSigRgbData | 8;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = 0;
    
    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfileIn, inputFmt, hProfileOut, outputFmt, intent, flags);
    
    if (!hTransform) {
        cmsCloseProfile(hProfileIn);
        cmsCloseProfile(hProfileOut);
        cmsDeleteContext(ctx);
        return 0;
    }

    struct _cmstransform_struct* transform = (struct _cmstransform_struct*)hTransform;

    cmsFormatterFloat fromInputFloat = NULL;
    cmsFormatterFloat toOutputFloat = NULL;
    _cmsGetTransformFormattersFloat(transform, &fromInputFloat, &toOutputFloat);
    _cmsGetTransformFormattersFloat(transform, NULL, NULL);
    _cmsGetTransformFormattersFloat(transform, &fromInputFloat, NULL);
    _cmsGetTransformFormattersFloat(transform, NULL, &toOutputFloat);

    void* userData = _cmsGetTransformUserData(transform);
    (void)userData;

    cmsInt32Number maxWorkers = _cmsGetTransformMaxWorkers(transform);
    (void)maxWorkers;

    cmsFormatter16 fromInput16 = NULL;
    cmsFormatter16 toOutput16 = NULL;
    _cmsGetTransformFormatters16(transform, &fromInput16, &toOutput16);
    _cmsGetTransformFormatters16(transform, NULL, NULL);
    _cmsGetTransformFormatters16(transform, &fromInput16, NULL);
    _cmsGetTransformFormatters16(transform, NULL, &toOutput16);

    _cmsTransform2Fn worker = _cmsGetTransformWorker(transform);
    (void)worker;

    _cmsSetTransformUserData(transform, dummy_user_data, cleanup_dummy);
    _cmsSetTransformUserData(transform, (void*)0x12345678, NULL);
    _cmsSetTransformUserData(transform, NULL, cleanup_dummy);

    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hProfileIn);
    cmsCloseProfile(hProfileOut);
    cmsDeleteContext(ctx);

    return 0;
}