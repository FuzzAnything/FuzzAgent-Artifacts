// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Static helper to ensure cleanup of allocated resources
static void cleanup_resources(cmsContext ctx, cmsMLU *mlu, cmsSEQ *seq, void *mem) {
    if (mlu != NULL) {
        cmsMLUfree(mlu);
    }
    if (seq != NULL) {
        cmsFreeProfileSequenceDescription(seq);
    }
    if (mem != NULL) {
        _cmsFree(ctx, mem);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsMLU *mlu = NULL;
    cmsSEQ *seq = NULL;
    cmsSEQ *dup_seq = NULL;
    void *mem = NULL;

    // Extract parameters from fuzz data
    uint32_t n_items = 0;
    uint32_t n_seq = 0;
    uint32_t alloc_size = 0;

    if (Size >= 4) {
        n_items = *(const uint32_t *)(Data);
        if (Size >= 8) {
            n_seq = *(const uint32_t *)(Data + 4);
            if (Size >= 12) {
                alloc_size = *(const uint32_t *)(Data + 8);
            }
        }
    }

    // Fuzz cmsMLUalloc: Ensure nItems >= 2 as per spec, but also test boundary conditions
    // We force n_items to be at least 2 for valid allocation, or 0/1 to test failure paths
    uint32_t test_n_items = n_items;
    if (test_n_items < 2 && test_n_items != 0 && test_n_items != 1) {
        test_n_items = 2;
    }
    mlu = cmsMLUalloc(ctx, test_n_items);

    // Fuzz _cmsMalloc
    // Limit size to prevent OOM or excessive allocation in fuzzing
    if (alloc_size > 0 && alloc_size < 1024 * 1024) {
        mem = _cmsMalloc(ctx, alloc_size);
        if (mem != NULL) {
            // Write some data to the allocated memory to trigger potential memory corruption checks
            memset(mem, 0xAA, alloc_size);
        }
    }

    // Fuzz cmsAllocProfileSequenceDescription
    // Ensure n is within valid range (1-255) or 0 to test failure
    uint32_t test_n_seq = n_seq;
    if (test_n_seq > 255) test_n_seq = 256; // Test overflow boundary
    if (test_n_seq == 0) test_n_seq = 0;   // Test zero boundary
    else if (test_n_seq > 0 && test_n_seq <= 255) {
        // Valid range
    } else {
        test_n_seq = 256; // Force invalid
    }
    seq = cmsAllocProfileSequenceDescription(ctx, test_n_seq);

    // Fuzz cmsDupProfileSequenceDescription
    // Only attempt if seq is valid
    if (seq != NULL) {
        dup_seq = cmsDupProfileSequenceDescription(seq);
    }

    // Fuzz _cmsFree with potentially NULL or valid pointers
    // This tests the internal NULL check logic
    _cmsFree(ctx, NULL);
    if (mem != NULL) {
        _cmsFree(ctx, mem);
        mem = NULL; // Prevent double free
    }

    // Cleanup
    cleanup_resources(ctx, mlu, seq, NULL);
    if (dup_seq != NULL) {
        cmsFreeProfileSequenceDescription(dup_seq);
    }

    // Destroy context
    cmsDeleteContext(ctx);

    return 0;
}