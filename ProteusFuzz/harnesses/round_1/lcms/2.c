// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    cmsHPROFILE hProfile = NULL;
    
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with the fuzzing data
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Attempt to open the profile
    hProfile = cmsOpenProfileFromFile(dummy_file, "r");
    if (hProfile == NULL) {
        // Profile could not be opened, which is a valid state to test
        return 0;
    }

    // Step 1: Get tag count
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    
    if (tagCount < 0) {
        cleanup_profile(hProfile);
        return 0;
    }

    // Step 2 & 3: Iterate through tags to get signature and read tag
    // We use a safe upper bound to prevent integer overflow issues if tagCount is unexpectedly large
    cmsUInt32Number maxIter = (tagCount > 10000) ? 10000 : (cmsUInt32Number)tagCount;
    
    for (cmsUInt32Number i = 0; i < maxIter; i++) {
        cmsTagSignature sig = cmsGetTagSignature(hProfile, i);
        
        // If signature is 0, it indicates an invalid index or missing tag
        if (sig == 0) {
            break;
        }

        // Attempt to read the tag data
        void *tagData = cmsReadTag(hProfile, sig);
        
        // We don't need to free tagData manually; lcms manages the memory internally
        // The function returns a pointer to internal data or NULL
        (void)tagData;
    }

    cleanup_profile(hProfile);
    
    // Clean up the dummy file
    remove(dummy_file);

    return 0;
}