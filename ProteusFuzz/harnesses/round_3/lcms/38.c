// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsAllocProfileSequenceDescription at cmsnamed.c:985:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDupProfileSequenceDescription at cmsnamed.c:1037:19 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsFreeProfileSequenceDescription at cmsnamed.c:1017:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static void fuzz_mlu_get_ascii(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return;

    if (Size < 4) {
        cmsDeleteContext(ctx);
        return;
    }

    cmsUInt32Number nItems = (Data[0] & 0x1F) + 2; // Ensure nItems >= 2
    cmsMLU *mlu = cmsMLUalloc(ctx, nItems);
    if (!mlu) {
        cmsDeleteContext(ctx);
        return;
    }

    // Create some dummy entries if possible
    if (Size > 4) {
        cmsUInt32Number lang = Data[1];
        cmsUInt32Number country = Data[2];
        cmsUInt32Number strLen = Data[3];
        if (strLen > 0 && strLen < Size - 4) {
            // We cannot easily set entries without internal API, so we just test retrieval
            // The MLU is empty, so get should return 0 or fail gracefully
        }
    }

    char langCode[3] = {0};
    char countryCode[3] = {0};
    if (Size > 4) {
        langCode[0] = (char)(Data[4] % 26) + 'a';
        langCode[1] = (char)(Data[5] % 26) + 'a';
        countryCode[0] = (char)(Data[6] % 26) + 'A';
        countryCode[1] = (char)(Data[7] % 26) + 'A';
    }

    // Test 1: Get size (Buffer = NULL)
    cmsUInt32Number sizeNeeded = cmsMLUgetASCII(mlu, langCode, countryCode, NULL, 0);

    // Test 2: Get with buffer
    if (sizeNeeded > 0) {
        char *buf = (char *)malloc(sizeNeeded + 1);
        if (buf) {
            cmsUInt32Number copied = cmsMLUgetASCII(mlu, langCode, countryCode, buf, sizeNeeded + 1);
            free(buf);
        }
    }

    // Test 3: Get with small buffer
    char smallBuf[10];
    cmsMLUgetASCII(mlu, langCode, countryCode, smallBuf, sizeof(smallBuf));

    cmsMLUfree(mlu);
    cmsDeleteContext(ctx);
}

static void fuzz_profile_sequence(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return;

    if (Size < 1) {
        cmsDeleteContext(ctx);
        return;
    }

    cmsUInt32Number n = Data[0];
    if (n == 0) n = 1; // Avoid 0
    if (n > 255) n = 255; // Cap at 255

    cmsSEQ *seq = cmsAllocProfileSequenceDescription(ctx, n);
    if (!seq) {
        cmsDeleteContext(ctx);
        return;
    }

    // Test Dup
    cmsSEQ *dupSeq = cmsDupProfileSequenceDescription(seq);
    if (dupSeq) {
        cmsFreeProfileSequenceDescription(dupSeq);
    }

    // Test Free
    cmsFreeProfileSequenceDescription(seq);

    cmsDeleteContext(ctx);
}

static void fuzz_mlu_get_wide(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return;

    if (Size < 4) {
        cmsDeleteContext(ctx);
        return;
    }

    cmsUInt32Number nItems = (Data[0] & 0x1F) + 2;
    cmsMLU *mlu = cmsMLUalloc(ctx, nItems);
    if (!mlu) {
        cmsDeleteContext(ctx);
        return;
    }

    char langCode[3] = {0};
    char countryCode[3] = {0};
    if (Size > 4) {
        langCode[0] = (char)(Data[4] % 26) + 'a';
        langCode[1] = (char)(Data[5] % 26) + 'a';
        countryCode[0] = (char)(Data[6] % 26) + 'A';
        countryCode[1] = (char)(Data[7] % 26) + 'A';
    }

    // Test 1: Get size (Buffer = NULL)
    cmsUInt32Number sizeNeeded = cmsMLUgetWide(mlu, langCode, countryCode, NULL, 0);

    // Test 2: Get with buffer
    if (sizeNeeded > 0) {
        wchar_t *buf = (wchar_t *)malloc((sizeNeeded + 1) * sizeof(wchar_t));
        if (buf) {
            cmsUInt32Number copied = cmsMLUgetWide(mlu, langCode, countryCode, buf, sizeNeeded + 1);
            free(buf);
        }
    }

    // Test 3: Get with small buffer
    wchar_t smallBuf[10];
    cmsMLUgetWide(mlu, langCode, countryCode, smallBuf, sizeof(smallBuf));

    cmsMLUfree(mlu);
    cmsDeleteContext(ctx);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz MLU ASCII
    fuzz_mlu_get_ascii(Data, Size);

    // Fuzz Profile Sequence
    fuzz_profile_sequence(Data, Size);

    // Fuzz MLU Wide
    fuzz_mlu_get_wide(Data, Size);

    return 0;
}