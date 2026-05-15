// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8GetPropertyDbl at cmscgats.c:1591:28 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8GetDataDbl at cmscgats.c:2883:28 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void extract_string(const uint8_t *data, size_t size, size_t offset, char *buf, size_t buf_len) {
    if (buf_len == 0) return;
    size_t i;
    for (i = 0; i < buf_len - 1 && offset + i < size; i++) {
        buf[i] = data[offset + i];
    }
    buf[i] = '\0';
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    cmsHANDLE hIT8 = NULL;
    
    // Only try to load IT8 from memory if we have data
    if (Size > 0) {
        hIT8 = cmsIT8LoadFromMem(NULL, Data, (cmsUInt32Number)Size);
    }
    
    // If loading fails or no data, create a new IT8 handle
    if (hIT8 == NULL) {
        hIT8 = cmsIT8Alloc(NULL);
        if (hIT8 == NULL) {
            return 0;
        }
    }
    
    // Extract strings from fuzzer input for various parameters
    char patch[16], sample[16], property[16], subkey[16];
    extract_string(Data, Size, 0, patch, sizeof(patch));
    extract_string(Data, Size, 16, sample, sizeof(sample));
    extract_string(Data, Size, 32, property, sizeof(property));
    extract_string(Data, Size, 48, subkey, sizeof(subkey));
    
    // Extract double value from fuzzer input if available
    double val = 0.0;
    if (Size >= 64 + sizeof(double)) {
        memcpy(&val, Data + 64, sizeof(double));
    }
    
    // Test cmsIT8SetPropertyDbl
    cmsIT8SetPropertyDbl(hIT8, property, val);
    
    // Test cmsIT8GetPropertyDbl
    cmsIT8GetPropertyDbl(hIT8, property);
    
    // Test cmsIT8SetDataDbl
    cmsIT8SetDataDbl(hIT8, patch, sample, val);
    
    // Test cmsIT8GetDataDbl
    cmsIT8GetDataDbl(hIT8, patch, sample);
    
    // Test cmsIT8GetData
    cmsIT8GetData(hIT8, patch, sample);
    
    // Test cmsIT8GetPropertyMulti
    cmsIT8GetPropertyMulti(hIT8, property, subkey);
    
    // Clean up
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
    
    return 0;
}