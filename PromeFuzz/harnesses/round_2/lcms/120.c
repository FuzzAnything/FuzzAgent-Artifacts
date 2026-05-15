// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* build_tone_curve_from_data(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 2) return NULL;
    
    // Simple approach: create a tabulated tone curve with limited entries
    int nEntries = 256;
    if (size < nEntries * 2) return NULL;
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
    if (!table) return NULL;
    
    for (int i = 0; i < nEntries; i++) {
        if (i * 2 + 1 < size) {
            table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
        } else {
            table[i] = (cmsUInt16Number)((i * 65535) / (nEntries - 1));
        }
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, table);
    free(table);
    return curve;
}

static void fill_xyY_from_data(cmsCIExyY* xyY, const uint8_t* data, size_t size, size_t offset) {
    if (offset + 24 <= size) {
        // Read 3 doubles (24 bytes) from data
        memcpy(&xyY->x, data + offset, 8);
        memcpy(&xyY->y, data + offset + 8, 8);
        memcpy(&xyY->Y, data + offset + 16, 8);
    } else {
        // Use default D50 values if not enough data
        xyY->x = 0.3457;
        xyY->y = 0.3585;
        xyY->Y = 1.0;
    }
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    // Create a context for all operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use first byte to determine which function to test
    uint8_t selector = Size > 0 ? Data[0] % 6 : 0;
    size_t data_offset = Size > 0 ? 1 : 0;
    
    cmsHPROFILE profile = NULL;
    
    switch (selector) {
        case 0:
            profile = cmsCreate_OkLabProfile(ctx);
            break;
            
        case 1:
            profile = cmsCreateNULLProfileTHR(ctx);
            break;
            
        case 2: {
            cmsCIExyY whitePoint;
            fill_xyY_from_data(&whitePoint, Data, Size, data_offset);
            profile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
            break;
        }
            
        case 3:
            profile = cmsCreateXYZProfileTHR(ctx);
            break;
            
        case 4: {
            if (Size - data_offset < 72) {
                // Need at least 72 bytes for white point (24) + primaries (48)
                break;
            }
            
            cmsCIExyY whitePoint;
            cmsCIExyYTRIPLE primaries;
            
            fill_xyY_from_data(&whitePoint, Data, Size, data_offset);
            fill_xyY_from_data(&primaries.Red, Data, Size, data_offset + 24);
            fill_xyY_from_data(&primaries.Green, Data, Size, data_offset + 48);
            fill_xyY_from_data(&primaries.Blue, Data, Size, data_offset + 72);
            
            cmsToneCurve* transfer[3];
            size_t curve_offset = data_offset + 96;
            
            for (int i = 0; i < 3; i++) {
                transfer[i] = build_tone_curve_from_data(ctx, 
                    Data + curve_offset, 
                    Size - curve_offset);
                if (!transfer[i]) {
                    // If curve creation fails, use identity for remaining
                    for (int j = i; j < 3; j++) {
                        transfer[j] = cmsBuildGamma(ctx, 1.0);
                    }
                    break;
                }
                curve_offset += 512; // Approximate offset for next curve
            }
            
            profile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, transfer);
            
            // Clean up tone curves
            for (int i = 0; i < 3; i++) {
                if (transfer[i]) {
                    cmsFreeToneCurve(transfer[i]);
                }
            }
            break;
        }
            
        case 5:
            // Test creating another context
            cmsContext ctx2 = cmsCreateContext(NULL, NULL);
            if (ctx2) {
                cmsDeleteContext(ctx2);
            }
            break;
    }
    
    // Clean up profile if created
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    // Clean up main context
    cmsDeleteContext(ctx);
    
    return 0;
}