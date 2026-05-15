// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
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

static cmsToneCurve* create_tone_curve_from_data(const uint8_t* data, size_t size) {
    if (size < 256 * sizeof(cmsUInt16Number)) {
        return NULL;
    }
    
    cmsUInt16Number table[256];
    for (int i = 0; i < 256; i++) {
        table[i] = ((cmsUInt16Number)data[i] << 8) | data[i];
    }
    
    return cmsBuildTabulatedToneCurve16(NULL, 256, table);
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

static void fill_XYZ_from_data(cmsCIEXYZ* XYZ, const uint8_t* data, size_t* offset, size_t size) {
    if (*offset + 3 * sizeof(double) > size) {
        XYZ->X = 0.95047;
        XYZ->Y = 1.00000;
        XYZ->Z = 1.08883;
        return;
    }
    
    memcpy(&XYZ->X, data + *offset, sizeof(double));
    *offset += sizeof(double);
    memcpy(&XYZ->Y, data + *offset, sizeof(double));
    *offset += sizeof(double);
    memcpy(&XYZ->Z, data + *offset, sizeof(double));
    *offset += sizeof(double);
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) {
        return 0;
    }
    
    size_t offset = 0;
    
    cmsCIExyY whitePoint;
    fill_xyY_from_data(&whitePoint, Data, &offset, Size);
    
    cmsToneCurve* transferFunction = create_tone_curve_from_data(Data + offset, Size - offset);
    if (transferFunction) {
        offset += 256 * sizeof(cmsUInt16Number);
    }
    
    cmsHPROFILE profile1 = cmsCreateGrayProfile(&whitePoint, transferFunction);
    if (profile1) {
        cmsCloseProfile(profile1);
    }
    
    cmsHPROFILE profile2 = cmsCreateLab2Profile(&whitePoint);
    if (profile2) {
        cmsCloseProfile(profile2);
    }
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        cmsHPROFILE profile3 = cmsCreateGrayProfileTHR(context, &whitePoint, transferFunction);
        if (profile3) {
            cmsCloseProfile(profile3);
        }
        
        cmsCIExyYTRIPLE primaries;
        fill_xyY_from_data(&primaries.Red, Data, &offset, Size);
        fill_xyY_from_data(&primaries.Green, Data, &offset, Size);
        fill_xyY_from_data(&primaries.Blue, Data, &offset, Size);
        
        cmsToneCurve* transferFunctions[3] = {NULL, NULL, NULL};
        for (int i = 0; i < 3; i++) {
            if (offset < Size) {
                transferFunctions[i] = create_tone_curve_from_data(Data + offset, Size - offset);
                if (transferFunctions[i]) {
                    offset += 256 * sizeof(cmsUInt16Number);
                }
            }
        }
        
        cmsHPROFILE profile4 = cmsCreateRGBProfileTHR(context, &whitePoint, &primaries, transferFunctions);
        if (profile4) {
            cmsCloseProfile(profile4);
        }
        
        for (int i = 0; i < 3; i++) {
            if (transferFunctions[i]) {
                cmsFreeToneCurve(transferFunctions[i]);
            }
        }
        
        cmsDeleteContext(context);
    }
    
    cmsCIExyYTRIPLE primaries2;
    fill_xyY_from_data(&primaries2.Red, Data, &offset, Size);
    fill_xyY_from_data(&primaries2.Green, Data, &offset, Size);
    fill_xyY_from_data(&primaries2.Blue, Data, &offset, Size);
    
    cmsToneCurve* transferFunctions2[3] = {NULL, NULL, NULL};
    for (int i = 0; i < 3; i++) {
        if (offset < Size) {
            transferFunctions2[i] = create_tone_curve_from_data(Data + offset, Size - offset);
            if (transferFunctions2[i]) {
                offset += 256 * sizeof(cmsUInt16Number);
            }
        }
    }
    
    cmsHPROFILE profile5 = cmsCreateRGBProfile(&whitePoint, &primaries2, transferFunctions2);
    if (profile5) {
        cmsCloseProfile(profile5);
    }
    
    for (int i = 0; i < 3; i++) {
        if (transferFunctions2[i]) {
            cmsFreeToneCurve(transferFunctions2[i]);
        }
    }
    
    cmsCIEXYZ sourceXYZ;
    fill_XYZ_from_data(&sourceXYZ, Data, &offset, Size);
    
    cmsCIExyY destxyY;
    cmsXYZ2xyY(&destxyY, &sourceXYZ);
    
    if (transferFunction) {
        cmsFreeToneCurve(transferFunction);
    }
    
    return 0;
}