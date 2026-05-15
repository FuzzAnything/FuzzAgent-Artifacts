// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void dummy_free_user_data(struct _cmsContext_struct* context, void* ptr) {
    // Do nothing - just a placeholder for cleanup function
    (void)context;
    (void)ptr;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a simple transform for testing
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Create simple sRGB profiles for transform creation
    cmsHPROFILE src_profile = cmsCreate_sRGBProfileTHR(context);
    cmsHPROFILE dst_profile = cmsCreate_sRGBProfileTHR(context);
    
    if (!src_profile || !dst_profile) {
        if (src_profile) cmsCloseProfile(src_profile);
        if (dst_profile) cmsCloseProfile(dst_profile);
        cmsDeleteContext(context);
        return 0;
    }

    // Create a transform
    cmsUInt32Number input_format = TYPE_RGB_8;
    cmsUInt32Number output_format = TYPE_RGB_8;
    cmsUInt32Number intent = Data[0] % 4; // Use first byte for intent
    
    cmsHTRANSFORM hTransform = cmsCreateTransformTHR(context, 
                                                    src_profile, 
                                                    input_format,
                                                    dst_profile, 
                                                    output_format,
                                                    intent, 
                                                    0);
    
    if (!hTransform) {
        cmsCloseProfile(src_profile);
        cmsCloseProfile(dst_profile);
        cmsDeleteContext(context);
        return 0;
    }

    // Cast to internal structure for functions that need it
    struct _cmstransform_struct* transform_struct = (struct _cmstransform_struct*)hTransform;

    // Test 1: _cmsGetTransformWorker
    if (transform_struct) {
        _cmsTransform2Fn worker = _cmsGetTransformWorker(transform_struct);
        (void)worker; // Avoid unused variable warning
    }

    // Test 2: cmsGetTransformInputFormat
    cmsUInt32Number retrieved_format = cmsGetTransformInputFormat(hTransform);
    (void)retrieved_format;

    // Test 3: _cmsGetTransformFormattersFloat
    if (transform_struct) {
        cmsFormatterFloat from_input = NULL;
        cmsFormatterFloat to_output = NULL;
        _cmsGetTransformFormattersFloat(transform_struct, &from_input, &to_output);
        
        // Also test with NULL pointers
        _cmsGetTransformFormattersFloat(transform_struct, NULL, NULL);
    }

    // Test 4: _cmsGetTransformFlags
    if (transform_struct) {
        cmsUInt32Number flags = _cmsGetTransformFlags(transform_struct);
        (void)flags;
    }

    // Test 5: _cmsGetTransformUserData (should be NULL initially)
    if (transform_struct) {
        void* user_data = _cmsGetTransformUserData(transform_struct);
        (void)user_data;
    }

    // Test 6: _cmsSetTransformUserData and then get it again
    if (transform_struct) {
        // Use some data from the input for user data
        void* test_data = NULL;
        if (Size > 1) {
            test_data = malloc(Size > 16 ? 16 : Size);
            if (test_data) {
                memcpy(test_data, Data, Size > 16 ? 16 : Size);
            }
        }
        
        _cmsSetTransformUserData(transform_struct, test_data, dummy_free_user_data);
        
        // Get it back
        void* retrieved_data = _cmsGetTransformUserData(transform_struct);
        (void)retrieved_data;
        
        // Clean up if we allocated
        if (test_data) {
            free(test_data);
        }
        
        // Set to NULL with cleanup
        _cmsSetTransformUserData(transform_struct, NULL, dummy_free_user_data);
    }

    // Cleanup
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(src_profile);
    cmsCloseProfile(dst_profile);
    cmsDeleteContext(context);

    return 0;
}