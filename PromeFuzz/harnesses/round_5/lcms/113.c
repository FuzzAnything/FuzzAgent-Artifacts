// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static double read_double(const uint8_t** data, size_t* size) {
    if (*size < sizeof(double)) return 0.0;
    double value;
    memcpy(&value, *data, sizeof(double));
    *data += sizeof(double);
    *size -= sizeof(double);
    return value;
}

static cmsUInt16Number read_u16(const uint8_t** data, size_t* size) {
    if (*size < sizeof(cmsUInt16Number)) return 0;
    cmsUInt16Number value;
    memcpy(&value, *data, sizeof(cmsUInt16Number));
    *data += sizeof(cmsUInt16Number);
    *size -= sizeof(cmsUInt16Number);
    return value;
}

static cmsUInt32Number read_u32(const uint8_t** data, size_t* size) {
    if (*size < sizeof(cmsUInt32Number)) return 0;
    cmsUInt32Number value;
    memcpy(&value, *data, sizeof(cmsUInt32Number));
    *data += sizeof(cmsUInt32Number);
    *size -= sizeof(cmsUInt32Number);
    return value;
}

static void populate_curve(cmsToneCurve* curve, const uint8_t** data, size_t* size) {
    if (!curve || *size < sizeof(cmsUInt32Number)) return;
    
    cmsUInt32Number nEntries = read_u32(data, size);
    if (nEntries > 0 && nEntries <= 65536 && *size >= nEntries * sizeof(cmsUInt16Number)) {
        for (cmsUInt32Number i = 0; i < nEntries; i++) {
            cmsUInt16Number val = read_u16(data, size);
            // The actual table is internal, but we can't directly access it
            // This is just to show we would populate if we had access
        }
    }
}

static cmsToneCurve* create_tone_curve_from_data(const uint8_t** data, size_t* size) {
    if (*size < sizeof(cmsUInt32Number)) return NULL;
    
    // Try to create a parametric curve first (simpler)
    cmsUInt32Number type = read_u32(data, size) % 10; // Type 0-9
    double params[10];
    
    for (int i = 0; i < 10 && *size >= sizeof(double); i++) {
        params[i] = read_double(data, size);
    }
    
    // Create the tone curve
    cmsToneCurve* curve = cmsBuildParametricToneCurve(NULL, type, params);
    if (curve) {
        // If we have more data, try to populate internal structures
        populate_curve(curve, data, size);
    }
    
    return curve;
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3) return 0;
    
    const uint8_t* data_ptr = Data;
    size_t data_size = Size;
    
    // Create context for functions that need it
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // 1. Test cmsCreateLab4Profile
    {
        cmsCIExyY whitePoint;
        whitePoint.x = read_double(&data_ptr, &data_size);
        whitePoint.y = read_double(&data_ptr, &data_size);
        whitePoint.Y = read_double(&data_ptr, &data_size);
        
        cmsHPROFILE profile = cmsCreateLab4Profile(&whitePoint);
        if (profile) cmsCloseProfile(profile);
    }
    
    // 2. Test cmsCreateLab2Profile
    {
        cmsCIExyY whitePoint;
        if (data_size >= sizeof(double) * 3) {
            whitePoint.x = read_double(&data_ptr, &data_size);
            whitePoint.y = read_double(&data_ptr, &data_size);
            whitePoint.Y = read_double(&data_ptr, &data_size);
        } else {
            // Use D50 if not enough data
            const cmsCIExyY* d50 = cmsD50_xyY();
            if (d50) whitePoint = *d50;
        }
        
        cmsHPROFILE profile = cmsCreateLab2Profile(&whitePoint);
        if (profile) cmsCloseProfile(profile);
    }
    
    // 3. Test cmsCreate_OkLabProfile
    {
        cmsHPROFILE profile = cmsCreate_OkLabProfile(ctx);
        if (profile) cmsCloseProfile(profile);
    }
    
    // 4. Test cmsCreateLab4ProfileTHR
    {
        cmsCIExyY whitePoint;
        if (data_size >= sizeof(double) * 3) {
            whitePoint.x = read_double(&data_ptr, &data_size);
            whitePoint.y = read_double(&data_ptr, &data_size);
            whitePoint.Y = read_double(&data_ptr, &data_size);
        } else {
            // Use D50 if not enough data
            const cmsCIExyY* d50 = cmsD50_xyY();
            if (d50) whitePoint = *d50;
        }
        
        cmsHPROFILE profile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
        if (profile) cmsCloseProfile(profile);
    }
    
    // 5. Test cmsCreateRGBProfileTHR
    {
        cmsCIExyY whitePoint;
        cmsCIExyYTRIPLE primaries;
        
        // Read white point
        if (data_size >= sizeof(double) * 3) {
            whitePoint.x = read_double(&data_ptr, &data_size);
            whitePoint.y = read_double(&data_ptr, &data_size);
            whitePoint.Y = read_double(&data_ptr, &data_size);
        } else {
            const cmsCIExyY* d50 = cmsD50_xyY();
            if (d50) whitePoint = *d50;
        }
        
        // Read primaries (Red, Green, Blue)
        if (data_size >= sizeof(double) * 9) {
            primaries.Red.x = read_double(&data_ptr, &data_size);
            primaries.Red.y = read_double(&data_ptr, &data_size);
            primaries.Red.Y = read_double(&data_ptr, &data_size);
            
            primaries.Green.x = read_double(&data_ptr, &data_size);
            primaries.Green.y = read_double(&data_ptr, &data_size);
            primaries.Green.Y = read_double(&data_ptr, &data_size);
            
            primaries.Blue.x = read_double(&data_ptr, &data_size);
            primaries.Blue.y = read_double(&data_ptr, &data_size);
            primaries.Blue.Y = read_double(&data_ptr, &data_size);
        } else {
            // Use some default values if not enough data
            primaries.Red.x = 0.64; primaries.Red.y = 0.33; primaries.Red.Y = 1.0;
            primaries.Green.x = 0.30; primaries.Green.y = 0.60; primaries.Green.Y = 1.0;
            primaries.Blue.x = 0.15; primaries.Blue.y = 0.06; primaries.Blue.Y = 1.0;
        }
        
        // Create tone curves
        cmsToneCurve* transferFunction[3] = {NULL, NULL, NULL};
        for (int i = 0; i < 3 && data_size > 0; i++) {
            transferFunction[i] = create_tone_curve_from_data(&data_ptr, &data_size);
        }
        
        cmsHPROFILE profile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, transferFunction);
        if (profile) cmsCloseProfile(profile);
        
        // Clean up tone curves
        for (int i = 0; i < 3; i++) {
            if (transferFunction[i]) cmsFreeToneCurve(transferFunction[i]);
        }
    }
    
    // 6. Test cmsD50_xyY
    {
        const cmsCIExyY* d50 = cmsD50_xyY();
        // Just reference it to ensure it's called
        (void)d50;
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}