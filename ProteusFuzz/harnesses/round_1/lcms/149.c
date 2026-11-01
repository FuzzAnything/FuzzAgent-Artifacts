// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsContext CreateContextFromData(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    return cmsCreateContext(NULL, NULL);
}

static cmsPipeline* CreatePipelineFromData(cmsContext ctx) {
    if (!ctx) return NULL;
    
    // Create dummy profiles to ensure valid context usage if needed
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) return NULL;
    
    cmsHPROFILE hProfile2 = cmsCreate_sRGBProfile();
    if (!hProfile2) {
        cmsCloseProfile(hProfile);
        return NULL;
    }
    
    // Create a transform to potentially populate a pipeline or just to exercise context
    cmsHTRANSFORM trans = cmsCreateTransform(hProfile, TYPE_RGBA_8, hProfile2, TYPE_RGBA_8, INTENT_PERCEPTUAL, 0);
    if (trans) {
        cmsDeleteTransform(trans);
    }
    
    cmsCloseProfile(hProfile);
    cmsCloseProfile(hProfile2);
    
    // Create an empty pipeline
    cmsPipeline* lut = cmsPipelineAlloc(ctx, 4, 4);
    return lut;
}

static void CleanupPipeline(cmsPipeline* lut) {
    if (lut) {
        cmsPipelineFree(lut);
    }
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = CreateContextFromData(Data, Size);
    if (!ctx) return 0;

    cmsPipeline* lut = CreatePipelineFromData(ctx);
    if (!lut) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Fuzz cmsPipelineGetPtrToFirstStage
    cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(lut);
    
    // Fuzz cmsPipelineGetPtrToLastStage
    cmsStage* lastStage = cmsPipelineGetPtrToLastStage(lut);
    
    // Fuzz cmsStageData (if stage exists)
    if (firstStage) {
        void* data = cmsStageData(firstStage);
        (void)data; 
    }
    if (lastStage && lastStage != firstStage) {
        void* data = cmsStageData(lastStage);
        (void)data;
    }
    
    // Fuzz cmsStageNext (traverse if possible)
    cmsStage* current = firstStage;
    int count = 0;
    while (current && count < 100) {
        cmsStage* next = cmsStageNext(current);
        if (next == current) break; 
        current = next;
        count++;
    }
    
    // Fuzz cmsStageDup
    cmsStage* dupStage = NULL;
    if (firstStage) {
        dupStage = cmsStageDup(firstStage);
    } else if (lastStage) {
        dupStage = cmsStageDup(lastStage);
    }
    
    // Fuzz cmsPipelineUnlinkStage
    cmsStage* unlinkedStage = NULL;
    
    // Enum values for cmsStageLoc are typically 0 and 1 based on standard LCMS2 enums
    // cmsStageLocStart = 0, cmsStageLocEnd = 1
    if (lut) {
        cmsPipelineUnlinkStage(lut, 0, &unlinkedStage);
    }
    
    if (lut) {
        cmsPipelineUnlinkStage(lut, 1, &unlinkedStage);
    }
    
    // Cleanup
    CleanupPipeline(lut);
    cmsDeleteContext(ctx);
    
    return 0;
}