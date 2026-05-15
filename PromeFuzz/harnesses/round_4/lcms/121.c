// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    // We cannot allocate a complete _cmstransform_struct because it's opaque.
    // Instead, we need to create a transform using public API functions.
    // First, create some simple profiles to get a valid transform handle.
    
    // Create a simple sRGB profile
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    if (!hSRGB) return 0;
    
    // Create a simple Lab profile
    cmsHPROFILE hLab = cmsCreateLab4Profile(NULL);
    if (!hLab) {
        cmsCloseProfile(hSRGB);
        return 0;
    }
    
    // Create a transform using the profiles
    cmsHTRANSFORM hTransform = cmsCreateTransform(hSRGB, TYPE_RGB_8, hLab, TYPE_Lab_8, INTENT_PERCEPTUAL, 0);
    
    if (hTransform) {
        // Cast to the internal struct pointer type
        struct _cmstransform_struct* transform = (struct _cmstransform_struct*)hTransform;
        
        // Test each API function
        // 1. _cmsGetTransformWorker
        _cmsTransform2Fn worker = _cmsGetTransformWorker(transform);
        (void)worker;
        
        // 2. _cmsGetTransformWorkerFlags
        cmsUInt32Number worker_flags = _cmsGetTransformWorkerFlags(transform);
        (void)worker_flags;
        
        // 3. _cmsGetTransformFormattersFloat
        cmsFormatterFloat from_input_float = NULL;
        cmsFormatterFloat to_output_float = NULL;
        _cmsGetTransformFormattersFloat(transform, &from_input_float, &to_output_float);
        
        // 4. _cmsGetTransformFlags
        cmsUInt32Number flags = _cmsGetTransformFlags(transform);
        (void)flags;
        
        // 5. _cmsGetTransformUserData
        void* user_data = _cmsGetTransformUserData(transform);
        (void)user_data;
        
        // 6. _cmsGetTransformFormatters16
        cmsFormatter16 from_input_16 = NULL;
        cmsFormatter16 to_output_16 = NULL;
        _cmsGetTransformFormatters16(transform, &from_input_16, &to_output_16);
        
        // Clean up the transform
        cmsDeleteTransform(hTransform);
    }
    
    // Clean up profiles
    cmsCloseProfile(hSRGB);
    cmsCloseProfile(hLab);
    
    return 0;
}

#ifdef __cplusplus
}
#endif