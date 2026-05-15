// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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

static struct _cmstransform_struct* create_dummy_transform(const uint8_t* data, size_t size) {
    // Since struct _cmstransform_struct is incomplete, we cannot allocate it directly.
    // Instead, we'll create a minimal transform using lcms API functions.
    
    // Create simple sRGB profiles for source and destination
    cmsHPROFILE srcProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE dstProfile = cmsCreate_sRGBProfile();
    
    if (!srcProfile || !dstProfile) {
        if (srcProfile) cmsCloseProfile(srcProfile);
        if (dstProfile) cmsCloseProfile(dstProfile);
        return NULL;
    }
    
    // Create a simple transform
    cmsHTRANSFORM hTransform = cmsCreateTransform(srcProfile, TYPE_RGB_8, 
                                                  dstProfile, TYPE_RGB_8, 
                                                  INTENT_PERCEPTUAL, 0);
    
    // Close profiles
    cmsCloseProfile(srcProfile);
    cmsCloseProfile(dstProfile);
    
    if (!hTransform) {
        return NULL;
    }
    
    // Cast to the internal type
    struct _cmstransform_struct* transform = (struct _cmstransform_struct*)hTransform;
    
    return transform;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    // Create a transform using lcms API
    struct _cmstransform_struct* transform = create_dummy_transform(Data, Size);
    if (!transform) return 0;
    
    // Test _cmsGetTransformFormattersFloat
    cmsFormatterFloat fromInputFloat = NULL;
    cmsFormatterFloat toOutputFloat = NULL;
    _cmsGetTransformFormattersFloat(transform, &fromInputFloat, &toOutputFloat);
    
    // Test _cmsGetTransformUserData
    void* userData = _cmsGetTransformUserData(transform);
    (void)userData;
    
    // Test _cmsGetTransformFlags
    cmsUInt32Number flags = _cmsGetTransformFlags(transform);
    (void)flags;
    
    // Test _cmsGetTransformWorker
    _cmsTransform2Fn worker = _cmsGetTransformWorker(transform);
    (void)worker;
    
    // Test _cmsGetTransformFormatters16
    cmsFormatter16 fromInput16 = NULL;
    cmsFormatter16 toOutput16 = NULL;
    _cmsGetTransformFormatters16(transform, &fromInput16, &toOutput16);
    
    // Test _cmsGetTransformWorkerFlags
    cmsUInt32Number workerFlags = _cmsGetTransformWorkerFlags(transform);
    (void)workerFlags;
    
    // Clean up - need to cast back to cmsHTRANSFORM for proper cleanup
    cmsDeleteTransform((cmsHTRANSFORM)transform);
    
    return 0;
}