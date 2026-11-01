// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineEvalReverseFloat at cmslut.c:1753:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static cmsContext gContext = NULL;

static void cleanup_context(void) {
    if (gContext != NULL) {
        cmsDeleteContext(gContext);
        gContext = NULL;
    }
}

static cmsPipeline* create_simple_pipeline(cmsContext ctx, cmsUInt32Number inCh, cmsUInt32Number outCh) {
    cmsPipeline* p = cmsPipelineAlloc(ctx, inCh, outCh);
    if (p == NULL) return NULL;
    // Create a simple identity stage if possible, or just return the empty pipeline
    // For fuzzing, an empty pipeline is a valid state to test
    return p;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize context if not already done
    if (gContext == NULL) {
        gContext = cmsCreateContext(NULL, NULL);
        if (gContext == NULL) return 0;
    }

    // Parse input data to determine behavior
    uint8_t op = Data[0] % 6; // 0-5 for the 6 functions
    cmsBool on_flag = (Data[0] & 1);
    
    // Prepare buffers
    cmsFloat32Number in[4] = {0.0, 0.0, 0.0, 0.0};
    cmsFloat32Number out[4] = {0.0, 0.0, 0.0, 0.0};
    cmsFloat32Number target[3] = {0.0, 0.0, 0.0};
    cmsFloat32Number result[4] = {0.0, 0.0, 0.0, 0.0};
    cmsFloat32Number hint[4] = {0.3, 0.3, 0.3, 0.3};

    cmsPipeline *lut1 = NULL;
    cmsPipeline *lut2 = NULL;
    cmsPipeline *lut3 = NULL;

    // Create pipelines for testing
    // Use data to vary channel counts (1 to 4)
    cmsUInt32Number ch1 = (Size > 1) ? (Data[1] % 4) + 1 : 3;
    cmsUInt32Number ch2 = (Size > 2) ? (Data[2] % 4) + 1 : 3;

    lut1 = create_simple_pipeline(gContext, ch1, ch1);
    lut2 = create_simple_pipeline(gContext, ch2, ch2);
    lut3 = create_simple_pipeline(gContext, ch1, ch2);

    switch (op) {
        case 0: {
            // cmsPipelineCheckAndRetreiveStages
            // We need a list of expected types and pointers.
            // Since we created empty pipelines, n=0 is a safe bet, or we try to match nothing.
            // The function takes varargs: n, types..., pointers...
            // Let's try with n=0 first (should match empty pipeline)
            cmsStage *stage_ptr = NULL;
            cmsBool res = cmsPipelineCheckAndRetreiveStages(lut1, 0);
            (void)res;
            (void)stage_ptr;
            break;
        }
        case 1: {
            // cmsPipelineEvalReverseFloat
            // Requires 3 or 4 input channels and exactly 3 output channels.
            // We need to ensure our pipeline matches this.
            // Let's create a specific pipeline for this if possible, or just pass what we have and expect failure.
            cmsFloat32Number t[3] = {0.5, 0.5, 0.5};
            cmsFloat32Number r[4] = {0.0};
            cmsFloat32Number h[4] = {0.3};
            
            // Try with current lut1 if it has 3 outputs, otherwise it will return FALSE safely
            cmsBool res = cmsPipelineEvalReverseFloat(t, r, h, lut1);
            (void)res;
            break;
        }
        case 2: {
            // cmsPipelineFree
            // Free one of the pipelines
            cmsPipelineFree(lut1);
            lut1 = NULL;
            break;
        }
        case 3: {
            // cmsPipelineEvalFloat
            // Fill input with fuzz data
            for (int i = 0; i < 4 && i < Size; i++) {
                in[i] = (float)(Data[i+1]) / 255.0f;
            }
            if (lut1 != NULL) {
                cmsPipelineEvalFloat(in, out, lut1);
            }
            break;
        }
        case 4: {
            // cmsPipelineSetSaveAs8bitsFlag
            if (lut1 != NULL) {
                cmsBool prev = cmsPipelineSetSaveAs8bitsFlag(lut1, on_flag);
                (void)prev;
            }
            break;
        }
        case 5: {
            // cmsPipelineCat
            // Concatenate lut2 to lut1
            if (lut1 != NULL && lut2 != NULL) {
                cmsBool res = cmsPipelineCat(lut1, lut2);
                (void)res;
            }
            break;
        }
    }

    // Cleanup
    cmsPipelineFree(lut1);
    cmsPipelineFree(lut2);
    cmsPipelineFree(lut3);

    // If this is the last input (unlikely in fuzzing loop, but good practice), we could cleanup context
    // But usually we keep context alive for efficiency.
    
    return 0;
}