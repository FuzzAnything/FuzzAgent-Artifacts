// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_173(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsContext dup_ctx = NULL;
    void *ptr = NULL;
    cmsUInt32Number size_val = 0;
    cmsUInt32Number realloc_size = 0;
    cmsUInt32Number intents_count = 0;
    cmsUInt32Number *codes = NULL;
    char **descriptions = NULL;
    cmsUInt32Number max_intents = 10;

    // Parse input for diversification
    size_val = (Size > 0) ? Data[0] : 0;
    if (Size > 1) {
        realloc_size = Data[1];
    }
    if (Size > 2) {
        max_intents = Data[2] % 20 + 1;
    }

    // Test 1: Create a context and duplicate it
    ctx = cmsDupContext(NULL, NULL);
    if (ctx != NULL) {
        dup_ctx = cmsDupContext(ctx, (void *)Data);
        if (dup_ctx != NULL) {
            // Test _cmsMallocZero on duplicated context
            ptr = _cmsMallocZero(dup_ctx, size_val + 10);
            if (ptr != NULL) {
                // Test _cmsRealloc on duplicated context
                void *new_ptr = _cmsRealloc(dup_ctx, ptr, realloc_size + 10);
                if (new_ptr != NULL) {
                    ptr = new_ptr;
                }
                // Free manually allocated memory via context by reallocating to 0
                _cmsRealloc(dup_ctx, ptr, 0); 
                ptr = NULL;
            }

            // Test cmsGetSupportedIntentsTHR on duplicated context
            // Only allocate codes, do not allocate descriptions array or its elements
            // because the API fills descriptions with internal strings that must not be freed.
            codes = (cmsUInt32Number *)malloc(max_intents * sizeof(cmsUInt32Number));
            if (codes != NULL) {
                // Pass NULL for descriptions to avoid managing internal strings
                intents_count = cmsGetSupportedIntentsTHR(dup_ctx, max_intents, codes, NULL);
                free(codes);
                codes = NULL;
            }

            cleanup_context(dup_ctx);
        }

        // Test _cmsMalloc on original context
        ptr = _cmsMalloc(ctx, size_val + 10);
        if (ptr != NULL) {
            // Test _cmsRealloc on original context
            void *new_ptr = _cmsRealloc(ctx, ptr, realloc_size + 10);
            if (new_ptr != NULL) {
                ptr = new_ptr;
            }
            // Free manually allocated memory via context
            _cmsRealloc(ctx, ptr, 0);
            ptr = NULL;
        }

        // Test cmsGetSupportedIntentsTHR on original context
        codes = (cmsUInt32Number *)malloc(max_intents * sizeof(cmsUInt32Number));
        if (codes != NULL) {
            intents_count = cmsGetSupportedIntentsTHR(ctx, max_intents, codes, NULL);
            free(codes);
            codes = NULL;
        }

        cleanup_context(ctx);
    }

    // Test 2: Create a new context and test _cmsMallocZero
    ctx = cmsDupContext(NULL, NULL);
    if (ctx != NULL) {
        ptr = _cmsMallocZero(ctx, size_val + 10);
        if (ptr != NULL) {
            // Test _cmsRealloc
            void *new_ptr = _cmsRealloc(ctx, ptr, realloc_size + 10);
            if (new_ptr != NULL) {
                ptr = new_ptr;
            }
            // Free manually allocated memory via context
            _cmsRealloc(ctx, ptr, 0);
            ptr = NULL;
        }
        cleanup_context(ctx);
    }

    // Test 3: Test cmsGetSupportedIntentsTHR with NULL arrays
    ctx = cmsDupContext(NULL, NULL);
    if (ctx != NULL) {
        intents_count = cmsGetSupportedIntentsTHR(ctx, max_intents, NULL, NULL);
        cleanup_context(ctx);
    }

    // Test 4: Test cmsUnregisterPlugins (safe to call multiple times)
    cmsUnregisterPlugins();

    return 0;
}