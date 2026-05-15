// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void cleanup_handle(cmsHANDLE hIT8) {
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
}

static char* create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len) {
    if (max_len == 0 || size == 0) return NULL;
    
    size_t len = (size < max_len - 1) ? size : max_len - 1;
    char* str = (char*)malloc(max_len);
    if (!str) return NULL;
    
    if (len > 0) {
        memcpy(str, data, len);
    }
    str[len] = '\0';
    return str;
}

static int extract_int(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return 0;
    return (int)data[offset];
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;

    // Extract strings from fuzzer input with bounds checking
    size_t str_len = 50;
    size_t max_offset = 5 * str_len;
    
    // Ensure we don't read beyond available data
    char* str1 = NULL;
    char* str2 = NULL;
    char* str3 = NULL;
    char* str4 = NULL;
    char* str5 = NULL;
    
    // Calculate safe sizes for each string
    size_t remaining = Size;
    const uint8_t* current_ptr = Data;
    
    if (remaining > 0) {
        size_t copy_len = (remaining < str_len) ? remaining : str_len;
        str1 = create_null_terminated_string(current_ptr, copy_len, str_len);
        if (copy_len <= remaining) {
            remaining -= copy_len;
            current_ptr += copy_len;
        }
    }
    
    if (remaining > 0) {
        size_t copy_len = (remaining < str_len) ? remaining : str_len;
        str2 = create_null_terminated_string(current_ptr, copy_len, str_len);
        if (copy_len <= remaining) {
            remaining -= copy_len;
            current_ptr += copy_len;
        }
    }
    
    if (remaining > 0) {
        size_t copy_len = (remaining < str_len) ? remaining : str_len;
        str3 = create_null_terminated_string(current_ptr, copy_len, str_len);
        if (copy_len <= remaining) {
            remaining -= copy_len;
            current_ptr += copy_len;
        }
    }
    
    if (remaining > 0) {
        size_t copy_len = (remaining < str_len) ? remaining : str_len;
        str4 = create_null_terminated_string(current_ptr, copy_len, str_len);
        if (copy_len <= remaining) {
            remaining -= copy_len;
            current_ptr += copy_len;
        }
    }
    
    if (remaining > 0) {
        size_t copy_len = (remaining < str_len) ? remaining : str_len;
        str5 = create_null_terminated_string(current_ptr, copy_len, str_len);
    }

    // Test cmsIT8SetData
    if (str1 && str2 && str3) {
        cmsIT8SetData(hIT8, str1, str2, str3);
    }

    // Test cmsIT8GetPropertyMulti
    if (str1 && str2) {
        const char* result = cmsIT8GetPropertyMulti(hIT8, str1, str2);
        (void)result;
    }

    // Test cmsIT8SetTableByLabel
    if (str1 && str2 && str3) {
        int table_result = cmsIT8SetTableByLabel(hIT8, str1, str2, str3);
        (void)table_result;
    }

    // Test cmsIT8SetPropertyUncooked
    if (str1 && str4) {
        cmsIT8SetPropertyUncooked(hIT8, str1, str4);
    }

    // Test cmsstrcasecmp
    if (str1 && str5) {
        int cmp_result = cmsstrcasecmp(str1, str5);
        (void)cmp_result;
    }

    // Test cmsIT8GetDataRowCol
    int row = extract_int(Data, Size, 0);
    int col = extract_int(Data, Size, 1);
    const char* data_result = cmsIT8GetDataRowCol(hIT8, row, col);
    (void)data_result;

    // Cleanup
    free(str1);
    free(str2);
    free(str3);
    free(str4);
    free(str5);
    cleanup_handle(hIT8);

    return 0;
}