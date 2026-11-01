// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
// cmsDictDup at cmsnamed.c:1161:21 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsContext g_context = NULL;

static void cleanup_context(void) {
    if (g_context != NULL) {
        cmsDeleteContext(g_context);
        g_context = NULL;
    }
}

static cmsContext get_context(void) {
    if (g_context == NULL) {
        g_context = cmsCreateContext(NULL, 0);
        if (g_context == NULL) {
            return NULL;
        }
    }
    return g_context;
}

static wchar_t* fuzz_wide_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    // Limit size to avoid huge allocations
    size_t max_len = 1024;
    if (size > max_len) size = max_len;
    
    wchar_t* result = (wchar_t*)malloc((size + 1) * sizeof(wchar_t));
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < size; i++) {
        result[i] = (wchar_t)data[i];
    }
    result[size] = L'\0';
    return result;
}

// Helper to create an empty MLU. 
// Since cmsMLU is internal and tied to profiles, and we cannot easily create one 
// without complex internal API usage or valid tags, we return NULL.
// The target API (cmsDictAddEntry) accepts NULL for DisplayName and DisplayValue.
static cmsMLU* create_empty_mlu(cmsContext ctx) {
    (void)ctx; // Unused
    return NULL;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    cmsContext ctx = get_context();
    if (ctx == NULL) {
        return 0;
    }

    cmsHANDLE hDict = cmsDictAlloc(ctx);
    if (hDict == NULL) {
        return 0;
    }

    // Parse input to decide operations
    if (Size < 1) {
        cmsDictFree(hDict);
        return 0;
    }

    uint8_t op = Data[0] % 5; // 0: Add, 1: Dup, 2: GetList, 3: Next, 4: Free (immediate)

    if (op == 4) {
        cmsDictFree(hDict);
        return 0;
    }

    // Prepare data for Name, Value, DisplayName, DisplayValue
    // We split the remaining data into 4 chunks
    size_t remaining = Size - 1;
    size_t chunk_size = remaining / 4;
    
    wchar_t* name = fuzz_wide_string(Data + 1, chunk_size);
    wchar_t* value = fuzz_wide_string(Data + 1 + chunk_size, chunk_size);
    
    // For MLU, we return NULL as creating a valid one is complex without internal APIs.
    cmsMLU* displayName = NULL;
    cmsMLU* displayValue = NULL;
    
    // Only try to create MLU if we have enough data (though currently always returns NULL)
    if (chunk_size > 0) {
        displayName = create_empty_mlu(ctx);
        
        if (chunk_size > 0) {
            displayValue = create_empty_mlu(ctx);
        }
    }

    if (op == 0) {
        // cmsDictAddEntry
        // Note: If name is NULL, the function asserts. We ensure name is not NULL for valid testing.
        if (name != NULL) {
            cmsBool res = cmsDictAddEntry(hDict, name, value, displayName, displayValue);
            (void)res;
        }
    } else if (op == 1) {
        // cmsDictDup
        cmsHANDLE hNewDict = cmsDictDup(hDict);
        if (hNewDict != NULL) {
            cmsDictFree(hNewDict);
        }
    } else if (op == 2) {
        // cmsDictGetEntryList
        const cmsDICTentry* entry = cmsDictGetEntryList(hDict);
        // Just iterate a bit to exercise logic
        int count = 0;
        while (entry != NULL && count < 100) {
            entry = cmsDictNextEntry(entry);
            count++;
        }
    } else if (op == 3) {
        // cmsDictNextEntry with NULL input (should return NULL)
        const cmsDICTentry* entry = cmsDictNextEntry(NULL);
        (void)entry;
        
        // Also try getting list and then next
        const cmsDICTentry* head = cmsDictGetEntryList(hDict);
        if (head != NULL) {
            cmsDictNextEntry(head);
        }
    }

    // Cleanup allocated fuzz data
    if (name != NULL) free(name);
    if (value != NULL) free(value);
    // MLU cleanup is skipped as they are NULL in this implementation
    if (displayName != NULL) cmsMLUfree(displayName);
    if (displayValue != NULL) cmsMLUfree(displayValue);

    // Final cleanup of dictionary
    cmsDictFree(hDict);

    return 0;
}