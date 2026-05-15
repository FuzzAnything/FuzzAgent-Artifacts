// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
// cmsMLUsetUTF8 at cmsnamed.c:368:19 in lcms2.h
// cmsMLUsetUTF8 at cmsnamed.c:368:19 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsMLUsetASCII at cmsnamed.c:336:19 in lcms2.h
// cmsMLUsetASCII at cmsnamed.c:336:19 in lcms2.h
// cmsMLUgetTranslation at cmsnamed.c:663:26 in lcms2.h
// cmsMLUgetTranslation at cmsnamed.c:663:26 in lcms2.h
// cmsMLUgetTranslation at cmsnamed.c:663:26 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
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

static wchar_t* create_wide_string(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) return NULL;
    
    size_t len = size > max_len ? max_len : size;
    wchar_t* wstr = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    if (!wstr) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        wstr[i] = (wchar_t)data[i];
    }
    wstr[len] = L'\0';
    return wstr;
}

static char* create_utf8_string(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) return NULL;
    
    size_t len = size > max_len ? max_len : size;
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

static void init_language_country(const uint8_t* data, char lang[3], char country[3]) {
    if (data) {
        lang[0] = data[0] ? data[0] : 'e';
        lang[1] = data[1] ? data[1] : 'n';
        lang[2] = '\0';
        
        country[0] = data[2] ? data[2] : 'U';
        country[1] = data[3] ? data[3] : 'S';
        country[2] = '\0';
    } else {
        lang[0] = 'e'; lang[1] = 'n'; lang[2] = '\0';
        country[0] = 'U'; country[1] = 'S'; country[2] = '\0';
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Initialize context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Create dictionary
    cmsHANDLE hDict = cmsDictAlloc(context);
    
    // Create MLU objects
    cmsMLU* mlu1 = cmsMLUalloc(context, 0);
    cmsMLU* mlu2 = cmsMLUalloc(context, 0);
    cmsMLU* mlu3 = cmsMLUalloc(context, 0);
    
    if (hDict && mlu1 && mlu2 && mlu3) {
        // Prepare data for API calls
        size_t data_offset = 0;
        
        // For cmsDictAddEntry
        wchar_t* name = create_wide_string(Data, Size, 50);
        wchar_t* value = create_wide_string(Data + (Size > 50 ? 50 : 0), 
                                          Size > 50 ? Size - 50 : 0, 50);
        
        // For MLU functions - prepare language/country codes
        char lang1[3], country1[3], lang2[3], country2[3];
        char obtainedLang[3], obtainedCountry[3];
        
        if (Size > 8) {
            init_language_country(Data, lang1, country1);
            init_language_country(Data + 4, lang2, country2);
            data_offset = 8;
        } else {
            init_language_country(NULL, lang1, country1);
            init_language_country(NULL, lang2, country2);
        }
        
        // Prepare strings for MLU functions
        char* utf8_str = create_utf8_string(Data + data_offset, 
                                           Size > data_offset ? Size - data_offset : 0, 100);
        wchar_t* wide_str = create_wide_string(Data + data_offset + 
                                              (Size > data_offset + 100 ? 100 : 0),
                                              Size > data_offset + 100 ? 
                                              Size - data_offset - 100 : 0, 50);
        char* ascii_str = create_utf8_string(Data + data_offset + 
                                            (Size > data_offset + 150 ? 150 : 0),
                                            Size > data_offset + 150 ? 
                                            Size - data_offset - 150 : 0, 50);
        
        // Test cmsDictAddEntry
        if (name && value) {
            cmsDictAddEntry(hDict, name, value, mlu1, mlu2);
        }
        
        // Test cmsMLUsetUTF8
        if (utf8_str) {
            cmsMLUsetUTF8(mlu1, lang1, country1, utf8_str);
            cmsMLUsetUTF8(mlu2, lang2, country2, utf8_str);
        }
        
        // Test cmsMLUsetWide
        if (wide_str) {
            cmsMLUsetWide(mlu1, lang2, country1, wide_str);
            cmsMLUsetWide(mlu2, lang1, country2, wide_str);
        }
        
        // Test cmsMLUsetASCII
        if (ascii_str) {
            cmsMLUsetASCII(mlu3, lang1, country1, ascii_str);
            cmsMLUsetASCII(mlu3, lang2, country2, ascii_str);
        }
        
        // Test cmsMLUgetTranslation
        cmsMLUgetTranslation(mlu1, lang1, country1, obtainedLang, obtainedCountry);
        cmsMLUgetTranslation(mlu2, lang2, country2, obtainedLang, obtainedCountry);
        cmsMLUgetTranslation(mlu3, lang1, country2, obtainedLang, obtainedCountry);
        
        // Test cmsMLUdup
        cmsMLU* dup_mlu = cmsMLUdup(mlu1);
        if (dup_mlu) {
            cmsMLUfree(dup_mlu);
        }
        
        // Clean up allocated strings
        free(name);
        free(value);
        free(utf8_str);
        free(wide_str);
        free(ascii_str);
    }
    
    // Clean up
    if (hDict) cmsDictFree(hDict);
    if (mlu1) cmsMLUfree(mlu1);
    if (mlu2) cmsMLUfree(mlu2);
    if (mlu3) cmsMLUfree(mlu3);
    cmsDeleteContext(context);
    
    return 0;
}