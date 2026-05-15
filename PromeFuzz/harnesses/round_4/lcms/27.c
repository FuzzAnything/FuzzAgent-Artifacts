// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsHPROFILE create_test_profile(void) {
    // Create a simple sRGB profile for testing
    return cmsCreate_sRGBProfile();
}

static void* get_fuzz_data(const uint8_t *Data, size_t Size, size_t *out_size) {
    if (Size == 0) {
        *out_size = 0;
        return NULL;
    }
    
    // Use first 4 bytes as size hint, but limit to actual available data
    size_t data_size = Size;
    if (Size >= 4) {
        data_size = *(const uint32_t*)Data % (Size + 1);
        if (data_size == 0) data_size = 1;
    }
    
    // Allocate and copy data
    void* result = malloc(data_size);
    if (result) {
        memcpy(result, Data, data_size > Size ? Size : data_size);
        *out_size = data_size;
    } else {
        *out_size = 0;
    }
    
    return result;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create test profile
    cmsHPROFILE hProfile = create_test_profile();
    if (!hProfile) return 0;
    
    // Use fuzzer data to determine tag signatures
    cmsTagSignature tag1 = *(const cmsTagSignature*)Data;
    cmsTagSignature tag2 = *(const cmsTagSignature*)(Data + 4 % Size);
    
    // Test cmsWriteTag with random data
    size_t write_data_size;
    void* write_data = get_fuzz_data(Data, Size, &write_data_size);
    if (write_data) {
        cmsWriteTag(hProfile, tag1, write_data);
        free(write_data);
    }
    
    // Test cmsWriteTag with NULL (delete)
    cmsWriteTag(hProfile, tag1, NULL);
    
    // Test cmsIsTag
    cmsIsTag(hProfile, tag1);
    cmsIsTag(hProfile, tag2);
    
    // Test cmsWriteRawTag
    size_t raw_data_size;
    void* raw_data = get_fuzz_data(Data, Size, &raw_data_size);
    if (raw_data) {
        cmsWriteRawTag(hProfile, tag2, raw_data, (cmsUInt32Number)raw_data_size);
        free(raw_data);
    }
    
    // Test cmsMD5computeID
    cmsMD5computeID(hProfile);
    
    // Test cmsSaveProfileToMem
    cmsUInt32Number bytes_needed = 0;
    cmsSaveProfileToMem(hProfile, NULL, &bytes_needed);
    
    if (bytes_needed > 0 && bytes_needed < 1024 * 1024) { // Limit to 1MB
        void* buffer = malloc(bytes_needed);
        if (buffer) {
            cmsSaveProfileToMem(hProfile, buffer, &bytes_needed);
            free(buffer);
        }
    }
    
    // Test cmsLinkTag
    cmsLinkTag(hProfile, tag1, tag2);
    
    // Cleanup
    cmsCloseProfile(hProfile);
    
    return 0;
}