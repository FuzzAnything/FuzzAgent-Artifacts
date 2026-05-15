// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8GetDataDbl at cmscgats.c:2883:28 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8GetPropertyDbl at cmscgats.c:1591:28 in lcms2.h
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

static char* read_string(const uint8_t** data, size_t* size) {
    if (*size == 0) return NULL;
    
    size_t len = 0;
    while (len < *size && (*data)[len] != 0) {
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

int LLVMFuzzerTestOneInput_81(const uint8_t* Data, size_t Size) {
    if (Size < 8) return 0;
    
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    const uint8_t* ptr = Data;
    size_t remaining = Size;
    
    while (remaining > 0) {
        char* prop = read_string(&ptr, &remaining);
        if (!prop) break;
        
        char* patch = read_string(&ptr, &remaining);
        char* sample = read_string(&ptr, &remaining);
        
        if (remaining >= sizeof(cmsFloat64Number)) {
            cmsFloat64Number val;
            memcpy(&val, ptr, sizeof(cmsFloat64Number));
            ptr += sizeof(cmsFloat64Number);
            remaining -= sizeof(cmsFloat64Number);
            
            cmsIT8GetProperty(hIT8, prop);
            cmsIT8DefineDblFormat(hIT8, prop);
            
            if (patch && sample) {
                cmsIT8GetDataDbl(hIT8, patch, sample);
                cmsIT8SetDataDbl(hIT8, patch, sample, val);
            }
            
            cmsIT8SetPropertyDbl(hIT8, prop, val);
            cmsIT8GetPropertyDbl(hIT8, prop);
        }
        
        free(prop);
        free(patch);
        free(sample);
    }
    
    cmsIT8Free(hIT8);
    return 0;
}