// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictDup at cmsnamed.c:1161:21 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "lcms2.h"

static wchar_t* create_wide_string(const uint8_t* data, size_t size) {
    if (size == 0) {
        wchar_t* ws = (wchar_t*)malloc(sizeof(wchar_t));
        if (ws) ws[0] = L'\0';
        return ws;
    }
    
    size_t wlen = size / sizeof(wchar_t);
    if (size % sizeof(wchar_t) != 0) wlen++;
    
    wchar_t* ws = (wchar_t*)malloc((wlen + 1) * sizeof(wchar_t));
    if (!ws) return NULL;
    
    size_t i;
    for (i = 0; i < wlen && i * sizeof(wchar_t) < size; i++) {
        ws[i] = (wchar_t)data[i * sizeof(wchar_t)];
        if (i * sizeof(wchar_t) + 1 < size) {
            ws[i] |= (wchar_t)data[i * sizeof(wchar_t) + 1] << 8;
        }
    }
    ws[i] = L'\0';
    return ws;
}

static cmsMLU* create_test_mlu(cmsContext ctx, const uint8_t* data, size_t size) {
    cmsMLU* mlu = cmsMLUalloc(ctx, 1);
    if (!mlu) return NULL;
    
    wchar_t* wstr = create_wide_string(data, size);
    if (wstr) {
        cmsMLUsetWide(mlu, "en", "US", wstr);
        free(wstr);
    }
    return mlu;
}

int LLVMFuzzerTestOneInput_61(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsHANDLE dict = cmsDictAlloc(ctx);
    if (!dict) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    size_t offset = 0;
    uint8_t num_entries = Data[offset++] % 10;
    
    for (int i = 0; i < num_entries && offset < Size; i++) {
        size_t name_len = Data[offset++] % 32;
        if (offset + name_len > Size) break;
        
        size_t value_len = Data[offset++] % 32;
        if (offset + name_len + value_len > Size) break;
        
        wchar_t* name = create_wide_string(Data + offset, name_len);
        offset += name_len;
        
        wchar_t* value = create_wide_string(Data + offset, value_len);
        offset += value_len;
        
        size_t mlu1_len = Data[offset++] % 16;
        cmsMLU* display_name = NULL;
        if (offset + mlu1_len <= Size && mlu1_len > 0) {
            display_name = create_test_mlu(ctx, Data + offset, mlu1_len);
            offset += mlu1_len;
        }
        
        size_t mlu2_len = Data[offset++] % 16;
        cmsMLU* display_value = NULL;
        if (offset + mlu2_len <= Size && mlu2_len > 0) {
            display_value = create_test_mlu(ctx, Data + offset, mlu2_len);
            offset += mlu2_len;
        }
        
        if (name && value) {
            cmsDictAddEntry(dict, name, value, display_name, display_value);
        }
        
        if (name) free(name);
        if (value) free(value);
        if (display_name) cmsMLUfree(display_name);
        if (display_value) cmsMLUfree(display_value);
    }
    
    const cmsDICTentry* entry = cmsDictGetEntryList(dict);
    while (entry) {
        entry = cmsDictNextEntry(entry);
    }
    
    cmsHANDLE dup_dict = cmsDictDup(dict);
    if (dup_dict) {
        const cmsDICTentry* dup_entry = cmsDictGetEntryList(dup_dict);
        while (dup_entry) {
            dup_entry = cmsDictNextEntry(dup_entry);
        }
        cmsDictFree(dup_dict);
    }
    
    cmsDictFree(dict);
    cmsDeleteContext(ctx);
    
    return 0;
}