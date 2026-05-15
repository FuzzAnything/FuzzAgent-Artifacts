// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsGetPipelineContextID at cmslut.c:1407:22 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/lcms/latest/code/include/lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_free(void* ptr) {
    free(ptr);
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    // Create a context first
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;

    // Test cmsCreateContext with fuzzer data as user data
    if (Size > 0) {
        void* user_data = malloc(Size);
        if (user_data) {
            memcpy(user_data, Data, Size);
            cmsContext context2 = cmsCreateContext(NULL, user_data);
            if (context2) {
                cmsDeleteContext(context2);
            }
            free(user_data);
        }
    }

    // Create a dummy stage for testing
    cmsUInt32Number grid_points = 2;
    cmsUInt32Number input_chan = 3;
    cmsUInt32Number output_chan = 3;
    size_t table_size = grid_points * grid_points * grid_points * output_chan;
    cmsFloat32Number* table = NULL;
    
    if (Size >= table_size * sizeof(cmsFloat32Number)) {
        table = (cmsFloat32Number*)malloc(table_size * sizeof(cmsFloat32Number));
        if (table) {
            // Use fuzzer data for table if available
            for (size_t i = 0; i < table_size && i < Size/sizeof(cmsFloat32Number); i++) {
                table[i] = ((cmsFloat32Number*)Data)[i];
            }
        }
    }
    
    cmsStage* stage = cmsStageAllocCLutFloat(context, grid_points, input_chan, output_chan, table);
    if (stage) {
        // Test cmsGetStageContextID
        cmsContext stage_context = cmsGetStageContextID(stage);
        (void)stage_context;
        
        // Create a simple pipeline with the stage
        cmsPipeline* pipeline = cmsPipelineAlloc(context, input_chan, output_chan);
        if (pipeline) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
            
            // Test cmsGetPipelineContextID
            cmsContext pipeline_context = cmsGetPipelineContextID(pipeline);
            (void)pipeline_context;
            
            // Create a simple profile for testing
            cmsCIExyY white_point = {0.3127, 0.3290, 1.0};
            cmsCIExyYTRIPLE primaries = {
                {0.64, 0.33, 1.0},
                {0.30, 0.60, 1.0},
                {0.15, 0.06, 1.0}
            };
            
            cmsHPROFILE profile = cmsCreateRGBProfile(&white_point, &primaries, NULL);
            if (profile) {
                // Test cmsGetProfileContextID
                cmsContext profile_context = cmsGetProfileContextID(profile);
                (void)profile_context;
                
                // Create a transform for testing
                cmsHTRANSFORM transform = cmsCreateTransform(profile, TYPE_RGB_8, profile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
                if (transform) {
                    // Test cmsGetTransformContextID
                    cmsContext transform_context = cmsGetTransformContextID(transform);
                    (void)transform_context;
                    
                    cmsDeleteTransform(transform);
                }
                
                cmsCloseProfile(profile);
            }
            
            cmsPipelineFree(pipeline);
        } else {
            cmsStageFree(stage);
        }
    }
    
    if (table) free(table);
    cmsDeleteContext(context);
    
    return 0;
}