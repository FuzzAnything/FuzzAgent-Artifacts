// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2_plugin.h"

// Since we cannot directly allocate _cmstransform_struct (it's opaque),
// we need to create a transform using library functions
static cmsHTRANSFORM create_test_transform(const uint8_t *Data, size_t Size) {
    // Create simple profiles for testing
    cmsHPROFILE hProfile1, hProfile2;
    cmsHTRANSFORM hTransform = NULL;
    
    // Use built-in sRGB profile for input
    hProfile1 = cmsCreate_sRGBProfile();
    if (!hProfile1) return NULL;
    
    // Try to create output profile from fuzzer data if possible
    if (Size > 128) {
        // Try to open profile from memory
        hProfile2 = cmsOpenProfileFromMem(Data, Size);
        if (!hProfile2) {
            // If that fails, use built-in sRGB
            hProfile2 = cmsCreate_sRGBProfile();
        }
    } else {
        // Not enough data for profile, use built-in
        hProfile2 = cmsCreate_sRGBProfile();
    }
    
    if (hProfile2) {
        // Create a simple transform
        hTransform = cmsCreateTransform(hProfile1, TYPE_RGB_8, hProfile2, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
        cmsCloseProfile(hProfile2);
    }
    
    cmsCloseProfile(hProfile1);
    return hTransform;
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    // Create a transform using library functions
    cmsHTRANSFORM hTransform = create_test_transform(Data, Size);
    if (!hTransform) {
        return 0;
    }
    
    // Cast to internal structure pointer (this is what the API expects)
    struct _cmstransform_struct* transform = (struct _cmstransform_struct*)hTransform;
    
    // Test _cmsGetTransformWorkerFlags
    cmsUInt32Number worker_flags = _cmsGetTransformWorkerFlags(transform);
    (void)worker_flags;
    
    // Test _cmsGetTransformWorker
    _cmsTransform2Fn worker = _cmsGetTransformWorker(transform);
    (void)worker;
    
    // Test _cmsGetTransformFormattersFloat
    cmsFormatterFloat fromInputFloat = NULL;
    cmsFormatterFloat toOutputFloat = NULL;
    _cmsGetTransformFormattersFloat(transform, &fromInputFloat, &toOutputFloat);
    (void)fromInputFloat;
    (void)toOutputFloat;
    
    // Test _cmsGetTransformFlags
    cmsUInt32Number flags = _cmsGetTransformFlags(transform);
    (void)flags;
    
    // Test _cmsGetTransformUserData
    void* user_data = _cmsGetTransformUserData(transform);
    (void)user_data;
    
    // Test _cmsGetTransformFormatters16
    cmsFormatter16 fromInput16 = NULL;
    cmsFormatter16 toOutput16 = NULL;
    _cmsGetTransformFormatters16(transform, &fromInput16, &toOutput16);
    (void)fromInput16;
    (void)toOutput16;
    
    // Clean up
    cmsDeleteTransform(hTransform);
    
    return 0;
}