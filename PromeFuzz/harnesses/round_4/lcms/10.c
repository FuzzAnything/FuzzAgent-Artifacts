// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_tone_curve_from_data(const uint8_t* data, size_t size) {
    if (size < 256 * sizeof(cmsUInt16Number)) {
        return cmsBuildGamma(NULL, 2.2);
    }
    
    cmsUInt16Number table[256];
    for (int i = 0; i < 256; i++) {
        table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    
    return cmsBuildTabulatedToneCurve16(NULL, 256, table);
}

static void fill_xyY_from_data(cmsCIExyY* xyY, const uint8_t* data, size_t* offset, size_t size) {
    if (*offset + 3 * sizeof(double) <= size) {
        memcpy(&xyY->x, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&xyY->y, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&xyY->Y, data + *offset, sizeof(double));
        *offset += sizeof(double);
    } else {
        xyY->x = 0.3127;
        xyY->y = 0.3290;
        xyY->Y = 1.0;
    }
}

static void fill_xyY_triple_from_data(cmsCIExyYTRIPLE* triple, const uint8_t* data, size_t* offset, size_t size) {
    fill_xyY_from_data(&triple->Red, data, offset, size);
    fill_xyY_from_data(&triple->Green, data, offset, size);
    fill_xyY_from_data(&triple->Blue, data, offset, size);
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    size_t offset = 0;
    
    cmsCIExyY whitePoint1;
    fill_xyY_from_data(&whitePoint1, Data, &offset, Size);
    
    cmsToneCurve* toneCurve = create_tone_curve_from_data(Data + offset, Size - offset);
    offset += 256 * sizeof(cmsUInt16Number);
    
    cmsCIExyY whitePoint2;
    fill_xyY_from_data(&whitePoint2, Data, &offset, Size);
    
    const cmsCIExyY* d50WhitePoint = cmsD50_xyY();
    
    cmsCIExyY whitePoint3;
    fill_xyY_from_data(&whitePoint3, Data, &offset, Size);
    
    cmsCIExyYTRIPLE primaries;
    fill_xyY_triple_from_data(&primaries, Data, &offset, Size);
    
    cmsToneCurve* transferFunctions[3] = {toneCurve, toneCurve, toneCurve};
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    cmsHPROFILE lab2Profile = cmsCreateLab2Profile(&whitePoint1);
    if (lab2Profile) {
        cmsCloseProfile(lab2Profile);
    }
    
    cmsHPROFILE grayProfile = cmsCreateGrayProfile(&whitePoint2, toneCurve);
    if (grayProfile) {
        cmsCloseProfile(grayProfile);
    }
    
    cmsHPROFILE lab4Profile = cmsCreateLab4Profile(&whitePoint3);
    if (lab4Profile) {
        cmsCloseProfile(lab4Profile);
    }
    
    cmsHPROFILE rgbProfileTHR = cmsCreateRGBProfileTHR(context, d50WhitePoint, &primaries, transferFunctions);
    if (rgbProfileTHR) {
        cmsCloseProfile(rgbProfileTHR);
    }
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(d50WhitePoint, &primaries, transferFunctions);
    if (rgbProfile) {
        cmsCloseProfile(rgbProfile);
    }
    
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    
    cmsDeleteContext(context);
    
    return 0;
}