// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLinearizationDeviceLink at cmsvirt.c:340:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lcms2.h"

static cmsToneCurve* create_tone_curve_from_data(const uint8_t* data, size_t size) {
    if (size < 256) {
        return NULL;
    }
    
    cmsUInt16Number table[256];
    for (int i = 0; i < 256; i++) {
        table[i] = (cmsUInt16Number)((data[i] << 8) | data[i]);
    }
    
    return cmsBuildTabulatedToneCurve16(NULL, 256, table);
}

static cmsToneCurve* create_parametric_curve_from_data(const uint8_t* data, size_t size) {
    if (size < 10) {
        return NULL;
    }
    
    cmsFloat64Number params[10];
    for (int i = 0; i < 10; i++) {
        params[i] = (cmsFloat64Number)data[i] / 255.0;
    }
    
    return cmsBuildParametricToneCurve(NULL, 4, params);
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
        return 0;
    }
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    cmsColorSpaceSignature color_spaces[] = {
        cmsSigRgbData,
        cmsSigCmykData,
        cmsSigLabData,
        cmsSigXYZData,
        cmsSigGrayData
    };
    
    int color_space_index = Data[0] % (sizeof(color_spaces) / sizeof(color_spaces[0]));
    cmsColorSpaceSignature color_space = color_spaces[color_space_index];
    
    cmsToneCurve* curves[3] = {NULL, NULL, NULL};
    cmsToneCurve* transfer_functions[3] = {NULL, NULL, NULL};
    
    size_t offset = 1;
    for (int i = 0; i < 3; i++) {
        if (offset >= Size) break;
        
        if (Data[offset] % 2 == 0) {
            curves[i] = create_tone_curve_from_data(Data + offset, Size - offset);
        } else {
            curves[i] = create_parametric_curve_from_data(Data + offset, Size - offset);
        }
        
        transfer_functions[i] = curves[i];
        if (curves[i]) {
            offset += 256;
        } else {
            offset += 10;
        }
    }
    
    cmsHPROFILE profile_thr = cmsCreateLinearizationDeviceLinkTHR(context, color_space, transfer_functions);
    if (profile_thr) {
        cmsCloseProfile(profile_thr);
    }
    
    cmsHPROFILE profile = cmsCreateLinearizationDeviceLink(color_space, transfer_functions);
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    for (int i = 0; i < 3; i++) {
        if (curves[i]) {
            cmsToneCurve* reversed = cmsReverseToneCurve(curves[i]);
            if (reversed) {
                cmsToneCurve* dup = cmsDupToneCurve(reversed);
                if (dup) {
                    cmsFreeToneCurve(dup);
                }
                cmsFreeToneCurve(reversed);
            }
        }
    }
    
    cmsFreeToneCurveTriple(curves);
    
    for (int i = 0; i < 3; i++) {
        if (curves[i]) {
            cmsFreeToneCurve(curves[i]);
        }
    }
    
    cmsDeleteContext(context);
    
    return 0;
}