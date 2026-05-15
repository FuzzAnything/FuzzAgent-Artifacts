// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void generate_lab_from_data(const uint8_t *data, size_t size, size_t offset, cmsCIELab *lab) {
    if (offset + sizeof(cmsCIELab) <= size) {
        memcpy(lab, data + offset, sizeof(cmsCIELab));
    } else {
        lab->L = 50.0;
        lab->a = 0.0;
        lab->b = 0.0;
    }
}

static double generate_double_from_data(const uint8_t *data, size_t size, size_t offset) {
    double value = 0.0;
    if (offset + sizeof(double) <= size) {
        memcpy(&value, data + offset, sizeof(double));
    }
    return value;
}

static cmsUInt32Number generate_flags_from_data(const uint8_t *data, size_t size, size_t offset) {
    cmsUInt32Number flags = 0;
    if (offset + sizeof(cmsUInt32Number) <= size) {
        memcpy(&flags, data + offset, sizeof(cmsUInt32Number));
    }
    return flags;
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsContext context = NULL;
    cmsHANDLE hGBD = NULL;
    cmsBool result = FALSE;
    cmsCIELab lab_point;
    cmsUInt32Number flags;
    double amax, amin, bmax, bmin;
    size_t offset = 0;

    // Create a GBD handle
    hGBD = cmsGBDAlloc(context);
    if (hGBD == NULL) {
        return 0;
    }

    // Add multiple points to GBD using different offsets
    for (int i = 0; i < 5 && offset < Size; i++) {
        generate_lab_from_data(Data, Size, offset, &lab_point);
        cmsGDBAddPoint(hGBD, &lab_point);
        offset += sizeof(cmsCIELab);
    }

    // Check a point
    if (offset < Size) {
        generate_lab_from_data(Data, Size, offset, &lab_point);
        cmsGDBCheckPoint(hGBD, &lab_point);
        offset += sizeof(cmsCIELab);
    }

    // Compute GDB with flags
    if (offset < Size) {
        flags = generate_flags_from_data(Data, Size, offset);
        cmsGDBCompute(hGBD, flags);
        offset += sizeof(cmsUInt32Number);
    }

    // Test desaturation with various parameters
    for (int i = 0; i < 3 && offset + 4 * sizeof(double) < Size; i++) {
        generate_lab_from_data(Data, Size, offset, &lab_point);
        offset += sizeof(cmsCIELab);
        
        amax = generate_double_from_data(Data, Size, offset);
        offset += sizeof(double);
        amin = generate_double_from_data(Data, Size, offset);
        offset += sizeof(double);
        bmax = generate_double_from_data(Data, Size, offset);
        offset += sizeof(double);
        bmin = generate_double_from_data(Data, Size, offset);
        offset += sizeof(double);
        
        cmsDesaturateLab(&lab_point, amax, amin, bmax, bmin);
    }

    // Clean up
    cmsGBDFree(hGBD);

    return 0;
}