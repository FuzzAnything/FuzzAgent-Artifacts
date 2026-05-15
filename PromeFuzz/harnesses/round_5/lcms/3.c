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
#include <stddef.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a profile from the fuzzer input
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    // Get tag count
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    
    // If tagCount is valid (non-negative), iterate through tags
    if (tagCount >= 0) {
        // Limit the iteration to avoid excessive loops from corrupted data
        cmsUInt32Number maxIterations = (tagCount < 1000) ? (cmsUInt32Number)tagCount : 1000;
        
        for (cmsUInt32Number i = 0; i < maxIterations; i++) {
            // Get tag signature
            cmsTagSignature sig = cmsGetTagSignature(hProfile, i);
            
            // Read the tag if signature is non-zero
            if (sig != 0) {
                void* tagData = cmsReadTag(hProfile, sig);
                // We don't need to do anything with tagData, just ensure the call doesn't crash
                (void)tagData;
            }
        }
    }

    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}