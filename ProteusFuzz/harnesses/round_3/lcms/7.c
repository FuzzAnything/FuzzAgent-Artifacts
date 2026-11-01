// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
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

static void fuzz_cmsMLU(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return;

    // Extract parameters from fuzz data if available
    cmsUInt32Number nItems = 2;
    if (Size >= 4) {
        nItems = *(const cmsUInt32Number*)(Data);
        if (nItems < 2) nItems = 2;
        if (nItems > 10000) nItems = 10000; // Limit to prevent OOM
    }

    cmsMLU *mlu = cmsMLUalloc(ctx, nItems);
    if (!mlu) {
        cmsDeleteContext(ctx);
        return;
    }

    // Attempt to duplicate
    cmsMLU *mlu_dup = cmsMLUdup(mlu);

    // Count translations for both original and duplicate
    cmsUInt32Number count_orig = cmsMLUtranslationsCount(mlu);
    cmsUInt32Number count_dup = (mlu_dup) ? cmsMLUtranslationsCount(mlu_dup) : 0;

    // Prepare locale codes
    char lang[3] = {'e', 'n', '\0'};
    char country[3] = {'U', 'S', '\0'};

    // Fuzz locale codes if data is available
    if (Size >= 6) {
        lang[0] = Data[0];
        lang[1] = Data[1];
        lang[2] = '\0';
        country[0] = Data[2];
        country[1] = Data[3];
        country[2] = '\0';
    }

    // Test cmsMLUgetASCII
    // First, get required size
    cmsUInt32Number ascii_size = cmsMLUgetASCII(mlu, lang, country, NULL, 0);
    if (ascii_size > 0) {
        char *ascii_buf = (char*)malloc(ascii_size + 1);
        if (ascii_buf) {
            cmsMLUgetASCII(mlu, lang, country, ascii_buf, ascii_size + 1);
            free(ascii_buf);
        }
    }

    // Test cmsMLUgetWide
    // First, get required size
    cmsUInt32Number wide_size = cmsMLUgetWide(mlu, lang, country, NULL, 0);
    if (wide_size > 0) {
        wchar_t *wide_buf = (wchar_t*)malloc(wide_size + 1);
        if (wide_buf) {
            cmsMLUgetWide(mlu, lang, country, wide_buf, wide_size + 1);
            free(wide_buf);
        }
    }

    // Cleanup
    cmsMLUfree(mlu);
    if (mlu_dup) {
        cmsMLUfree(mlu_dup);
    }
    cmsDeleteContext(ctx);
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_cmsMLU(Data, Size);

    return 0;
}