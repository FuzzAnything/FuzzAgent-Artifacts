// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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

static cmsCIExyY create_white_point_from_data(const uint8_t *data, size_t size) {
    cmsCIExyY white_point;
    
    // Initialize with default D50 values
    white_point.x = 0.3457;
    white_point.y = 0.3585;
    white_point.Y = 1.0;
    
    // Use fuzzer data to modify white point if we have enough bytes
    if (size >= 24) {  // 3 doubles * 8 bytes each
        double *dbl_data = (double *)data;
        white_point.x = dbl_data[0];
        white_point.y = dbl_data[1];
        white_point.Y = dbl_data[2];
    }
    
    return white_point;
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    // Create context for THR functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    // Create various profiles
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    cmsHPROFILE null_profile = cmsCreateNULLProfile();
    
    // Create white point from fuzzer data
    cmsCIExyY white_point = create_white_point_from_data(Data, Size);
    
    // Create Lab profiles with the white point
    cmsHPROFILE lab2_profile = cmsCreateLab2ProfileTHR(context, &white_point);
    cmsHPROFILE lab4_profile = cmsCreateLab4ProfileTHR(context, &white_point);
    
    // Use fuzzer data to determine device class signature
    cmsProfileClassSignature device_class = cmsSigInputClass;  // Default
    
    if (Size > 0) {
        // Use first byte to select from common device classes
        uint8_t class_selector = Data[0] % 6;
        switch (class_selector) {
            case 0: device_class = cmsSigInputClass; break;
            case 1: device_class = cmsSigDisplayClass; break;
            case 2: device_class = cmsSigOutputClass; break;
            case 3: device_class = cmsSigLinkClass; break;
            case 4: device_class = cmsSigAbstractClass; break;
            case 5: device_class = cmsSigColorSpaceClass; break;
        }
    }
    
    // Apply device class to profiles if they exist
    if (xyz_profile) {
        cmsSetDeviceClass(xyz_profile, device_class);
        cmsGetColorSpace(xyz_profile);
        cmsCloseProfile(xyz_profile);
    }
    
    if (null_profile) {
        cmsSetDeviceClass(null_profile, device_class);
        cmsGetColorSpace(null_profile);
        cmsCloseProfile(null_profile);
    }
    
    if (lab2_profile) {
        cmsSetDeviceClass(lab2_profile, device_class);
        cmsGetColorSpace(lab2_profile);
        cmsCloseProfile(lab2_profile);
    }
    
    if (lab4_profile) {
        cmsSetDeviceClass(lab4_profile, device_class);
        cmsGetColorSpace(lab4_profile);
        cmsCloseProfile(lab4_profile);
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}