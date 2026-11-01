// Fuzz driver for lcms ParseCube function
// Target: Uncovered lines in ParseCube (cmscgats.c:3077-3208)
// Method: Generate CUBE files from fuzz input and parse via cmsCreateDeviceLinkFromCubeFileTHR

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f != NULL) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size) {
    // Safety check for empty input
    if (Size == 0) {
        return 0;
    }

    const char *dummy_file = "./dummy_file";
    
    // Write fuzz input to a temporary file
    write_dummy_file(dummy_file, Data, Size);

    // Create a context for thread-safe operations
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        remove(dummy_file);
        return 0;
    }

    // Invoke the API that calls ParseCube
    // cmsCreateDeviceLinkFromCubeFileTHR -> ParseCube
    cmsHPROFILE profile = cmsCreateDeviceLinkFromCubeFileTHR(ContextID, dummy_file);

    // Cleanup profile if created
    cleanup_profile(profile);

    // Cleanup context
    cmsDeleteContext(ContextID);

    // Remove the temporary file
    remove(dummy_file);

    return 0;
}