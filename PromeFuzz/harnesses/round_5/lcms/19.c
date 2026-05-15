// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void generate_language_code(char* lang, const uint8_t* data, size_t* offset, size_t size) {
    if (*offset + 2 < size) {
        lang[0] = data[*offset] % 26 + 'a';
        lang[1] = data[*offset + 1] % 26 + 'a';
        lang[2] = '\0';
        *offset += 2;
    } else {
        lang[0] = 'e';
        lang[1] = 'n';
        lang[2] = '\0';
    }
}

static void generate_country_code(char* country, const uint8_t* data, size_t* offset, size_t size) {
    if (*offset + 2 < size) {
        country[0] = data[*offset] % 26 + 'A';
        country[1] = data[*offset + 1] % 26 + 'A';
        country[2] = '\0';
        *offset += 2;
    } else {
        country[0] = 'U';
        country[1] = 'S';
        country[2] = '\0';
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    size_t offset = 0;
    
    // Generate nItems for cmsMLUalloc
    cmsUInt32Number nItems = 2;
    if (Size >= sizeof(cmsUInt32Number)) {
        nItems = *(const cmsUInt32Number*)Data;
        offset += sizeof(cmsUInt32Number);
        nItems = nItems % 100 + 2; // Ensure minimum 2 items
    }
    
    // Test cmsMLUalloc
    cmsMLU* mlu = cmsMLUalloc(context, nItems);
    if (!mlu) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Test cmsMLUtranslationsCount
    cmsUInt32Number count = cmsMLUtranslationsCount(mlu);
    
    // Test cmsMLUdup
    cmsMLU* mlu_dup = cmsMLUdup(mlu);
    if (mlu_dup) {
        cmsUInt32Number dup_count = cmsMLUtranslationsCount(mlu_dup);
        
        // Test cmsMLUgetASCII with various buffer sizes
        char lang[3], country[3];
        generate_language_code(lang, Data, &offset, Size);
        generate_country_code(country, Data, &offset, Size);
        
        // First call to get required buffer size
        cmsUInt32Number ascii_len = cmsMLUgetASCII(mlu, lang, country, NULL, 0);
        
        // Try with actual buffer if we have data
        if (ascii_len > 0 && ascii_len < 256) {
            char* ascii_buf = (char*)malloc(ascii_len + 1);
            if (ascii_buf) {
                cmsUInt32Number copied = cmsMLUgetASCII(mlu, lang, country, ascii_buf, ascii_len + 1);
                free(ascii_buf);
            }
        }
        
        // Test cmsMLUgetUTF8
        cmsUInt32Number utf8_len = cmsMLUgetUTF8(mlu, lang, country, NULL, 0);
        
        if (utf8_len > 0 && utf8_len < 256) {
            char* utf8_buf = (char*)malloc(utf8_len + 1);
            if (utf8_buf) {
                cmsUInt32Number copied = cmsMLUgetUTF8(mlu, lang, country, utf8_buf, utf8_len + 1);
                free(utf8_buf);
            }
        }
        
        // Test with different language/country codes
        if (offset + 4 < Size) {
            char lang2[3], country2[3];
            generate_language_code(lang2, Data, &offset, Size);
            generate_country_code(country2, Data, &offset, Size);
            
            cmsUInt32Number ascii_len2 = cmsMLUgetASCII(mlu_dup, lang2, country2, NULL, 0);
            cmsUInt32Number utf8_len2 = cmsMLUgetUTF8(mlu_dup, lang2, country2, NULL, 0);
        }
        
        // Test cmsMLUfree on duplicate
        cmsMLUfree(mlu_dup);
    }
    
    // Test with NULL parameters
    cmsMLU* null_dup = cmsMLUdup(NULL);
    cmsMLUfree(NULL);
    cmsUInt32Number null_count = cmsMLUtranslationsCount(NULL);
    
    char dummy_lang[3] = "en";
    char dummy_country[3] = "US";
    cmsUInt32Number null_ascii = cmsMLUgetASCII(NULL, dummy_lang, dummy_country, NULL, 0);
    cmsUInt32Number null_utf8 = cmsMLUgetUTF8(NULL, dummy_lang, dummy_country, NULL, 0);
    
    // Test cmsMLUfree on original
    cmsMLUfree(mlu);
    
    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}