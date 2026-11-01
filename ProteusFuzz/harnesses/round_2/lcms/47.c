// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_pipeline(cmsPipeline *p) {
    if (p) {
        cmsPipelineFree(p);
    }
}

static void cleanup_profiles(cmsHPROFILE *profiles, size_t count) {
    if (profiles) {
        for (size_t i = 0; i < count; i++) {
            if (profiles[i]) {
                cmsCloseProfile(profiles[i]);
            }
        }
        free(profiles);
    }
}

static cmsPipeline* create_dummy_pipeline(cmsContext ctx) {
    // Create a simple identity pipeline for testing
    cmsPipeline *p = cmsPipelineAlloc(ctx, 3, 3);
    if (!p) return NULL;
    return p;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use data to determine operations
    uint8_t op = Data[0] % 6;
    
    cmsPipeline *p1 = NULL;
    cmsPipeline *p2 = NULL;
    cmsPipeline *p_dup = NULL;
    cmsBool result = FALSE;
    
    // Create base pipelines
    p1 = create_dummy_pipeline(ctx);
    p2 = create_dummy_pipeline(ctx);
    
    if (!p1 || !p2) {
        cleanup_pipeline(p1);
        cleanup_pipeline(p2);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    switch (op) {
        case 0: {
            // Test cmsPipelineCat
            // Concatenate p2 to p1
            result = cmsPipelineCat(p1, p2);
            break;
        }
        case 1: {
            // Test cmsPipelineSetSaveAs8bitsFlag
            cmsBool flag = (Size > 1 && Data[1] % 2) ? TRUE : FALSE;
            result = cmsPipelineSetSaveAs8bitsFlag(p1, flag);
            break;
        }
        case 2: {
            // Test cmsPipelineInsertStage
            // We need a stage to insert. Since we can't easily create one without more API,
            // we'll try with NULL stage to test error handling
            cmsStageLoc loc = (Size > 1 && Data[1] % 2) ? cmsAT_BEGIN : cmsAT_END;
            result = cmsPipelineInsertStage(p1, loc, NULL);
            break;
        }
        case 3: {
            // Test _cmsDefaultICCintents
            // Create dummy profiles
            cmsHPROFILE *profiles = malloc(2 * sizeof(cmsHPROFILE));
            if (!profiles) {
                cleanup_pipeline(p1);
                cleanup_pipeline(p2);
                cmsDeleteContext(ctx);
                return 0;
            }
            
            // Create simple sRGB profiles
            profiles[0] = cmsCreate_sRGBProfile();
            profiles[1] = cmsCreate_sRGBProfile();
            
            if (!profiles[0] || !profiles[1]) {
                cleanup_profiles(profiles, 2);
                cleanup_pipeline(p1);
                cleanup_pipeline(p2);
                cmsDeleteContext(ctx);
                return 0;
            }
            
            cmsUInt32Number intents[2] = {0, 1};
            cmsBool bpc[2] = {FALSE, FALSE};
            cmsFloat64Number adaptation[2] = {1.0, 1.0};
            cmsUInt32Number flags = (Size > 1) ? Data[1] : 0;
            
            cmsPipeline *p_intents = _cmsDefaultICCintents(
                ctx, 
                2, 
                intents, 
                profiles, 
                bpc, 
                adaptation, 
                flags
            );
            
            if (p_intents) {
                cmsPipelineFree(p_intents);
            }
            
            cleanup_profiles(profiles, 2);
            break;
        }
        case 4: {
            // Test cmsPipelineDup
            p_dup = cmsPipelineDup(p1);
            if (p_dup) {
                cmsPipelineFree(p_dup);
            }
            break;
        }
        case 5: {
            // Test cmsPipelineCheckAndRetreiveStages
            // Check for specific stage types (using NULL pointers for output)
            cmsStage *stage1 = NULL;
            cmsStage *stage2 = NULL;
            cmsStage *stage3 = NULL;
            
            // Try to check for 3 stages of unknown type
            result = cmsPipelineCheckAndRetreiveStages(
                p1, 
                3,
                NULL, // type1
                NULL, // type2
                NULL, // type3
                &stage1,
                &stage2,
                &stage3
            );
            break;
        }
    }
    
    // Cleanup
    cleanup_pipeline(p1);
    cleanup_pipeline(p2);
    cmsDeleteContext(ctx);
    
    return 0;
}