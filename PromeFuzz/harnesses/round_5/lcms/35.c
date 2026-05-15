// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
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
        char* str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size < max_len ? size : max_len;
    char* str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

static void cleanup_strings(char** strings, int count) {
    for (int i = 0; i < count; i++) {
        if (strings[i]) free(strings[i]);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create IT8 handle from file
    const char* filename = "./dummy_file";
    FILE* f = fopen(filename, "wb");
    if (!f) return 0;
    
    // Write some valid IT8 header data followed by fuzz data
    const char* header = "CTI1\nDESCRIPTOR \"Test\"\nORIGINATOR \"Fuzzer\"\n\n";
    fwrite(header, 1, strlen(header), f);
    fwrite(Data, 1, Size, f);
    fclose(f);
    
    // Load IT8 handle from file
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, filename);
    if (!hIT8) {
        // Try with empty handle if file loading fails
        hIT8 = cmsIT8Alloc(NULL);
        if (!hIT8) return 0;
    }
    
    // Extract parameters from fuzz data with bounds checking
    size_t offset = 0;
    
    // Create strings for function parameters
    char* strings[6] = {NULL};
    int string_count = 0;
    
    // Create first string (for sample names)
    if (Size > 0) {
        size_t str1_len = (Data[0] % 64) + 1;
        if (str1_len > Size - 1) {
            str1_len = Size > 1 ? Size - 1 : 0;
        }
        if (str1_len > 0 && 1 < Size) {
            strings[0] = create_null_terminated_string(Data + 1, str1_len, 63);
            string_count++;
            offset = 1 + str1_len;
        } else {
            strings[0] = malloc(1);
            if (strings[0]) strings[0][0] = '\0';
            string_count++;
            offset = 1;
        }
    }
    
    // Create remaining strings if we have enough data
    for (int i = 1; i < 6 && offset < Size; i++) {
        size_t str_len = (Data[offset % Size] % 32) + 1;
        
        // Check if we have enough data remaining
        if (offset + 1 >= Size) {
            // Not enough data for string content
            break;
        }
        
        size_t available = Size - offset - 1;
        if (str_len > available) {
            str_len = available;
        }
        
        if (str_len > 0) {
            strings[i] = create_null_terminated_string(Data + offset + 1, str_len, 31);
            string_count++;
            offset += 1 + str_len;
        } else {
            // Create empty string
            strings[i] = malloc(1);
            if (strings[i]) strings[i][0] = '\0';
            string_count++;
            offset += 1;
        }
    }
    
    // Get integer parameter
    int n = Size > 0 ? Data[0] : 0;
    
    // Call target functions in various combinations
    if (strings[0]) {
        cmsIT8SetDataFormat(hIT8, n, strings[0]);
    }
    
    if (strings[1] && strings[2] && strings[3]) {
        cmsIT8SetData(hIT8, strings[1], strings[2], strings[3]);
    }
    
    if (strings[0]) {
        cmsIT8FindDataFormat(hIT8, strings[0]);
    }
    
    if (strings[1]) {
        cmsIT8SetIndexColumn(hIT8, strings[1]);
    }
    
    if (strings[2] && strings[3] && strings[4]) {
        cmsIT8SetTableByLabel(hIT8, strings[2], strings[3], strings[4]);
    }
    
    // Try with NULL parameters
    cmsIT8SetDataFormat(hIT8, 0, NULL);
    cmsIT8SetData(hIT8, NULL, NULL, NULL);
    cmsIT8FindDataFormat(hIT8, NULL);
    cmsIT8SetIndexColumn(hIT8, NULL);
    cmsIT8SetTableByLabel(hIT8, NULL, NULL, NULL);
    
    // Clean up
    cleanup_strings(strings, string_count);
    cmsIT8Free(hIT8);
    
    // Test with NULL handle
    cmsIT8Free(NULL);
    
    return 0;
}