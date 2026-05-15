// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetDataRowColDbl at cmscgats.c:2826:28 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsHANDLE create_test_it8(void) {
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return NULL;
    
    cmsIT8SetSheetType(hIT8, "CGATS.17");
    cmsIT8SetTable(hIT8, 0);
    
    const char* columns[] = {"Sample", "R", "G", "B", "X", "Y", "Z"};
    for (int i = 0; i < 7; i++) {
        cmsIT8SetDataFormat(hIT8, i, columns[i]);
    }
    
    for (int row = 0; row < 10; row++) {
        char patch[20];
        snprintf(patch, sizeof(patch), "Patch_%d", row);
        cmsIT8SetDataRowCol(hIT8, row, 0, patch);
        
        for (int col = 1; col < 7; col++) {
            double val = (row * 10.0 + col) / 100.0;
            cmsIT8SetDataRowColDbl(hIT8, row, col, val);
        }
    }
    
    return hIT8;
}

static const char* extract_string(const uint8_t** data, size_t* size, size_t max_len) {
    if (*size == 0) return "";
    
    size_t len = (*data)[0] % max_len;
    if (len > *size - 1) len = *size - 1;
    
    static char buf[256];
    size_t copy_len = len < sizeof(buf) - 1 ? len : sizeof(buf) - 1;
    memcpy(buf, *data + 1, copy_len);
    buf[copy_len] = '\0';
    
    *data += len + 1;
    *size -= len + 1;
    
    return buf;
}

static double extract_double(const uint8_t** data, size_t* size) {
    if (*size < sizeof(double)) return 0.0;
    
    double val;
    memcpy(&val, *data, sizeof(double));
    *data += sizeof(double);
    *size -= sizeof(double);
    
    return val;
}

static int extract_int(const uint8_t** data, size_t* size) {
    if (*size < sizeof(int)) return 0;
    
    int val;
    memcpy(&val, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    
    return val;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    cmsHANDLE hIT8 = create_test_it8();
    if (!hIT8) return 0;
    
    const uint8_t* ptr = Data;
    size_t remaining = Size;
    
    while (remaining > 0) {
        uint8_t op = ptr[0] % 6;
        ptr++;
        remaining--;
        
        switch (op) {
            case 0: {
                const char* sample = extract_string(&ptr, &remaining, 50);
                cmsIT8SetIndexColumn(hIT8, sample);
                break;
            }
            case 1: {
                int row = extract_int(&ptr, &remaining) % 10;
                int col = extract_int(&ptr, &remaining) % 7;
                double val = extract_double(&ptr, &remaining);
                cmsIT8SetDataRowColDbl(hIT8, row, col, val);
                break;
            }
            case 2: {
                int row = extract_int(&ptr, &remaining) % 10;
                int col = extract_int(&ptr, &remaining) % 7;
                cmsIT8GetDataRowColDbl(hIT8, row, col);
                break;
            }
            case 3: {
                const char* patch = extract_string(&ptr, &remaining, 50);
                const char* sample = extract_string(&ptr, &remaining, 50);
                double val = extract_double(&ptr, &remaining);
                cmsIT8SetDataDbl(hIT8, patch, sample, val);
                break;
            }
            case 4: {
                int row = extract_int(&ptr, &remaining) % 10;
                int col = extract_int(&ptr, &remaining) % 7;
                const char* val = extract_string(&ptr, &remaining, 50);
                cmsIT8SetDataRowCol(hIT8, row, col, val);
                break;
            }
            case 5: {
                const char* prop = extract_string(&ptr, &remaining, 50);
                double val = extract_double(&ptr, &remaining);
                cmsIT8SetPropertyDbl(hIT8, prop, val);
                break;
            }
        }
    }
    
    cmsIT8Free(hIT8);
    return 0;
}