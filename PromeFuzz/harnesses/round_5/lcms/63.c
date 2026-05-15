// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
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

static cmsHANDLE create_test_it8(void) {
    // Create a minimal IT8 handle for testing
    // Using NULL context for simplicity
    return cmsIT8Alloc(NULL);
}

static void cleanup_it8(cmsHANDLE hIT8) {
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
}

static const char* extract_string(const uint8_t** data, size_t* size) {
    if (*size == 0) {
        return "";
    }
    
    // Find null terminator or use available bytes
    size_t len = 0;
    while (len < *size && (*data)[len] != 0) {
        len++;
    }
    
    static char buffer[256];
    size_t copy_len = (len < 255) ? len : 255;
    memcpy(buffer, *data, copy_len);
    buffer[copy_len] = '\0';
    
    // Advance data pointer
    *data += (len < *size) ? len + 1 : *size;
    *size -= (len < *size) ? len + 1 : *size;
    
    return buffer;
}

static int extract_int(const uint8_t** data, size_t* size) {
    if (*size < sizeof(int)) {
        *data += *size;
        *size = 0;
        return 0;
    }
    
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    
    return value;
}

static double extract_double(const uint8_t** data, size_t* size) {
    if (*size < sizeof(double)) {
        *data += *size;
        *size = 0;
        return 0.0;
    }
    
    double value;
    memcpy(&value, *data, sizeof(double));
    *data += sizeof(double);
    *size -= sizeof(double);
    
    return value;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    cmsHANDLE hIT8 = create_test_it8();
    if (!hIT8) {
        return 0;
    }
    
    const uint8_t* current_data = Data;
    size_t remaining_size = Size;
    
    // Extract parameters for each function call
    int row = extract_int(&current_data, &remaining_size);
    int col = extract_int(&current_data, &remaining_size);
    const char* val1 = extract_string(&current_data, &remaining_size);
    
    // Call cmsIT8SetDataRowCol
    cmsIT8SetDataRowCol(hIT8, row, col, val1);
    
    // Extract parameters for cmsIT8SetPropertyStr
    const char* cProp2 = extract_string(&current_data, &remaining_size);
    const char* Str2 = extract_string(&current_data, &remaining_size);
    
    // Call cmsIT8SetPropertyStr
    cmsIT8SetPropertyStr(hIT8, cProp2, Str2);
    
    // Extract parameters for cmsIT8SetPropertyDbl
    const char* cProp3 = extract_string(&current_data, &remaining_size);
    double val3 = extract_double(&current_data, &remaining_size);
    
    // Call cmsIT8SetPropertyDbl
    cmsIT8SetPropertyDbl(hIT8, cProp3, val3);
    
    // Extract parameters for cmsIT8SetPropertyMulti
    const char* Key4 = extract_string(&current_data, &remaining_size);
    const char* SubKey4 = extract_string(&current_data, &remaining_size);
    const char* Buffer4 = extract_string(&current_data, &remaining_size);
    
    // Call cmsIT8SetPropertyMulti
    cmsIT8SetPropertyMulti(hIT8, Key4, SubKey4, Buffer4);
    
    // Call cmsIT8EnumDataFormat
    char** SampleNames = NULL;
    int count = cmsIT8EnumDataFormat(hIT8, &SampleNames);
    (void)count; // Use count to avoid unused variable warning
    
    // Extract parameters for cmsIT8SetDataDbl
    const char* cPatch6 = extract_string(&current_data, &remaining_size);
    const char* cSample6 = extract_string(&current_data, &remaining_size);
    double val6 = extract_double(&current_data, &remaining_size);
    
    // Call cmsIT8SetDataDbl
    cmsIT8SetDataDbl(hIT8, cPatch6, cSample6, val6);
    
    // Cleanup
    cleanup_it8(hIT8);
    
    return 0;
}