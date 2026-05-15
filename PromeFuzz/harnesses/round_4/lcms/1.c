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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Create a dummy file and write the fuzzer data to it
    const char* filename = "./dummy_file";
    FILE* file = fopen(filename, "wb");
    if (!file) return 0;
    fwrite(Data, 1, Size, file);
    fclose(file);

    // Open the profile from the dummy file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile(filename, "r");
    if (!hProfile) {
        remove(filename);
        return 0;
    }

    // Get tag count
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    
    // Only proceed if we have valid tags
    if (tagCount > 0) {
        // Limit the number of tags we process to avoid timeouts
        const cmsUInt32Number maxTagsToProcess = 100;
        cmsUInt32Number tagsToProcess = (cmsUInt32Number)tagCount;
        if (tagsToProcess > maxTagsToProcess) {
            tagsToProcess = maxTagsToProcess;
        }

        // Iterate through tags
        for (cmsUInt32Number i = 0; i < tagsToProcess; i++) {
            // Get tag signature
            cmsTagSignature sig = cmsGetTagSignature(hProfile, i);
            if (sig == 0) continue;

            // First call to get required buffer size
            cmsUInt32Number requiredSize = cmsReadRawTag(hProfile, sig, NULL, 0);
            
            if (requiredSize > 0 && requiredSize < 10 * 1024 * 1024) { // Limit to 10MB
                // Allocate buffer and read tag data
                void* buffer = malloc(requiredSize);
                if (buffer) {
                    cmsUInt32Number readSize = cmsReadRawTag(hProfile, sig, buffer, requiredSize);
                    // Use the buffer to avoid optimization
                    if (readSize > 0) {
                        volatile char dummy = *((char*)buffer);
                        (void)dummy;
                    }
                    free(buffer);
                }
            }
        }
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    remove(filename);
    
    return 0;
}