// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsChannelsOf at cmspcs.c:945:27 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsCreateLinearizationDeviceLink at cmsvirt.c:340:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
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

static cmsToneCurve* create_test_tone_curve(void) {
    // Create a simple gamma curve for testing
    return cmsBuildGamma(NULL, 2.2);
}

static void cleanup_tone_curves(cmsToneCurve** curves, int count) {
    for (int i = 0; i < count; i++) {
        if (curves[i]) {
            cmsFreeToneCurve(curves[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine color space
    uint8_t color_space_selector = Data[0];
    
    // Map selector to color space signatures (common ones)
    cmsColorSpaceSignature color_spaces[] = {
        cmsSigRgbData,
        cmsSigGrayData,
        cmsSigCmykData,
        cmsSigLabData,
        cmsSigXYZData,
        cmsSigYCbCrData,
        cmsSigYxyData,
        cmsSigHsvData,
        cmsSigHlsData,
        cmsSigCmyData
    };
    
    int num_color_spaces = sizeof(color_spaces) / sizeof(color_spaces[0]);
    cmsColorSpaceSignature color_space = color_spaces[color_space_selector % num_color_spaces];
    
    // Test cmsChannelsOf
    cmsUInt32Number channels = cmsChannelsOf(color_space);
    (void)channels; // Use result to avoid unused variable warning
    
    // Test _cmsLCMScolorSpace
    int internal_code = _cmsLCMScolorSpace(color_space);
    (void)internal_code;
    
    // Create context for THR function
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Create tone curves for linearization functions
    int max_channels = 16; // Reasonable upper bound
    cmsToneCurve* transfer_functions[max_channels];
    
    for (int i = 0; i < max_channels; i++) {
        transfer_functions[i] = create_test_tone_curve();
    }
    
    // Test cmsCreateLinearizationDeviceLinkTHR
    cmsHPROFILE profile_thr = cmsCreateLinearizationDeviceLinkTHR(
        context,
        color_space,
        transfer_functions
    );
    
    // Test cmsCreateLinearizationDeviceLink (without context)
    cmsHPROFILE profile = cmsCreateLinearizationDeviceLink(
        color_space,
        transfer_functions
    );
    
    // Clean up profiles
    if (profile_thr) {
        cmsCloseProfile(profile_thr);
    }
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    // Create a dummy profile for get/set color space tests
    cmsHPROFILE dummy_profile = cmsCreateProfilePlaceholder(context);
    if (dummy_profile) {
        // Test cmsSetColorSpace
        cmsSetColorSpace(dummy_profile, color_space);
        
        // Test cmsGetColorSpace
        cmsColorSpaceSignature retrieved_space = cmsGetColorSpace(dummy_profile);
        (void)retrieved_space;
        
        cmsCloseProfile(dummy_profile);
    }
    
    // Clean up
    cleanup_tone_curves(transfer_functions, max_channels);
    cmsDeleteContext(context);
    
    return 0;
}