// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictDup at cmsnamed.c:1161:21 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

static wchar_t* create_wide_string_from_data(const uint8_t* data, size_t size, size_t max_chars) {
    if (size == 0 || max_chars == 0) {
        return NULL;
    }
    
    size_t chars_to_copy = size / sizeof(wchar_t);
    if (chars_to_copy > max_chars - 1) {
        chars_to_copy = max_chars - 1;
    }
    
    wchar_t* ws = (wchar_t*)malloc(max_chars * sizeof(wchar_t));
    if (!ws) {
        return NULL;
    }
    
    if (chars_to_copy > 0) {
        memcpy(ws, data, chars_to_copy * sizeof(wchar_t));
    }
    ws[chars_to_copy] = L'\0';
    
    return ws;
}

static cmsMLU* create_test_mlu(cmsContext ctx, const uint8_t* data, size_t size) {
    cmsMLU* mlu = cmsMLUalloc(ctx, 1);
    if (!mlu) {
        return NULL;
    }
    
    wchar_t* test_str = create_wide_string_from_data(data, size, 32);
    if (test_str) {
        char lang[3] = "en";
        char country[3] = "US";
        
        if (size >= 2) {
            lang[0] = (char)data[0];
            lang[1] = (char)data[1];
        }
        if (size >= 4) {
            country[0] = (char)data[2];
            country[1] = (char)data[3];
        }
        
        cmsMLUsetWide(mlu, lang, country, test_str);
        free(test_str);
    }
    
    return mlu;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }
    
    cmsHANDLE hDict = cmsDictAlloc(ctx);
    if (!hDict) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    size_t offset = 0;
    size_t max_entries = 10;
    
    for (size_t i = 0; i < max_entries && offset < Size; i++) {
        wchar_t* name = NULL;
        wchar_t* value = NULL;
        cmsMLU* displayName = NULL;
        cmsMLU* displayValue = NULL;
        
        if (offset < Size) {
            size_t remaining = Size - offset;
            size_t chunk_size = remaining / 4;
            if (chunk_size < 4) {
                chunk_size = remaining;
            }
            
            name = create_wide_string_from_data(Data + offset, chunk_size, 64);
            offset += chunk_size;
        }
        
        if (offset < Size) {
            size_t remaining = Size - offset;
            size_t chunk_size = remaining / 3;
            if (chunk_size < 4) {
                chunk_size = remaining;
            }
            
            value = create_wide_string_from_data(Data + offset, chunk_size, 64);
            offset += chunk_size;
        }
        
        if (offset < Size) {
            size_t remaining = Size - offset;
            size_t chunk_size = remaining / 2;
            if (chunk_size >= 4) {
                displayName = create_test_mlu(ctx, Data + offset, chunk_size);
                offset += chunk_size;
            }
        }
        
        if (offset < Size) {
            size_t remaining = Size - offset;
            size_t chunk_size = remaining;
            if (chunk_size >= 4) {
                displayValue = create_test_mlu(ctx, Data + offset, chunk_size);
                offset += chunk_size;
            }
        }
        
        if (name && value) {
            cmsDictAddEntry(hDict, name, value, displayName, displayValue);
        }
        
        if (name) free(name);
        if (value) free(value);
        if (displayName) cmsMLUfree(displayName);
        if (displayValue) cmsMLUfree(displayValue);
    }
    
    const cmsDICTentry* entry = cmsDictGetEntryList(hDict);
    while (entry != NULL) {
        entry = cmsDictNextEntry(entry);
    }
    
    cmsHANDLE hDictDup = cmsDictDup(hDict);
    if (hDictDup) {
        cmsDictFree(hDictDup);
    }
    
    cmsDictFree(hDict);
    cmsDeleteContext(ctx);
    
    return 0;
}