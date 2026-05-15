// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void generate_tone_curve(cmsToneCurve** curve, const uint8_t* data, size_t size) {
    if (size < 2) {
        *curve = NULL;
        return;
    }
    
    int num_entries = 256;
    if (size < num_entries * sizeof(cmsUInt16Number)) {
        num_entries = size / sizeof(cmsUInt16Number);
    }
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(num_entries * sizeof(cmsUInt16Number));
    if (!table) {
        *curve = NULL;
        return;
    }
    
    for (int i = 0; i < num_entries; i++) {
        table[i] = (cmsUInt16Number)((data[i % size] << 8) | data[(i + 1) % size]);
    }
    
    *curve = cmsBuildTabulatedToneCurve16(NULL, num_entries, table);
    free(table);
}

static void generate_xyY(cmsCIExyY* xyY, const uint8_t* data, size_t size, size_t offset) {
    if (size - offset < 3 * sizeof(double)) {
        xyY->x = 0.3127;
        xyY->y = 0.3290;
        xyY->Y = 1.0;
        return;
    }
    
    double* values = (double*)(data + offset);
    xyY->x = values[0];
    xyY->y = values[1];
    xyY->Y = values[2];
    
    if (xyY->x < 0.0) xyY->x = 0.0;
    if (xyY->y < 0.0) xyY->y = 0.0;
    if (xyY->Y < 0.0) xyY->Y = 0.0;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsCIExyY whitePoint;
    cmsCIExyYTRIPLE primaries;
    cmsToneCurve* transferFunctions[3] = {NULL, NULL, NULL};
    
    size_t offset = 0;
    
    generate_xyY(&whitePoint, Data, Size, offset);
    offset += 3 * sizeof(double);
    
    if (Size > offset + 9 * sizeof(double)) {
        generate_xyY(&primaries.Red, Data, Size, offset);
        offset += 3 * sizeof(double);
        generate_xyY(&primaries.Green, Data, Size, offset);
        offset += 3 * sizeof(double);
        generate_xyY(&primaries.Blue, Data, Size, offset);
        offset += 3 * sizeof(double);
    } else {
        primaries.Red.x = 0.64; primaries.Red.y = 0.33; primaries.Red.Y = 1.0;
        primaries.Green.x = 0.30; primaries.Green.y = 0.60; primaries.Green.Y = 1.0;
        primaries.Blue.x = 0.15; primaries.Blue.y = 0.06; primaries.Blue.Y = 1.0;
    }
    
    if (Size > offset) {
        size_t remaining = Size - offset;
        const uint8_t* curve_data = Data + offset;
        for (int i = 0; i < 3; i++) {
            generate_tone_curve(&transferFunctions[i], curve_data, remaining);
            if (remaining > 256) {
                curve_data += 256;
                remaining -= 256;
            }
        }
    }
    
    cmsHPROFILE profiles[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    profiles[0] = cmsCreateLab2Profile(&whitePoint);
    profiles[1] = cmsCreate_OkLabProfile(ctx);
    profiles[2] = cmsCreateLab4Profile(&whitePoint);
    profiles[3] = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    
    const cmsCIExyY* d50 = cmsD50_xyY();
    if (d50) {
        profiles[4] = cmsCreateLab4Profile(d50);
    }
    
    profiles[5] = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, transferFunctions);
    
    for (int i = 0; i < 6; i++) {
        if (profiles[i]) {
            cmsCloseProfile(profiles[i]);
        }
    }
    
    for (int i = 0; i < 3; i++) {
        if (transferFunctions[i]) {
            cmsFreeToneCurve(transferFunctions[i]);
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}