// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_mlu_operations(const uint8_t *Data, size_t Size) {
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return;
    }

    if (Size < 2) {
        cmsDeleteContext(ContextID);
        return;
    }

    // Extract nItems from input, ensure minimum of 2
    cmsUInt32Number nItems = 2;
    if (Size >= 4) {
        nItems = ((const uint32_t*)Data)[0];
        if (nItems < 2) nItems = 2;
        // Cap nItems to prevent excessive allocation
        if (nItems > 10000) nItems = 10000;
    }

    cmsMLU* mlu = cmsMLUalloc(ContextID, nItems);
    if (mlu == NULL) {
        cmsDeleteContext(ContextID);
        return;
    }

    // Try to duplicate the MLU
    cmsMLU* mlu_dup = cmsMLUdup(mlu);
    if (mlu_dup != NULL) {
        cmsMLUfree(mlu_dup);
    }

    // Try to get translation count
    cmsUInt32Number count = cmsMLUtranslationsCount(mlu);
    (void)count; // Suppress unused warning

    // Prepare language and country codes from input
    char lang_code[3] = "en";
    char country_code[3] = "US";
    if (Size >= 6) {
        lang_code[0] = Data[4];
        lang_code[1] = Data[5];
        lang_code[2] = '\0';
        if (Size >= 8) {
            country_code[0] = Data[6];
            country_code[1] = Data[7];
            country_code[2] = '\0';
        }
    }

    // Buffer for retrieval
    char buffer[256] = {0};
    cmsUInt32Number buf_size = 256;

    if (Size >= 10) {
        buf_size = ((const uint32_t*)Data)[2];
        if (buf_size > 256) buf_size = 256;
    }

    // Test cmsMLUgetASCII
    cmsUInt32Number ascii_len = cmsMLUgetASCII(mlu, lang_code, country_code, buffer, buf_size);
    (void)ascii_len;

    // Test cmsMLUgetUTF8
    cmsUInt32Number utf8_len = cmsMLUgetUTF8(mlu, lang_code, country_code, buffer, buf_size);
    (void)utf8_len;

    // Test with NULL buffer to get required size
    cmsUInt32Number ascii_req = cmsMLUgetASCII(mlu, lang_code, country_code, NULL, 0);
    (void)ascii_req;

    cmsUInt32Number utf8_req = cmsMLUgetUTF8(mlu, lang_code, country_code, NULL, 0);
    (void)utf8_req;

    // Free the original MLU
    cmsMLUfree(mlu);

    // Cleanup context
    cmsDeleteContext(ContextID);
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzz_mlu_operations(Data, Size);

    return 0;
}