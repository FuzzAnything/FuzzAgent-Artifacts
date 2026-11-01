// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Helper function to create a dummy transform for fuzzing
static cmsHTRANSFORM create_dummy_transform(cmsContext ctx, cmsHPROFILE hProfile) {
    if (hProfile == NULL) return NULL;

    // Use standard 8-bit RGB format for both input and output
    cmsUInt32Number inputFormat = cmsSigRgbData; 
    cmsUInt32Number outputFormat = cmsSigRgbData;

    // Use intent 0 (Perceptual) explicitly as an integer to avoid macro issues
    cmsUInt32Number intent = 0; 
    
    // cmsCreateTransform signature: (InputProfile, InputFormat, OutputProfile, OutputFormat, Intent, Flags)
    cmsHTRANSFORM tr = cmsCreateTransform(hProfile, inputFormat, 
                                          hProfile, outputFormat, 
                                          intent, 0);
    return tr;
}

// Free user data callback matching the signature: void (*)(cmsContext, void*)
static void free_user_data(cmsContext ctx, void* ptr) {
    (void)ctx;
    if (ptr) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to dummy file if needed
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Try to open a profile from the dummy file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Try to create a transform
    cmsHTRANSFORM tr = create_dummy_transform(ctx, hProfile);
    cmsCloseProfile(hProfile);

    if (tr == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Cast to internal struct for testing internal APIs
    struct _cmstransform_struct* cargo = (struct _cmstransform_struct*)tr;

    // Test 1: _cmsGetTransformUserData
    void* userData = _cmsGetTransformUserData(cargo);
    (void)userData;

    // Test 2: _cmsGetTransformFlags
    cmsUInt32Number flags = _cmsGetTransformFlags(cargo);
    (void)flags;

    // Test 3: _cmsGetTransformWorker
    _cmsTransform2Fn worker = _cmsGetTransformWorker(cargo);
    (void)worker;

    // Test 4: _cmsSetTransformUserData
    void* dummyData = malloc(Size > 0 ? Size : 1);
    if (dummyData) {
        memcpy(dummyData, Data, Size > 0 ? Size : 1);
        _cmsSetTransformUserData(cargo, dummyData, free_user_data);
        
        // Verify it was set
        void* retrievedData = _cmsGetTransformUserData(cargo);
        (void)retrievedData;
    }

    // Test 5: _cmsGetTransformWorkerFlags
    cmsUInt32Number workerFlags = _cmsGetTransformWorkerFlags(cargo);
    (void)workerFlags;

    // Test 6: _cmsGetTransformFormattersFloat
    cmsFormatterFloat fromInput = NULL;
    cmsFormatterFloat toOutput = NULL;
    _cmsGetTransformFormattersFloat(cargo, &fromInput, &toOutput);
    (void)fromInput;
    (void)toOutput;

    // Cleanup
    cmsDeleteTransform(tr);
    cmsDeleteContext(ctx);

    return 0;
}