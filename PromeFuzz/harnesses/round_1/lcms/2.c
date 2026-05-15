// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer input data
    const char* filename = "./dummy_file";
    FILE* file = fopen(filename, "wb");
    if (!file) {
        return 0;
    }
    fwrite(Data, 1, Size, file);
    fclose(file);
    
    // Open the ICC profile from the dummy file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile(filename, "r");
    if (!hProfile) {
        // Clean up the dummy file
        remove(filename);
        return 0;
    }
    
    // First API: cmsGetTagCount
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    
    // Only proceed if we have valid tags
    if (tagCount > 0) {
        // Iterate through tags (limit to reasonable number for fuzzing)
        cmsUInt32Number maxTags = tagCount > 100 ? 100 : (cmsUInt32Number)tagCount;
        
        for (cmsUInt32Number i = 0; i < maxTags; i++) {
            // Second API: cmsGetTagSignature
            cmsTagSignature sig = cmsGetTagSignature(hProfile, i);
            
            if (sig != 0) {
                // Third API: cmsReadRawTag - first call to get required size
                cmsUInt32Number requiredSize = cmsReadRawTag(hProfile, sig, NULL, 0);
                
                if (requiredSize > 0 && requiredSize < 10*1024*1024) { // Limit to 10MB
                    // Allocate buffer and read the tag data
                    void* buffer = malloc(requiredSize);
                    if (buffer) {
                        cmsUInt32Number copiedSize = cmsReadRawTag(hProfile, sig, buffer, requiredSize);
                        // Use the data to avoid optimization
                        if (copiedSize > 0) {
                            volatile uint8_t dummy = 0;
                            for (cmsUInt32Number j = 0; j < (copiedSize > 1024 ? 1024 : copiedSize); j++) {
                                dummy ^= ((uint8_t*)buffer)[j];
                            }
                        }
                        free(buffer);
                    }
                }
            }
        }
    }
    
    // Cleanup
    cmsCloseProfile(hProfile);
    remove(filename);
    
    return 0;
}