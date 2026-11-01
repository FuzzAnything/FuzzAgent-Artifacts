// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformFormatters16 at cmsxform.c:765:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    cmsUInt8Number *data = malloc(128);
    if (!data) return NULL;
    
    memset(data, 0, 128);
    
    // Set size (bytes) at offset 0-3 (little endian)
    data[0] = 128; data[1] = 0; data[2] = 0; data[3] = 0;
    
    // Set signature "acsp" at offset 4-7
    data[4] = 'a'; data[5] = 'c'; data[6] = 's'; data[7] = 'p';
    
    // cmsOpenProfileFromMem takes only 2 arguments: (MemPtr, dwSize)
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, 128);
    free(data);
    return profile;
}

static cmsHTRANSFORM create_dummy_transform(cmsContext ctx) {
    cmsHPROFILE input_profile = create_dummy_profile(ctx);
    cmsHPROFILE output_profile = create_dummy_profile(ctx);
    
    if (!input_profile || !output_profile) {
        if (input_profile) cmsCloseProfile(input_profile);
        if (output_profile) cmsCloseProfile(output_profile);
        return NULL;
    }
    
    // cmsCreateTransform signature: (Input, InputFormat, Output, OutputFormat, Intent, dwFlags)
    cmsHTRANSFORM transform = cmsCreateTransform(
        input_profile, 
        cmsSigRgbData, 
        output_profile, 
        cmsSigRgbData, 
        0, 
        0
    );
    
    cmsCloseProfile(input_profile);
    cmsCloseProfile(output_profile);
    
    return transform;
}

static void dummy_free_fn(cmsContext ctx, void *ptr) {
    (void)ctx;
    if (ptr) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsHTRANSFORM transform = create_dummy_transform(ctx);
    
    if (transform) {
        // Test _cmsGetTransformMaxWorkers
        cmsInt32Number max_workers = _cmsGetTransformMaxWorkers(transform);
        (void)max_workers;
        
        // Test _cmsGetTransformFormatters16
        cmsFormatter16 from_input_16 = NULL;
        cmsFormatter16 to_output_16 = NULL;
        _cmsGetTransformFormatters16(transform, &from_input_16, &to_output_16);
        
        // Test with NULL pointers
        _cmsGetTransformFormatters16(transform, NULL, NULL);
        
        // Test _cmsGetTransformFormattersFloat
        cmsFormatterFloat from_input_float = NULL;
        cmsFormatterFloat to_output_float = NULL;
        _cmsGetTransformFormattersFloat(transform, &from_input_float, &to_output_float);
        
        // Test with NULL pointers
        _cmsGetTransformFormattersFloat(transform, NULL, NULL);
        
        // Test _cmsSetTransformUserData and _cmsGetTransformUserData
        void *user_data = malloc(64);
        if (user_data) {
            memset(user_data, Data[0] % 256, 64);
            _cmsSetTransformUserData(transform, user_data, dummy_free_fn);
            
            void *retrieved_data = _cmsGetTransformUserData(transform);
            (void)retrieved_data;
        }
        
        // Test with NULL free function
        void *user_data2 = malloc(32);
        if (user_data2) {
            memset(user_data2, 0, 32);
            _cmsSetTransformUserData(transform, user_data2, NULL);
            // Note: This data won't be freed automatically by lcms, manual free needed
            free(user_data2);
        }
        
        // Test _cmsGetTransformWorker
        _cmsTransform2Fn worker = _cmsGetTransformWorker(transform);
        (void)worker;
        
        cmsDeleteTransform(transform);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}