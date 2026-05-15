// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8GetPropertyDbl at cmscgats.c:1591:28 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetDataDbl at cmscgats.c:2883:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static const char* extract_string(const uint8_t** data, size_t* size) {
    if (*size == 0) return "";
    
    const uint8_t* start = *data;
    const uint8_t* end = start + *size;
    const uint8_t* ptr = start;
    
    while (ptr < end && *ptr != 0) {
        ptr++;
    }
    
    size_t len = ptr - start;
    static char buf[256];
    size_t copy_len = len < sizeof(buf) - 1 ? len : sizeof(buf) - 1;
    
    if (copy_len > 0) {
        memcpy(buf, start, copy_len);
    }
    buf[copy_len] = '\0';
    
    *data += len;
    *size -= len;
    
    if (ptr < end && *ptr == 0) {
        (*data)++;
        (*size)--;
    }
    
    return buf;
}

static double extract_double(const uint8_t** data, size_t* size) {
    if (*size < sizeof(double)) return 0.0;
    
    double value;
    memcpy(&value, *data, sizeof(double));
    *data += sizeof(double);
    *size -= sizeof(double);
    
    return value;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    const uint8_t* current_data = Data;
    size_t remaining_size = Size;
    
    while (remaining_size > 0) {
        uint8_t op = current_data[0];
        current_data++;
        remaining_size--;
        
        if (remaining_size == 0) break;
        
        switch (op % 6) {
            case 0: {
                const char* prop = extract_string(&current_data, &remaining_size);
                double val = extract_double(&current_data, &remaining_size);
                cmsIT8SetPropertyDbl(hIT8, prop, val);
                break;
            }
            case 1: {
                const char* prop = extract_string(&current_data, &remaining_size);
                cmsIT8GetPropertyDbl(hIT8, prop);
                break;
            }
            case 2: {
                const char* patch = extract_string(&current_data, &remaining_size);
                const char* sample = extract_string(&current_data, &remaining_size);
                double val = extract_double(&current_data, &remaining_size);
                cmsIT8SetDataDbl(hIT8, patch, sample, val);
                break;
            }
            case 3: {
                const char* key = extract_string(&current_data, &remaining_size);
                const char* subkey = extract_string(&current_data, &remaining_size);
                cmsIT8GetPropertyMulti(hIT8, key, subkey);
                break;
            }
            case 4: {
                const char* patch = extract_string(&current_data, &remaining_size);
                const char* sample = extract_string(&current_data, &remaining_size);
                cmsIT8GetDataDbl(hIT8, patch, sample);
                break;
            }
            case 5: {
                const char* patch = extract_string(&current_data, &remaining_size);
                const char* sample = extract_string(&current_data, &remaining_size);
                cmsIT8GetData(hIT8, patch, sample);
                break;
            }
        }
    }
    
    cmsIT8Free(hIT8);
    return 0;
}