// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_resources(cmsContext ctx, cmsSEQ *seq1, cmsSEQ *seq2, cmsMLU *mlu) {
    if (seq1 != NULL) {
        cmsFreeProfileSequenceDescription(seq1);
    }
    if (seq2 != NULL) {
        cmsFreeProfileSequenceDescription(seq2);
    }
    if (mlu != NULL) {
        cmsMLUfree(mlu);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsSEQ *seq1 = NULL;
    cmsSEQ *seq2 = NULL;
    cmsMLU *mlu = NULL;

    // Extract parameters from fuzz input
    unsigned int n = 0;
    unsigned int nItems = 0;
    unsigned int op = 0;

    if (Size >= 12) {
        n = *(const unsigned int *)(Data);
        nItems = *(const unsigned int *)(Data + 4);
        op = *(const unsigned int *)(Data + 8);
    }

    // Fuzz cmsAllocProfileSequenceDescription
    // Clamp n to valid range [1, 255] for allocation, but also test edge cases
    if (op % 3 == 0) {
        // Test with potentially invalid n (0 or > 255) to trigger NULL returns
        seq1 = cmsAllocProfileSequenceDescription(ctx, n);
    } else if (op % 3 == 1) {
        // Test with clamped valid n
        unsigned int safe_n = (n == 0 || n > 255) ? 1 : n;
        if (safe_n > 255) safe_n = 255;
        seq1 = cmsAllocProfileSequenceDescription(ctx, safe_n);
    } else {
        // Test with specific boundary values
        unsigned int boundary_n = (n % 3 == 0) ? 0 : ((n % 3 == 1) ? 255 : 128);
        seq1 = cmsAllocProfileSequenceDescription(ctx, boundary_n);
    }

    // Fuzz cmsDupProfileSequenceDescription
    if (seq1 != NULL) {
        seq2 = cmsDupProfileSequenceDescription(seq1);
    } else {
        // Test with NULL input
        seq2 = cmsDupProfileSequenceDescription(NULL);
    }

    // Fuzz cmsMLUalloc
    // Ensure nItems >= 2 for valid allocation, but test edge cases too
    if (op % 3 == 0) {
        // Test with potentially invalid nItems (< 2)
        mlu = cmsMLUalloc(ctx, nItems);
    } else if (op % 3 == 1) {
        // Test with clamped valid nItems
        unsigned int safe_items = (nItems < 2) ? 2 : nItems;
        mlu = cmsMLUalloc(ctx, safe_items);
    } else {
        // Test with specific boundary values
        unsigned int boundary_items = (nItems % 3 == 0) ? 0 : ((nItems % 3 == 1) ? 1 : 2);
        mlu = cmsMLUalloc(ctx, boundary_items);
    }

    // Cleanup
    cleanup_resources(ctx, seq1, seq2, mlu);

    return 0;
}