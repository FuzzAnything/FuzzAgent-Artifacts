// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cmsCreateRGBProfileTHR(cmsContext context, const uint8_t *data, size_t size) {
    cmsCIExyY whitePoint;
    cmsCIExyYTRIPLE primaries;
    cmsToneCurve* transferFunction[3] = {NULL, NULL, NULL};
    
    // Use fuzzer data to initialize structures if we have enough data
    if (size >= sizeof(whitePoint) + sizeof(primaries)) {
        memcpy(&whitePoint, data, sizeof(whitePoint));
        memcpy(&primaries, data + sizeof(whitePoint), sizeof(primaries));
        
        // Create simple gamma curves using fuzzer data
        // We'll use the remaining data to create simple tone curves
        size_t offset = sizeof(whitePoint) + sizeof(primaries);
        size_t remaining = size - offset;
        
        if (remaining >= 3 * sizeof(cmsUInt16Number) * 256) {
            // Create simple table-based tone curves
            for (int i = 0; i < 3; i++) {
                cmsUInt16Number* table = (cmsUInt16Number*)malloc(256 * sizeof(cmsUInt16Number));
                if (table && offset + 256 * sizeof(cmsUInt16Number) <= size) {
                    memcpy(table, data + offset, 256 * sizeof(cmsUInt16Number));
                    offset += 256 * sizeof(cmsUInt16Number);
                    transferFunction[i] = cmsBuildTabulatedToneCurve16(context, 256, table);
                    free(table);
                }
            }
        }
    } else {
        // Use default values if not enough data
        whitePoint.x = 0.3127;
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
        
        primaries.Red.x = 0.64;
        primaries.Red.y = 0.33;
        primaries.Red.Y = 1.0;
        primaries.Green.x = 0.30;
        primaries.Green.y = 0.60;
        primaries.Green.Y = 1.0;
        primaries.Blue.x = 0.15;
        primaries.Blue.y = 0.06;
        primaries.Blue.Y = 1.0;
    }
    
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(context, &whitePoint, &primaries, transferFunction);
    
    // Clean up tone curves
    for (int i = 0; i < 3; i++) {
        if (transferFunction[i]) {
            cmsFreeToneCurve(transferFunction[i]);
        }
    }
    
    if (profile) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a context for all operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    // Use first byte to select which function to test
    uint8_t selector = Data[0];
    
    switch (selector % 6) {
        case 0: {
            cmsHPROFILE profile = cmsCreateNULLProfileTHR(context);
            if (profile) {
                cmsCloseProfile(profile);
            }
            break;
        }
        case 1: {
            cmsHPROFILE profile = cmsCreateXYZProfileTHR(context);
            if (profile) {
                cmsCloseProfile(profile);
            }
            break;
        }
        case 2: {
            cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(context);
            if (profile) {
                cmsCloseProfile(profile);
            }
            break;
        }
        case 3: {
            cmsHPROFILE profile = cmsCreateProfilePlaceholder(context);
            if (profile) {
                cmsCloseProfile(profile);
            }
            break;
        }
        case 4: {
            fuzz_cmsCreateRGBProfileTHR(context, Data + 1, Size - 1);
            break;
        }
        case 5: {
            // Test creating additional contexts with different parameters
            cmsContext context2 = cmsCreateContext(NULL, (void*)(Data + 1));
            if (context2) {
                cmsDeleteContext(context2);
            }
            break;
        }
    }
    
    // Clean up
    cmsDeleteContext(context);
    
    return 0;
}