// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Create a dummy ICC profile from fuzzer input
    if (Size == 0) {
        return 0;
    }
    
    // Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (hProfile == NULL) {
        return 0;
    }
    
    // Get tag count
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    
    // Only proceed if we have valid tags
    if (tagCount > 0) {
        // Iterate through all tags
        for (cmsUInt32Number i = 0; i < (cmsUInt32Number)tagCount; i++) {
            // Get tag signature
            cmsTagSignature sig = cmsGetTagSignature(hProfile, i);
            
            // Read tag data
            void* tagData = cmsReadTag(hProfile, sig);
            
            // We don't need to do anything with the tag data,
            // just calling the function exercises the code
            (void)tagData;
        }
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}