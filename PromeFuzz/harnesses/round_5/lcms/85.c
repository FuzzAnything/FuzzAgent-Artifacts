// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8GetDataRowColDbl at cmscgats.c:2826:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lcms2.h"

static cmsHANDLE create_dummy_it8(void) {
    // Create a dummy IT8 handle for testing
    cmsHANDLE hIT8 = cmsIT8Alloc(0);
    if (!hIT8) return NULL;
    
    // Set some basic properties to make it usable
    cmsIT8SetPropertyStr(hIT8, "ORIGINATOR", "Fuzzer");
    cmsIT8SetPropertyStr(hIT8, "DESCRIPTOR", "Test IT8");
    cmsIT8SetPropertyStr(hIT8, "MANUFACTURER", "FuzzLab");
    
    return hIT8;
}

static void cleanup_it8(cmsHANDLE hIT8) {
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsHANDLE hIT8 = create_dummy_it8();
    if (!hIT8) return 0;
    
    // Use the input data to generate parameters
    // We'll use simple modulo operations to get reasonable values
    int col = Data[0] % 10;
    int row = (Size > 1) ? Data[1] % 10 : 0;
    
    // Extract a double value from the data if possible
    cmsFloat64Number dbl_val = 0.0;
    if (Size >= sizeof(cmsFloat64Number) + 2) {
        memcpy(&dbl_val, Data + 2, sizeof(cmsFloat64Number));
    } else if (Size > 2) {
        // Use a simple conversion if we don't have enough bytes
        dbl_val = (cmsFloat64Number)Data[2];
    }
    
    // Create null-terminated strings from the data
    char* sample_str = NULL;
    char* val_str = NULL;
    char* prop_str = NULL;
    char* patch_str = NULL;
    
    if (Size > 0) {
        size_t str_len = Size > 100 ? 100 : Size;
        
        sample_str = (char*)malloc(str_len + 1);
        if (sample_str) {
            memcpy(sample_str, Data, str_len);
            sample_str[str_len] = '\0';
        }
        
        val_str = (char*)malloc(str_len + 1);
        if (val_str) {
            memcpy(val_str, Data, str_len);
            val_str[str_len] = '\0';
        }
        
        prop_str = (char*)malloc(str_len + 1);
        if (prop_str) {
            memcpy(prop_str, Data, str_len);
            prop_str[str_len] = '\0';
        }
        
        patch_str = (char*)malloc(str_len + 1);
        if (patch_str) {
            memcpy(patch_str, Data, str_len);
            patch_str[str_len] = '\0';
        }
    }
    
    // Test cmsIT8SetDataFormat
    if (sample_str) {
        cmsIT8SetDataFormat(hIT8, col, sample_str);
    }
    
    // Test cmsIT8SetDataRowCol
    if (val_str) {
        cmsIT8SetDataRowCol(hIT8, row, col, val_str);
    }
    
    // Test cmsIT8GetDataRowColDbl
    cmsFloat64Number retrieved_val = cmsIT8GetDataRowColDbl(hIT8, row, col);
    (void)retrieved_val; // Use the value to avoid compiler warnings
    
    // Test cmsIT8SetDataRowColDbl
    cmsIT8SetDataRowColDbl(hIT8, row, col, dbl_val);
    
    // Test cmsIT8SetPropertyDbl
    if (prop_str) {
        cmsIT8SetPropertyDbl(hIT8, prop_str, dbl_val);
    }
    
    // Test cmsIT8SetDataDbl
    if (sample_str && patch_str) {
        cmsIT8SetDataDbl(hIT8, patch_str, sample_str, dbl_val);
    }
    
    // Cleanup
    if (sample_str) free(sample_str);
    if (val_str) free(val_str);
    if (prop_str) free(prop_str);
    if (patch_str) free(patch_str);
    
    cleanup_it8(hIT8);
    
    return 0;
}