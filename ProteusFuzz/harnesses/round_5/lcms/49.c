// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "lcms2.h"

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_seq(cmsSEQ* seq) {
    if (seq != NULL) {
        cmsFreeProfileSequenceDescription(seq);
    }
}

static void cleanup_mlu(cmsMLU* mlu) {
    if (mlu != NULL) {
        cmsMLUfree(mlu);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = NULL;
    cmsMLU* mlu = NULL;
    cmsSEQ* seq = NULL;
    cmsSEQ* dup_seq = NULL;

    // Initialize context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Extract parameters from fuzz data safely
    cmsUInt32Number nItems = (Data[0] % 255) + 2; // Ensure nItems >= 2
    cmsUInt32Number nSeq = (Data[1] % 255) + 1;   // Ensure nSeq >= 1 and <= 255

    // Fuzz cmsMLUalloc
    if (nItems > 0) {
        mlu = cmsMLUalloc(ctx, nItems);
        if (mlu != NULL) {
            // Prepare language and country codes safely
            char langCode[3];
            char countryCode[3];
            
            // Use Data[2] and Data[3] which are guaranteed to exist due to Size check
            langCode[0] = (char)((Data[2] % 26) + 'a');
            langCode[1] = (char)((Data[2] >> 4) % 26 + 'a');
            langCode[2] = '\0';

            countryCode[0] = (char)((Data[3] % 26) + 'a');
            countryCode[1] = (char)((Data[3] >> 4) % 26 + 'a');
            countryCode[2] = '\0';

            // Fuzz cmsMLUgetWide with NULL buffer to get size
            cmsUInt32Number sizeNeeded = cmsMLUgetWide(mlu, langCode, countryCode, NULL, 0);

            // Allocate buffer if size needed
            wchar_t* buffer = NULL;
            if (sizeNeeded > 0) {
                buffer = (wchar_t*)malloc(sizeNeeded * sizeof(wchar_t));
                if (buffer != NULL) {
                    cmsMLUgetWide(mlu, langCode, countryCode, buffer, sizeNeeded);
                    free(buffer);
                }
            }

            // Fuzz cmsMLUtranslationsCodes
            // Iterate through a safe range based on nItems allocated.
            // The actual UsedEntries might be 0 initially, so we try index 0 and potentially others.
            for (cmsUInt32Number idx = 0; idx < nItems; idx++) {
                char langOut[3], countryOut[3];
                cmsMLUtranslationsCodes(mlu, idx, langOut, countryOut);
            }
        }
    }

    // Fuzz cmsAllocProfileSequenceDescription
    seq = cmsAllocProfileSequenceDescription(ctx, nSeq);
    if (seq != NULL) {
        // Fuzz cmsDupProfileSequenceDescription
        dup_seq = cmsDupProfileSequenceDescription(seq);
    }

    // Cleanup
    cleanup_seq(seq);
    cleanup_seq(dup_seq);
    cleanup_mlu(mlu);
    cleanup_context(ctx);

    return 0;
}