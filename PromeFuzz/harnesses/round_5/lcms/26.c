// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDictDup at cmsnamed.c:1161:21 in lcms2.h
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

static wchar_t* create_wide_string(const uint8_t* data, size_t size) {
    if (size == 0) {
        wchar_t* empty = (wchar_t*)calloc(1, sizeof(wchar_t));
        return empty;
    }
    
    size_t wlen = size / sizeof(wchar_t);
    if (wlen == 0) wlen = 1;
    
    wchar_t* wstr = (wchar_t*)calloc(wlen + 1, sizeof(wchar_t));
    if (!wstr) return NULL;
    
    size_t copy_len = size < wlen * sizeof(wchar_t) ? size : wlen * sizeof(wchar_t);
    memcpy(wstr, data, copy_len);
    
    return wstr;
}

static cmsMLU* create_dummy_mlu(const uint8_t* data, size_t size) {
    cmsMLU* mlu = cmsMLUalloc(NULL, 1);
    if (!mlu) return NULL;
    
    if (size > 0) {
        wchar_t* wstr = create_wide_string(data, size > 100 ? 100 : size);
        if (wstr) {
            cmsMLUsetWide(mlu, "en", "US", wstr);
            free(wstr);
        }
    }
    
    return mlu;
}

int LLVMFuzzerTestOneInput_26(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext context = NULL;
    cmsHANDLE hDict = cmsDictAlloc(context);
    if (!hDict) return 0;
    
    cmsHANDLE hDict2 = NULL;
    cmsMLU* displayName = NULL;
    cmsMLU* displayValue = NULL;
    
    size_t offset = 0;
    int entry_count = 0;
    
    while (offset < Size && entry_count < 10) {
        size_t remaining = Size - offset;
        if (remaining < 2) break;
        
        size_t name_len = Data[offset] % (remaining - 1);
        if (name_len == 0) name_len = 1;
        
        size_t value_len = remaining - name_len - 1;
        if (value_len == 0) value_len = 1;
        
        const uint8_t* name_data = Data + offset + 1;
        const uint8_t* value_data = Data + offset + 1 + name_len;
        
        wchar_t* name = create_wide_string(name_data, name_len);
        wchar_t* value = create_wide_string(value_data, value_len);
        
        if (Size % 3 == 0) {
            displayName = create_dummy_mlu(name_data, name_len);
            displayValue = create_dummy_mlu(value_data, value_len);
        }
        
        cmsBool success = cmsDictAddEntry(hDict, name, value, displayName, displayValue);
        
        if (displayName) {
            cmsMLUfree(displayName);
            displayName = NULL;
        }
        if (displayValue) {
            cmsMLUfree(displayValue);
            displayValue = NULL;
        }
        
        free(name);
        free(value);
        
        offset += 1 + name_len + value_len;
        entry_count++;
        
        if (Size % 2 == 0 && hDict2 == NULL) {
            hDict2 = cmsDictDup(hDict);
        }
    }
    
    const cmsDICTentry* entry = cmsDictGetEntryList(hDict);
    while (entry) {
        entry = cmsDictNextEntry(entry);
    }
    
    if (hDict2) {
        const cmsDICTentry* entry2 = cmsDictGetEntryList(hDict2);
        while (entry2) {
            entry2 = cmsDictNextEntry(entry2);
        }
        cmsDictFree(hDict2);
    }
    
    cmsDictFree(hDict);
    
    return 0;
}