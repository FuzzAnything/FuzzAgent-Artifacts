// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictDup at cmsnamed.c:1161:21 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <lcms2.h>

static cmsContext create_context(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return NULL;
    return ctx;
}

static void free_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static cmsHANDLE create_dict(cmsContext ctx) {
    if (ctx == NULL) return NULL;
    return cmsDictAlloc(ctx);
}

static void free_dict(cmsHANDLE hDict) {
    if (hDict != NULL) {
        cmsDictFree(hDict);
    }
}

static wchar_t* fuzz_wchar_from_data(const uint8_t *Data, size_t Size, size_t *out_len) {
    if (Size == 0) {
        if (out_len) *out_len = 0;
        return NULL;
    }
    
    size_t max_len = 1024;
    if (Size > max_len) Size = max_len;
    
    wchar_t *wstr = (wchar_t*)malloc((Size + 1) * sizeof(wchar_t));
    if (wstr == NULL) {
        if (out_len) *out_len = 0;
        return NULL;
    }
    
    for (size_t i = 0; i < Size; i++) {
        wstr[i] = (wchar_t)(Data[i] & 0x7F);
    }
    wstr[Size] = L'\0';
    if (out_len) *out_len = Size;
    return wstr;
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsContext ctx = create_context(Data, Size);
    if (ctx == NULL) return 0;
    
    cmsHANDLE hDict = create_dict(ctx);
    if (hDict == NULL) {
        free_context(ctx);
        return 0;
    }
    
    wchar_t *key = fuzz_wchar_from_data(Data, Size, NULL);
    wchar_t *val = fuzz_wchar_from_data(Data, Size, NULL);
    
    // Note: cmsMLUset is not a public API in standard lcms2, so we skip MLU population
    // to ensure compilation. We pass NULL for MLU arguments.
    cmsBool result = cmsDictAddEntry(hDict, key, val, NULL, NULL);
    (void)result;
    
    const cmsDICTentry *entry = cmsDictGetEntryList(hDict);
    while (entry != NULL) {
        entry = cmsDictNextEntry(entry);
    }
    
    cmsHANDLE hDictDup = cmsDictDup(hDict);
    if (hDictDup != NULL) {
        const cmsDICTentry *dup_entry = cmsDictGetEntryList(hDictDup);
        while (dup_entry != NULL) {
            dup_entry = cmsDictNextEntry(dup_entry);
        }
        free_dict(hDictDup);
    }
    
    if (key) free(key);
    if (val) free(val);
    
    free_dict(hDict);
    free_context(ctx);
    
    return 0;
}