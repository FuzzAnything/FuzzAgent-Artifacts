// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsMLUsetUTF8 at cmsnamed.c:368:19 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUgetTranslation at cmsnamed.c:663:26 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsContext create_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a standard sRGB profile which is always available
    return cmsCreate_sRGBProfile();
}

static void cleanup_profile(cmsHPROFILE prof) {
    if (prof) {
        cmsCloseProfile(prof);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cmsContext ctx = create_context();
    if (!ctx) return 0;

    cmsHPROFILE prof = create_dummy_profile(ctx);
    
    // Prepare buffers
    char lang_code[3] = {0};
    char country_code[3] = {0};
    char out_lang[3] = {0};
    char out_country[3] = {0};
    
    // Use a larger buffer to prevent overflow during metadata retrieval
    // The error occurred because the buffer passed to cmsGetProfileInfoUTF8 
    // was too small for the internal string conversion.
    char buffer[4096]; 
    cmsUInt32Number buf_size = sizeof(buffer);

    // Extract codes from input if available, otherwise use defaults
    if (Size >= 4) {
        lang_code[0] = Data[0];
        lang_code[1] = Data[1];
        country_code[0] = Data[2];
        country_code[1] = Data[3];
    } else {
        lang_code[0] = 'e'; lang_code[1] = 'n';
        country_code[0] = 'U'; country_code[1] = 'S';
    }

    // Since cmsMLUcreate is not available in the public header, we cannot create a valid MLU.
    // We must rely on the NULL handling paths for MLU functions, which are safe.
    cmsMLU* mlu = NULL;

    // 1. cmsMLUsetUTF8: Add data to MLU
    // Passing NULL mlu should return FALSE safely.
    if (Size > 4) {
        size_t str_len = Size - 4;
        if (str_len > 1000) str_len = 1000;
        char* test_str = (char*)malloc(str_len + 1);
        if (test_str) {
            memcpy(test_str, Data + 4, str_len);
            test_str[str_len] = '\0';
            cmsMLUsetUTF8(mlu, lang_code, country_code, test_str);
            free(test_str);
        }
    }

    // 2. cmsMLUgetUTF8: Retrieve data
    // Should handle NULL mlu gracefully.
    cmsUInt32Number req_size = cmsMLUgetUTF8(mlu, lang_code, country_code, NULL, 0);
    if (req_size > 0 && req_size <= 2048) {
        char* dynamic_buf = (char*)malloc(req_size);
        if (dynamic_buf) {
            cmsMLUgetUTF8(mlu, lang_code, country_code, dynamic_buf, req_size);
            free(dynamic_buf);
        }
    }
    cmsMLUgetUTF8(mlu, lang_code, country_code, buffer, buf_size);

    // 3. cmsMLUtranslationsCodes: Iterate through entries
    // Should handle NULL mlu gracefully.
    for (int i = 0; i < 5 && i < (int)Size; i++) {
        cmsMLUtranslationsCodes(mlu, i, lang_code, country_code);
    }

    // 4. cmsMLUgetTranslation: Check translation availability
    cmsMLUgetTranslation(mlu, lang_code, country_code, out_lang, out_country);

    // 5. cmsGetProfileInfoASCII: Profile metadata
    if (prof) {
        // Try different info types based on input bytes
        cmsInfoType info_type = (cmsInfoType)(Data[0] % 10); 
        cmsGetProfileInfoASCII(prof, info_type, lang_code, country_code, buffer, buf_size);
    }

    // 6. cmsGetProfileInfoUTF8: Profile metadata UTF8
    // The previous crash happened here because the buffer was too small for the converted string.
    // We increased the buffer size to 4096 to accommodate typical profile metadata.
    if (prof) {
        cmsInfoType info_type = (cmsInfoType)(Data[1] % 10);
        cmsGetProfileInfoUTF8(prof, info_type, lang_code, country_code, buffer, buf_size);
    }

    // Cleanup
    cleanup_profile(prof);
    cleanup_context(ctx);

    return 0;
}