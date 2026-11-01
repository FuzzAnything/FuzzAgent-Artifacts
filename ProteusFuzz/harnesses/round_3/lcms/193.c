// Source: Fuzz Driver for lcms2 ParseCube
// Target: ParseCube in cmscgats.c
// Goal: Cover lines 3094-3203 by constructing valid and invalid CUBE file structures.

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static int write_dummy_file(const char *filename, const uint8_t *Data, size_t Size) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;

    // Construct a minimal CUBE file structure to trigger specific parsing paths.
    // We inject a header to set up the parser state, then append raw data.
    
    // 1. Title (Triggers lines 3094-3099)
    fprintf(fp, "TITLE \"TestCube\"\n");

    // 2. Domain Min (Triggers lines 3103-3106)
    fprintf(fp, "DOMAIN_MIN 0 0 0\n");

    // 3. Domain Max (Triggers lines 3108-3111)
    fprintf(fp, "DOMAIN_MAX 1 1 1\n");

    // 4. LUT1D Size (Triggers lines 3114-3119)
    // Set a small size (e.g., 2) to trigger shaper reading logic if data is present
    fprintf(fp, "LUT1D_SIZE 2\n");

    // 5. LUT3D Size (Triggers lines 3122-3127)
    // Set a small size (e.g., 2) to trigger CLUT reading logic
    fprintf(fp, "LUT3D_SIZE 2\n");

    // 6. Input Range (Triggers lines 3130-3137)
    fprintf(fp, "LUT3D_INPUT_RANGE 0 1.0\n");

    // 7. Inject raw data. 
    // The parser expects numbers (SINUM/SDNUM) here for the LUT data.
    // We write the raw input data as text. The parser will try to read numbers.
    // If the data is malformed, it might trigger error paths or just read what it can.
    
    // Write raw data as a block of text. 
    size_t written = 0;
    for (size_t i = 0; i < Size; i++) {
        if (Data[i] == 0) continue; // Skip nulls for text file
        fputc((char)Data[i], fp);
        written++;
        if (written > 4096) break; // Cap size
    }
    
    // Ensure we have a newline at the end
    fputc('\n', fp);

    fclose(fp);
    return 1;
}

static int write_error_case_file(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;

    // This case specifically targets lines 3144-3146 (Unsupported format)
    // by including LUT_IN_VIDEO_RANGE or LUT_OUT_VIDEO_RANGE.
    fprintf(fp, "TITLE \"ErrorTest\"\n");
    fprintf(fp, "DOMAIN_MIN 0 0 0\n");
    fprintf(fp, "DOMAIN_MAX 1 1 1\n");
    
    // Trigger unsupported format error
    fprintf(fp, "LUT_IN_VIDEO_RANGE\n");
    
    // Also try LUT_OUT_VIDEO_RANGE
    fprintf(fp, "LUT_OUT_VIDEO_RANGE\n");

    fclose(fp);
    return 1;
}

int LLVMFuzzerTestOneInput_193(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *dummy_file = "./dummy_file";
    cmsHPROFILE profile = NULL;

    // Test Case 1: Normal parsing with raw data injection
    // This aims to cover lines 3094-3137 and the data reading loops (3152-3201)
    if (write_dummy_file(dummy_file, Data, Size)) {
        profile = cmsCreateDeviceLinkFromCubeFile(dummy_file);
        cleanup_profile(profile);
        profile = NULL;
    }

    // Test Case 2: Error path for unsupported keywords
    // This aims to cover lines 3144-3146
    if (write_error_case_file(dummy_file)) {
        profile = cmsCreateDeviceLinkFromCubeFile(dummy_file);
        cleanup_profile(profile);
        profile = NULL;
    }

    // Cleanup
    remove(dummy_file);

    return 0;
}