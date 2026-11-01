#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <lcms2.h>

static void write_cube_file(const char* filename, const uint8_t* data, size_t size) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;

    // Construct a .cube file with specific keywords to trigger uncovered switch cases
    // STITLE (3094), SDOMAIN_MIN (3103), SDOMAIN_MAX (3108)
    fprintf(f, "TITLE \"FuzzTest\"\n");
    fprintf(f, "DOMAIN_MIN 0 0 0\n");
    fprintf(f, "DOMAIN_MAX 1 1 1\n");

    // S_LUT1D_SIZE (3114) -> triggers shaper logic (3152, 3156, 3158, 3162, 3169, 3173)
    fprintf(f, "LUT1D_SIZE 1\n");

    // S_LUT3D_SIZE (3122) -> triggers CLUT logic (3181, 3186, 3188, 3192)
    fprintf(f, "LUT3D_SIZE 1\n");

    // S_LUT1D_INPUT_RANGE / S_LUT3D_INPUT_RANGE (3130, 3131)
    fprintf(f, "LUT1D_INPUT_RANGE 0 1\n");
    fprintf(f, "LUT3D_INPUT_RANGE 0 1\n");

    // S_LUT_IN_VIDEO_RANGE (3144) -> triggers error path (3146)
    fprintf(f, "LUT_IN_VIDEO_RANGE\n");

    // Write fuzzed data as the numeric payload. 
    // Even if parsing fails, the allocation and loop setup lines are executed.
    fwrite(data, 1, size, f);

    // Ensure a newline at the end to help parser
    fprintf(f, "\n");

    fclose(f);
}

int LLVMFuzzerTestOneInput_183(const uint8_t *Data, size_t Size) {
    const char* dummy_file = "./dummy_file";
    
    // Fix: cmsContext is a typedef for a pointer (struct _cmsContext_struct *).
    // Declaring it as 'cmsContext *' creates a double pointer, causing type mismatches.
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    // Generate the cube file with specific keywords to hit uncovered lines
    write_cube_file(dummy_file, Data, Size);

    // Invoke the API that calls ParseCube
    cmsHPROFILE hProfile = cmsCreateDeviceLinkFromCubeFileTHR(ContextID, dummy_file);

    // Cleanup
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    cmsDeleteContext(ContextID);
    unlink(dummy_file);

    return 0;
}