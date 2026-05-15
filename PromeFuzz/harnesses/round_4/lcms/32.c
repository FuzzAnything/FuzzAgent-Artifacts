// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDictDup at cmsnamed.c:1161:21 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
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

static wchar_t* create_wstring_from_data(const uint8_t* data, size_t size, size_t max_chars) {
    if (size == 0 || max_chars == 0) {
        return NULL;
    }
    
    size_t chars_to_copy = size / sizeof(wchar_t);
    if (chars_to_copy > max_chars) {
        chars_to_copy = max_chars;
    }
    
    if (chars_to_copy == 0) {
        return NULL;
    }
    
    wchar_t* wstr = (wchar_t*)malloc((chars_to_copy + 1) * sizeof(wchar_t));
    if (!wstr) {
        return NULL;
    }
    
    memcpy(wstr, data, chars_to_copy * sizeof(wchar_t));
    wstr[chars_to_copy] = L'\0';
    return wstr;
}

static void free_wstring(wchar_t* wstr) {
    if (wstr) {
        free(wstr);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    cmsHANDLE dict = cmsDictAlloc(context);
    if (!dict) {
        cmsDeleteContext(context);
        return 0;
    }
    
    size_t offset = 0;
    size_t max_entries = 10;
    
    for (size_t i = 0; i < max_entries && offset < Size; i++) {
        size_t remaining = Size - offset;
        if (remaining < 8) {
            break;
        }
        
        size_t name_len = Data[offset] % 32;
        size_t value_len = Data[offset + 1] % 32;
        offset += 2;
        
        if (offset >= Size) {
            break;
        }
        
        wchar_t* name = create_wstring_from_data(Data + offset, Size - offset, name_len);
        offset += name_len * sizeof(wchar_t);
        if (offset > Size) {
            offset = Size;
        }
        
        wchar_t* value = NULL;
        if (offset < Size) {
            value = create_wstring_from_data(Data + offset, Size - offset, value_len);
            offset += value_len * sizeof(wchar_t);
            if (offset > Size) {
                offset = Size;
            }
        }
        
        if (name != NULL) {
            cmsBool success = cmsDictAddEntry(dict, name, value, NULL, NULL);
            (void)success;
        }
        
        free_wstring(name);
        free_wstring(value);
        
        if (offset >= Size) {
            break;
        }
    }
    
    cmsHANDLE dict_dup = cmsDictDup(dict);
    if (dict_dup) {
        const cmsDICTentry* entry = cmsDictGetEntryList(dict_dup);
        while (entry != NULL) {
            entry = cmsDictNextEntry(entry);
        }
        cmsDictFree(dict_dup);
    }
    
    const cmsDICTentry* entry = cmsDictGetEntryList(dict);
    while (entry != NULL) {
        entry = cmsDictNextEntry(entry);
    }
    
    cmsDictFree(dict);
    cmsDeleteContext(context);
    
    return 0;
}