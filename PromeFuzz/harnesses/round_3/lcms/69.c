// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8GetDataRowColDbl at cmscgats.c:2826:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static cmsHANDLE create_test_it8(void) {
    return cmsIT8Alloc(NULL);
}

static void cleanup_it8(cmsHANDLE hIT8) {
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
}

static char* extract_string(const uint8_t** data, size_t* size) {
    if (*size == 0) return NULL;
    
    size_t max_len = *size;
    size_t len = 0;
    while (len < max_len && (*data)[len] != 0) {
        len++;
    }
    
    char* str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, *data, len);
    str[len] = '\0';
    
    *data += len;
    *size -= len;
    
    if (*size > 0 && (*data)[0] == 0) {
        (*data)++;
        (*size)--;
    }
    
    return str;
}

static int extract_int(const uint8_t** data, size_t* size) {
    if (*size < sizeof(int)) return 0;
    
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    
    return value;
}

static double extract_double(const uint8_t** data, size_t* size) {
    if (*size < sizeof(double)) return 0.0;
    
    double value;
    memcpy(&value, *data, sizeof(double));
    *data += sizeof(double);
    *size -= sizeof(double);
    
    return value;
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;
    
    cmsHANDLE hIT8 = create_test_it8();
    if (!hIT8) return 0;
    
    const uint8_t* current_data = Data;
    size_t remaining_size = Size;
    
    char* patch = extract_string(&current_data, &remaining_size);
    char* sample = extract_string(&current_data, &remaining_size);
    char* prop = extract_string(&current_data, &remaining_size);
    char* sample_format = extract_string(&current_data, &remaining_size);
    char* cell_value = extract_string(&current_data, &remaining_size);
    
    int row = extract_int(&current_data, &remaining_size);
    int col = extract_int(&current_data, &remaining_size);
    int format_n = extract_int(&current_data, &remaining_size);
    
    double val1 = extract_double(&current_data, &remaining_size);
    double val2 = extract_double(&current_data, &remaining_size);
    double val3 = extract_double(&current_data, &remaining_size);
    double val4 = extract_double(&current_data, &remaining_size);
    
    if (patch && sample) {
        cmsIT8SetDataDbl(hIT8, patch, sample, val1);
    }
    
    if (row >= 0 && col >= 0) {
        double retrieved = cmsIT8GetDataRowColDbl(hIT8, row, col);
        (void)retrieved;
        
        cmsIT8SetDataRowColDbl(hIT8, row, col, val2);
    }
    
    if (prop) {
        cmsIT8SetPropertyDbl(hIT8, prop, val3);
    }
    
    if (sample_format && format_n >= 0) {
        cmsIT8SetDataFormat(hIT8, format_n, sample_format);
    }
    
    if (cell_value && row >= 0 && col >= 0) {
        cmsIT8SetDataRowCol(hIT8, row, col, cell_value);
    }
    
    free(patch);
    free(sample);
    free(prop);
    free(sample_format);
    free(cell_value);
    
    cleanup_it8(hIT8);
    
    return 0;
}