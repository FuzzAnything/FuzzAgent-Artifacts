// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcms2.h"

static void dummy_handler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text)
{
    // Suppress error messages during fuzzing
    (void)ContextID;
    (void)ErrorCode;
    (void)Text;
}

static cmsHPROFILE create_test_profile(void)
{
    // Create a simple RGB profile for testing
    cmsToneCurve* curves[3];
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},
        {0.30, 0.60, 1.0},
        {0.15, 0.06, 1.0}
    };
    
    // Create a simple gamma curve
    cmsFloat64Number gamma = 2.2;
    curves[0] = curves[1] = curves[2] = cmsBuildGamma(NULL, gamma);
    
    cmsHPROFILE profile = cmsCreateRGBProfile(&whitePoint, &primaries, curves);
    
    // Free the curves
    cmsFreeToneCurve(curves[0]);
    
    return profile;
}

static cmsPipeline* create_test_pipeline(void)
{
    // Create a simple pipeline for testing
    cmsPipeline* pipeline = cmsPipelineAlloc(NULL, 3, 3);
    if (!pipeline) return NULL;
    
    // Add a simple gamma curve
    cmsToneCurve* gamma = cmsBuildGamma(NULL, 2.2);
    if (gamma) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, cmsStageAllocToneCurves(NULL, 3, &gamma));
        cmsFreeToneCurve(gamma);
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size)
{
    // Set error handler to suppress error messages
    cmsSetLogErrorHandler(dummy_handler);
    
    // Create test profile
    cmsHPROFILE hProfile = create_test_profile();
    if (!hProfile) return 0;
    
    // Create test pipeline
    cmsPipeline* pipeline = create_test_pipeline();
    
    // Use fuzz data to generate parameters
    if (Size > 0)
    {
        // Extract tag signatures from fuzz data
        cmsTagSignature sig1 = 0, sig2 = 0;
        if (Size >= 8)
        {
            memcpy(&sig1, Data, 4);
            memcpy(&sig2, Data + 4, 4);
        }
        else if (Size >= 4)
        {
            memcpy(&sig1, Data, 4);
            sig2 = sig1;
        }
        else
        {
            // Use default signatures if not enough data
            sig1 = cmsSigRedTRCTag;
            sig2 = cmsSigGreenTRCTag;
        }
        
        // Test cmsLinkTag
        cmsLinkTag(hProfile, sig1, sig2);
        
        // Test cmsSaveProfileToFile
        cmsSaveProfileToFile(hProfile, "./dummy_file");
        // Clean up the file
        remove("./dummy_file");
        
        // Test cmsWriteRawTag
        if (Size > 8)
        {
            size_t raw_size = Size - 8;
            if (raw_size > 1024) raw_size = 1024; // Limit size
            const void* raw_data = Data + 8;
            cmsWriteRawTag(hProfile, sig1, raw_data, (cmsUInt32Number)raw_size);
        }
        
        // Test cmsWriteTag
        if (Size > 4)
        {
            size_t tag_size = Size - 4;
            if (tag_size > 256) tag_size = 256; // Limit size
            const void* tag_data = Data + 4;
            
            // Test writing tag
            cmsWriteTag(hProfile, sig1, tag_data);
            
            // Test deleting tag (pass NULL)
            if (Size % 2 == 0)
            {
                cmsWriteTag(hProfile, sig1, NULL);
            }
        }
        
        // Test cmsPipelineSetSaveAs8bitsFlag
        if (pipeline)
        {
            cmsBool flag = (Size > 0) ? (Data[0] % 2) : FALSE;
            cmsPipelineSetSaveAs8bitsFlag(pipeline, flag);
        }
        
        // Test cmsSaveProfileToMem
        cmsUInt32Number bytes_needed = 0;
        // First call to get required size
        cmsSaveProfileToMem(hProfile, NULL, &bytes_needed);
        
        if (bytes_needed > 0 && bytes_needed < 1024 * 1024) // Reasonable size limit
        {
            void* buffer = malloc(bytes_needed);
            if (buffer)
            {
                cmsUInt32Number bytes_written = bytes_needed;
                cmsSaveProfileToMem(hProfile, buffer, &bytes_written);
                free(buffer);
            }
        }
    }
    
    // Clean up
    if (pipeline)
    {
        cmsPipelineFree(pipeline);
    }
    cmsCloseProfile(hProfile);
    
    return 0;
}