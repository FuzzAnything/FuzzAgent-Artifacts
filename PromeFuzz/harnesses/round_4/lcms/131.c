// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsGetPipelineContextID at cmslut.c:1407:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    // Create a context first
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Use fuzz data to determine parameters for cmsStageAllocCLutFloat
    // Ensure we have enough data for basic parameters
    if (Size < 12) {
        cmsDeleteContext(context);
        return 0;
    }

    // Extract parameters from fuzz data
    cmsUInt32Number nGridPoints = *(const cmsUInt32Number*)Data;
    cmsUInt32Number inputChan = *(const cmsUInt32Number*)(Data + 4);
    cmsUInt32Number outputChan = *(const cmsUInt32Number*)(Data + 8);

    // Limit values to reasonable ranges to avoid excessive memory allocation
    if (nGridPoints < 2) nGridPoints = 2;
    if (nGridPoints > 16) nGridPoints = 16;
    if (inputChan < 1) inputChan = 1;
    if (inputChan > 16) inputChan = 16;
    if (outputChan < 1) outputChan = 1;
    if (outputChan > 16) outputChan = 16;

    // Calculate table size
    size_t tableSize = 1;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        tableSize *= nGridPoints;
    }
    tableSize *= outputChan;

    // Allocate and fill table with fuzz data
    cmsFloat32Number* table = (cmsFloat32Number*)malloc(tableSize * sizeof(cmsFloat32Number));
    if (!table) {
        cmsDeleteContext(context);
        return 0;
    }

    // Fill table with fuzz data, using remaining input
    size_t dataOffset = 12;
    for (size_t i = 0; i < tableSize && dataOffset + 3 < Size; i++) {
        // Convert 4 bytes to float (simple conversion, not proper IEEE)
        uint32_t temp = 0;
        memcpy(&temp, Data + dataOffset, 4);
        table[i] = (cmsFloat32Number)temp / (cmsFloat32Number)UINT32_MAX;
        dataOffset += 4;
    }

    // Create CLUT stage
    cmsStage* stage = cmsStageAllocCLutFloat(context, nGridPoints, inputChan, outputChan, table);
    free(table);

    if (stage) {
        // Test cmsGetStageContextID
        cmsContext stageContext = cmsGetStageContextID(stage);
        
        // Create a simple pipeline to test cmsGetPipelineContextID
        cmsPipeline* pipeline = cmsPipelineAlloc(context, inputChan, outputChan);
        if (pipeline) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
            cmsContext pipelineContext = cmsGetPipelineContextID(pipeline);
            cmsPipelineFree(pipeline);
        } else {
            cmsStageFree(stage);
        }
    }

    // Test cmsGetProfileContextID with dummy profiles
    cmsHPROFILE profile = cmsCreate_sRGBProfile();
    if (profile) {
        cmsContext profileContext = cmsGetProfileContextID(profile);
        cmsCloseProfile(profile);
    }

    // Test cmsGetTransformContextID with dummy transform
    cmsHPROFILE inProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE outProfile = cmsCreateLab4Profile(NULL);
    if (inProfile && outProfile) {
        cmsHTRANSFORM transform = cmsCreateTransform(inProfile, TYPE_RGB_8, 
                                                    outProfile, TYPE_Lab_DBL, 
                                                    INTENT_PERCEPTUAL, 0);
        if (transform) {
            cmsContext transformContext = cmsGetTransformContextID(transform);
            cmsDeleteTransform(transform);
        }
    }
    if (inProfile) cmsCloseProfile(inProfile);
    if (outProfile) cmsCloseProfile(outProfile);

    // Clean up
    cmsDeleteContext(context);
    return 0;
}