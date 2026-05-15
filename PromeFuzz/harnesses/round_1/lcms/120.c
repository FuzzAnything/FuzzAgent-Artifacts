// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    // We cannot directly create a _cmstransform_struct because it's opaque.
    // Instead, we need to create a valid transform using lcms API functions.
    
    // Create dummy profiles to create a transform
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    // Create simple sRGB profiles
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    if (!hSRGB) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Create a transform
    cmsHTRANSFORM hTransform = cmsCreateTransform(hSRGB, TYPE_RGB_8, hSRGB, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    if (!hTransform) {
        cmsCloseProfile(hSRGB);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Cast to internal structure pointer
    struct _cmstransform_struct* transform = (struct _cmstransform_struct*)hTransform;
    
    // Test all target API functions
    
    // 1. Test _cmsGetTransformFormattersFloat
    cmsFormatterFloat fromInputFloat = NULL;
    cmsFormatterFloat toOutputFloat = NULL;
    _cmsGetTransformFormattersFloat(transform, &fromInputFloat, &toOutputFloat);
    
    // 2. Test _cmsGetTransformUserData
    void* userData = _cmsGetTransformUserData(transform);
    
    // 3. Test _cmsGetTransformFlags
    cmsUInt32Number flags = _cmsGetTransformFlags(transform);
    
    // 4. Test _cmsGetTransformWorker
    _cmsTransform2Fn worker = _cmsGetTransformWorker(transform);
    
    // 5. Test _cmsGetTransformFormatters16
    cmsFormatter16 fromInput16 = NULL;
    cmsFormatter16 toOutput16 = NULL;
    _cmsGetTransformFormatters16(transform, &fromInput16, &toOutput16);
    
    // 6. Test _cmsGetTransformWorkerFlags
    cmsUInt32Number workerFlags = _cmsGetTransformWorkerFlags(transform);
    
    // Try to call formatter functions if they exist (with dummy data)
    if (fromInput16) {
        cmsUInt16Number dummyValues[16] = {0};
        cmsUInt8Number dummyBuffer[128] = {0};
        fromInput16(transform, dummyValues, dummyBuffer, 1);
    }
    
    if (toOutput16) {
        cmsUInt16Number dummyValues[16] = {0};
        cmsUInt8Number dummyBuffer[128] = {0};
        toOutput16(transform, dummyValues, dummyBuffer, 1);
    }
    
    if (fromInputFloat) {
        cmsFloat32Number dummyValues[16] = {0.0f};
        cmsUInt8Number dummyBuffer[128] = {0};
        fromInputFloat(transform, dummyValues, dummyBuffer, 1);
    }
    
    if (toOutputFloat) {
        cmsFloat32Number dummyValues[16] = {0.0f};
        cmsUInt8Number dummyBuffer[128] = {0};
        toOutputFloat(transform, dummyValues, dummyBuffer, 1);
    }
    
    // Clean up
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hSRGB);
    cmsDeleteContext(context);
    
    return 0;
}