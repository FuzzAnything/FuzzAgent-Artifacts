// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGetPipelineContextID at cmslut.c:1407:22 in lcms2.h
// cmsGetPipelineContextID at cmslut.c:1407:22 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_contexts(cmsContext ctx, cmsContext dup_ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
    if (dup_ctx != NULL && dup_ctx != ctx) {
        cmsDeleteContext(dup_ctx);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsContext dup_ctx = NULL;
    cmsPipeline *pipeline = NULL;
    cmsHTRANSFORM hTransform = NULL;
    void *mutex = NULL;
    cmsHANDLE gdb = NULL;
    void *user_data = NULL;
    cmsHPROFILE hProfileIn = NULL;
    cmsHPROFILE hProfileOut = NULL;

    // Initialize context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Fuzz cmsDupContext
    if (Size > 0) {
        void *new_user_data = (void *)(uintptr_t)Data[0];
        dup_ctx = cmsDupContext(ctx, new_user_data);
    } else {
        dup_ctx = cmsDupContext(ctx, NULL);
    }

    // Fuzz cmsGetContextUserData
    if (dup_ctx != NULL) {
        user_data = cmsGetContextUserData(dup_ctx);
        (void)user_data; // Use variable to avoid unused warning
    }

    // Fuzz _cmsCreateMutex
    if (ctx != NULL) {
        mutex = _cmsCreateMutex(ctx);
        // Mutex is not freed here as lcms manages it or it's opaque
    }

    // Fuzz cmsGBDAlloc
    if (ctx != NULL) {
        gdb = cmsGBDAlloc(ctx);
        if (gdb != NULL) {
            // GDB is managed by the context, no explicit free needed here
        }
    }

    // Fuzz cmsGetPipelineContextID
    // We need a valid pipeline. Since we can't easily create one without more complex setup,
    // we test with NULL and a potentially invalid pointer derived from fuzz data.
    cmsGetPipelineContextID(NULL);
    if (Size >= sizeof(cmsPipeline *)) {
        cmsPipeline *fake_pipeline = *(cmsPipeline **)Data;
        // This might crash if fake_pipeline is invalid, which is expected in fuzzing
        cmsGetPipelineContextID(fake_pipeline);
    }

    // Fuzz cmsGetTransformContextID
    cmsGetTransformContextID(NULL);
    
    // Attempt to create a transform if possible. 
    if (Size >= 128) {
        // Write data to a dummy file
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
            
            hProfileIn = cmsOpenProfileFromFile("./dummy_file", "r");
            hProfileOut = hProfileIn; // Use same for identity
            
            if (hProfileIn != NULL) {
                // Create a transform (Identity)
                hTransform = cmsCreateTransform(hProfileIn, cmsSigXYZData, hProfileOut, cmsSigXYZData, 8, 0);
                if (hTransform != NULL) {
                    cmsContext trans_ctx = cmsGetTransformContextID(hTransform);
                    (void)trans_ctx;
                }
            }
        }
    }

    // Cleanup
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
    if (hProfileIn != NULL) {
        cmsCloseProfile(hProfileIn);
    }
    if (hProfileOut != NULL && hProfileOut != hProfileIn) {
        cmsCloseProfile(hProfileOut);
    }
    
    // Remove dummy file
    remove("./dummy_file");

    cleanup_contexts(ctx, dup_ctx);

    return 0;
}