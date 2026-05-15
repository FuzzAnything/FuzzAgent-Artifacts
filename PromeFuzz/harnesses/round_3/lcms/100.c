// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8GetPropertyDbl at cmscgats.c:1591:28 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8GetDataDbl at cmscgats.c:2883:28 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
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

static const uint8_t* extract_string(const uint8_t* data, size_t size, size_t* offset, char* buf, size_t buf_size) {
    if (*offset >= size || buf_size == 0) return NULL;
    
    size_t i = 0;
    while (*offset + i < size && i < buf_size - 1 && data[*offset + i] != 0) {
        buf[i] = data[*offset + i];
        i++;
    }
    buf[i] = '\0';
    *offset += i + 1; // Skip null terminator if present
    return buf;
}

static double extract_double(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + sizeof(double) > size) return 0.0;
    
    double value;
    memcpy(&value, data + *offset, sizeof(double));
    *offset += sizeof(double);
    return value;
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    size_t offset = 0;
    
    // Create IT8 handle from memory
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    // Extract strings for various parameters
    char prop_buf1[256], prop_buf2[256], prop_buf3[256];
    char patch_buf1[256], patch_buf2[256];
    char sample_buf1[256], sample_buf2[256];
    char formatter_buf[256];
    
    // Extract parameters from fuzzer input
    const char* prop1 = extract_string(Data, Size, &offset, prop_buf1, sizeof(prop_buf1));
    const char* prop2 = extract_string(Data, Size, &offset, prop_buf2, sizeof(prop_buf2));
    const char* prop3 = extract_string(Data, Size, &offset, prop_buf3, sizeof(prop_buf3));
    const char* patch1 = extract_string(Data, Size, &offset, patch_buf1, sizeof(patch_buf1));
    const char* patch2 = extract_string(Data, Size, &offset, patch_buf2, sizeof(patch_buf2));
    const char* sample1 = extract_string(Data, Size, &offset, sample_buf1, sizeof(sample_buf1));
    const char* sample2 = extract_string(Data, Size, &offset, sample_buf2, sizeof(sample_buf2));
    const char* formatter = extract_string(Data, Size, &offset, formatter_buf, sizeof(formatter_buf));
    
    // Extract double values
    double val1 = extract_double(Data, Size, &offset);
    double val2 = extract_double(Data, Size, &offset);
    double val3 = extract_double(Data, Size, &offset);
    
    // Call target API functions
    if (prop1) {
        cmsFloat64Number result = cmsIT8GetPropertyDbl(hIT8, prop1);
        (void)result; // Use result to avoid unused variable warning
    }
    
    if (formatter) {
        cmsIT8DefineDblFormat(hIT8, formatter);
    } else {
        cmsIT8DefineDblFormat(hIT8, NULL);
    }
    
    if (patch1 && sample1) {
        cmsBool success = cmsIT8SetDataDbl(hIT8, patch1, sample1, val1);
        (void)success;
    }
    
    if (patch2 && sample2) {
        cmsFloat64Number result = cmsIT8GetDataDbl(hIT8, patch2, sample2);
        (void)result;
    }
    
    if (prop2) {
        cmsBool success = cmsIT8SetPropertyDbl(hIT8, prop2, val2);
        (void)success;
    }
    
    if (prop3) {
        const char* result = cmsIT8GetProperty(hIT8, prop3);
        (void)result;
    }
    
    // Clean up
    cmsIT8Free(hIT8);
    
    return 0;
}