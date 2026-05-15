// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
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

static void populate_mlu_with_dummy_data(cmsMLU* mlu) {
    // Add some dummy translations to make the MLU non-empty
    static const char* lang_codes[] = {"en", "fr", "de", "es"};
    static const char* country_codes[] = {"US", "FR", "DE", "ES"};
    static const char* ascii_texts[] = {"Test", "Test FR", "Test DE", "Test ES"};
    static const wchar_t* wide_texts[] = {L"Test", L"Test FR", L"Test DE", L"Test ES"};
    
    for (int i = 0; i < 4; i++) {
        // Note: We cannot directly add entries since cmsMLUsetWide/ASCII aren't in the API list
        // The MLU will remain empty but functions should handle this gracefully
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // Use default context
    cmsContext context = NULL;
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // Get nItems for allocation (limit to reasonable size)
    cmsUInt32Number nItems = 2;
    if (Size >= offset + sizeof(cmsUInt32Number)) {
        nItems = *(cmsUInt32Number*)(Data + offset);
        offset += sizeof(cmsUInt32Number);
        // Ensure minimum of 2 and limit to prevent excessive allocation
        if (nItems < 2) nItems = 2;
        if (nItems > 100) nItems = 100;
    }
    
    // Extract language and country codes (3 bytes each)
    char languageCode[3] = {'e', 'n', '\0'};
    char countryCode[3] = {'U', 'S', '\0'};
    
    if (Size >= offset + 3) {
        memcpy(languageCode, Data + offset, 3);
        offset += 3;
    }
    
    if (Size >= offset + 3) {
        memcpy(countryCode, Data + offset, 3);
        offset += 3;
    }
    
    // Extract buffer size
    cmsUInt32Number bufferSize = 100;
    if (Size >= offset + sizeof(cmsUInt32Number)) {
        bufferSize = *(cmsUInt32Number*)(Data + offset);
        offset += sizeof(cmsUInt32Number);
        // Limit buffer size
        if (bufferSize > 1000) bufferSize = 1000;
    }
    
    // Test 1: cmsMLUalloc and cmsMLUfree
    cmsMLU* mlu = cmsMLUalloc(context, nItems);
    if (!mlu) {
        return 0;
    }
    
    // Test 2: cmsMLUtranslationsCount
    cmsUInt32Number count = cmsMLUtranslationsCount(mlu);
    
    // Test 3: cmsMLUdup
    cmsMLU* mlu_dup = cmsMLUdup(mlu);
    if (mlu_dup) {
        cmsUInt32Number dup_count = cmsMLUtranslationsCount(mlu_dup);
        cmsMLUfree(mlu_dup);
    }
    
    // Test 4: cmsMLUgetWide with NULL buffer (get required size)
    cmsUInt32Number wide_needed = cmsMLUgetWide(mlu, languageCode, countryCode, NULL, 0);
    
    // Test 5: cmsMLUgetWide with buffer
    if (wide_needed > 0 && bufferSize > 0) {
        wchar_t* wide_buffer = (wchar_t*)calloc(bufferSize, sizeof(wchar_t));
        if (wide_buffer) {
            cmsUInt32Number wide_copied = cmsMLUgetWide(mlu, languageCode, countryCode, wide_buffer, bufferSize);
            free(wide_buffer);
        }
    }
    
    // Test 6: cmsMLUgetASCII with NULL buffer (get required size)
    cmsUInt32Number ascii_needed = cmsMLUgetASCII(mlu, languageCode, countryCode, NULL, 0);
    
    // Test 7: cmsMLUgetASCII with buffer
    if (ascii_needed > 0 && bufferSize > 0) {
        char* ascii_buffer = (char*)calloc(bufferSize, sizeof(char));
        if (ascii_buffer) {
            cmsUInt32Number ascii_copied = cmsMLUgetASCII(mlu, languageCode, countryCode, ascii_buffer, bufferSize);
            free(ascii_buffer);
        }
    }
    
    // Test edge cases with NULL MLU
    cmsUInt32Number null_count = cmsMLUtranslationsCount(NULL);
    cmsMLU* null_dup = cmsMLUdup(NULL);
    if (null_dup) {
        cmsMLUfree(null_dup);
    }
    
    cmsUInt32Number null_wide = cmsMLUgetWide(NULL, languageCode, countryCode, NULL, 0);
    cmsUInt32Number null_ascii = cmsMLUgetASCII(NULL, languageCode, countryCode, NULL, 0);
    
    // Test with invalid language/country codes
    char invalid_code[3] = {0, 0, 0};
    if (Size >= offset + 2) {
        invalid_code[0] = Data[offset];
        invalid_code[1] = Data[offset + 1];
        invalid_code[2] = '\0';
    }
    
    cmsUInt32Number invalid_wide = cmsMLUgetWide(mlu, invalid_code, invalid_code, NULL, 0);
    cmsUInt32Number invalid_ascii = cmsMLUgetASCII(mlu, invalid_code, invalid_code, NULL, 0);
    
    // Cleanup
    cmsMLUfree(mlu);
    
    return 0;
}