// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
}

int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Avoid calling functions with NULL pointers that cause immediate crashes (SEGV)
    // as per the sanitizer error. Instead, we focus on testing with valid or
    // intentionally malformed but non-NULL structures if possible, or skip NULL tests.
    
    // Test with NULL inputs directly to trigger potential crashes or undefined behavior
    // REMOVED: cmsStageData(NULL) - causes SEGV
    // REMOVED: cmsPipelineUnlinkStage(NULL, ...) - may cause SEGV
    // REMOVED: cmsStageDup(NULL) - returns NULL safely, but let's skip to be safe
    // REMOVED: cmsPipelineGetPtrToFirstStage(NULL) - causes SEGV
    // REMOVED: cmsPipelineGetPtrToLastStage(NULL) - may cause SEGV
    // REMOVED: cmsStageNext(NULL) - causes SEGV

    // Attempt to create a valid pipeline to test non-NULL paths
    // We create a dummy profile from a minimal ICC structure written to a file
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a minimal ICC profile header (128 bytes)
        uint8_t header[128] = {0};
        // Size
        header[0] = 0; header[1] = 0; header[2] = 0; header[3] = 128;
        // CMM Type
        header[4] = 'a'; header[5] = 'c'; header[6] = 's'; header[7] = 'p';
        // Version
        header[8] = 2; header[9] = 0; header[10] = 0; header[11] = 0;
        // Device Class
        header[12] = 'm'; header[13] = 'n'; header[14] = 't'; header[15] = ' ';
        // Color Space
        header[16] = 's'; header[17] = 'R'; header[18] = 'G'; header[19] = 'B';
        // Connection Space
        header[20] = 'X'; header[21] = 'Y'; header[22] = 'Z'; header[23] = ' ';
        // Date
        header[24] = 0; header[25] = 0; header[26] = 0; header[27] = 0;
        header[28] = 0; header[29] = 0; header[30] = 0; header[31] = 0;
        // Manufacturer
        header[32] = 'a'; header[33] = 'c'; header[34] = 's'; header[35] = 'p';
        // Model
        header[36] = 'a'; header[37] = 'c'; header[38] = 's'; header[39] = 'p';
        // Attributes
        header[40] = 0; header[41] = 0; header[42] = 0; header[43] = 0;
        header[44] = 0; header[45] = 0; header[46] = 0; header[47] = 0;
        // Rendering Intent
        header[48] = 0; header[49] = 0; header[50] = 0; header[51] = 0;
        // PCP XYZ
        header[52] = 0; header[53] = 0; header[54] = 0; header[55] = 0;
        header[56] = 0; header[57] = 0; header[58] = 0; header[59] = 0;
        header[60] = 0; header[61] = 0; header[62] = 0; header[63] = 0;
        // White Point
        header[64] = 0; header[65] = 0; header[66] = 0; header[67] = 0;
        header[68] = 0; header[69] = 0; header[70] = 0; header[71] = 0;
        header[72] = 0; header[73] = 0; header[74] = 0; header[75] = 0;
        // Rest is zeroed
        
        fwrite(header, 1, 128, f);
        fclose(f);

        cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
        if (hProfile) {
            // Create an empty pipeline manually
            cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3); // 3 input, 3 output channels
            
            if (pipeline) {
                // Test with empty pipeline (valid pointer, but no stages)
                cmsStage* emptyFirst = cmsPipelineGetPtrToFirstStage(pipeline);
                (void)emptyFirst;
                
                cmsStage* emptyLast = cmsPipelineGetPtrToLastStage(pipeline);
                (void)emptyLast;
                
                cmsStage* emptyUnlinked = NULL;
                // Unlink from an empty pipeline should return NULL safely
                cmsPipelineUnlinkStage(pipeline, 0, &emptyUnlinked);
                (void)emptyUnlinked;
                
                // Try to unlink from the other end
                cmsPipelineUnlinkStage(pipeline, 1, &emptyUnlinked);
                (void)emptyUnlinked;

                // Free the pipeline
                cleanup_pipeline(pipeline);
            }
            
            cmsCloseProfile(hProfile);
        }
    }

    // Create a pipeline and try to add a stage if possible, but LCMS2 doesn't expose
    // easy ways to create stages without internal knowledge.
    // We can try to create a transform and see if we can extract a pipeline,
    // but that's complex. For now, we stick to the empty pipeline tests.
    
    // Test cmsStageDup with a NULL pointer (should return NULL safely)
    cmsStage* dupStage = cmsStageDup(NULL);
    (void)dupStage;

    cmsDeleteContext(ctx);
    return 0;
}