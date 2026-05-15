// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_error_handler(cmsContext context, cmsUInt32Number error_code, const char *text) {
    // Silently ignore errors during fuzzing
    (void)context;
    (void)error_code;
    (void)text;
}

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    // Try to open the input as a profile first
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    
    // If that fails, create a synthetic profile for testing
    if (hProfile == NULL) {
        hProfile = cmsCreate_sRGBProfile();
    }
    
    return hProfile;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    // Set error handler to avoid error messages during fuzzing
    cmsSetLogErrorHandler(dummy_error_handler);
    
    // Create or open a profile for testing
    cmsHPROFILE hProfile = create_test_profile(Data, Size);
    if (hProfile == NULL) {
        return 0;
    }
    
    // Get tag count to ensure we don't access out of bounds
    cmsUInt32Number tag_count = cmsGetTagCount(hProfile);
    
    // Extract some values from fuzzer input for testing
    // Use first 4 bytes for tag signature, next 4 for index, next 4 for buffer size
    cmsTagSignature sig = *(cmsTagSignature*)Data;
    cmsUInt32Number index = 0;
    cmsUInt32Number buffer_size = 0;
    
    if (Size >= 8) {
        index = *(cmsUInt32Number*)(Data + 4);
    }
    
    if (Size >= 12) {
        buffer_size = *(cmsUInt32Number*)(Data + 8);
    }
    
    // Limit index to valid range
    if (tag_count > 0) {
        index = index % tag_count;
    }
    
    // Test cmsIsTag
    (void)cmsIsTag(hProfile, sig);
    
    // Test cmsGetTagSignature
    if (tag_count > 0) {
        (void)cmsGetTagSignature(hProfile, index);
    }
    
    // Test cmsReadTag
    void *tag_data = cmsReadTag(hProfile, sig);
    (void)tag_data;
    
    // Test cmsReadRawTag
    // First call with NULL buffer to get required size
    cmsUInt32Number required_size = cmsReadRawTag(hProfile, sig, NULL, 0);
    
    // Then try to read with actual buffer if we have enough data
    if (required_size > 0 && required_size <= 1024 * 1024) { // Limit to 1MB
        void *buffer = malloc(required_size);
        if (buffer) {
            (void)cmsReadRawTag(hProfile, sig, buffer, required_size);
            free(buffer);
        }
    }
    
    // Also test with a small buffer from fuzzer input
    if (buffer_size > 0 && buffer_size <= 1024) {
        uint8_t small_buffer[1024];
        if (buffer_size <= sizeof(small_buffer)) {
            (void)cmsReadRawTag(hProfile, sig, small_buffer, buffer_size);
        }
    }
    
    // Test cmsLinkTag - try to link to a different tag
    if (tag_count > 1) {
        cmsTagSignature dest_sig = cmsGetTagSignature(hProfile, (index + 1) % tag_count);
        (void)cmsLinkTag(hProfile, sig, dest_sig);
    }
    
    // Test cmsTagLinkedTo
    (void)cmsTagLinkedTo(hProfile, sig);
    
    // Cleanup
    cmsCloseProfile(hProfile);
    
    return 0;
}