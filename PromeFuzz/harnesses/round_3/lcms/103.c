// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_tone_curve_from_data(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 4) {
        return NULL;
    }
    
    size_t table_size = size / 2;
    if (table_size > 256) {
        table_size = 256;
    }
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
    if (!table) {
        return NULL;
    }
    
    for (size_t i = 0; i < table_size; i++) {
        size_t offset = i * 2;
        if (offset + 1 < size) {
            table[i] = (data[offset] << 8) | data[offset + 1];
        } else {
            table[i] = 0;
        }
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(ctx, table_size, table);
    free(table);
    return curve;
}

static void cleanup_tone_curves(cmsToneCurve* curves[3]) {
    for (int i = 0; i < 3; i++) {
        if (curves[i]) {
            cmsFreeToneCurve(curves[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }
    
    cmsHPROFILE profiles[3] = {NULL, NULL, NULL};
    cmsToneCurve* tone_curves[3] = {NULL, NULL, NULL};
    
    cmsCIExyY white_point = {
        .x = 0.3127,
        .y = 0.3290,
        .Y = 1.0
    };
    
    cmsCIExyYTRIPLE primaries = {
        .Red = {.x = 0.64, .y = 0.33, .Y = 1.0},
        .Green = {.x = 0.30, .y = 0.60, .Y = 1.0},
        .Blue = {.x = 0.15, .y = 0.06, .Y = 1.0}
    };
    
    for (int i = 0; i < 3; i++) {
        size_t offset = i * 256;
        if (offset < Size) {
            size_t remaining = Size - offset;
            tone_curves[i] = create_tone_curve_from_data(ctx, Data + offset, remaining);
        }
        
        if (!tone_curves[i]) {
            tone_curves[i] = cmsBuildGamma(ctx, 2.2);
        }
    }
    
    profiles[0] = cmsCreateRGBProfileTHR(ctx, &white_point, &primaries, tone_curves);
    
    if (profiles[0]) {
        cmsUInt32Number model = cmsGetHeaderModel(profiles[0]);
        cmsProfileClassSignature device_class = cmsGetDeviceClass(profiles[0]);
        
        cmsProfileClassSignature new_class = (Data[0] % 6) + 1;
        cmsSetDeviceClass(profiles[0], new_class);
        
        cmsCloseProfile(profiles[0]);
    }
    
    profiles[1] = cmsCreateXYZProfileTHR(ctx);
    if (profiles[1]) {
        cmsGetHeaderModel(profiles[1]);
        cmsGetDeviceClass(profiles[1]);
        cmsCloseProfile(profiles[1]);
    }
    
    profiles[2] = cmsCreateGrayProfileTHR(ctx, &white_point, tone_curves[0]);
    if (profiles[2]) {
        cmsGetHeaderModel(profiles[2]);
        cmsGetDeviceClass(profiles[2]);
        cmsCloseProfile(profiles[2]);
    }
    
    cleanup_tone_curves(tone_curves);
    cmsDeleteContext(ctx);
    
    return 0;
}