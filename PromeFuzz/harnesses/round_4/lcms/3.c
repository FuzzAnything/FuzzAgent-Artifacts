// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    // Create a dummy file with the fuzzer input
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open profile from the dummy file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile(filename, "r");
    if (hProfile == NULL) {
        // Try opening from memory as alternative path
        hProfile = cmsOpenProfileFromMem(Data, Size);
        if (hProfile == NULL) {
            return 0;
        }
    }
    
    // Call cmsGetTagCount
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    
    // If tagCount is valid (>= 0), iterate through tags
    if (tagCount > 0) {
        // Use fuzzer data to determine how many tags to process
        size_t maxTags = tagCount;
        if (Size >= 8) {
            uint32_t fuzzIndex = *(uint32_t*)Data;
            maxTags = (fuzzIndex % tagCount) + 1;
            if (maxTags > 100) maxTags = 100; // Limit to reasonable number
        }
        
        for (cmsUInt32Number i = 0; i < maxTags && i < (cmsUInt32Number)tagCount; i++) {
            // Call cmsGetTagSignature
            cmsTagSignature sig = cmsGetTagSignature(hProfile, i);
            
            if (sig != 0) {
                // Call cmsReadTag
                void* tagData = cmsReadTag(hProfile, sig);
                // We don't need to do anything with tagData, just ensure function doesn't crash
                (void)tagData;
            }
        }
    }
    
    // Additional fuzzing: try reading specific signatures from fuzzer data
    if (Size >= 12) {
        cmsTagSignature testSig = *(cmsTagSignature*)(Data + 4);
        void* tagData = cmsReadTag(hProfile, testSig);
        (void)tagData;
    }
    
    // Cleanup
    cleanup_profile(hProfile);
    
    return 0;
}