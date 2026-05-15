// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsUInt32Number get_uint32(const uint8_t* data, size_t size, size_t offset, cmsUInt32Number default_val) {
    if (offset + sizeof(cmsUInt32Number) <= size) {
        cmsUInt32Number val;
        memcpy(&val, data + offset, sizeof(cmsUInt32Number));
        return val;
    }
    return default_val;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // Create a profile from fuzzer input
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    
    // If profile creation failed, use NULL to test error handling
    if (!hProfile && Size > 0) {
        // Try to create a dummy file and open from file
        FILE* fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
            hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
            remove("./dummy_file");
        }
    }
    
    // Extract parameters from fuzzer input with bounds checking
    size_t offset = 0;
    cmsUInt32Number intent = get_uint32(Data, Size, offset, 0);
    offset += sizeof(cmsUInt32Number);
    cmsUInt32Number flags = get_uint32(Data, Size, offset, 0);
    offset += sizeof(cmsUInt32Number);
    cmsUInt32Number direction = get_uint32(Data, Size, offset, 0);
    
    // Test cmsIsMatrixShaper
    if (hProfile) {
        cmsIsMatrixShaper(hProfile);
    }
    
    // Test cmsDetectBlackPoint
    cmsCIEXYZ blackPoint;
    if (hProfile) {
        cmsDetectBlackPoint(&blackPoint, hProfile, intent, flags);
    }
    
    // Test cmsGetPostScriptCSA
    if (hProfile) {
        // First call to get required buffer size
        cmsUInt32Number bufferSize = cmsGetPostScriptCSA(NULL, hProfile, intent, flags, NULL, 0);
        
        if (bufferSize > 0 && bufferSize < 1024 * 1024) { // Reasonable size limit
            void* buffer = malloc(bufferSize);
            if (buffer) {
                cmsGetPostScriptCSA(NULL, hProfile, intent, flags, buffer, bufferSize);
                free(buffer);
            }
        }
    }
    
    // Test cmsIsCLUT
    if (hProfile) {
        cmsIsCLUT(hProfile, intent, direction);
    }
    
    // Test cmsDetectDestinationBlackPoint
    if (hProfile) {
        cmsDetectDestinationBlackPoint(&blackPoint, hProfile, intent, flags);
    }
    
    // Test cmsIsIntentSupported
    if (hProfile) {
        cmsIsIntentSupported(hProfile, intent, direction);
    }
    
    // Clean up
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    return 0;
}