// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "lcms2.h"

static void fuzzer_test_mlu_operations(const uint8_t *Data, size_t Size) {
    cmsContext ctx = NULL;
    cmsMLU *mlu = NULL;
    cmsMLU *mlu_dup = NULL;
    char lang_code[3] = {'\0', '\0', '\0'};
    char country_code[3] = {'\0', '\0', '\0'};
    char ascii_buffer[256];
    wchar_t wide_buffer[256];
    cmsUInt32Number n_items = 0;

    // Create context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return;

    // Extract nItems from fuzz data, ensure >= 2
    if (Size >= 4) {
        n_items = *(const cmsUInt32Number *)Data;
        if (n_items < 2) n_items = 2;
        if (n_items > 10000) n_items = 10000; // Cap to prevent huge allocations
    } else {
        n_items = 2;
    }

    // Allocate MLU
    mlu = cmsMLUalloc(ctx, n_items);
    if (mlu == NULL) {
        cmsDeleteContext(ctx);
        return;
    }

    // Test cmsMLUtranslationsCount on allocated (empty) MLU
    cmsMLUtranslationsCount(mlu);

    // Test cmsMLUdup
    mlu_dup = cmsMLUdup(mlu);
    if (mlu_dup != NULL) {
        cmsMLUtranslationsCount(mlu_dup);
        cmsMLUfree(mlu_dup);
    }

    // Prepare language and country codes from fuzz data if available
    if (Size >= 6) {
        lang_code[0] = Data[4];
        lang_code[1] = Data[5];
        lang_code[2] = '\0';
        country_code[0] = Data[6 % Size];
        country_code[1] = Data[(6 + 1) % Size];
        country_code[2] = '\0';
    } else {
        strcpy(lang_code, "en");
        strcpy(country_code, "US");
    }

    // Test cmsMLUgetASCII with NULL buffer to get size
    cmsUInt32Number ascii_size = cmsMLUgetASCII(mlu, lang_code, country_code, NULL, 0);
    
    // Test cmsMLUgetASCII with buffer
    if (ascii_size > 0 && ascii_size < sizeof(ascii_buffer)) {
        memset(ascii_buffer, 0, sizeof(ascii_buffer));
        cmsMLUgetASCII(mlu, lang_code, country_code, ascii_buffer, sizeof(ascii_buffer));
    }

    // Test cmsMLUgetWide with NULL buffer to get size
    cmsUInt32Number wide_size = cmsMLUgetWide(mlu, lang_code, country_code, NULL, 0);

    // Test cmsMLUgetWide with buffer
    if (wide_size > 0 && wide_size < sizeof(wide_buffer)) {
        memset(wide_buffer, 0, sizeof(wide_buffer));
        cmsMLUgetWide(mlu, lang_code, country_code, wide_buffer, sizeof(wide_buffer));
    }

    // Test cmsMLUfree
    cmsMLUfree(mlu);
    cmsMLUfree(mlu_dup); // Should be safe even if already freed or NULL

    // Cleanup context
    cmsDeleteContext(ctx);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzzer_test_mlu_operations(Data, Size);

    return 0;
}