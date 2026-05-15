// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static cmsCIELab generate_lab_from_data(const uint8_t *data, size_t size, size_t offset) {
    cmsCIELab lab = {0.0, 0.0, 0.0};
    if (size - offset >= 24) { // 3 doubles = 24 bytes
        memcpy(&lab.L, data + offset, 8);
        memcpy(&lab.a, data + offset + 8, 8);
        memcpy(&lab.b, data + offset + 16, 8);
    } else if (size - offset >= 12) { // 3 floats = 12 bytes
        float f[3];
        memcpy(f, data + offset, 12);
        lab.L = f[0];
        lab.a = f[1];
        lab.b = f[2];
    } else if (size - offset >= 3) { // 3 bytes
        lab.L = (double)data[offset];
        lab.a = (double)data[offset + 1] - 128.0;
        lab.b = (double)data[offset + 2] - 128.0;
    }
    return lab;
}

static double generate_double_from_data(const uint8_t *data, size_t size, size_t offset) {
    double value = 0.0;
    if (size - offset >= 8) {
        memcpy(&value, data + offset, 8);
    } else if (size - offset >= 4) {
        float f;
        memcpy(&f, data + offset, 4);
        value = (double)f;
    } else if (size - offset >= 1) {
        value = (double)data[offset];
    }
    return value;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Write data to dummy file for any potential file operations
    write_dummy_file(Data, Size);

    // Test cmsGBDAlloc and cmsGBDFree
    cmsHANDLE hGBD = cmsGBDAlloc(NULL);
    if (!hGBD) return 0;

    // Test cmsGDBAddPoint with various Lab points from fuzzer data
    size_t offset = 0;
    int points_added = 0;
    while (offset + 3 <= Size && points_added < 10) {
        cmsCIELab lab = generate_lab_from_data(Data, Size, offset);
        cmsGDBAddPoint(hGBD, &lab);
        offset += 24; // Try to use different offsets
        points_added++;
    }

    // Test cmsGDBCheckPoint with some Lab points
    offset = Size > 24 ? Size - 24 : 0;
    for (int i = 0; i < 3 && offset < Size; i++) {
        cmsCIELab check_lab = generate_lab_from_data(Data, Size, offset);
        cmsGDBCheckPoint(hGBD, &check_lab);
        offset = offset > 24 ? offset - 24 : 0;
    }

    // Test cmsGDBCompute with flags from fuzzer data
    cmsUInt32Number flags = 0;
    if (Size >= 4) {
        memcpy(&flags, Data, 4);
    } else {
        flags = (cmsUInt32Number)Data[0];
    }
    cmsGDBCompute(hGBD, flags);

    // Test cmsDesaturateLab with bounds from fuzzer data
    if (Size >= 40) { // Need at least 24 bytes for Lab + 16 bytes for 4 doubles
        cmsCIELab desat_lab = generate_lab_from_data(Data, Size, 0);
        double amax = generate_double_from_data(Data, Size, 24);
        double amin = generate_double_from_data(Data, Size, 32);
        double bmax = generate_double_from_data(Data, Size, 40);
        double bmin = Size >= 48 ? generate_double_from_data(Data, Size, 48) : 0.0;
        
        cmsDesaturateLab(&desat_lab, amax, amin, bmax, bmin);
    }

    // Cleanup
    cmsGBDFree(hGBD);

    return 0;
}