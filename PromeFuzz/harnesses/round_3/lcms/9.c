// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
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

static void dummy_cleanup(void) {
    // Placeholder for any global cleanup if needed
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Ensure minimum size for basic operations
    if (Size < 10) return 0;

    // Use default context
    cmsContext context = NULL;

    // 1. Test cmsMLUalloc and cmsMLUfree
    // Extract nItems from fuzzer data (ensure it's at least 2 as per API minimum)
    cmsUInt32Number nItems = (cmsUInt32Number)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
    if (nItems < 2) nItems = 2;
    
    cmsMLU* mlu = cmsMLUalloc(context, nItems);
    if (!mlu) return 0;

    // 2. Test cmsMLUdup
    cmsMLU* mlu_dup = cmsMLUdup(mlu);
    
    // 3. Test cmsMLUgetWide with various inputs
    // Extract language and country codes from fuzzer data
    char langCode[3], countryCode[3];
    size_t offset = 4;
    
    if (offset + 6 <= Size) {
        memcpy(langCode, Data + offset, 3);
        memcpy(countryCode, Data + offset + 3, 3);
        offset += 6;
    } else {
        // Use default values if not enough data
        memcpy(langCode, "en", 3);  // Includes null terminator
        memcpy(countryCode, "US", 3);
    }
    
    // First call with NULL buffer to get required size
    cmsUInt32Number needed = cmsMLUgetWide(mlu, langCode, countryCode, NULL, 0);
    
    // Try with actual buffer if we have data
    if (needed > 0 && offset + sizeof(wchar_t) <= Size) {
        // Allocate buffer with size from fuzzer data
        cmsUInt32Number bufSize = (cmsUInt32Number)((Data[offset] << 8) | Data[offset + 1]) % 256;
        if (bufSize == 0) bufSize = 1;
        offset += 2;
        
        wchar_t* buffer = (wchar_t*)calloc(bufSize, sizeof(wchar_t));
        if (buffer) {
            cmsMLUgetWide(mlu, langCode, countryCode, buffer, bufSize);
            free(buffer);
        }
    }

    // 4. Test dictionary functions
    // Create a dummy dictionary handle (simulate allocation)
    cmsHANDLE dict = NULL;
    
    // Try to add entry if we have enough data for wide strings
    if (offset + 8 <= Size) {
        // Create dummy wide strings from fuzzer data
        size_t nameLen = Data[offset] % 32;
        size_t valueLen = Data[offset + 1] % 32;
        offset += 2;
        
        if (nameLen > 0 && offset + nameLen * sizeof(wchar_t) <= Size) {
            wchar_t* name = (wchar_t*)malloc((nameLen + 1) * sizeof(wchar_t));
            wchar_t* value = NULL;
            
            if (name) {
                // Convert bytes to wide chars (simplified)
                for (size_t i = 0; i < nameLen && offset + i < Size; i++) {
                    name[i] = (wchar_t)Data[offset + i];
                }
                name[nameLen] = 0;
                offset += nameLen;
                
                if (valueLen > 0 && offset + valueLen * sizeof(wchar_t) <= Size) {
                    value = (wchar_t*)malloc((valueLen + 1) * sizeof(wchar_t));
                    if (value) {
                        for (size_t i = 0; i < valueLen && offset + i < Size; i++) {
                            value[i] = (wchar_t)Data[offset + i];
                        }
                        value[valueLen] = 0;
                        offset += valueLen;
                    }
                }
                
                // Try adding entry (dict may be NULL, function should handle gracefully)
                cmsDictAddEntry(dict, name, value, mlu, mlu_dup);
                
                free(name);
                if (value) free(value);
            }
        }
    }
    
    // 5. Cleanup
    if (mlu_dup) cmsMLUfree(mlu_dup);
    if (mlu) cmsMLUfree(mlu);
    if (dict) cmsDictFree(dict);
    
    dummy_cleanup();
    return 0;
}