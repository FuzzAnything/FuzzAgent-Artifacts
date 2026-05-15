// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
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

static void dummy_free_user_data(cmsContext ContextID, void* ptr) {
    // Do nothing, just for testing
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    // Create dummy profiles for testing
    cmsHPROFILE input_profile = cmsCreate_sRGBProfile();
    cmsHPROFILE output_profile = cmsCreate_sRGBProfile();
    
    if (!input_profile || !output_profile) {
        if (input_profile) cmsCloseProfile(input_profile);
        if (output_profile) cmsCloseProfile(output_profile);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    cmsUInt32Number InputFormat = 0;
    cmsUInt32Number OutputFormat = 0;
    cmsUInt32Number Intent = 0;
    cmsUInt32Number dwFlags = 0;
    
    if (Size >= sizeof(cmsUInt32Number) * 4) {
        memcpy(&InputFormat, Data, sizeof(cmsUInt32Number));
        memcpy(&OutputFormat, Data + sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
        memcpy(&Intent, Data + 2 * sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
        memcpy(&dwFlags, Data + 3 * sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
    } else {
        // Use default values if not enough data
        InputFormat = TYPE_RGB_8;
        OutputFormat = TYPE_RGB_8;
        Intent = INTENT_PERCEPTUAL;
        dwFlags = 0;
    }
    
    // Target API 1: cmsCreateTransform
    cmsHTRANSFORM transform = cmsCreateTransform(
        input_profile,
        InputFormat,
        output_profile,
        OutputFormat,
        Intent,
        dwFlags
    );
    
    if (transform) {
        // Cast to internal structure for internal APIs
        struct _cmstransform_struct* internal_transform = (struct _cmstransform_struct*)transform;
        
        // Target API 2: _cmsGetTransformFlags
        cmsUInt32Number flags = _cmsGetTransformFlags(internal_transform);
        
        // Target API 3: _cmsGetTransformWorker
        _cmsTransform2Fn worker = _cmsGetTransformWorker(internal_transform);
        
        // Target API 4: _cmsSetTransformUserData
        void* user_data = (void*)(Data + 4 * sizeof(cmsUInt32Number));
        size_t user_data_size = Size > 4 * sizeof(cmsUInt32Number) ? 
                               Size - 4 * sizeof(cmsUInt32Number) : 0;
        
        // Create a dummy user data buffer
        static uint8_t dummy_buffer[256];
        if (user_data_size > 0 && user_data_size < sizeof(dummy_buffer)) {
            memcpy(dummy_buffer, user_data, user_data_size);
        }
        
        _cmsSetTransformUserData(internal_transform, dummy_buffer, dummy_free_user_data);
        
        // Target API 5: _cmsGetTransformFormattersFloat
        cmsFormatterFloat from_input = NULL;
        cmsFormatterFloat to_output = NULL;
        _cmsGetTransformFormattersFloat(internal_transform, &from_input, &to_output);
        
        // Target API 6: _cmsGetTransformUserData
        void* retrieved_data = _cmsGetTransformUserData(internal_transform);
        
        // Clean up transform
        cmsDeleteTransform(transform);
    }
    
    // Clean up profiles
    cmsCloseProfile(input_profile);
    cmsCloseProfile(output_profile);
    
    return 0;
}

#ifdef __cplusplus
}
#endif