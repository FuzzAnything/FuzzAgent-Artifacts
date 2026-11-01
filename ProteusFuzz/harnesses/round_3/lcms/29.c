// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictGetEntryList at cmsnamed.c:1189:31 in lcms2.h
// cmsDictNextEntry at cmsnamed.c:1198:31 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
// cmsDictAddEntry at cmsnamed.c:1137:19 in lcms2.h
// cmsDictDup at cmsnamed.c:1161:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <lcms2.h>

static cmsContext create_context(const uint8_t *Data, size_t Size) {
    // For fuzzing, we primarily use the global context (NULL) which is safe and requires no cleanup.
    // Creating a custom context requires a memory manager which is complex to set up correctly for fuzzing
    // without external dependencies. Using NULL is the standard approach for simple dictionary fuzzing.
    return NULL; 
}

static void cleanup_context(cmsContext ctx) {
    // If we were using a custom context, we would call cmsCloseContext here.
    // Since create_context returns NULL (global context), we do nothing.
    // The global context is managed by the library and should not be closed by the user.
    (void)ctx;
}

static void cleanup_dict(cmsHANDLE hDict) {
    if (hDict != NULL) {
        cmsDictFree(hDict);
    }
}

static wchar_t* create_wide_string(const uint8_t *Data, size_t Size) {
    if (Size == 0) return NULL;
    
    // Limit size to prevent allocation of huge strings
    size_t max_size = 1024;
    if (Size > max_size) Size = max_size;
    
    wchar_t *str = (wchar_t*)malloc((Size + 1) * sizeof(wchar_t));
    if (!str) return NULL;
    
    // Copy data, converting bytes to wide chars (simple mapping for fuzzing)
    for (size_t i = 0; i < Size; i++) {
        str[i] = (wchar_t)Data[i];
    }
    str[Size] = L'\0';
    
    return str;
}

static cmsMLU* create_dummy_mlu(cmsContext ctx) {
    // Creating a valid MLU is complex and requires internal API or specific setup.
    // For fuzzing cmsDictAddEntry, passing NULL for MLU is valid (no localized strings).
    // We will return NULL to keep the fuzzer simple and avoid complex internal state setup.
    (void)ctx;
    return NULL;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_context(Data, Size);
    cmsHANDLE hDict = NULL;
    cmsHANDLE hDictDup = NULL;
    
    // Step 1: Allocate a dictionary
    hDict = cmsDictAlloc(ctx);
    if (hDict == NULL) {
        cleanup_context(ctx);
        return 0;
    }

    // Step 2: Prepare data for adding entries
    // We split the input data to generate Name and Value
    size_t mid = Size / 2;
    wchar_t *name = create_wide_string(Data, mid);
    wchar_t *value = create_wide_string(Data + mid, Size - mid);
    
    cmsMLU *displayName = create_dummy_mlu(ctx);
    cmsMLU *displayValue = create_dummy_mlu(ctx);

    // Step 3: Add entries (multiple times to stress test)
    if (name != NULL && value != NULL) {
        // Try adding the entry
        cmsBool result = cmsDictAddEntry(hDict, name, value, displayName, displayValue);
        (void)result; // Suppress unused variable warning
    }
    
    // Add a few more entries with modified data to explore states
    if (name != NULL && value != NULL) {
        wchar_t *name2 = (wchar_t*)malloc((mid + 1) * sizeof(wchar_t));
        if (name2) {
            memcpy(name2, name, mid * sizeof(wchar_t));
            name2[mid] = L'\0';
            cmsDictAddEntry(hDict, name2, value, NULL, NULL);
            free(name2);
        }
    }

    // Step 4: Duplicate the dictionary
    hDictDup = cmsDictDup(hDict);
    
    // Step 5: Iterate through the original dictionary
    const cmsDICTentry *entry = cmsDictGetEntryList(hDict);
    while (entry != NULL) {
        // Just traverse to ensure no crashes
        entry = cmsDictNextEntry(entry);
    }

    // Step 6: Iterate through the duplicated dictionary
    if (hDictDup != NULL) {
        entry = cmsDictGetEntryList(hDictDup);
        while (entry != NULL) {
            entry = cmsDictNextEntry(entry);
        }
    }

    // Step 7: Cleanup
    // Free the duplicated dictionary
    cleanup_dict(hDictDup);
    
    // Free the original dictionary
    cleanup_dict(hDict);
    
    // Free allocated strings
    free(name);
    free(value);
    
    // Cleanup context (no-op for global context)
    cleanup_context(ctx);

    return 0;
}