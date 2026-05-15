// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDictDup at cmsnamed.c:1161:21 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
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

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static wchar_t* create_wide_string(const uint8_t* data, size_t size, size_t max_chars) {
    if (size == 0 || max_chars == 0) return NULL;
    
    size_t chars_to_copy = size / sizeof(wchar_t);
    if (chars_to_copy > max_chars) chars_to_copy = max_chars;
    if (chars_to_copy == 0) chars_to_copy = 1;
    
    wchar_t* str = (wchar_t*)calloc(chars_to_copy + 1, sizeof(wchar_t));
    if (!str) return NULL;
    
    memcpy(str, data, chars_to_copy * sizeof(wchar_t));
    str[chars_to_copy] = 0;
    return str;
}

static cmsMLU* create_test_mlu(cmsContext ctx, const uint8_t* data, size_t size) {
    if (!ctx || size < 8) return NULL;
    
    cmsMLU* mlu = cmsMLUalloc(ctx, 1);
    if (!mlu) return NULL;
    
    wchar_t* wstr = create_wide_string(data, size, 16);
    if (wstr) {
        cmsMLUsetWide(mlu, "en", "US", wstr);
        free(wstr);
    }
    return mlu;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = create_test_context();
    if (!ctx) return 0;
    
    cmsHANDLE dict = cmsDictAlloc(ctx);
    if (!dict) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    size_t offset = 0;
    size_t max_entries = 10;
    
    for (size_t i = 0; i < max_entries && offset < Size; i++) {
        size_t chunk_size = Size - offset;
        if (chunk_size < 8) break;
        
        size_t name_len = (Data[offset] % 32) + 1;
        size_t value_len = (Data[offset + 1] % 32) + 1;
        
        wchar_t* name = create_wide_string(Data + offset, 
                                          chunk_size > name_len * sizeof(wchar_t) ? name_len * sizeof(wchar_t) : chunk_size,
                                          name_len);
        offset += name_len * sizeof(wchar_t);
        if (offset > Size) offset = Size;
        
        chunk_size = Size - offset;
        wchar_t* value = create_wide_string(Data + offset,
                                           chunk_size > value_len * sizeof(wchar_t) ? value_len * sizeof(wchar_t) : chunk_size,
                                           value_len);
        offset += value_len * sizeof(wchar_t);
        if (offset > Size) offset = Size;
        
        cmsMLU* display_name = NULL;
        cmsMLU* display_value = NULL;
        
        if (Size - offset >= 16) {
            display_name = create_test_mlu(ctx, Data + offset, Size - offset);
            offset += 8;
            if (offset > Size) offset = Size;
            
            if (Size - offset >= 16) {
                display_value = create_test_mlu(ctx, Data + offset, Size - offset);
                offset += 8;
                if (offset > Size) offset = Size;
            }
        }
        
        if (name && value) {
            cmsDictAddEntry(dict, name, value, display_name, display_value);
        }
        
        free(name);
        free(value);
        
        if (display_name) cmsMLUfree(display_name);
        if (display_value) cmsMLUfree(display_value);
    }
    
    cmsHANDLE dict_copy = cmsDictDup(dict);
    
    const cmsDICTentry* entry = cmsDictGetEntryList(dict);
    while (entry) {
        const cmsDICTentry* next = cmsDictNextEntry(entry);
        entry = next;
    }
    
    if (dict_copy) {
        entry = cmsDictGetEntryList(dict_copy);
        while (entry) {
            const cmsDICTentry* next = cmsDictNextEntry(entry);
            entry = next;
        }
        
        cmsDictFree(dict_copy);
    }
    
    cmsDictFree(dict);
    cmsDeleteContext(ctx);
    
    return 0;
}