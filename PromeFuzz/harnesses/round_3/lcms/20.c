// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateLinearizationDeviceLink at cmsvirt.c:340:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsChannelsOf at cmspcs.c:945:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
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

static cmsToneCurve* create_dummy_tone_curve(void) {
    // Create a simple gamma curve
    cmsFloat64Number gamma = 2.2;
    return cmsBuildGamma(NULL, gamma);
}

static void cleanup_tone_curves(cmsToneCurve** curves, int count) {
    for (int i = 0; i < count; i++) {
        if (curves[i]) {
            cmsFreeToneCurve(curves[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract color space signature from fuzzer input
    cmsColorSpaceSignature colorSpaceSig;
    memcpy(&colorSpaceSig, Data, sizeof(cmsColorSpaceSignature));
    
    // Create dummy tone curves (up to 4 channels for typical color spaces)
    cmsToneCurve* transferFunctions[4] = {NULL, NULL, NULL, NULL};
    int numChannels = 3; // Default to RGB-like
    
    // Determine number of channels based on color space
    switch (colorSpaceSig) {
        case cmsSigCmykData:
            numChannels = 4;
            break;
        case cmsSigGrayData:
            numChannels = 1;
            break;
        case cmsSigRgbData:
        case cmsSigLabData:
        case cmsSigLuvData:
        case cmsSigYCbCrData:
        case cmsSigYxyData:
        case cmsSigHsvData:
        case cmsSigHlsData:
            numChannels = 3;
            break;
        default:
            numChannels = 3;
    }
    
    // Create tone curves for each channel
    for (int i = 0; i < numChannels; i++) {
        transferFunctions[i] = create_dummy_tone_curve();
        if (!transferFunctions[i]) {
            cleanup_tone_curves(transferFunctions, i);
            return 0;
        }
    }

    // Test cmsCreateLinearizationDeviceLink
    cmsHPROFILE profile1 = cmsCreateLinearizationDeviceLink(colorSpaceSig, transferFunctions);
    if (profile1) {
        // Test cmsGetColorSpace on the created profile
        cmsColorSpaceSignature retrievedSig = cmsGetColorSpace(profile1);
        
        // Test _cmsLCMScolorSpace
        int internalCode = _cmsLCMScolorSpace(colorSpaceSig);
        (void)internalCode; // Use result to avoid unused variable warning
        
        // Test cmsSetColorSpace
        cmsSetColorSpace(profile1, colorSpaceSig);
        
        // Test cmsChannelsOf (deprecated but still in API)
        cmsUInt32Number channels = cmsChannelsOf(colorSpaceSig);
        (void)channels; // Use result to avoid unused variable warning
        
        cmsCloseProfile(profile1);
    }

    // Test cmsCreateLinearizationDeviceLinkTHR with a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        cmsHPROFILE profile2 = cmsCreateLinearizationDeviceLinkTHR(context, colorSpaceSig, transferFunctions);
        if (profile2) {
            cmsCloseProfile(profile2);
        }
        cmsDeleteContext(context);
    }

    // Clean up tone curves
    cleanup_tone_curves(transferFunctions, numChannels);

    return 0;
}