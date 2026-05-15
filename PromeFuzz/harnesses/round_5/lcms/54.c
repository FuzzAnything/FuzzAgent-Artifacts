// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char* create_null_terminated_string(const uint8_t* data, size_t size) {
    if (size == 0) {
        char* str = (char*)malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    char* str = (char*)malloc(size + 1);
    if (!str) return NULL;
    
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_54(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    // Prepare strings from fuzzer input
    size_t str1_len = Size / 3;
    size_t str2_len = Size / 3;
    size_t str3_len = Size - str1_len - str2_len;
    
    char* sheet_type = create_null_terminated_string(Data, str1_len);
    char* comment = create_null_terminated_string(Data + str1_len, str2_len);
    char* property = create_null_terminated_string(Data + str1_len + str2_len, str3_len);
    
    if (!sheet_type || !comment || !property) {
        free(sheet_type);
        free(comment);
        free(property);
        cmsIT8Free(hIT8);
        return 0;
    }
    
    // Test cmsIT8SetSheetType
    cmsIT8SetSheetType(hIT8, sheet_type);
    
    // Test cmsIT8SetTable with various table indices
    cmsUInt32Number table_indices[] = {0, 1, 2, 100, (cmsUInt32Number)-1};
    for (size_t i = 0; i < sizeof(table_indices)/sizeof(table_indices[0]); i++) {
        cmsIT8SetTable(hIT8, table_indices[i]);
    }
    
    // Use fuzzer data to generate table index
    cmsUInt32Number fuzz_table_index = *(cmsUInt32Number*)Data % 100;
    cmsIT8SetTable(hIT8, fuzz_table_index);
    
    // Test cmsIT8SetComment
    cmsIT8SetComment(hIT8, comment);
    
    // Test cmsIT8SetPropertyHex
    cmsUInt32Number hex_value = *(cmsUInt32Number*)Data;
    cmsIT8SetPropertyHex(hIT8, property, hex_value);
    
    // Test cmsIT8TableCount
    cmsUInt32Number table_count = cmsIT8TableCount(hIT8);
    
    // Test cmsIT8SaveToMem
    cmsUInt32Number bytes_needed = 0;
    cmsBool save_result = cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
    
    if (save_result && bytes_needed > 0 && bytes_needed < 1024 * 1024) {
        void* buffer = malloc(bytes_needed);
        if (buffer) {
            cmsIT8SaveToMem(hIT8, buffer, &bytes_needed);
            free(buffer);
        }
    }
    
    // Cleanup
    free(sheet_type);
    free(comment);
    free(property);
    cmsIT8Free(hIT8);
    
    return 0;
}