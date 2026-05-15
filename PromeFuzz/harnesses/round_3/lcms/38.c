// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
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

static const char* read_string(const uint8_t** data, size_t* size, size_t max_len) {
    if (*size == 0) return NULL;
    
    size_t len = 0;
    while (len < max_len && len < *size && (*data)[len] != 0) {
        len++;
    }
    
    if (len == *size) return NULL;
    
    char* str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, *data, len);
    str[len] = '\0';
    
    *data += len + 1;
    *size -= len + 1;
    
    return str;
}

static int read_int(const uint8_t** data, size_t* size) {
    if (*size < sizeof(int)) return 0;
    
    int value;
    memcpy(&value, *data, sizeof(int));
    
    *data += sizeof(int);
    *size -= sizeof(int);
    
    return value;
}

static double read_double(const uint8_t** data, size_t* size) {
    if (*size < sizeof(double)) return 0.0;
    
    double value;
    memcpy(&value, *data, sizeof(double));
    
    *data += sizeof(double);
    *size -= sizeof(double);
    
    return value;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    const uint8_t* data_ptr = Data;
    size_t data_size = Size;
    
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    // Test cmsIT8SetSheetType
    const char* sheet_type = read_string(&data_ptr, &data_size, 50);
    if (sheet_type) {
        cmsIT8SetSheetType(hIT8, sheet_type);
        free((void*)sheet_type);
    }
    
    // Test cmsIT8SetPropertyDbl
    const char* prop_name = read_string(&data_ptr, &data_size, 50);
    if (prop_name) {
        double prop_value = read_double(&data_ptr, &data_size);
        cmsIT8SetPropertyDbl(hIT8, prop_name, prop_value);
        free((void*)prop_name);
    }
    
    // Test cmsIT8EnumDataFormat
    char** sample_names = NULL;
    int format_count = cmsIT8EnumDataFormat(hIT8, &sample_names);
    
    // Test cmsIT8SetDataDbl
    const char* patch = read_string(&data_ptr, &data_size, 50);
    const char* sample = read_string(&data_ptr, &data_size, 50);
    if (patch && sample) {
        double data_value = read_double(&data_ptr, &data_size);
        cmsIT8SetDataDbl(hIT8, patch, sample, data_value);
        free((void*)patch);
        free((void*)sample);
    }
    
    // Test cmsIT8GetDataRowCol
    int row = read_int(&data_ptr, &data_size);
    int col = read_int(&data_ptr, &data_size);
    const char* cell_data = cmsIT8GetDataRowCol(hIT8, row, col);
    
    // Test cmsIT8SaveToFile
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (data_size > 0) {
            fwrite(data_ptr, 1, data_size, dummy_file);
        }
        fclose(dummy_file);
        
        cmsIT8SaveToFile(hIT8, "./dummy_file");
        remove("./dummy_file");
    }
    
    // Cleanup
    cmsIT8Free(hIT8);
    
    return 0;
}