// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"

static cmsToneCurve* create_tone_curve_from_data(const uint8_t* data, size_t size) {
    if (size < 2) {
        return cmsBuildGamma(NULL, 2.2);
    }
    
    size_t table_size = data[0] % 256;
    if (table_size < 2) table_size = 2;
    if (table_size > 4096) table_size = 4096;
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
    if (!table) {
        return cmsBuildGamma(NULL, 2.2);
    }
    
    for (size_t i = 0; i < table_size && i < size - 1; i++) {
        table[i] = (data[(i + 1) % size] << 8) | data[(i + 2) % size];
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, table_size, table);
    free(table);
    return curve ? curve : cmsBuildGamma(NULL, 2.2);
}

static void fill_xyY_from_data(cmsCIExyY* xyY, const uint8_t* data, size_t* offset, size_t size) {
    if (*offset + 24 <= size) {
        double* dbl_ptr = (double*)&data[*offset];
        xyY->x = dbl_ptr[0];
        xyY->y = dbl_ptr[1];
        xyY->Y = dbl_ptr[2];
        *offset += 24;
    } else {
        xyY->x = 0.3127;
        xyY->y = 0.3290;
        xyY->Y = 1.0;
    }
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    size_t offset = 0;
    
    cmsCIExyY whitePoint;
    fill_xyY_from_data(&whitePoint, Data, &offset, Size);
    
    cmsCIExyYTRIPLE primaries;
    fill_xyY_from_data(&primaries.Red, Data, &offset, Size);
    fill_xyY_from_data(&primaries.Green, Data, &offset, Size);
    fill_xyY_from_data(&primaries.Blue, Data, &offset, Size);
    
    cmsToneCurve* transfer[3];
    for (int i = 0; i < 3; i++) {
        transfer[i] = create_tone_curve_from_data(&Data[offset % Size], Size - (offset % Size));
        offset = (offset + 1) % Size;
    }
    
    cmsHPROFILE rgb_profile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, transfer);
    
    for (int i = 0; i < 3; i++) {
        if (transfer[i]) cmsFreeToneCurve(transfer[i]);
    }
    
    if (rgb_profile) {
        cmsUInt32Number model = cmsGetHeaderModel(rgb_profile);
        cmsProfileClassSignature device_class = cmsGetDeviceClass(rgb_profile);
        
        if (Size > 0) {
            cmsProfileClassSignature new_class = (cmsProfileClassSignature)(Data[0] % 12);
            cmsSetDeviceClass(rgb_profile, new_class);
            
            device_class = cmsGetDeviceClass(rgb_profile);
            model = cmsGetHeaderModel(rgb_profile);
        }
        
        cmsCloseProfile(rgb_profile);
    }
    
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(ctx);
    if (xyz_profile) {
        cmsGetHeaderModel(xyz_profile);
        cmsGetDeviceClass(xyz_profile);
        
        if (Size > 1) {
            cmsProfileClassSignature new_class = (cmsProfileClassSignature)(Data[1] % 12);
            cmsSetDeviceClass(xyz_profile, new_class);
        }
        
        cmsCloseProfile(xyz_profile);
    }
    
    cmsToneCurve* gray_transfer = create_tone_curve_from_data(&Data[offset % Size], Size - (offset % Size));
    cmsHPROFILE gray_profile = cmsCreateGrayProfileTHR(ctx, &whitePoint, gray_transfer);
    
    if (gray_transfer) cmsFreeToneCurve(gray_transfer);
    
    if (gray_profile) {
        cmsGetHeaderModel(gray_profile);
        cmsGetDeviceClass(gray_profile);
        
        if (Size > 2) {
            cmsProfileClassSignature new_class = (cmsProfileClassSignature)(Data[2] % 12);
            cmsSetDeviceClass(gray_profile, new_class);
        }
        
        cmsCloseProfile(gray_profile);
    }
    
    cmsDeleteContext(ctx);
    return 0;
}