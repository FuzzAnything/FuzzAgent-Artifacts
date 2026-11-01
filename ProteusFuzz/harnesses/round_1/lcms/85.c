// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictDup at cmsnamed.c:1161:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

static cmsContext create_context(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return cmsCreateContext(NULL, NULL);
    }
    // Use data to seed context creation if needed, but lcms doesn't take raw bytes for context
    // Just create a standard context
    return cmsCreateContext(NULL, NULL);
}

static void free_context(cmsContext ContextID) {
    if (ContextID != NULL) {
        cmsDeleteContext(ContextID);
    }
}

static cmsHANDLE create_dict(cmsContext ContextID) {
    return cmsDictAlloc(ContextID);
}

static void free_dict(cmsHANDLE hDict) {
    if (hDict != NULL) {
        cmsDictFree(hDict);
    }
}

static wchar_t* create_wide_string(const uint8_t *Data, size_t Size) {
    if (Size == 0) return NULL;
    // Limit size to avoid huge allocations
    if (Size > 1024) Size = 1024;
    
    wchar_t *str = (wchar_t *)malloc((Size + 1) * sizeof(wchar_t));
    if (str == NULL) return NULL;
    
    for (size_t i = 0; i < Size; i++) {
        str[i] = (wchar_t)Data[i];
    }
    str[Size] = L'\0';
    return str;
}

static cmsMLU* create_mlu(cmsContext ContextID, const uint8_t *Data, size_t Size) {
    // Simplified MLU creation for fuzzing
    // In real usage, cmsMLU is complex, but for fuzzing we can try to create one
    // Since there's no direct public API to create an empty MLU easily without context,
    // we might skip this or use NULL if the API allows.
    // However, cmsDictAddEntry accepts NULL for DisplayName/DisplayValue.
    // Let's return NULL for simplicity as creating a valid MLU requires more setup.
    return NULL;
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = create_context(Data, Size);
    if (ContextID == NULL) return 0;

    cmsHANDLE hDict = create_dict(ContextID);
    if (hDict == NULL) {
        free_context(ContextID);
        return 0;
    }

    // Fuzz cmsDictAddEntry
    // We need to split data into Name and Value
    size_t split_point = Size / 2;
    if (split_point == 0) split_point = 1;
    
    wchar_t *Name = create_wide_string(Data, split_point);
    wchar_t *Value = create_wide_string(Data + split_point, Size - split_point);

    if (Name != NULL && Value != NULL) {
        cmsBool result = cmsDictAddEntry(hDict, Name, Value, NULL, NULL);
        (void)result; // Ignore result for fuzzing, just ensure no crash
    }

    // Fuzz cmsDictGetEntryList
    const cmsDICTentry *entry = cmsDictGetEntryList(hDict);
    
    // Fuzz cmsDictNextEntry by iterating
    const cmsDICTentry *current = entry;
    while (current != NULL) {
        current = cmsDictNextEntry(current);
    }

    // Fuzz cmsDictDup
    cmsHANDLE hDupDict = cmsDictDup(hDict);
    if (hDupDict != NULL) {
        // Verify we can traverse the duplicate
        const cmsDICTentry *dupEntry = cmsDictGetEntryList(hDupDict);
        while (dupEntry != NULL) {
            dupEntry = cmsDictNextEntry(dupEntry);
        }
        free_dict(hDupDict);
    }

    // Cleanup
    if (Name != NULL) free(Name);
    if (Value != NULL) free(Value);
    free_dict(hDict);
    free_context(ContextID);

    return 0;
}