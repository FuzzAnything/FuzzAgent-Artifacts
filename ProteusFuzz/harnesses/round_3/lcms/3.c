// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a dummy file for profile loading
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f != NULL) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    } else {
        return 0;
    }

    // Attempt to open the profile from the dummy file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile(dummy_file, "r");
    if (hProfile == NULL) {
        return 0;
    }

    // Step 1: Get tag count
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    if (tagCount < 0) {
        cleanup_profile(hProfile);
        return 0;
    }

    // Step 2 & 3: Iterate over tags to get signature and read raw data
    cmsUInt32Number i;
    for (i = 0; i < (cmsUInt32Number)tagCount; i++) {
        cmsTagSignature sig = cmsGetTagSignature(hProfile, i);
        if (sig == 0) {
            continue; // Skip invalid tags
        }

        // Allocate buffer for raw tag data
        cmsUInt32Number bufferSize = 4096; // Reasonable max size for fuzzing
        uint8_t *buffer = (uint8_t *)malloc(bufferSize);
        if (buffer == NULL) {
            cleanup_profile(hProfile);
            return 0;
        }

        cmsUInt32Number bytesRead = cmsReadRawTag(hProfile, sig, buffer, bufferSize);
        (void)bytesRead; // Use the return value to ensure it's compiled in

        free(buffer);
    }

    // Cleanup
    cleanup_profile(hProfile);
    remove(dummy_file);

    return 0;
}