// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create a dummy profile from the input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) return 0;
    
    // Get tag count
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    if (tagCount < 0) {
        cmsCloseProfile(hProfile);
        return 0;
    }
    
    // Limit the number of tags to process to avoid timeouts
    int maxTags = tagCount < 100 ? tagCount : 100;
    
    for (int i = 0; i < maxTags; i++) {
        // Get tag signature by index
        cmsTagSignature sig = cmsGetTagSignature(hProfile, i);
        if (sig == 0) continue;
        
        // First call to get required buffer size
        cmsUInt32Number requiredSize = cmsReadRawTag(hProfile, sig, NULL, 0);
        if (requiredSize == 0 || requiredSize > 10*1024*1024) continue; // Skip if too large
        
        // Allocate buffer and read raw tag data
        void *buffer = malloc(requiredSize);
        if (!buffer) continue;
        
        cmsUInt32Number copiedSize = cmsReadRawTag(hProfile, sig, buffer, requiredSize);
        
        free(buffer);
    }
    
    cmsCloseProfile(hProfile);
    return 0;
}