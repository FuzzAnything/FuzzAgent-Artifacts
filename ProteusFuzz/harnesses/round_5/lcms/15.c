// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
// cmsDictDup at cmsnamed.c:1161:21 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void free_wide_string(wchar_t *s) {
    if (s) {
        free(s);
    }
}

static cmsMLU* create_dummy_mlu(cmsContext ctx) {
    if (!ctx) return NULL;
    cmsMLU *mlu = cmsMLUalloc(ctx, 0);
    return mlu;
}

static void free_mlu(cmsMLU *mlu) {
    if (mlu) {
        cmsMLUfree(mlu);
    }
}

static wchar_t* create_wide_string(const uint8_t *data, size_t len) {
    if (len == 0) return NULL;
    // Allocate enough space for 'len' characters plus a null terminator
    wchar_t *wstr = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    if (!wstr) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        wstr[i] = (wchar_t)data[i];
    }
    wstr[len] = L'\0';
    return wstr;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsHANDLE hDict = cmsDictAlloc(ctx);
    if (!hDict) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Fuzz cmsDictAddEntry
    // We split the input data to generate Name and Value
    // Ensure we have at least 1 byte for name and 1 for value
    // Prevent buffer overflow by checking bounds strictly
    size_t nameLen = (Size % 100) + 1;
    size_t valLen = Size - nameLen;
    
    // Safety check: ensure valLen is at least 1 to avoid empty value string issues
    // and that nameLen does not exceed Size
    if (nameLen >= Size) {
        nameLen = Size - 1;
        if (nameLen == 0) nameLen = 1;
        valLen = Size - nameLen;
    }
    
    if (valLen > 0) {
        wchar_t *name = create_wide_string(Data, nameLen);
        wchar_t *value = create_wide_string(Data + nameLen, valLen);

        cmsMLU *dispName = create_dummy_mlu(ctx);
        cmsMLU *dispValue = create_dummy_mlu(ctx);

        if (name && value) {
            cmsDictAddEntry(hDict, name, value, dispName, dispValue);
        }

        free_wide_string(name);
        free_wide_string(value);
        free_mlu(dispName);
        free_mlu(dispValue);
    }

    // Fuzz cmsDictDup
    cmsHANDLE hDictDup = cmsDictDup(hDict);

    // Fuzz cmsDictGetEntryList and cmsDictNextEntry on original
    const cmsDICTentry *entry = cmsDictGetEntryList(hDict);
    while (entry != NULL) {
        entry = cmsDictNextEntry(entry);
    }

    // Fuzz cmsDictGetEntryList and cmsDictNextEntry on duplicate
    if (hDictDup) {
        const cmsDICTentry *entryDup = cmsDictGetEntryList(hDictDup);
        while (entryDup != NULL) {
            entryDup = cmsDictNextEntry(entryDup);
        }
        cmsDictFree(hDictDup);
    }

    // Cleanup
    cmsDictFree(hDict);
    cmsDeleteContext(ctx);

    return 0;
}