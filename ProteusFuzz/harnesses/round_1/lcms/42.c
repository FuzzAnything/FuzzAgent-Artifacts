// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_stage(cmsStage *stage) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Extract parameters from input data
    cmsUInt32Number size = Data[0];
    cmsUInt32Number num = Data[1];
    cmsUInt32Number size_elem = Data[2];
    cmsUInt32Number intent = Data[3];
    cmsUInt32Number flags = 0;
    if (Size > 4) flags = Data[4];
    if (Size > 5) flags |= (Data[5] << 8);
    if (Size > 6) flags |= (Data[6] << 16);
    if (Size > 7) flags |= (Data[7] << 24);

    // Test _cmsMallocZero
    // We must free this using the context's free function to avoid leaks.
    // Since _cmsMallocZero uses the plugin's MallocZeroPtr, we use _cmsFree with the same context.
    void *malloced = _cmsMallocZero(ctx, size);
    if (malloced != NULL) {
        _cmsFree(ctx, malloced);
    }

    // Test _cmsCalloc
    void *calloced = _cmsCalloc(ctx, num, size_elem);
    if (calloced != NULL) {
        _cmsFree(ctx, calloced);
    }

    // Create dummy profiles from input data if possible
    cmsHPROFILE inputProfile = NULL;
    cmsHPROFILE outputProfile = NULL;
    cmsHPROFILE proofingProfile = NULL;

    if (Size >= 100) {
        inputProfile = cmsOpenProfileFromMem((const uint8_t *)Data, Size);
        if (inputProfile == NULL) {
            inputProfile = cmsCreate_sRGBProfile();
        }
    } else {
        inputProfile = cmsCreate_sRGBProfile();
    }

    if (inputProfile != NULL) {
        outputProfile = cmsCreate_sRGBProfile();
        proofingProfile = cmsCreate_sRGBProfile();
    }

    cmsContext profileCtx = NULL;
    if (inputProfile != NULL) {
        profileCtx = cmsGetProfileContextID(inputProfile);
    }

    cmsUInt32Number inputFormat = (Size > 8) ? Data[8] : 0;
    cmsUInt32Number outputFormat = (Size > 9) ? Data[9] : 0;

    // Test cmsCreateTransformTHR
    cmsHTRANSFORM transform = cmsCreateTransformTHR(
        ctx,
        inputProfile,
        inputFormat,
        outputProfile,
        outputFormat,
        intent,
        flags
    );
    cleanup_transform(transform);

    // Test cmsCreateProofingTransformTHR
    cmsHTRANSFORM proofTransform = cmsCreateProofingTransformTHR(
        ctx,
        inputProfile,
        inputFormat,
        outputProfile,
        outputFormat,
        proofingProfile,
        intent,
        intent,
        flags
    );
    cleanup_transform(proofTransform);

    // Test cmsStageAllocCLutFloatGranular
    cmsUInt32Number clutPoints[4] = {1, 1, 1, 1};
    if (Size > 10) clutPoints[0] = Data[10];
    if (Size > 11) clutPoints[1] = Data[11];
    if (Size > 12) clutPoints[2] = Data[12];
    if (Size > 13) clutPoints[3] = Data[13];

    cmsUInt32Number inputChan = (Size > 14) ? Data[14] : 3;
    cmsUInt32Number outputChan = (Size > 15) ? Data[15] : 3;

    cmsFloat32Number *table = NULL;
    size_t tableSize = 1;
    for (int i = 0; i < 4; i++) {
        if (clutPoints[i] > 0) tableSize *= clutPoints[i];
    }
    // Prevent huge allocations
    if (tableSize > 1000000) tableSize = 1000000;

    table = (cmsFloat32Number *)malloc(tableSize * sizeof(cmsFloat32Number));
    if (table != NULL) {
        for (size_t i = 0; i < tableSize; i++) {
            table[i] = (float)((i % 256) / 255.0f);
        }
    }

    cmsStage *stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, table);
    cleanup_stage(stage);
    if (table != NULL) free(table);

    // Cleanup profiles
    cleanup_profile(inputProfile);
    cleanup_profile(outputProfile);
    cleanup_profile(proofingProfile);

    // Cleanup context to free any remaining allocations associated with it
    cmsDeleteContext(ctx);

    return 0;
}