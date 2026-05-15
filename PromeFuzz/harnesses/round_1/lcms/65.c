// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUgetTranslation at cmsnamed.c:663:26 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUsetUTF8 at cmsnamed.c:368:19 in lcms2.h
// cmsMLUsetASCII at cmsnamed.c:336:19 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
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

static void extract_codes(const uint8_t* data, size_t size, char* lang, char* country) {
    if (size >= 6) {
        lang[0] = data[0];
        lang[1] = data[1];
        lang[2] = data[2];
        country[0] = data[3];
        country[1] = data[4];
        country[2] = data[5];
    } else {
        lang[0] = 'e'; lang[1] = 'n'; lang[2] = 'g';
        country[0] = 'U'; country[1] = 'S'; country[2] = 'A';
    }
}

static char* extract_string(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) {
        char* str = (char*)malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t str_size = size - offset;
    char* str = (char*)malloc(str_size + 1);
    if (!str) return NULL;
    
    memcpy(str, data + offset, str_size);
    str[str_size] = '\0';
    return str;
}

static wchar_t* extract_wide_string(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) {
        wchar_t* wstr = (wchar_t*)malloc(sizeof(wchar_t));
        if (wstr) wstr[0] = L'\0';
        return wstr;
    }
    
    size_t str_size = size - offset;
    wchar_t* wstr = (wchar_t*)malloc((str_size + 1) * sizeof(wchar_t));
    if (!wstr) return NULL;
    
    for (size_t i = 0; i < str_size; i++) {
        wstr[i] = (wchar_t)data[offset + i];
    }
    wstr[str_size] = L'\0';
    return wstr;
}

int LLVMFuzzerTestOneInput_65(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsMLU* mlu = create_test_mlu();
    if (!mlu) return 0;
    
    char lang[3], country[3];
    extract_codes(Data, Size, lang, country);
    
    size_t str_offset = Size > 6 ? 6 : 0;
    
    char* utf8_str = extract_string(Data, Size, str_offset);
    if (utf8_str) {
        cmsMLUsetUTF8(mlu, lang, country, utf8_str);
        free(utf8_str);
    }
    
    char* ascii_str = extract_string(Data, Size, str_offset);
    if (ascii_str) {
        cmsMLUsetASCII(mlu, lang, country, ascii_str);
        free(ascii_str);
    }
    
    wchar_t* wide_str = extract_wide_string(Data, Size, str_offset);
    if (wide_str) {
        cmsMLUsetWide(mlu, lang, country, wide_str);
        free(wide_str);
    }
    
    char obtained_lang[3], obtained_country[3];
    for (cmsUInt32Number i = 0; i < 10; i++) {
        cmsMLUtranslationsCodes(mlu, i, obtained_lang, obtained_country);
    }
    
    cmsMLUgetTranslation(mlu, lang, country, obtained_lang, obtained_country);
    
    cmsUInt32Number needed = cmsMLUgetWide(mlu, lang, country, NULL, 0);
    if (needed > 0) {
        wchar_t* buffer = (wchar_t*)malloc(needed * sizeof(wchar_t));
        if (buffer) {
            cmsMLUgetWide(mlu, lang, country, buffer, needed);
            free(buffer);
        }
    }
    
    cleanup_mlu(mlu);
    return 0;
}