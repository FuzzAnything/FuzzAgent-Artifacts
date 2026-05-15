// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void dummy_free_user_data(struct _cmsContext_struct* context, void* ptr) {
    // Do nothing - just a placeholder free function
    (void)context;
    (void)ptr;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    // Create a dummy context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Create dummy profiles for transform creation
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfileTHR(context);
    cmsHPROFILE hLab = cmsCreateLab4ProfileTHR(context, NULL);
    
    if (!hSRGB || !hLab) {
        if (hSRGB) cmsCloseProfile(hSRGB);
        if (hLab) cmsCloseProfile(hLab);
        cmsDeleteContext(context);
        return 0;
    }

    // Create a transform using the profiles
    cmsHTRANSFORM hTransform = cmsCreateTransformTHR(context, 
                                                     hSRGB, 
                                                     TYPE_RGB_8, 
                                                     hLab, 
                                                     TYPE_Lab_DBL, 
                                                     INTENT_PERCEPTUAL, 
                                                     0);
    
    if (!hTransform) {
        cmsCloseProfile(hSRGB);
        cmsCloseProfile(hLab);
        cmsDeleteContext(context);
        return 0;
    }

    // Cast to internal structure for functions that require it
    struct _cmstransform_struct* internalTransform = (struct _cmstransform_struct*)hTransform;

    // Test 1: _cmsGetTransformUserData
    void* userData = _cmsGetTransformUserData(internalTransform);
    (void)userData; // Use result to avoid unused variable warning

    // Test 2: _cmsGetTransformFlags
    cmsUInt32Number flags = _cmsGetTransformFlags(internalTransform);
    (void)flags;

    // Test 3: _cmsGetTransformFormattersFloat
    cmsFormatterFloat fromInputFloat = NULL;
    cmsFormatterFloat toOutputFloat = NULL;
    _cmsGetTransformFormattersFloat(internalTransform, &fromInputFloat, &toOutputFloat);
    (void)fromInputFloat;
    (void)toOutputFloat;

    // Test 4: cmsGetTransformInputFormat
    cmsUInt32Number inputFormat = cmsGetTransformInputFormat(hTransform);
    (void)inputFormat;

    // Test 5: _cmsGetTransformWorker
    _cmsTransform2Fn worker = _cmsGetTransformWorker(internalTransform);
    (void)worker;

    // Test 6: _cmsSetTransformUserData
    // Use the fuzzer data as user data (truncated to avoid overflow)
    size_t dataSize = Size > 1024 ? 1024 : Size;
    void* customData = NULL;
    if (dataSize > 0) {
        customData = malloc(dataSize);
        if (customData) {
            memcpy(customData, Data, dataSize);
            _cmsSetTransformUserData(internalTransform, customData, dummy_free_user_data);
        }
    }

    // Get the user data again after setting it
    void* retrievedData = _cmsGetTransformUserData(internalTransform);
    (void)retrievedData;

    // Clean up
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hSRGB);
    cmsCloseProfile(hLab);
    cmsDeleteContext(context);
    
    // Free custom data if it was allocated (the transform's free function may not have been called)
    if (customData) {
        free(customData);
    }

    return 0;
}