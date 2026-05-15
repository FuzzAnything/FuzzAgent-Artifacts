// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

#ifndef MAXSTR
#define MAXSTR 2048
#endif

static void extract_string(const uint8_t** data, size_t* size, char* buf, size_t buf_size) {
    if (buf_size == 0) {
        buf[0] = '\0';
        return;
    }
    
    size_t i = 0;
    while (i < buf_size - 1 && i < *size && (*data)[i] != 0) {
        buf[i] = (*data)[i];
        i++;
    }
    buf[i] = '\0';
    
    // Skip the bytes we used, plus null terminator if we found one
    if (i < *size && (*data)[i] == 0) {
        *data += i + 1;
        *size -= i + 1;
    } else {
        *data += i;
        *size -= i;
    }
}

static cmsUInt32Number get_uint32_from_data(const uint8_t** data, size_t* size) {
    if (*size < sizeof(cmsUInt32Number)) {
        cmsUInt32Number val = 0;
        size_t copy_size = *size;
        if (copy_size > 0) {
            memcpy(&val, *data, copy_size);
        }
        *data += copy_size;
        *size = 0;
        return val;
    }
    
    cmsUInt32Number val;
    memcpy(&val, *data, sizeof(cmsUInt32Number));
    *data += sizeof(cmsUInt32Number);
    *size -= sizeof(cmsUInt32Number);
    
    return val;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(0);
    if (!hIT8) return 0;
    
    const uint8_t* data_ptr = Data;
    size_t remaining_size = Size;
    
    // Extract strings with proper bounds checking
    char sheet_type_buf[MAXSTR];
    extract_string(&data_ptr, &remaining_size, sheet_type_buf, MAXSTR);
    
    char prop_name_buf[MAXSTR];
    extract_string(&data_ptr, &remaining_size, prop_name_buf, MAXSTR);
    
    cmsUInt32Number hex_val = get_uint32_from_data(&data_ptr, &remaining_size);
    cmsUInt32Number table_num = get_uint32_from_data(&data_ptr, &remaining_size);
    
    // Use default values if extraction failed
    if (sheet_type_buf[0] == '\0') {
        strncpy(sheet_type_buf, "CGATS.17", MAXSTR - 1);
        sheet_type_buf[MAXSTR - 1] = '\0';
    }
    
    if (prop_name_buf[0] == '\0') {
        strncpy(prop_name_buf, "TestProperty", MAXSTR - 1);
        prop_name_buf[MAXSTR - 1] = '\0';
    }
    
    // 1. Test cmsIT8SetSheetType
    cmsIT8SetSheetType(hIT8, sheet_type_buf);
    
    // 2. Test cmsIT8SetPropertyHex
    cmsIT8SetPropertyHex(hIT8, prop_name_buf, hex_val);
    
    // 3. Test cmsIT8SaveToMem
    cmsUInt32Number bytes_needed = 0;
    cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
    
    if (bytes_needed > 0 && bytes_needed < 1024 * 1024) { // Limit to 1MB
        void* buffer = malloc(bytes_needed);
        if (buffer) {
            cmsUInt32Number bytes_written = bytes_needed;
            cmsIT8SaveToMem(hIT8, buffer, &bytes_written);
            free(buffer);
        }
    }
    
    // 4. Test cmsIT8TableCount
    cmsUInt32Number table_count = cmsIT8TableCount(hIT8);
    
    // 5. Test cmsIT8SetTable - try with different table numbers
    for (cmsUInt32Number i = 0; i < 4; i++) {
        cmsIT8SetTable(hIT8, i);
        cmsIT8SetTable(hIT8, table_num + i);
    }
    
    // 6. Test cmsIT8SaveToFile
    const char* filename = "./dummy_file";
    cmsIT8SaveToFile(hIT8, filename);
    
    // Clean up
    cmsIT8Free(hIT8);
    
    return 0;
}