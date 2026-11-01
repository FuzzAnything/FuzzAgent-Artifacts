// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
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

static void cleanup_mlu(cmsMLU *mlu) {
    if (mlu != NULL) {
        cmsMLUfree(mlu);
    }
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }

    cmsUInt32Number nItems = 0;
    if (Size >= 4) {
        nItems = *(const cmsUInt32Number *)(Data);
        Data += 4;
        Size -= 4;
    }

    // Ensure nItems >= 2 as required by cmsMLUalloc
    if (nItems < 2) {
        nItems = 2;
    }
    // Cap nItems to prevent excessive allocation
    if (nItems > 1000) {
        nItems = 1000;
    }

    cmsMLU *mlu = cmsMLUalloc(context, nItems);
    if (mlu == NULL) {
        cmsDeleteContext(context);
        return 0;
    }

    // Fuzz cmsMLUtranslationsCount
    cmsUInt32Number count = cmsMLUtranslationsCount(mlu);
    (void)count;

    // Fuzz cmsMLUdup
    cmsMLU *mlu_dup = cmsMLUdup(mlu);
    if (mlu_dup != NULL) {
        cmsMLUtranslationsCount(mlu_dup);
        cleanup_mlu(mlu_dup);
    }

    // Prepare language and country codes from input
    char lang_code[3] = {'e', 'n', '\0'};
    char country_code[3] = {'U', 'S', '\0'};

    if (Size >= 6) {
        memcpy(lang_code, Data, 2);
        lang_code[2] = '\0';
        memcpy(country_code, Data + 2, 2);
        country_code[2] = '\0';
        Data += 6;
        Size -= 6;
    }

    // Buffer for retrieving strings
    char buffer[256] = {0};
    cmsUInt32Number buffer_size = sizeof(buffer);

    // Fuzz cmsMLUgetASCII
    cmsUInt32Number ascii_len = cmsMLUgetASCII(mlu, lang_code, country_code, buffer, buffer_size);
    (void)ascii_len;

    // Fuzz cmsMLUgetASCII with NULL buffer to get size
    cmsUInt32Number ascii_size = cmsMLUgetASCII(mlu, lang_code, country_code, NULL, 0);
    (void)ascii_size;

    // Fuzz cmsMLUgetUTF8
    cmsUInt32Number utf8_len = cmsMLUgetUTF8(mlu, lang_code, country_code, buffer, buffer_size);
    (void)utf8_len;

    // Fuzz cmsMLUgetUTF8 with NULL buffer to get size
    cmsUInt32Number utf8_size = cmsMLUgetUTF8(mlu, lang_code, country_code, NULL, 0);
    (void)utf8_size;

    // Cleanup
    cleanup_mlu(mlu);
    cmsDeleteContext(context);

    return 0;
}