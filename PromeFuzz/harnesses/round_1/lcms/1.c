// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Create a dummy file with fuzzer data
    FILE *file = fopen("./dummy_file", "wb");
    if (!file) return 0;
    fwrite(Data, 1, Size, file);
    fclose(file);

    // Open the profile from the dummy file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hProfile) {
        remove("./dummy_file");
        return 0;
    }

    // 1. Get tag count
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    
    // Only proceed if we have valid tags
    if (tagCount > 0) {
        // Iterate through available tags
        for (cmsUInt32Number i = 0; i < (cmsUInt32Number)tagCount; i++) {
            // 2. Get tag signature for index i
            cmsTagSignature sig = cmsGetTagSignature(hProfile, i);
            
            // Skip if signature is 0 (invalid index)
            if (sig == 0) continue;
            
            // 3. Read the tag
            void *tagData = cmsReadTag(hProfile, sig);
            
            // Optionally explore different program states by varying parameters
            // Try reading with a modified signature (fuzzed variation)
            if (Size > 0) {
                cmsTagSignature modifiedSig = sig ^ (Data[0] << 24);
                void *modifiedTagData = cmsReadTag(hProfile, modifiedSig);
                // No need to use the result
            }
        }
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    remove("./dummy_file");
    
    return 0;
}