// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static void dummy_free_user_data(struct _cmsContext_struct* context, void* ptr) {
    // Do nothing - just for testing
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Create dummy profiles for testing
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) {
        return 0;
    }
    
    // Use fuzzer data to determine parameters
    cmsUInt32Number inputFormat = 0;
    cmsUInt32Number outputFormat = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    
    if (Size >= 4) {
        inputFormat = *(cmsUInt32Number*)Data;
        Data += 4;
        Size -= 4;
    }
    
    if (Size >= 4) {
        outputFormat = *(cmsUInt32Number*)Data;
        Data += 4;
        Size -= 4;
    }
    
    if (Size >= 4) {
        intent = *(cmsUInt32Number*)Data;
        Data += 4;
        Size -= 4;
    }
    
    if (Size >= 4) {
        flags = *(cmsUInt32Number*)Data;
        Data += 4;
        Size -= 4;
    }
    
    // Create transform
    cmsHTRANSFORM hTransform = cmsCreateTransform(
        hProfile, inputFormat,
        hProfile, outputFormat,
        intent, flags
    );
    
    if (hTransform) {
        struct _cmstransform_struct* transform = (struct _cmstransform_struct*)hTransform;
        
        // Test _cmsGetTransformFormattersFloat
        cmsFormatterFloat fromInput = NULL;
        cmsFormatterFloat toOutput = NULL;
        _cmsGetTransformFormattersFloat(transform, &fromInput, &toOutput);
        
        // Test _cmsGetTransformUserData
        void* userData = _cmsGetTransformUserData(transform);
        
        // Test _cmsGetTransformFlags
        cmsUInt32Number transformFlags = _cmsGetTransformFlags(transform);
        
        // Test _cmsGetTransformWorker
        _cmsTransform2Fn worker = _cmsGetTransformWorker(transform);
        
        // Test _cmsSetTransformUserData
        void* dummyData = NULL;
        if (Size > 0) {
            dummyData = malloc(Size);
            if (dummyData) {
                memcpy(dummyData, Data, Size);
                _cmsSetTransformUserData(transform, dummyData, dummy_free_user_data);
                
                // Get user data again to verify
                void* retrievedData = _cmsGetTransformUserData(transform);
                
                // Clean up dummy data
                if (dummyData) {
                    free(dummyData);
                }
            }
        } else {
            // Test with NULL data
            _cmsSetTransformUserData(transform, NULL, NULL);
        }
        
        // Clean up transform
        cmsDeleteTransform(hTransform);
    }
    
    // Clean up profile
    cmsCloseProfile(hProfile);
    
    return 0;
}