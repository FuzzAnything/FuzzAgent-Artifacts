// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
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
    
    if (len > 0) {
        memcpy(str, data, len);
    }
    str[len] = '\0';
    return str;
}

static int get_random_int(const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(int) <= size) {
        int value;
        memcpy(&value, data + offset, sizeof(int));
        return value;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_42(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    // Split input into parts for different parameters
    size_t quarter = Size / 4;
    size_t eighth = Size / 8;
    
    // Test cmsIT8SetPropertyMulti
    size_t key_size = quarter;
    size_t subkey_offset = quarter;
    size_t subkey_size = (subkey_offset < Size) ? (Size - subkey_offset < eighth ? Size - subkey_offset : eighth) : 0;
    size_t buffer_offset = quarter + eighth;
    size_t buffer_size = (buffer_offset < Size) ? (Size - buffer_offset < eighth ? Size - buffer_offset : eighth) : 0;
    
    char* key = create_null_terminated_string(Data, key_size, 50);
    char* subkey = create_null_terminated_string(Data + subkey_offset, subkey_size, 50);
    char* buffer = create_null_terminated_string(Data + buffer_offset, buffer_size, 100);
    
    if (key && subkey && buffer) {
        cmsIT8SetPropertyMulti(hIT8, key, subkey, buffer);
    }
    
    free(key);
    free(subkey);
    free(buffer);
    
    // Test cmsIT8SetDataFormat
    size_t sample_offset = quarter * 2 + sizeof(int);
    size_t sample_size = (sample_offset < Size) ? (Size - sample_offset < eighth ? Size - sample_offset : eighth) : 0;
    
    int col_index = get_random_int(Data, Size, quarter * 2);
    char* sample = create_null_terminated_string(Data + sample_offset, sample_size, 50);
    
    if (sample) {
        cmsIT8SetDataFormat(hIT8, col_index, sample);
    }
    free(sample);
    
    // Test cmsIT8SetDataRowCol
    int row = get_random_int(Data, Size, quarter * 3);
    int col = get_random_int(Data, Size, quarter * 3 + sizeof(int));
    
    size_t val_offset = quarter * 3 + sizeof(int) * 2;
    size_t val_size = (val_offset < Size) ? Size - val_offset : 0;
    char* value = create_null_terminated_string(Data + val_offset, val_size, 100);
    
    if (value) {
        cmsIT8SetDataRowCol(hIT8, row, col, value);
    }
    free(value);
    
    // Test cmsIT8GetDataRowCol
    const char* retrieved = cmsIT8GetDataRowCol(hIT8, row, col);
    (void)retrieved;
    
    // Test cmsIT8EnumDataFormat
    char** sampleNames = NULL;
    int count = cmsIT8EnumDataFormat(hIT8, &sampleNames);
    (void)count;
    
    // Test cmsIT8GetPatchByName
    size_t patch_name_size = Size < 100 ? Size : 100;
    char* patch_name = create_null_terminated_string(Data, patch_name_size, 100);
    if (patch_name) {
        int patch_index = cmsIT8GetPatchByName(hIT8, patch_name);
        (void)patch_index;
    }
    free(patch_name);
    
    // Cleanup
    cmsIT8Free(hIT8);
    
    return 0;
}