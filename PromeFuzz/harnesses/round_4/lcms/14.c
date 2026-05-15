// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

static cmsToneCurve* create_tone_curve_from_data(cmsContext context, const uint8_t* data, size_t size) {
    // Create a simple gamma tone curve
    if (size == 0) {
        return cmsBuildGamma(context, 2.2);
    }
    
    // Use first byte to determine gamma value (0.1-10.0 range)
    double gamma = 0.1 + (data[0] / 255.0) * 9.9;
    return cmsBuildGamma(context, gamma);
}

static void populate_xyY(cmsCIExyY* xyY, const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + 3 * sizeof(double) <= size) {
        double x, y, Y;
        memcpy(&x, data + *offset, sizeof(double));
        memcpy(&y, data + *offset + sizeof(double), sizeof(double));
        memcpy(&Y, data + *offset + 2 * sizeof(double), sizeof(double));
        
        // Validate and use values
        if (x > 0.0 && x < 1.0 && y > 0.0 && y < 1.0) {
            xyY->x = x;
            xyY->y = y;
            xyY->Y = Y > 0.0 ? Y : 1.0;
        } else {
            // Default D50 values
            xyY->x = 0.3457;
            xyY->y = 0.3585;
            xyY->Y = 1.0;
        }
        *offset += 3 * sizeof(double);
    } else {
        // Default D50 values
        xyY->x = 0.3457;
        xyY->y = 0.3585;
        xyY->Y = 1.0;
    }
}

static void populate_primaries(cmsCIExyYTRIPLE* primaries, const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + 9 * sizeof(double) <= size) {
        double values[9];
        memcpy(values, data + *offset, 9 * sizeof(double));
        
        // Red
        if (values[0] > 0.0 && values[0] < 1.0 && values[1] > 0.0 && values[1] < 1.0) {
            primaries->Red.x = values[0];
            primaries->Red.y = values[1];
            primaries->Red.Y = values[2] > 0.0 ? values[2] : 1.0;
        } else {
            primaries->Red.x = 0.6400;
            primaries->Red.y = 0.3300;
            primaries->Red.Y = 1.0;
        }
        
        // Green
        if (values[3] > 0.0 && values[3] < 1.0 && values[4] > 0.0 && values[4] < 1.0) {
            primaries->Green.x = values[3];
            primaries->Green.y = values[4];
            primaries->Green.Y = values[5] > 0.0 ? values[5] : 1.0;
        } else {
            primaries->Green.x = 0.3000;
            primaries->Green.y = 0.6000;
            primaries->Green.Y = 1.0;
        }
        
        // Blue
        if (values[6] > 0.0 && values[6] < 1.0 && values[7] > 0.0 && values[7] < 1.0) {
            primaries->Blue.x = values[6];
            primaries->Blue.y = values[7];
            primaries->Blue.Y = values[8] > 0.0 ? values[8] : 1.0;
        } else {
            primaries->Blue.x = 0.1500;
            primaries->Blue.y = 0.0600;
            primaries->Blue.Y = 1.0;
        }
        
        *offset += 9 * sizeof(double);
    } else {
        // Default sRGB primaries
        primaries->Red.x = 0.6400;
        primaries->Red.y = 0.3300;
        primaries->Red.Y = 1.0;
        primaries->Green.x = 0.3000;
        primaries->Green.y = 0.6000;
        primaries->Green.Y = 1.0;
        primaries->Blue.x = 0.1500;
        primaries->Blue.y = 0.0600;
        primaries->Blue.Y = 1.0;
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    size_t offset = 0;
    
    // Test cmsCreateNULLProfileTHR
    cmsHPROFILE null_profile = cmsCreateNULLProfileTHR(context);
    if (null_profile) {
        cmsCloseProfile(null_profile);
    }
    
    // Test cmsCreate_sRGBProfileTHR
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfileTHR(context);
    if (srgb_profile) {
        cmsCloseProfile(srgb_profile);
    }
    
    // Test cmsCreateGrayProfileTHR
    cmsCIExyY whitePoint;
    populate_xyY(&whitePoint, Data, Size, &offset);
    
    cmsToneCurve* gray_curve = create_tone_curve_from_data(context, Data + offset, Size - offset);
    cmsHPROFILE gray_profile = cmsCreateGrayProfileTHR(context, &whitePoint, gray_curve);
    if (gray_profile) {
        cmsCloseProfile(gray_profile);
    }
    if (gray_curve) {
        cmsFreeToneCurve(gray_curve);
    }
    
    // Test cmsCreateLab4ProfileTHR
    cmsCIExyY labWhitePoint;
    populate_xyY(&labWhitePoint, Data, Size, &offset);
    cmsHPROFILE lab_profile = cmsCreateLab4ProfileTHR(context, &labWhitePoint);
    if (lab_profile) {
        cmsCloseProfile(lab_profile);
    }
    
    // Test cmsCreateRGBProfileTHR
    cmsCIExyY rgbWhitePoint;
    cmsCIExyYTRIPLE primaries;
    cmsToneCurve* transferFunctions[3];
    
    populate_xyY(&rgbWhitePoint, Data, Size, &offset);
    populate_primaries(&primaries, Data, Size, &offset);
    
    for (int i = 0; i < 3; i++) {
        // Use different parts of the input for each transfer function
        size_t func_offset = (offset + i) % (Size > 0 ? Size : 1);
        transferFunctions[i] = create_tone_curve_from_data(context, 
            Data + func_offset, 
            Size - func_offset);
    }
    
    cmsHPROFILE rgb_profile = cmsCreateRGBProfileTHR(context, &rgbWhitePoint, &primaries, transferFunctions);
    if (rgb_profile) {
        cmsCloseProfile(rgb_profile);
    }
    
    for (int i = 0; i < 3; i++) {
        if (transferFunctions[i]) {
            cmsFreeToneCurve(transferFunctions[i]);
        }
    }
    
    // Test cmsCreateXYZProfileTHR
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    if (xyz_profile) {
        cmsCloseProfile(xyz_profile);
    }
    
    cmsDeleteContext(context);
    return 0;
}