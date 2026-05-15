// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUgetTranslation at cmsnamed.c:663:26 in lcms2.h
// cmsMLUsetUTF8 at cmsnamed.c:368:19 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUsetASCII at cmsnamed.c:336:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

static cmsMLU* create_test_mlu(void) {
    return cmsMLUalloc(NULL, 0);
}

static void cleanup_mlu(cmsMLU* mlu) {
    if (mlu) {
        cmsMLUfree(mlu);
    }
}

static void extract_codes(const uint8_t *data, size_t size, size_t offset,
                          char lang[3], char country[3]) {
    if (offset + 6 <= size) {
        memcpy(lang, data + offset, 3);
        memcpy(country, data + offset + 3, 3);
    } else {
        // Default codes if not enough data
        memcpy(lang, "en", 3);
        memcpy(country, "US", 3);
    }
}

static wchar_t* create_wide_string(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return NULL;
    
    size_t remaining = size - offset;
    size_t max_chars = remaining / sizeof(wchar_t);
    if (max_chars == 0) return NULL;
    
    wchar_t* wstr = (wchar_t*)malloc((max_chars + 1) * sizeof(wchar_t));
    if (!wstr) return NULL;
    
    size_t copy_bytes = max_chars * sizeof(wchar_t);
    if (copy_bytes > remaining) copy_bytes = remaining;
    
    memcpy(wstr, data + offset, copy_bytes);
    wstr[max_chars] = 0;
    return wstr;
}

static char* create_ascii_string(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return NULL;
    
    size_t remaining = size - offset;
    char* str = (char*)malloc(remaining + 1);
    if (!str) return NULL;
    
    memcpy(str, data + offset, remaining);
    str[remaining] = 0;
    return str;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;
    
    cmsMLU* mlu = create_test_mlu();
    if (!mlu) return 0;
    
    // Test cmsMLUsetWide and cmsMLUgetWide
    char lang1[3], country1[3];
    extract_codes(Data, Size, 0, lang1, country1);
    
    wchar_t* wide_str = create_wide_string(Data, Size, 6);
    if (wide_str) {
        cmsMLUsetWide(mlu, lang1, country1, wide_str);
        
        // First call to get required buffer size
        cmsUInt32Number needed = cmsMLUgetWide(mlu, lang1, country1, NULL, 0);
        
        // Second call to actually get the string
        if (needed > 0 && needed < 1024) {
            wchar_t buffer[1024];
            cmsMLUgetWide(mlu, lang1, country1, buffer, needed);
        }
        free(wide_str);
    }
    
    // Test cmsMLUsetASCII
    char lang2[3], country2[3];
    extract_codes(Data, Size, Size/4, lang2, country2);
    
    char* ascii_str = create_ascii_string(Data, Size, Size/3);
    if (ascii_str) {
        cmsMLUsetASCII(mlu, lang2, country2, ascii_str);
        free(ascii_str);
    }
    
    // Test cmsMLUtranslationsCodes
    if (Size > 24) {
        char lang3[3], country3[3];
        cmsUInt32Number idx = *(const cmsUInt32Number*)(Data + Size/2) % 10;
        cmsMLUtranslationsCodes(mlu, idx, lang3, country3);
    }
    
    // Test cmsMLUgetTranslation
    char lang4[3], country4[3];
    char obtained_lang[3], obtained_country[3];
    extract_codes(Data, Size, Size/5, lang4, country4);
    cmsMLUgetTranslation(mlu, lang4, country4, obtained_lang, obtained_country);
    
    // Test cmsMLUsetUTF8
    char lang5[3], country5[3];
    extract_codes(Data, Size, Size/6, lang5, country5);
    
    char* utf8_str = create_ascii_string(Data, Size, Size/7);
    if (utf8_str) {
        cmsMLUsetUTF8(mlu, lang5, country5, utf8_str);
        free(utf8_str);
    }
    
    cleanup_mlu(mlu);
    return 0;
}