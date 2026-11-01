// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "lcms2.h"

static void fuzzer_static_cleanup(cmsMLU* mlu) {
    if (mlu != NULL) {
        cmsMLUfree(mlu);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Parse nItems from input safely, ensuring alignment and bounds
    uint32_t nItems;
    memcpy(&nItems, Data, sizeof(uint32_t));
    
    // Ensure nItems is >= 2 as per API requirement
    if (nItems < 2) {
        nItems = 2;
    }
    // Cap nItems to prevent excessive allocation
    if (nItems > 1000) {
        nItems = 1000;
    }

    cmsMLU* mlu = cmsMLUalloc(ctx, nItems);
    if (mlu == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Test cmsMLUtranslationsCount on fresh allocation
    cmsUInt32Number count = cmsMLUtranslationsCount(mlu);
    (void)count;

    // Test cmsMLUgetASCII with various buffer sizes and codes
    char lang_code[3] = "en";
    char country_code[3] = "US";
    char ascii_buf[256];
    wchar_t wide_buf[256];

    // Get required size for ASCII
    cmsUInt32Number ascii_len = cmsMLUgetASCII(mlu, lang_code, country_code, NULL, 0);
    (void)ascii_len;

    // Try to get ASCII with buffer
    if (Size > 10) {
        lang_code[0] = Data[0];
        lang_code[1] = Data[1];
        lang_code[2] = '\0';
        country_code[0] = Data[2];
        country_code[1] = Data[3];
        country_code[2] = '\0';
    }

    cmsUInt32Number ascii_copied = cmsMLUgetASCII(mlu, lang_code, country_code, ascii_buf, sizeof(ascii_buf));
    (void)ascii_copied;

    // Test cmsMLUgetWide
    cmsUInt32Number wide_len = cmsMLUgetWide(mlu, lang_code, country_code, NULL, 0);
    (void)wide_len;

    cmsUInt32Number wide_copied = cmsMLUgetWide(mlu, lang_code, country_code, wide_buf, sizeof(wide_buf) / sizeof(wchar_t));
    (void)wide_copied;

    // Test cmsMLUdup
    cmsMLU* mlu_dup = cmsMLUdup(mlu);
    if (mlu_dup != NULL) {
        cmsUInt32Number dup_count = cmsMLUtranslationsCount(mlu_dup);
        (void)dup_count;
        
        cmsUInt32Number dup_ascii_len = cmsMLUgetASCII(mlu_dup, lang_code, country_code, NULL, 0);
        (void)dup_ascii_len;
        
        cmsUInt32Number dup_wide_len = cmsMLUgetWide(mlu_dup, lang_code, country_code, NULL, 0);
        (void)dup_wide_len;

        fuzzer_static_cleanup(mlu_dup);
    }

    // Test with NULL pointer for safety checks
    cmsUInt32Number null_count = cmsMLUtranslationsCount(NULL);
    (void)null_count;
    
    cmsUInt32Number null_ascii = cmsMLUgetASCII(NULL, lang_code, country_code, ascii_buf, sizeof(ascii_buf));
    (void)null_ascii;

    cmsUInt32Number null_wide = cmsMLUgetWide(NULL, lang_code, country_code, wide_buf, sizeof(wide_buf) / sizeof(wchar_t));
    (void)null_wide;

    cmsMLU* null_dup = cmsMLUdup(NULL);
    (void)null_dup;

    // Cleanup
    fuzzer_static_cleanup(mlu);
    cmsDeleteContext(ctx);

    return 0;
}