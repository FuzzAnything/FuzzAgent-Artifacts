// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_pipeline(cmsPipeline* p) {
    if (p != NULL) {
        cmsPipelineFree(p);
    }
}

static cmsPipeline* create_dummy_pipeline(cmsContext ctx) {
    if (ctx == NULL) return NULL;

    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        return NULL;
    }

    cmsPipeline* p = cmsPipelineAlloc(ctx, 3, 3);
    if (p == NULL) {
        cleanup_profile(hProfile);
        return NULL;
    }

    cmsStage* s = cmsStageAllocIdentity(ctx, 3);
    if (s == NULL) {
        cleanup_pipeline(p);
        cleanup_profile(hProfile);
        return NULL;
    }

    cmsPipelineInsertStage(p, cmsAT_END, s);
    cleanup_profile(hProfile);
    return p;
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Create a base pipeline for testing
    cmsPipeline* p1 = create_dummy_pipeline(ctx);
    if (p1 == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsPipeline* p2 = create_dummy_pipeline(ctx);
    if (p2 == NULL) {
        cleanup_pipeline(p1);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Fuzzing logic based on input data
    uint8_t mode = Data[0] % 6;

    switch (mode) {
        case 0: {
            // Test cmsPipelineInsertStage
            cmsStage* newStage = cmsStageAllocIdentity(ctx, 3);
            if (newStage != NULL) {
                cmsBool loc = (Data[1] % 2) ? cmsAT_BEGIN : cmsAT_END;
                cmsPipelineInsertStage(p1, loc, newStage);
                // Stage is now owned by pipeline, do not free manually
            }
            break;
        }
        case 1: {
            // Test cmsPipelineDup
            cmsPipeline* dup = cmsPipelineDup(p1);
            if (dup != NULL) {
                cleanup_pipeline(dup);
            }
            break;
        }
        case 2: {
            // Test cmsPipelineCheckAndRetreiveStages
            cmsStage* s1 = NULL, *s2 = NULL;
            // Try to check for 1 stage of type cmsSigIdentityElemType
            cmsPipelineCheckAndRetreiveStages(p1, 1, cmsSigIdentityElemType, &s1);
            // Try to check for 2 stages (might fail if only 1 exists)
            cmsPipelineCheckAndRetreiveStages(p1, 2, cmsSigIdentityElemType, &s1, cmsSigIdentityElemType, &s2);
            break;
        }
        case 3: {
            // Test _cmsDefaultICCintents
            // Ensure arrays are large enough for the maximum possible nProfiles we will use
            cmsHPROFILE hProfiles[4];
            cmsUInt32Number intents[4];
            cmsBool bpc[4];
            cmsFloat64Number adaptation[4];
            
            cmsUInt32Number nProfiles = 0;
            cmsUInt32Number flags = 0;

            // Initialize arrays to NULL/0
            for (int i = 0; i < 4; i++) {
                hProfiles[i] = NULL;
                intents[i] = 0;
                bpc[i] = FALSE;
                adaptation[i] = 0.0;
            }

            // Create valid profiles if data allows
            // Limit nProfiles to 1 or 2 to keep it simple and safe
            if (Data[1] % 2 == 0) {
                hProfiles[0] = cmsCreate_sRGBProfile();
                if (hProfiles[0] != NULL) nProfiles = 1;
            }
            if (nProfiles > 0 && Data[1] % 3 == 0) {
                hProfiles[1] = cmsCreate_sRGBProfile();
                if (hProfiles[1] != NULL) nProfiles = 2;
            }

            if (nProfiles > 0) {
                // Ensure intents, bpc, and adaptation are populated for nProfiles
                for (int i = 0; i < nProfiles; i++) {
                    intents[i] = Data[1] % 4; // 0 to 3 intents
                    bpc[i] = (Data[1] % 2) ? TRUE : FALSE;
                    adaptation[i] = (double)(Data[1] % 100) / 100.0;
                }
                
                flags = Data[1] % 8;

                cmsPipeline* res = _cmsDefaultICCintents(ctx, nProfiles, intents, hProfiles, bpc, adaptation, flags);
                if (res != NULL) {
                    cleanup_pipeline(res);
                }
            }

            for (int i = 0; i < nProfiles; i++) {
                cleanup_profile(hProfiles[i]);
            }
            break;
        }
        case 4: {
            // Test cmsPipelineCat
            cmsPipeline* p3 = create_dummy_pipeline(ctx);
            if (p3 != NULL) {
                cmsPipelineCat(p1, p3);
                cleanup_pipeline(p3);
            }
            break;
        }
        case 5: {
            // Test cmsPipelineSetSaveAs8bitsFlag
            cmsBool on = (Data[1] % 2) ? TRUE : FALSE;
            cmsPipelineSetSaveAs8bitsFlag(p1, on);
            break;
        }
    }

    // Cleanup
    cleanup_pipeline(p1);
    cleanup_pipeline(p2);
    cmsDeleteContext(ctx);

    return 0;
}