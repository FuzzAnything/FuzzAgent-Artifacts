// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
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

// Dummy free function for user data with correct signature
static void dummy_free(struct _cmsContext_struct* ContextID, void* ptr) {
    // Do nothing
}

// Helper function to create a transform using lcms API
static cmsHTRANSFORM create_test_transform(const uint8_t* data, size_t size) {
    // Create simple sRGB profiles for testing
    cmsHPROFILE src_profile = cmsCreate_sRGBProfile();
    if (!src_profile) return NULL;
    
    cmsHPROFILE dst_profile = cmsCreate_sRGBProfile();
    if (!dst_profile) {
        cmsCloseProfile(src_profile);
        return NULL;
    }
    
    // Use fuzzer data to determine intent and flags if available
    cmsUInt32Number intent = 0; // INTENT_PERCEPTUAL = 0
    cmsUInt32Number flags = 0;
    
    if (size > 0) {
        intent = data[0] % 4;  // 4 standard intents (0-3)
    }
    if (size > 1) {
        flags = data[1];
    }
    
    // Create a transform
    cmsHTRANSFORM transform = cmsCreateTransform(
        src_profile, 
        TYPE_RGB_8, 
        dst_profile, 
        TYPE_RGB_8, 
        intent, 
        flags
    );
    
    // Set user data if we have enough input
    if (transform && size > 2) {
        static int dummy_user_data = 42;
        _cmsSetTransformUserData(transform, &dummy_user_data, dummy_free);
    }
    
    // Clean up profiles
    cmsCloseProfile(src_profile);
    cmsCloseProfile(dst_profile);
    
    return transform;
}

// Fuzzer entry point
int LLVMFuzzerTestOneInput_113(const uint8_t* Data, size_t Size) {
    // Create a transform using the lcms API
    cmsHTRANSFORM hTransform = create_test_transform(Data, Size);
    if (!hTransform) return 0;
    
    // Cast to internal structure type
    struct _cmstransform_struct* transform = (struct _cmstransform_struct*)hTransform;
    
    // Test _cmsGetTransformUserData
    void* user_data = _cmsGetTransformUserData(transform);
    (void)user_data; // Use the result to avoid unused variable warning
    
    // Test _cmsGetTransformFlags
    cmsUInt32Number flags = _cmsGetTransformFlags(transform);
    (void)flags;
    
    // Test _cmsGetTransformFormattersFloat
    cmsFormatterFloat from_input_float = NULL;
    cmsFormatterFloat to_output_float = NULL;
    _cmsGetTransformFormattersFloat(transform, &from_input_float, &to_output_float);
    
    // Test _cmsGetTransformWorker
    _cmsTransform2Fn worker = _cmsGetTransformWorker(transform);
    (void)worker;
    
    // Test _cmsGetTransformFormatters16
    cmsFormatter16 from_input_16 = NULL;
    cmsFormatter16 to_output_16 = NULL;
    _cmsGetTransformFormatters16(transform, &from_input_16, &to_output_16);
    
    // Test _cmsGetTransformWorkerFlags
    cmsUInt32Number worker_flags = _cmsGetTransformWorkerFlags(transform);
    (void)worker_flags;
    
    // Clean up
    cmsDeleteTransform(hTransform);
    
    return 0;
}