// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetPipelineContextID at cmslut.c:1407:22 in lcms2.h
// cmsGetPipelineContextID at cmslut.c:1407:22 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Custom callback implementations matching the expected signatures in lcms2_plugin.h
// Signature: void (*_cmsPipelineEval16Fn)(const cmsUInt16Number *In, cmsUInt16Number *Out, const void *UserData)
static void Eval16Fn(const cmsUInt16Number *In, cmsUInt16Number *Out, const void *UserData) {
    // Dummy evaluator: just copy input to output if pointers are valid
    if (In && Out) {
        // We don't know the pixel count here, so we assume a small safe copy or do nothing
        // In real usage, the pipeline passes the count, but the callback signature here 
        // doesn't expose it directly in this specific version's typedef if not careful.
        // However, standard lcms2 eval functions usually take count. 
        // Based on the error, the signature is (const In, Out, UserData).
        // Let's just do a safe no-op or minimal copy if we can infer size, 
        // but strictly adhering to the error message's implied signature:
        // The error says: 'void (*)(const unsigned short *, unsigned short *, const void *)'
        // So we implement exactly that.
        if (In[0] != 0) { // Simple heuristic to avoid infinite loops if we tried to guess size
             Out[0] = In[0];
        }
    }
}

// Signature: void (*_cmsFreeUserDataFn)(cmsContext ContextID, void *UserData)
static void FreePrivateDataFn(cmsContext ContextID, void *UserData) {
    if (UserData != NULL && UserData != (void*)0x12345678) {
        free(UserData);
    }
}

// Signature: void *(*_cmsDupUserDataFn)(cmsContext ContextID, const void *UserData)
static void* DupPrivateDataFn(cmsContext ContextID, const void *UserData) {
    if (UserData == NULL) return NULL;
    if (UserData == (void*)0x12345678) return (void*)0x12345678;
    
    size_t size = 64;
    void* new_data = malloc(size);
    if (new_data) {
        memcpy(new_data, UserData, size);
    }
    return new_data;
}

static cmsContext CreateContextFromInput(const uint8_t *Data, size_t Size) {
    if (Size < 4) return NULL;
    uint32_t seed = *(uint32_t*)Data;
    // Always return a valid context or NULL based on simple logic
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = NULL;
    cmsPipeline* pipe = NULL;
    cmsPipeline* dup_pipe = NULL;
    cmsPipeline* intent_pipe = NULL;

    // 1. Prepare environment
    ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Parse input for parameters
    cmsUInt32Number in_ch = (Data[0] % 10) + 1; // 1 to 10
    cmsUInt32Number out_ch = (Data[1] % 10) + 1; // 1 to 10
    cmsUInt32Number n_profiles = (Data[2] % 5); // 0 to 4
    cmsUInt32Number intent = (Data[3] % 4); // 0 to 3
    cmsUInt32Number flags = (Data[4] % 100);

    // 2. Test cmsPipelineAlloc
    if (in_ch < cmsMAXCHANNELS && out_ch < cmsMAXCHANNELS) {
        pipe = cmsPipelineAlloc(ctx, in_ch, out_ch);
    }

    // 3. Test _cmsPipelineSetOptimizationParameters
    if (pipe) {
        void* priv_data = NULL;
        if (Size > 5 && (Data[5] % 2) == 0) {
            priv_data = malloc(64);
            if (priv_data) {
                memset(priv_data, Data[5], 64);
            }
        } else {
            priv_data = (void*)0x12345678; // Magic pointer handled by callbacks
        }

        _cmsPipelineSetOptimizationParameters(pipe, Eval16Fn, priv_data, FreePrivateDataFn, DupPrivateDataFn);
    }

    // 4. Test cmsPipelineDup
    if (pipe) {
        dup_pipe = cmsPipelineDup(pipe);
    }

    // 5. Test cmsGetPipelineContextID
    if (pipe) {
        cmsContext pipe_ctx = cmsGetPipelineContextID(pipe);
        (void)pipe_ctx; 
    }
    if (dup_pipe) {
        cmsContext dup_ctx = cmsGetPipelineContextID(dup_pipe);
        (void)dup_ctx;
    }

    // 6. Test _cmsDefaultICCintents
    cmsHPROFILE hProfiles[5] = {0};
    cmsUInt32Number Intents[5] = {0};
    cmsBool BPC[5] = {0};
    cmsFloat64Number AdaptationStates[5] = {0.0};

    if (n_profiles > 0) {
        for (int i = 0; i < n_profiles && i < 5; i++) {
            Intents[i] = (Size > (size_t)(4 + i)) ? (Data[4 + i] % 4) : 0;
            hProfiles[i] = NULL; 
            BPC[i] = (Size > (size_t)(9 + i)) ? (Data[9 + i] % 2) : 0;
            AdaptationStates[i] = (Size > (size_t)(14 + i)) ? (double)Data[14 + i] / 255.0 : 0.0;
        }
        intent_pipe = _cmsDefaultICCintents(ctx, n_profiles, Intents, hProfiles, BPC, AdaptationStates, flags);
    }

    // 7. Cleanup
    if (pipe) cmsPipelineFree(pipe);
    if (dup_pipe) cmsPipelineFree(dup_pipe);
    if (intent_pipe) cmsPipelineFree(intent_pipe);
    if (ctx) cmsDeleteContext(ctx);

    return 0;
}