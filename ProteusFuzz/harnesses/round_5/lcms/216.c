#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

// Helper to write a dummy cube file based on fuzzer input
static void write_cube_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file.cube", "wb");
    if (!f) return;

    // We construct a minimal valid CUBE file structure to hit all cases.
    // The fuzzer input is used to vary the numeric values to exercise different paths.
    
    // 1. TITLE (Hits lines 3094-3100)
    fprintf(f, "TITLE \"FuzzTest\"\n");

    // 2. DOMAIN_MIN (Hits lines 3103-3106)
    // Use fuzzer data for values if available, else defaults
    double d_min[3] = {0.0, 0.0, 0.0};
    if (Size >= 3) {
        d_min[0] = (double)Data[0] / 255.0;
        d_min[1] = (double)Data[1] / 255.0;
        d_min[2] = (double)Data[2] / 255.0;
    }
    fprintf(f, "DOMAIN_MIN %.6f %.6f %.6f\n", d_min[0], d_min[1], d_min[2]);

    // 3. DOMAIN_MAX (Hits lines 3108-3111)
    double d_max[3] = {1.0, 1.0, 1.0};
    if (Size >= 6) {
        d_max[0] = 1.0 - (double)Data[3] / 255.0;
        d_max[1] = 1.0 - (double)Data[4] / 255.0;
        d_max[2] = 1.0 - (double)Data[5] / 255.0;
    }
    fprintf(f, "DOMAIN_MAX %.6f %.6f %.6f\n", d_max[0], d_max[1], d_max[2]);

    // 4. LUT1D_SIZE (Hits lines 3114-3119)
    // Set a small size (e.g., 2) to trigger shaper logic
    int shaper_size = 2;
    if (Size > 6 && Data[6] > 0) {
        shaper_size = Data[6] % 10 + 1; // Keep it small (1-10)
    }
    fprintf(f, "LUT1D_SIZE %d\n", shaper_size);

    // 5. LUT3D_SIZE (Hits lines 3122-3127)
    // Set a small size (e.g., 2) to trigger CLUT logic
    int lut_size = 2;
    if (Size > 7 && Data[7] > 0) {
        lut_size = Data[7] % 4 + 1; // Keep it very small (1-4) to avoid huge files
    }
    fprintf(f, "LUT3D_SIZE %d\n", lut_size);

    // 6. LUT3D_INPUT_RANGE (Hits lines 3130-3137)
    // Must be 0 1.0 to pass, otherwise hits error path at 3135
    // We force valid 0 1.0 to reach the data parsing logic.
    fprintf(f, "LUT3D_INPUT_RANGE 0 1.0\n");

    // 7. Shaper Data (Hits lines 3152-3179)
    // Need shaper_size * 3 numbers
    for (int i = 0; i < shaper_size; i++) {
        double v1 = (double)((Size > 8 + i*3) ? Data[8 + i*3] : 0) / 255.0;
        double v2 = (double)((Size > 9 + i*3) ? Data[9 + i*3] : 0) / 255.0;
        double v3 = (double)((Size > 10 + i*3) ? Data[10 + i*3] : 0) / 255.0;
        fprintf(f, "%.6f %.6f %.6f\n", v1, v2, v3);
    }

    // 8. CLUT Data (Hits lines 3181-3201)
    // Need lut_size^3 * 3 numbers
    int nodes = lut_size * lut_size * lut_size;
    for (int i = 0; i < nodes; i++) {
        double v1 = (double)((Size > 20 + i*3) ? Data[20 + i*3] : 0) / 255.0;
        double v2 = (double)((Size > 21 + i*3) ? Data[21 + i*3] : 0) / 255.0;
        double v3 = (double)((Size > 22 + i*3) ? Data[22 + i*3] : 0) / 255.0;
        fprintf(f, "%.6f %.6f %.6f\n", v1, v2, v3);
    }

    // 9. EOF Check (Hits line 3203)
    // The parser expects EOF after data. We just close the file.
    
    fclose(f);
}

// Helper to write a file with an unsupported keyword to trigger error paths
static void write_error_cube_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file.cube", "wb");
    if (!f) return;

    fprintf(f, "TITLE \"ErrorTest\"\n");
    fprintf(f, "DOMAIN_MIN 0 0 0\n");
    fprintf(f, "DOMAIN_MAX 1 1 1\n");
    
    // Trigger S_LUT_IN_VIDEO_RANGE (line 3144-3146)
    fprintf(f, "LUT_IN_VIDEO_RANGE\n");
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_216(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Strategy 1: Generate a valid cube file to cover the main parsing logic
    // This covers lines: 3094, 3103, 3108, 3114, 3122, 3130, 3152, 3181, 3203
    write_cube_file(Data, Size);

    cmsHPROFILE hProfile = cmsCreateDeviceLinkFromCubeFile("./dummy_file.cube");
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }

    // Strategy 2: Generate a file with unsupported keywords to cover error paths
    // This covers lines: 3144, 3145, 3146 (S_LUT_IN_VIDEO_RANGE)
    write_error_cube_file(Data, Size);
    
    cmsHPROFILE hProfile2 = cmsCreateDeviceLinkFromCubeFile("./dummy_file.cube");
    if (hProfile2) {
        cmsCloseProfile(hProfile2);
    }

    // Strategy 3: Try with a custom context to ensure THR path is also exercised
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx) {
        write_cube_file(Data, Size);
        cmsHPROFILE hProfile3 = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "./dummy_file.cube");
        if (hProfile3) {
            cmsCloseProfile(hProfile3);
        }
        
        write_error_cube_file(Data, Size);
        cmsHPROFILE hProfile4 = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "./dummy_file.cube");
        if (hProfile4) {
            cmsCloseProfile(hProfile4);
        }

        cmsDeleteContext(ctx);
    }

    return 0;
}