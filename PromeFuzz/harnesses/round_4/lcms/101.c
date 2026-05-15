// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8GetDataDbl at cmscgats.c:2883:28 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetPropertyDbl at cmscgats.c:1591:28 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"

static const char* extract_string(const uint8_t** data, size_t* size) {
    if (*size == 0) return NULL;
    
    size_t max_len = *size;
    size_t len = 0;
    
    // Find null terminator or use entire remaining data
    while (len < max_len && (*data)[len] != 0) {
        len++;
    }
    
    char* str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, *data, len);
    str[len] = '\0';
    
    *data += len;
    *size -= len;
    
    // Skip the null terminator if present
    if (len < max_len && (*data)[0] == 0) {
        (*data)++;
        (*size)--;
    }
    
    return str;
}

static double extract_double(const uint8_t** data, size_t* size) {
    if (*size < sizeof(double)) return 0.0;
    
    double value;
    memcpy(&value, *data, sizeof(double));
    
    *data += sizeof(double);
    *size -= sizeof(double);
    
    return value;
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    const uint8_t* data_ptr = Data;
    size_t remaining = Size;
    
    // Create a dummy IT8 handle - in real usage this would come from cmsIT8Alloc or cmsIT8LoadFromFile
    // For fuzzing, we'll create a minimal valid handle by loading from a file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    // Write minimal IT8 data to file
    const char* minimal_it8 = "CGATS.17\n\nBEGIN_DATA_FORMAT\nSAMPLE_ID\nEND_DATA_FORMAT\n\nBEGIN_DATA\n1\nEND_DATA\n";
    fwrite(minimal_it8, 1, strlen(minimal_it8), fp);
    fclose(fp);
    
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, "./dummy_file");
    if (!hIT8) {
        remove("./dummy_file");
        return 0;
    }
    
    // Extract test parameters from fuzzer input
    const char* prop_name = extract_string(&data_ptr, &remaining);
    const char* patch_name = extract_string(&data_ptr, &remaining);
    const char* sample_name = extract_string(&data_ptr, &remaining);
    double double_val = extract_double(&data_ptr, &remaining);
    
    // Test cmsIT8GetProperty
    if (prop_name) {
        const char* prop_value = cmsIT8GetProperty(hIT8, prop_name);
        (void)prop_value; // Use result to avoid compiler warning
    }
    
    // Test cmsIT8GetPropertyDbl
    if (prop_name) {
        cmsFloat64Number prop_dbl = cmsIT8GetPropertyDbl(hIT8, prop_name);
        (void)prop_dbl;
    }
    
    // Test cmsIT8GetData
    if (patch_name && sample_name) {
        const char* data_value = cmsIT8GetData(hIT8, patch_name, sample_name);
        (void)data_value;
    }
    
    // Test cmsIT8SetDataDbl
    if (patch_name && sample_name) {
        cmsBool set_result = cmsIT8SetDataDbl(hIT8, patch_name, sample_name, double_val);
        (void)set_result;
    }
    
    // Test cmsIT8SetPropertyDbl
    if (prop_name) {
        cmsBool set_prop_result = cmsIT8SetPropertyDbl(hIT8, prop_name, double_val);
        (void)set_prop_result;
    }
    
    // Test cmsIT8GetDataDbl
    if (patch_name && sample_name) {
        cmsFloat64Number data_dbl = cmsIT8GetDataDbl(hIT8, patch_name, sample_name);
        (void)data_dbl;
    }
    
    // Cleanup
    if (prop_name) free((void*)prop_name);
    if (patch_name) free((void*)patch_name);
    if (sample_name) free((void*)sample_name);
    
    cmsIT8Free(hIT8);
    remove("./dummy_file");
    
    return 0;
}