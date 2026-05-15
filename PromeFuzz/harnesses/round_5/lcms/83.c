// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8GetDataRowColDbl at cmscgats.c:2826:28 in lcms2.h
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
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
#include <stdio.h>

static const char* extract_string(const uint8_t** data, size_t* size) {
    if (*size == 0) {
        return "";
    }
    
    const uint8_t* start = *data;
    const uint8_t* end = start + *size;
    const uint8_t* ptr = start;
    
    while (ptr < end && *ptr != 0) {
        ptr++;
    }
    
    size_t len = ptr - start;
    char* str = malloc(len + 1);
    if (!str) {
        return "";
    }
    
    memcpy(str, start, len);
    str[len] = '\0';
    
    *data = (ptr < end) ? ptr + 1 : end;
    *size = (ptr < end) ? (size_t)(end - ptr - 1) : 0;
    
    return str;
}

static void free_string(const char* str) {
    if (str && str != "") {
        free((void*)str);
    }
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) {
        return 0;
    }
    
    const uint8_t* current_data = Data;
    size_t remaining_size = Size;
    
    const char* sample_format = extract_string(&current_data, &remaining_size);
    const char* cell_value = extract_string(&current_data, &remaining_size);
    const char* property_name = extract_string(&current_data, &remaining_size);
    const char* patch_name = extract_string(&current_data, &remaining_size);
    const char* sample_name = extract_string(&current_data, &remaining_size);
    
    int n = (remaining_size > 0) ? (current_data[0] % 100) : 0;
    int row = (remaining_size > 1) ? (current_data[1] % 100) : 0;
    int col = (remaining_size > 2) ? (current_data[2] % 100) : 0;
    
    cmsFloat64Number double_val = 0.0;
    if (remaining_size >= 12) {
        memcpy(&double_val, current_data + 3, sizeof(cmsFloat64Number));
    }
    
    cmsIT8SetDataFormat(hIT8, n, sample_format);
    
    cmsIT8SetDataRowCol(hIT8, row, col, cell_value);
    
    cmsFloat64Number retrieved = cmsIT8GetDataRowColDbl(hIT8, row, col);
    (void)retrieved;
    
    cmsIT8SetDataRowColDbl(hIT8, row, col, double_val);
    
    cmsIT8SetPropertyDbl(hIT8, property_name, double_val);
    
    cmsIT8SetDataDbl(hIT8, patch_name, sample_name, double_val);
    
    free_string(sample_format);
    free_string(cell_value);
    free_string(property_name);
    free_string(patch_name);
    free_string(sample_name);
    
    cmsIT8Free(hIT8);
    
    return 0;
}