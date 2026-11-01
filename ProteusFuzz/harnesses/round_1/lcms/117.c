// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profiles(cmsHPROFILE hProfiles[], int count) {
    for (int i = 0; i < count; i++) {
        if (hProfiles[i] != NULL) {
            cmsCloseProfile(hProfiles[i]);
            hProfiles[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsHPROFILE hInput = NULL;
    cmsHPROFILE hOutput = NULL;
    cmsHTRANSFORM hTransform = NULL;

    // Use input data to decide on formats and intents
    cmsUInt32Number inputFormat = Data[0] & 0xFF;
    cmsUInt32Number outputFormat = Data[1] & 0xFF;
    cmsUInt32Number intent = (Size > 2) ? (Data[2] & 0x03) : 0; // Clamp to 0-3
    cmsUInt32Number flags = (Size > 3) ? (Data[3] & 0xFF) : 0;

    // Create placeholder profiles
    hInput = cmsCreateProfilePlaceholder(ctx);
    hOutput = cmsCreateProfilePlaceholder(ctx);

    if (hInput == NULL || hOutput == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Attempt to create a transform
    // Signature: cmsCreateTransform(cmsHPROFILE Input, cmsUInt32Number InputFormat,
    //                               cmsHPROFILE Output, cmsUInt32Number OutputFormat,
    //                               cmsUInt32Number Intent, cmsUInt32Number dwFlags);
    hTransform = cmsCreateTransform(hInput, inputFormat, hOutput, outputFormat, intent, flags);

    if (hTransform != NULL) {
        // 1. _cmsGetTransformMaxWorkers
        cmsInt32Number maxWorkers = _cmsGetTransformMaxWorkers(hTransform);
        (void)maxWorkers;

        // 2. _cmsGetTransformFormatters16
        cmsFormatter16 fromInput16 = NULL;
        cmsFormatter16 toOutput16 = NULL;
        _cmsGetTransformFormatters16(hTransform, &fromInput16, &toOutput16);
        (void)fromInput16;
        (void)toOutput16;

        // 3. _cmsGetTransformUserData
        void *userData = _cmsGetTransformUserData(hTransform);
        (void)userData;

        // 4. _cmsGetTransformWorker
        _cmsTransform2Fn worker = _cmsGetTransformWorker(hTransform);
        (void)worker;

        // 5. _cmsSetTransformUserData
        void *dummyData = malloc(Size > 10 ? 10 : Size);
        if (dummyData) {
            memcpy(dummyData, Data, Size > 10 ? 10 : Size);
            _cmsSetTransformUserData(hTransform, dummyData, NULL);
            
            // Verify we can get it back
            void *retrievedData = _cmsGetTransformUserData(hTransform);
            (void)retrievedData;
            
            // Since FreePrivateDataFn was NULL, we must free it ourselves
            free(dummyData);
        }

        // 6. _cmsGetTransformFormattersFloat
        cmsFormatterFloat fromInputFloat = NULL;
        cmsFormatterFloat toOutputFloat = NULL;
        _cmsGetTransformFormattersFloat(hTransform, &fromInputFloat, &toOutputFloat);
        (void)fromInputFloat;
        (void)toOutputFloat;

        // Cleanup transform
        cmsDeleteTransform(hTransform);
    }

    // Cleanup profiles
    cleanup_profiles((cmsHPROFILE[]){hInput, hOutput}, 2);

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}