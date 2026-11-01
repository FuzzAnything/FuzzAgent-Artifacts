// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineEvalReverseFloat at cmslut.c:1753:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsContext CreateContext(void) {
    return cmsCreateContext(NULL, NULL);
}

static void CleanupContext(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsPipeline* CreateEmptyPipeline(cmsContext ctx, cmsUInt32Number inCh, cmsUInt32Number outCh) {
    if (!ctx) return NULL;
    return cmsPipelineAlloc(ctx, inCh, outCh);
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = CreateContext();
    if (!ctx) return 0;

    uint8_t op = Data[0] % 10;
    uint8_t channels = (Data[1] % 8) + 1; // 1 to 8 channels

    cmsPipeline *lut1 = CreateEmptyPipeline(ctx, channels, channels);
    cmsPipeline *lut2 = CreateEmptyPipeline(ctx, channels, channels);

    if (!lut1 || !lut2) {
        CleanupContext(ctx);
        return 0;
    }

    cmsFloat32Number *in = NULL;
    cmsFloat32Number *out = NULL;
    cmsFloat32Number *target = NULL;
    cmsFloat32Number *result = NULL;
    cmsFloat32Number *hint = NULL;

    size_t float_size = channels * sizeof(cmsFloat32Number);
    if (Size > 1 && float_size > 0) {
        in = (cmsFloat32Number*)malloc(float_size);
        out = (cmsFloat32Number*)malloc(float_size);
        target = (cmsFloat32Number*)malloc(float_size);
        result = (cmsFloat32Number*)malloc(float_size);
        hint = (cmsFloat32Number*)malloc(float_size);

        if (in && out && target && result && hint) {
            size_t copy_len = (Size - 1 < float_size) ? (Size - 1) : float_size;
            memcpy(in, Data + 1, copy_len);
            memcpy(target, Data + 1, copy_len);
            memset(hint, 0, float_size);
            
            if (Size > 1 + float_size + sizeof(float)) {
                float val = 0.3f;
                if (Size > 1 + float_size + 4) {
                    uint32_t raw = 0;
                    memcpy(&raw, Data + 1 + float_size, sizeof(uint32_t));
                    val = *(float*)&raw;
                }
                for (int i = 0; i < channels; i++) hint[i] = val;
            }
        }
    }

    switch (op) {
        case 0:
            cmsPipelineFree(lut1);
            lut1 = NULL;
            cmsPipelineFree(lut2);
            break;
        case 1:
            cmsPipelineFree(NULL);
            cmsPipelineFree(lut1);
            cmsPipelineFree(lut2);
            break;
        case 2:
            if (lut1 && lut2) {
                cmsPipelineCat(lut1, lut2);
                cmsPipelineFree(lut1);
                cmsPipelineFree(lut2);
            }
            break;
        case 3:
            if (lut1 && in && out) {
                cmsPipelineEvalFloat(in, out, lut1);
                cmsPipelineFree(lut1);
                cmsPipelineFree(lut2);
            }
            break;
        case 4:
            if (lut1 && target && result && hint && (channels == 3 || channels == 4)) {
                cmsPipelineEvalReverseFloat(target, result, hint, lut1);
            }
            cmsPipelineFree(lut1);
            cmsPipelineFree(lut2);
            break;
        case 5:
            if (lut1) {
                cmsPipelineInsertStage(lut1, cmsAT_BEGIN, NULL);
                cmsPipelineInsertStage(lut1, cmsAT_END, NULL);
                cmsPipelineInsertStage(NULL, cmsAT_BEGIN, NULL);
                cmsPipelineInsertStage(lut1, (cmsStageLoc)99, NULL);
            }
            cmsPipelineFree(lut1);
            cmsPipelineFree(lut2);
            break;
        case 6:
            if (lut1) {
                cmsBool prev = cmsPipelineSetSaveAs8bitsFlag(lut1, TRUE);
                cmsPipelineSetSaveAs8bitsFlag(lut1, FALSE);
                // Removed NULL call to prevent SEGV
            }
            cmsPipelineFree(lut1);
            cmsPipelineFree(lut2);
            break;
        case 7:
            if (lut1 && lut2) {
                cmsPipelineCat(lut1, lut2);
                if (in && out) cmsPipelineEvalFloat(in, out, lut1);
            }
            cmsPipelineFree(lut1);
            cmsPipelineFree(lut2);
            break;
        case 8:
            cmsPipeline *empty1 = CreateEmptyPipeline(ctx, 0, 0);
            cmsPipeline *empty2 = CreateEmptyPipeline(ctx, 0, 0);
            if (empty1 && empty2) {
                cmsPipelineCat(empty1, empty2);
                cmsPipelineFree(empty1);
                cmsPipelineFree(empty2);
            }
            cmsPipelineFree(lut1);
            cmsPipelineFree(lut2);
            break;
        case 9:
            cmsPipelineFree(lut1);
            // Avoid double free on same pointer if not handled by library
            // cmsPipelineFree(lut1); 
            cmsPipelineFree(lut2);
            break;
        default:
            cmsPipelineFree(lut1);
            cmsPipelineFree(lut2);
            break;
    }

    if (in) free(in);
    if (out) free(out);
    if (target) free(target);
    if (result) free(result);
    if (hint) free(hint);

    CleanupContext(ctx);
    return 0;
}