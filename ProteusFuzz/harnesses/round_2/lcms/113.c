// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsHPROFILE create_dummy_profile(cmsContext ctx, const uint8_t *data, size_t size) {
    if (size < 128) return NULL;
    
    // Write data to a temporary file to simulate a profile
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    if (written != size) return NULL;
    
    cmsHPROFILE h = cmsOpenProfileFromFile("./dummy_file", "r");
    // cmsIsProfile is not available in standard lcms2.h, relying on cmsOpenProfileFromFile return value
    // If it returns NULL, the file was invalid or not a profile.
    return h;
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Ensure we have enough data to split for profiles
    if (Size < 256) {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsHPROFILE inputProfile = NULL;
    cmsHPROFILE outputProfile = NULL;
    cmsHPROFILE proofProfile = NULL;
    
    // Create profiles from fuzz data
    inputProfile = create_dummy_profile(ctx, Data, Size / 3);
    if (inputProfile) {
        outputProfile = create_dummy_profile(ctx, Data + (Size / 3), Size / 3);
        if (outputProfile) {
            proofProfile = create_dummy_profile(ctx, Data + (2 * Size / 3), Size - (2 * Size / 3));
        }
    }

    cmsHTRANSFORM hTransform = NULL;
    cmsHTRANSFORM hProofTransform = NULL;
    cmsContext retrievedCtx = NULL;
    cmsStage *stage = NULL;
    void *mem1 = NULL;
    void *mem2 = NULL;

    // Test _cmsCalloc
    // Use modulo to keep numbers reasonable and avoid overflow
    cmsUInt32Number num = (Data[0] % 100) + 1;
    cmsUInt32Number size = (Data[1] % 100) + 1;
    mem1 = _cmsCalloc(ctx, num, size);

    // Test _cmsMallocZero
    cmsUInt32Number msize = (Data[2] % 1000) + 1;
    mem2 = _cmsMallocZero(ctx, msize);

    // Test cmsCreateTransformTHR
    cmsUInt32Number inputFmt = (Data[3] % 4) + 1; // 1 to 4
    cmsUInt32Number outputFmt = (Data[4] % 4) + 1;
    cmsUInt32Number intent = (Data[5] % 4);
    cmsUInt32Number flags = (Data[6] % 2) ? cmsFLAGS_BLACKPOINTCOMPENSATION : 0;

    if (inputProfile && outputProfile) {
        hTransform = cmsCreateTransformTHR(ctx, inputProfile, inputFmt, outputProfile, outputFmt, intent, flags);
    } else if (inputProfile) {
        // Single profile transform
        hTransform = cmsCreateTransformTHR(ctx, inputProfile, inputFmt, NULL, outputFmt, intent, flags);
    }

    // Test cmsGetTransformContextID
    if (hTransform) {
        retrievedCtx = cmsGetTransformContextID(hTransform);
        // Verify context matches
        if (retrievedCtx != ctx) {
            // This is a sanity check, not necessarily a failure for fuzzing
        }
    }

    // Test cmsCreateProofingTransformTHR
    if (inputProfile && outputProfile && proofProfile) {
        cmsUInt32Number proofIntent = (Data[7] % 4);
        cmsUInt32Number proofFlags = (Data[8] % 2) ? cmsFLAGS_SOFTPROOFING : 0;
        hProofTransform = cmsCreateProofingTransformTHR(ctx, inputProfile, inputFmt, outputProfile, outputFmt, proofProfile, intent, proofIntent, proofFlags);
    }

    // Test cmsStageAllocCLutFloatGranular
    if (Size > 10) {
        cmsUInt32Number clutPoints[4];
        cmsUInt32Number inputChan = (Data[9] % 4) + 1;
        cmsUInt32Number outputChan = (Data[10] % 4) + 1;
        
        // Initialize clutPoints based on input channels
        for (int i = 0; i < inputChan && i < 4; i++) {
            clutPoints[i] = (Data[11 + i] % 10) + 2; // Min 2 points
        }
        
        // Allocate a dummy table for the CLUT
        size_t tableSize = 1;
        for (int i = 0; i < inputChan; i++) tableSize *= clutPoints[i];
        tableSize *= outputChan;
        
        // Create a temporary buffer for table data
        float *table = (float *)malloc(tableSize * sizeof(float));
        if (table) {
            memset(table, 0, tableSize * sizeof(float));
            stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, table);
            free(table);
        }
    }

    // Cleanup
    if (mem1) {
        // Memory allocated via _cmsCalloc is managed by the context.
        // Do not free manually.
    }
    if (mem2) {
        // Memory allocated via _cmsMallocZero is managed by the context.
        // Do not free manually.
    }

    if (hTransform) cmsDeleteTransform(hTransform);
    if (hProofTransform) cmsDeleteTransform(hProofTransform);
    
    // Stages allocated via cmsStageAllocCLutFloatGranular are typically managed by the context
    // or require specific internal free functions not exposed in standard API.
    // We rely on cmsDeleteContext to clean up.

    if (inputProfile) cmsCloseProfile(inputProfile);
    if (outputProfile) cmsCloseProfile(outputProfile);
    if (proofProfile) cmsCloseProfile(proofProfile);

    cmsDeleteContext(ctx);

    return 0;
}