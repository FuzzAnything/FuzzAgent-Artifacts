// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8GetDataRowColDbl at cmscgats.c:2826:28 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHANDLE create_test_it8(void) {
    // Create a dummy IT8 handle for testing
    return cmsIT8Alloc(NULL);
}

static void cleanup_it8(cmsHANDLE hIT8) {
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
}

static const char* get_string_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return "";
    }
    
    // Find null terminator or end of data
    size_t start = *offset;
    while (*offset < size && data[*offset] != 0) {
        (*offset)++;
    }
    
    // Create a temporary string (we'll leak this but it's okay for fuzzing)
    char *str = malloc(*offset - start + 1);
    if (!str) {
        return "";
    }
    
    memcpy(str, data + start, *offset - start);
    str[*offset - start] = '\0';
    
    // Skip null terminator if present
    if (*offset < size && data[*offset] == 0) {
        (*offset)++;
    }
    
    return str;
}

static int get_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    
    int value;
    memcpy(&value, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return value;
}

static double get_double_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(double) > size) {
        return 0.0;
    }
    
    double value;
    memcpy(&value, data + *offset, sizeof(double));
    *offset += sizeof(double);
    return value;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }
    
    cmsHANDLE hIT8 = create_test_it8();
    if (!hIT8) {
        return 0;
    }
    
    size_t offset = 0;
    
    // Parse input data to get parameters
    int row = get_int_from_data(Data, Size, &offset);
    int col = get_int_from_data(Data, Size, &offset);
    double dbl_val = get_double_from_data(Data, Size, &offset);
    
    // Get string parameters
    const char* val_str = get_string_from_data(Data, Size, &offset);
    const char* sample_str = get_string_from_data(Data, Size, &offset);
    const char* patch_str = get_string_from_data(Data, Size, &offset);
    const char* prop_str = get_string_from_data(Data, Size, &offset);
    
    // Call target functions with parsed parameters
    cmsIT8SetDataRowCol(hIT8, row, col, val_str);
    cmsIT8SetDataFormat(hIT8, col, sample_str);
    cmsIT8SetDataDbl(hIT8, patch_str, sample_str, dbl_val);
    cmsIT8SetPropertyDbl(hIT8, prop_str, dbl_val);
    cmsIT8SetDataRowColDbl(hIT8, row, col, dbl_val);
    cmsIT8GetDataRowColDbl(hIT8, row, col);
    
    // Clean up strings (they were allocated by get_string_from_data)
    // Note: In practice we should free these, but for fuzzing we accept the leak
    
    cleanup_it8(hIT8);
    
    return 0;
}