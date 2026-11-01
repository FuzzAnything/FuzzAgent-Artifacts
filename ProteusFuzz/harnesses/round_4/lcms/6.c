// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void* dummy_user_data = NULL;

// The _cmsFreeUserDataFn type expects (cmsContext, void*)
static void dummy_free_fn(cmsContext ctx, void* ptr) {
    (void)ctx;
    if (ptr) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Create dummy profiles from input data
    cmsHPROFILE hIn = cmsCreateNULLProfile();
    cmsHPROFILE hOut = cmsCreateNULLProfile();

    if (!hIn || !hOut) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Use input data to determine transform parameters
    uint16_t intent = Data[0] % 4;
    uint16_t flags = Data[1] % 8;

    // cmsCreateTransform signature: (Input, InputFormat, Output, OutputFormat, Intent, Flags)
    cmsHTRANSFORM hTransform = cmsCreateTransform(hIn, TYPE_RGBA_8, hOut, TYPE_RGBA_8, intent, flags);

    if (!hTransform) {
        cmsCloseProfile(hIn);
        cmsCloseProfile(hOut);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Fuzz _cmsGetTransformMaxWorkers
    cmsInt32Number maxWorkers = _cmsGetTransformMaxWorkers(hTransform);
    (void)maxWorkers;

    // Fuzz _cmsGetTransformFlags
    cmsUInt32Number originalFlags = _cmsGetTransformFlags(hTransform);
    (void)originalFlags;

    // Fuzz _cmsGetTransformUserData
    void* userData = _cmsGetTransformUserData(hTransform);
    (void)userData;

    // Fuzz _cmsGetTransformFormatters16
    cmsFormatter16 fromInput = NULL;
    cmsFormatter16 toOutput = NULL;
    _cmsGetTransformFormatters16(hTransform, &fromInput, &toOutput);
    (void)fromInput;
    (void)toOutput;

    // Fuzz _cmsSetTransformUserData
    if (Size > 10) {
        dummy_user_data = malloc(Size - 10);
        if (dummy_user_data) {
            memcpy(dummy_user_data, &Data[10], Size - 10);
            _cmsSetTransformUserData(hTransform, dummy_user_data, dummy_free_fn);
        }
    }

    // Fuzz _cmsGetTransformWorkerFlags
    cmsUInt32Number workerFlags = _cmsGetTransformWorkerFlags(hTransform);
    (void)workerFlags;

    // Cleanup
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hIn);
    cmsCloseProfile(hOut);
    cmsDeleteContext(ctx);

    if (dummy_user_data) {
        free(dummy_user_data);
        dummy_user_data = NULL;
    }

    return 0;
}