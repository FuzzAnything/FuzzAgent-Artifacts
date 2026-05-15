// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    // Create a simple transform for testing
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) return 0;
    
    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, TYPE_RGB_8, hProfile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    cmsCloseProfile(hProfile);
    
    if (!hTransform) return 0;
    
    // Cast to internal structure
    struct _cmstransform_struct* transform = (struct _cmstransform_struct*)hTransform;
    
    // Test _cmsGetTransformFormattersFloat
    cmsFormatterFloat fromInputFloat = NULL;
    cmsFormatterFloat toOutputFloat = NULL;
    _cmsGetTransformFormattersFloat(transform, &fromInputFloat, &toOutputFloat);
    
    // Test _cmsGetTransformUserData
    void* userData = _cmsGetTransformUserData(transform);
    
    // Test _cmsGetTransformFlags
    cmsUInt32Number flags = _cmsGetTransformFlags(transform);
    
    // Test _cmsGetTransformWorker
    _cmsTransform2Fn worker = _cmsGetTransformWorker(transform);
    
    // Test _cmsGetTransformFormatters16
    cmsFormatter16 fromInput16 = NULL;
    cmsFormatter16 toOutput16 = NULL;
    _cmsGetTransformFormatters16(transform, &fromInput16, &toOutput16);
    
    // Test _cmsGetTransformWorkerFlags
    cmsUInt32Number workerFlags = _cmsGetTransformWorkerFlags(transform);
    
    // Clean up
    cmsDeleteTransform(hTransform);
    
    return 0;
}