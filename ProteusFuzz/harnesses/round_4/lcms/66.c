// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_sequence(cmsContext ctx, cmsSEQ *seq) {
    if (seq != NULL) {
        cmsFreeProfileSequenceDescription(seq);
    }
}

static void cleanup_mlu(cmsMLU *mlu) {
    if (mlu != NULL) {
        cmsMLUfree(mlu);
    }
}

static void cleanup_generic(cmsContext ctx, void *ptr) {
    if (ptr != NULL) {
        _cmsFree(ctx, ptr);
    }
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Extract parameters from fuzz input
    cmsUInt32Number nItems = 0;
    cmsUInt32Number nSeq = 0;
    cmsUInt32Number allocSize = 0;

    if (Size >= 4) {
        memcpy(&nItems, Data, 4);
        memcpy(&nSeq, Data + 4, 4);
        if (Size >= 8) {
            memcpy(&allocSize, Data + 8, 4);
        }
    }

    // Clamp values to reasonable ranges to avoid immediate crashes or OOM
    if (nItems > 10000) nItems = 10000;
    if (nSeq > 255) nSeq = 255;
    if (allocSize > 1000000) allocSize = 1000000;

    // Test 1: cmsMLUalloc
    cmsMLU *mlu = NULL;
    if (nItems >= 2) {
        mlu = cmsMLUalloc(ctx, nItems);
    }
    if (mlu != NULL) {
        // Try to free it
        cleanup_mlu(mlu);
    }

    // Test 2: cmsAllocProfileSequenceDescription
    cmsSEQ *seq = NULL;
    if (nSeq > 0 && nSeq <= 255) {
        seq = cmsAllocProfileSequenceDescription(ctx, nSeq);
    }

    // Test 3: cmsDupProfileSequenceDescription
    cmsSEQ *seq_dup = NULL;
    if (seq != NULL) {
        seq_dup = cmsDupProfileSequenceDescription(seq);
    }

    // Test 4: _cmsMalloc
    void *mem = NULL;
    if (allocSize > 0) {
        mem = _cmsMalloc(ctx, allocSize);
    }

    // Test 5: _cmsFree
    if (mem != NULL) {
        cleanup_generic(ctx, mem);
    }

    // Cleanup allocated sequences
    if (seq_dup != NULL) {
        cleanup_sequence(ctx, seq_dup);
    }
    if (seq != NULL) {
        cleanup_sequence(ctx, seq);
    }

    // Destroy context
    cmsDeleteContext(ctx);

    return 0;
}