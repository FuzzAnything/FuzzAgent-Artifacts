#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

namespace harness_001 {
// Fuzzing harness for Little CMS (lcms) library
// Target: Device link creation and virtual profile generation APIs
// Focus: cmsvirt.c module functions with high undiscovered complexity
// Semantically different from harness_000.cpp which focuses on profile parsing


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for multiple operations
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input for different operations
    // 1. Consume fixed-size parameters first
    cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 15);
    cmsFloat64Number version = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(2.0, 5.0);
    cmsFloat64Number ink_limit = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 400.0);
    cmsFloat64Number bright = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
    cmsFloat64Number contrast = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
    cmsFloat64Number hue = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-180.0, 180.0);
    cmsFloat64Number saturation = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
    cmsUInt32Number nLUTPoints = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 256);
    cmsUInt32Number temp_src = fdp.ConsumeIntegral<cmsUInt32Number>();
    cmsUInt32Number temp_dest = fdp.ConsumeIntegral<cmsUInt32Number>();
    cmsUInt32Number flags = fdp.ConsumeIntegral<cmsUInt32Number>();
    
    // 2. Create thread context for THR functions
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    // 3. Create source and destination profiles
    cmsHPROFILE src_profile = cmsCreate_sRGBProfileTHR(ctx);
    if (src_profile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create white point for Lab profiles
    cmsCIExyY white_point;
    white_point.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
    white_point.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
    white_point.Y = 1.0;
    
    cmsHPROFILE dst_profile = cmsCreateLab2ProfileTHR(ctx, &white_point);
    if (dst_profile == NULL) {
        cmsCloseProfile(src_profile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 4. Create transform between profiles
    cmsHTRANSFORM hTransform = cmsCreateTransformTHR(ctx, 
                                                     src_profile, 
                                                     TYPE_RGB_8, 
                                                     dst_profile, 
                                                     TYPE_Lab_8,
                                                     intent % 4,  // Limit to 4 basic intents
                                                     flags);
    
    if (hTransform != NULL) {
        // 5. Convert transform to device link profile
        cmsHPROFILE device_link = cmsTransform2DeviceLink(hTransform, version, flags);
        if (device_link != NULL) {
            cmsCloseProfile(device_link);
        }
        
        cmsDeleteTransform(hTransform);
    }
    
    // 6. Test ink limiting device links
    cmsColorSpaceSignature color_space = cmsSigRgbData;
    uint8_t color_space_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
    switch (color_space_choice) {
        case 0: color_space = cmsSigRgbData; break;
        case 1: color_space = cmsSigCmykData; break;
        case 2: color_space = cmsSigGrayData; break;
        case 3: color_space = cmsSigLabData; break;
        case 4: color_space = cmsSigXYZData; break;
    }
    
    // Test both regular and THR versions
    cmsHPROFILE ink_link = cmsCreateInkLimitingDeviceLink(color_space, ink_limit);
    if (ink_link != NULL) {
        cmsCloseProfile(ink_link);
    }
    
    cmsHPROFILE ink_link_thr = cmsCreateInkLimitingDeviceLinkTHR(ctx, color_space, ink_limit);
    if (ink_link_thr != NULL) {
        cmsCloseProfile(ink_link_thr);
    }
    
    // 7. Test linearization device links
    // Create tone curves dynamically based on color space channels
    cmsInt32Number nChannels = cmsChannelsOfColorSpace(color_space);
    if (nChannels <= 0 || nChannels > 16) {
        // Invalid channel count, skip linearization tests
    } else {
        cmsToneCurve** tone_curves = new cmsToneCurve*[nChannels];
        for (int i = 0; i < nChannels; i++) {
            tone_curves[i] = NULL;
        }
        
        // Create gamma curves for each channel
        for (int i = 0; i < nChannels; i++) {
            cmsFloat32Number gamma = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.1, 5.0);
            tone_curves[i] = cmsBuildGamma(NULL, gamma);
        }
        
        cmsHPROFILE linear_link = cmsCreateLinearizationDeviceLink(color_space, tone_curves);
        if (linear_link != NULL) {
            cmsCloseProfile(linear_link);
        }
        
        cmsHPROFILE linear_link_thr = cmsCreateLinearizationDeviceLinkTHR(ctx, color_space, tone_curves);
        if (linear_link_thr != NULL) {
            cmsCloseProfile(linear_link_thr);
        }
        
        // Free tone curves
        for (int i = 0; i < nChannels; i++) {
            if (tone_curves[i] != NULL) {
                cmsFreeToneCurve(tone_curves[i]);
            }
        }
        delete[] tone_curves;
    }
    
    // 8. Test BCHSW abstract profiles
    cmsHPROFILE bchsw_profile = cmsCreateBCHSWabstractProfile(nLUTPoints, bright, contrast, 
                                                             hue, saturation, temp_src, temp_dest);
    if (bchsw_profile != NULL) {
        cmsCloseProfile(bchsw_profile);
    }
    
    cmsHPROFILE bchsw_profile_thr = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, bright, contrast,
                                                                    hue, saturation, temp_src, temp_dest);
    if (bchsw_profile_thr != NULL) {
        cmsCloseProfile(bchsw_profile_thr);
    }
    
    // 9. Test other virtual profile types
    cmsHPROFILE lab4_profile = cmsCreateLab4ProfileTHR(ctx, &white_point);
    if (lab4_profile != NULL) {
        cmsCloseProfile(lab4_profile);
    }
    
    cmsHPROFILE ok_lab_profile = cmsCreate_OkLabProfile(ctx);
    if (ok_lab_profile != NULL) {
        cmsCloseProfile(ok_lab_profile);
    }
    
    // Create primaries for RGB profile
    cmsCIExyYTRIPLE primaries;
    for (int i = 0; i < 3; i++) {
        primaries.Red.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
        primaries.Red.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
        primaries.Red.Y = 1.0;
        
        primaries.Green.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
        primaries.Green.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
        primaries.Green.Y = 1.0;
        
        primaries.Blue.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
        primaries.Blue.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
        primaries.Blue.Y = 1.0;
    }
    
    // Create tone curves for RGB profile
    cmsToneCurve* rgb_curves[3];
    for (int i = 0; i < 3; i++) {
        cmsFloat32Number gamma = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.1, 5.0);
        rgb_curves[i] = cmsBuildGamma(NULL, gamma);
    }
    
    cmsHPROFILE rgb_profile = cmsCreateRGBProfile(&white_point, &primaries, rgb_curves);
    if (rgb_profile != NULL) {
        cmsCloseProfile(rgb_profile);
    }
    
    // Free RGB tone curves
    for (int i = 0; i < 3; i++) {
        if (rgb_curves[i] != NULL) {
            cmsFreeToneCurve(rgb_curves[i]);
        }
    }
    
    // 10. Cleanup
    cmsCloseProfile(src_profile);
    cmsCloseProfile(dst_profile);
    cmsDeleteContext(ctx);
    
    return 0;
}

} // namespace harness_001
