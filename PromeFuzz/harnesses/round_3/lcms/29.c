// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
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
#include <wchar.h>

static cmsMLU* create_test_mlu(cmsContext context, const uint8_t* data, size_t size) {
    // Create MLU with at least 2 entries as per documentation
    cmsMLU* mlu = cmsMLUalloc(context, 2);
    if (!mlu) return NULL;
    
    // We can't populate it properly without setter functions, but we have an allocated structure
    return mlu;
}

static void test_mlu_functions(cmsMLU* mlu, const uint8_t* data, size_t size) {
    if (!mlu) return;
    
    // Extract test parameters from fuzz data
    size_t offset = 0;
    
    // Get index for translationsCodes (use modulo to keep it reasonable)
    cmsUInt32Number idx = 0;
    if (offset < size) {
        idx = data[offset] % 256;
        offset++;
    }
    
    // Get language and country codes (3 bytes each)
    char lang_code[3] = {0};
    char country_code[3] = {0};
    
    for (int i = 0; i < 3 && offset < size; i++) {
        lang_code[i] = data[offset] % 128;  // Keep in ASCII range
        offset++;
    }
    for (int i = 0; i < 3 && offset < size; i++) {
        country_code[i] = data[offset] % 128;  // Keep in ASCII range
        offset++;
    }
    
    // Get buffer size
    cmsUInt32Number buffer_size = 0;
    if (offset < size) {
        buffer_size = data[offset] % 256;
        offset++;
    }
    
    // Test cmsMLUtranslationsCount
    cmsUInt32Number count = cmsMLUtranslationsCount(mlu);
    count = cmsMLUtranslationsCount(NULL);  // Test with NULL
    
    // Test cmsMLUtranslationsCodes
    char out_lang[3] = {0};
    char out_country[3] = {0};
    cmsBool result = cmsMLUtranslationsCodes(mlu, idx, out_lang, out_country);
    result = cmsMLUtranslationsCodes(NULL, idx, out_lang, out_country);  // Test with NULL
    
    // Test cmsMLUgetWide
    wchar_t* wide_buffer = NULL;
    if (buffer_size > 0 && buffer_size < 1024) {  // Limit size to avoid huge allocations
        wide_buffer = (wchar_t*)malloc(buffer_size * sizeof(wchar_t));
        if (wide_buffer) {
            cmsUInt32Number needed = cmsMLUgetWide(mlu, lang_code, country_code, NULL, 0);
            cmsUInt32Number copied = cmsMLUgetWide(mlu, lang_code, country_code, wide_buffer, buffer_size);
            free(wide_buffer);
        }
    } else {
        // Test with NULL buffer to get required size
        cmsUInt32Number needed = cmsMLUgetWide(mlu, lang_code, country_code, NULL, 0);
        needed = cmsMLUgetWide(NULL, lang_code, country_code, NULL, 0);  // Test with NULL MLU
    }
    
    // Test cmsMLUgetASCII
    char* ascii_buffer = NULL;
    if (buffer_size > 0 && buffer_size < 1024) {
        ascii_buffer = (char*)malloc(buffer_size);
        if (ascii_buffer) {
            cmsUInt32Number needed = cmsMLUgetASCII(mlu, lang_code, country_code, NULL, 0);
            cmsUInt32Number copied = cmsMLUgetASCII(mlu, lang_code, country_code, ascii_buffer, buffer_size);
            free(ascii_buffer);
        }
    } else {
        cmsUInt32Number needed = cmsMLUgetASCII(mlu, lang_code, country_code, NULL, 0);
        needed = cmsMLUgetASCII(NULL, lang_code, country_code, NULL, 0);
    }
    
    // Test cmsMLUgetUTF8
    char* utf8_buffer = NULL;
    if (buffer_size > 0 && buffer_size < 1024) {
        utf8_buffer = (char*)malloc(buffer_size);
        if (utf8_buffer) {
            cmsUInt32Number needed = cmsMLUgetUTF8(mlu, lang_code, country_code, NULL, 0);
            cmsUInt32Number copied = cmsMLUgetUTF8(mlu, lang_code, country_code, utf8_buffer, buffer_size);
            free(utf8_buffer);
        }
    } else {
        cmsUInt32Number needed = cmsMLUgetUTF8(mlu, lang_code, country_code, NULL, 0);
        needed = cmsMLUgetUTF8(NULL, lang_code, country_code, NULL, 0);
    }
    
    // Test with different indices and codes using remaining data
    while (offset + 2 < size && offset < 100) {  // Limit iterations
        char test_lang[3] = {0};
        char test_country[3] = {0};
        
        test_lang[0] = data[offset] % 128;
        offset++;
        if (offset < size) {
            test_lang[1] = data[offset] % 128;
            offset++;
        }
        if (offset < size) {
            test_lang[2] = data[offset] % 128;
            offset++;
        }
        
        if (offset < size) {
            test_country[0] = data[offset] % 128;
            offset++;
        }
        if (offset < size) {
            test_country[1] = data[offset] % 128;
            offset++;
        }
        if (offset < size) {
            test_country[2] = data[offset] % 128;
            offset++;
        }
        
        // Quick test with these codes
        cmsMLUgetWide(mlu, test_lang, test_country, NULL, 0);
        cmsMLUgetASCII(mlu, test_lang, test_country, NULL, 0);
        cmsMLUgetUTF8(mlu, test_lang, test_country, NULL, 0);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *data, size_t size) {
    // Test with default context (NULL)
    cmsMLU* mlu1 = create_test_mlu(NULL, data, size);
    test_mlu_functions(mlu1, data, size);
    if (mlu1) {
        cmsMLUfree(mlu1);
    }
    
    // Test with a dummy context structure
    // We can't create a real context without the proper API, but we can test with NULL
    cmsMLU* mlu2 = cmsMLUalloc(NULL, 5);  // Different number of items
    test_mlu_functions(mlu2, data, size);
    if (mlu2) {
        cmsMLUfree(mlu2);
    }
    
    // Test edge cases
    cmsMLU* mlu3 = cmsMLUalloc(NULL, 0);  // Should use minimum 2
    test_mlu_functions(mlu3, data, size);
    if (mlu3) {
        cmsMLUfree(mlu3);
    }
    
    // Test with very large number of items (but limit to avoid excessive memory)
    cmsUInt32Number large_items = 100;
    if (size > 0) {
        large_items = data[0] % 1000;  // Limit to 1000
    }
    cmsMLU* mlu4 = cmsMLUalloc(NULL, large_items);
    test_mlu_functions(mlu4, data, size);
    if (mlu4) {
        cmsMLUfree(mlu4);
    }
    
    return 0;
}