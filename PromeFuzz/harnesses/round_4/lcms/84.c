// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char* create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = (char*)malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    size_t len = size < max_len ? size : max_len;
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

static void free_strings(char** strings, int count) {
    for (int i = 0; i < count; i++) {
        if (strings[i]) free(strings[i]);
    }
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;

    // Extract strings from fuzzer input
    // We'll use the first byte to determine how many strings to extract
    int num_strings = Data[0] % 8 + 1; // 1-8 strings
    if (num_strings < 4) num_strings = 4; // Ensure minimum for testing
    
    char** strings = (char**)malloc(num_strings * sizeof(char*));
    if (!strings) {
        cmsIT8Free(hIT8);
        return 0;
    }
    
    size_t offset = 1;
    for (int i = 0; i < num_strings; i++) {
        if (offset >= Size) {
            strings[i] = create_null_terminated_string(NULL, 0, 0);
        } else {
            size_t str_len = Data[offset] % 64 + 1; // 1-64 bytes
            if (offset + str_len > Size) {
                str_len = Size - offset;
            }
            strings[i] = create_null_terminated_string(Data + offset, str_len, 255);
            offset += str_len;
        }
    }

    // Test cmsIT8SetTableByLabel
    if (num_strings >= 3) {
        cmsIT8SetTableByLabel(hIT8, strings[0], strings[1], strings[2]);
    }

    // Test cmsIT8SetData
    if (num_strings >= 3) {
        cmsIT8SetData(hIT8, strings[0], strings[1], strings[2]);
    }

    // Test cmsIT8SetPropertyUncooked
    if (num_strings >= 2) {
        cmsIT8SetPropertyUncooked(hIT8, strings[0], strings[1]);
    }

    // Test cmsIT8GetPatchName
    int patch_index = Data[0] % 100; // Random patch index
    char buffer[256];
    const char* patch_name = cmsIT8GetPatchName(hIT8, patch_index, buffer);
    if (patch_name) {
        // Use the result to avoid optimization
        volatile char dummy = patch_name[0];
        (void)dummy;
    }

    // Test cmsstrcasecmp
    if (num_strings >= 2) {
        int cmp_result = cmsstrcasecmp(strings[0], strings[1]);
        (void)cmp_result; // Use result to avoid optimization
    }

    // Test cmsIT8GetPropertyMulti
    if (num_strings >= 2) {
        const char* prop_value = cmsIT8GetPropertyMulti(hIT8, strings[0], strings[1]);
        if (prop_value) {
            volatile char dummy = prop_value[0];
            (void)dummy;
        }
    }

    // Cleanup
    free_strings(strings, num_strings);
    free(strings);
    cmsIT8Free(hIT8);
    
    return 0;
}