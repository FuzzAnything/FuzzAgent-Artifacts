// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
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

static cmsToneCurve* create_tone_curve_from_data(const uint8_t* data, size_t size, cmsContext context) {
    if (size < 256 * sizeof(cmsUInt16Number)) {
        return NULL;
    }
    
    cmsUInt16Number table[256];
    for (int i = 0; i < 256; i++) {
        table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    
    return cmsBuildTabulatedToneCurve16(context, 256, table);
}

static void fill_xyY_from_data(cmsCIExyY* xyY, const uint8_t* data, size_t* offset, size_t size) {
    if (*offset + 3 * sizeof(double) > size) {
        xyY->x = 0.3127;
        xyY->y = 0.3290;
        xyY->Y = 1.0;
        return;
    }
    
    memcpy(&xyY->x, data + *offset, sizeof(double));
    *offset += sizeof(double);
    memcpy(&xyY->y, data + *offset, sizeof(double));
    *offset += sizeof(double);
    memcpy(&xyY->Y, data + *offset, sizeof(double));
    *offset += sizeof(double);
}

static void fill_xyY_triple_from_data(cmsCIExyYTRIPLE* triple, const uint8_t* data, size_t* offset, size_t size) {
    fill_xyY_from_data(&triple->Red, data, offset, size);
    fill_xyY_from_data(&triple->Green, data, offset, size);
    fill_xyY_from_data(&triple->Blue, data, offset, size);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    size_t offset = 0;
    
    cmsCIExyY whitePoint;
    fill_xyY_from_data(&whitePoint, Data, &offset, Size);
    
    cmsCIExyYTRIPLE primaries;
    fill_xyY_triple_from_data(&primaries, Data, &offset, Size);
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    cmsToneCurve* toneCurve = create_tone_curve_from_data(Data + offset, Size - offset, context);
    if (!toneCurve) {
        cmsDeleteContext(context);
        return 0;
    }
    
    cmsToneCurve* transferFunctions[3] = {toneCurve, toneCurve, toneCurve};
    
    cmsHPROFILE grayProfileTHR = cmsCreateGrayProfileTHR(context, &whitePoint, toneCurve);
    if (grayProfileTHR) {
        cmsCloseProfile(grayProfileTHR);
    }
    
    cmsHPROFILE labProfile = cmsCreateLab2Profile(&whitePoint);
    if (labProfile) {
        cmsCloseProfile(labProfile);
    }
    
    cmsHPROFILE rgbProfileTHR = cmsCreateRGBProfileTHR(context, &whitePoint, &primaries, transferFunctions);
    if (rgbProfileTHR) {
        cmsCloseProfile(rgbProfileTHR);
    }
    
    cmsCIEXYZ xyzResult;
    cmsxyY2XYZ(&xyzResult, &whitePoint);
    
    cmsHPROFILE grayProfile = cmsCreateGrayProfile(&whitePoint, toneCurve);
    if (grayProfile) {
        cmsCloseProfile(grayProfile);
    }
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(&whitePoint, &primaries, transferFunctions);
    if (rgbProfile) {
        cmsCloseProfile(rgbProfile);
    }
    
    cmsFreeToneCurve(toneCurve);
    cmsDeleteContext(context);
    
    return 0;
}