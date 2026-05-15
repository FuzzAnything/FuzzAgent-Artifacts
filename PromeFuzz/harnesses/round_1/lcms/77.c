// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8GetDataRowColDbl at cmscgats.c:2826:28 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const char* property_names[] = {"prop1", "prop2", "prop3", "prop4", "prop5"};
static const char* sample_names[] = {"sample1", "sample2", "sample3", "sample4", "sample5"};
static const char* patch_names[] = {"patch1", "patch2", "patch3", "patch4", "patch5"};

static cmsHANDLE create_test_it8(void) {
    return cmsIT8Alloc(NULL);
}

static const char* get_string_from_data(const uint8_t** data, size_t* size, char* buffer, size_t buffer_size) {
    if (buffer_size == 0 || *size == 0) {
        buffer[0] = '\0';
        return buffer;
    }
    
    size_t len = (*data)[0] % buffer_size;
    if (len > *size - 1) len = *size - 1;
    
    for (size_t i = 0; i < len && i < buffer_size - 1; i++) {
        buffer[i] = (*data)[i];
    }
    buffer[len] = '\0';
    
    *data += len;
    *size -= len;
    
    return buffer;
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsHANDLE hIT8 = create_test_it8();
    if (!hIT8) return 0;
    
    size_t remaining = Size;
    const uint8_t* data_ptr = Data;
    
    char str_buffer[256];
    int row, col, n;
    cmsFloat64Number val;
    
    while (remaining >= 4) {
        uint8_t op = data_ptr[0] % 6;
        data_ptr++;
        remaining--;
        
        switch (op) {
            case 0:  // cmsIT8SetPropertyDbl
                if (remaining >= 8) {
                    memcpy(&val, data_ptr, sizeof(val));
                    const char* prop = property_names[data_ptr[8] % 5];
                    cmsIT8SetPropertyDbl(hIT8, prop, val);
                    data_ptr += 9;
                    remaining -= 9;
                }
                break;
                
            case 1:  // cmsIT8SetDataRowCol
                if (remaining >= 8) {
                    memcpy(&row, data_ptr, sizeof(row));
                    memcpy(&col, data_ptr + 4, sizeof(col));
                    row = abs(row) % 100;
                    col = abs(col) % 100;
                    get_string_from_data(&data_ptr, &remaining, str_buffer, sizeof(str_buffer));
                    cmsIT8SetDataRowCol(hIT8, row, col, str_buffer);
                }
                break;
                
            case 2:  // cmsIT8GetDataRowColDbl
                if (remaining >= 8) {
                    memcpy(&row, data_ptr, sizeof(row));
                    memcpy(&col, data_ptr + 4, sizeof(col));
                    row = abs(row) % 100;
                    col = abs(col) % 100;
                    cmsIT8GetDataRowColDbl(hIT8, row, col);
                    data_ptr += 8;
                    remaining -= 8;
                }
                break;
                
            case 3:  // cmsIT8SetDataFormat
                if (remaining >= 4) {
                    memcpy(&n, data_ptr, sizeof(n));
                    n = abs(n) % 100;
                    const char* sample = sample_names[data_ptr[4] % 5];
                    cmsIT8SetDataFormat(hIT8, n, sample);
                    data_ptr += 5;
                    remaining -= 5;
                }
                break;
                
            case 4:  // cmsIT8SetDataDbl
                if (remaining >= 8) {
                    memcpy(&val, data_ptr, sizeof(val));
                    const char* patch = patch_names[data_ptr[8] % 5];
                    const char* sample = sample_names[data_ptr[9] % 5];
                    cmsIT8SetDataDbl(hIT8, patch, sample, val);
                    data_ptr += 10;
                    remaining -= 10;
                }
                break;
                
            case 5:  // cmsIT8SetDataRowColDbl
                if (remaining >= 12) {
                    memcpy(&row, data_ptr, sizeof(row));
                    memcpy(&col, data_ptr + 4, sizeof(col));
                    memcpy(&val, data_ptr + 8, sizeof(val));
                    row = abs(row) % 100;
                    col = abs(col) % 100;
                    cmsIT8SetDataRowColDbl(hIT8, row, col, val);
                    data_ptr += 12;
                    remaining -= 12;
                }
                break;
        }
        
        if (remaining < 4) break;
    }
    
    cmsIT8Free(hIT8);
    return 0;
}