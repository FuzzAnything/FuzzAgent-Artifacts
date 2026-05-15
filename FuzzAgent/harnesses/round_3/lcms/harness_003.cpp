#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_003 {
// Fuzzing harness for LittleCMS (lcms) virtual profile and device link creation
// This harness specifically targets virtual profile and device link creation functions
// that are currently 0% covered according to coverage analysis (cmsvirt.c module)
// Target APIs: cmsTransform2DeviceLink, cmsCreateInkLimitingDeviceLink, 
// cmsCreateLinearizationDeviceLink, cmsCreateLab2Profile, cmsCreateBCHSWabstractProfile,
// cmsCreate_OkLabProfile, cmsCreateRGBProfile, cmsCreateGrayProfile


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful testing
    // Minimum: parameters for multiple function calls + profile data
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a context (use NULL for default context as per guidance)
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // 1. Create source and destination profiles using covered APIs
    // These will be used to create a transform for cmsTransform2DeviceLink
    cmsHPROFILE source_profile = NULL;
    cmsHPROFILE dest_profile = NULL;
    
    // Choose profile types based on consumed data
    uint8_t profile_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (profile_choice) {
        case 0:
            // XYZ and sRGB profiles
            source_profile = cmsCreateXYZProfile();
            dest_profile = cmsCreate_sRGBProfile();
            break;
        case 1:
            // Lab4 and XYZ profiles
            {
                cmsCIExyY d50;
                d50.x = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9);
                d50.y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9);
                d50.Y = 1.0;
                source_profile = cmsCreateLab4Profile(&d50);
                dest_profile = cmsCreateXYZProfile();
            }
            break;
        case 2:
            // sRGB and Lab4 profiles
            {
                cmsCIExyY d50;
                d50.x = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9);
                d50.y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9);
                d50.Y = 1.0;
                source_profile = cmsCreate_sRGBProfile();
                dest_profile = cmsCreateLab4Profile(&d50);
            }
            break;
        case 3:
            // NULL and sRGB profiles
            source_profile = cmsCreateNULLProfile();
            dest_profile = cmsCreate_sRGBProfile();
            break;
    }
    
    // Check if profiles were created successfully
    if (source_profile == NULL || dest_profile == NULL) {
        // Cleanup and exit if profile creation failed
        if (source_profile) cmsCloseProfile(source_profile);
        if (dest_profile) cmsCloseProfile(dest_profile);
        if (context) cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Create a transform using the profiles
    cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
    cmsUInt32Number flags = 0;
    
    // Randomly set some flags
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_BLACKPOINTCOMPENSATION;
    }
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_HIGHRESPRECALC;
    }
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_LOWRESPRECALC;
    }
    
    // Choose appropriate format based on profile channels
    cmsUInt32Number inputFormat, outputFormat;
    cmsColorSpaceSignature src_space = cmsGetColorSpace(source_profile);
    cmsColorSpaceSignature dst_space = cmsGetColorSpace(dest_profile);
    
    // Simplified format selection - real harness would be more sophisticated
    if (src_space == cmsSigGrayData) {
        inputFormat = TYPE_GRAY_8;
    } else if (src_space == cmsSigRgbData) {
        inputFormat = TYPE_RGB_8;
    } else if (src_space == cmsSigCmykData) {
        inputFormat = TYPE_CMYK_8;
    } else if (src_space == cmsSigLabData) {
        inputFormat = TYPE_Lab_8;
    } else {
        inputFormat = TYPE_RGB_8;
    }
    
    if (dst_space == cmsSigGrayData) {
        outputFormat = TYPE_GRAY_8;
    } else if (dst_space == cmsSigRgbData) {
        outputFormat = TYPE_RGB_8;
    } else if (dst_space == cmsSigCmykData) {
        outputFormat = TYPE_CMYK_8;
    } else if (dst_space == cmsSigLabData) {
        outputFormat = TYPE_Lab_8;
    } else {
        outputFormat = TYPE_RGB_8;
    }
    
    cmsHTRANSFORM hTransform = cmsCreateTransform(source_profile, inputFormat,
                                                 dest_profile, outputFormat,
                                                 intent, flags);
    
    // 3. Test cmsTransform2DeviceLink if we have a transform
    if (hTransform != NULL) {
        cmsFloat64Number version = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(2.0, 5.0);
        cmsUInt32Number dl_flags = 0;
        
        if (fdp.ConsumeBool()) {
            dl_flags |= cmsFLAGS_FORCE_CLUT;
        }
        
        cmsHPROFILE device_link = cmsTransform2DeviceLink(hTransform, version, dl_flags);
        if (device_link != NULL) {
            cmsCloseProfile(device_link);
        }
    }
    
    // 4. Test direct device link creation functions
    // cmsCreateInkLimitingDeviceLink
    cmsColorSpaceSignature color_space = cmsSigCmykData;  // CMYK is common for ink limiting
    cmsFloat64Number limit = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 400.0);
    cmsHPROFILE ink_limit_profile = cmsCreateInkLimitingDeviceLink(color_space, limit);
    if (ink_limit_profile != NULL) {
        cmsCloseProfile(ink_limit_profile);
    }
    
    // cmsCreateLinearizationDeviceLink
    // Need tone curves for this - create simple gamma curves
    cmsToneCurve* curves[3];
    for (int i = 0; i < 3; i++) {
        cmsFloat64Number gamma = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 3.0);
        curves[i] = cmsBuildGamma(NULL, gamma);
    }
    
    cmsHPROFILE linear_profile = cmsCreateLinearizationDeviceLink(cmsSigRgbData, curves);
    if (linear_profile != NULL) {
        cmsCloseProfile(linear_profile);
    }
    
    // Clean up tone curves
    for (int i = 0; i < 3; i++) {
        if (curves[i] != NULL) {
            cmsFreeToneCurve(curves[i]);
        }
    }
    
    // 5. Test virtual profile creation functions
    // cmsCreateLab2Profile
    cmsCIExyY white_point;
    white_point.x = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9);
    white_point.y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9);
    white_point.Y = 1.0;
    
    cmsHPROFILE lab2_profile = cmsCreateLab2Profile(&white_point);
    if (lab2_profile != NULL) {
        cmsCloseProfile(lab2_profile);
    }
    
    // cmsCreateBCHSWabstractProfile
    cmsUInt32Number nLUTPoints = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 256);
    cmsFloat64Number bright = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
    cmsFloat64Number contrast = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 10.0);
    cmsFloat64Number hue = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-180.0, 180.0);
    cmsFloat64Number saturation = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 3.0);
    cmsUInt32Number temp_src = fdp.ConsumeIntegral<cmsUInt32Number>();
    cmsUInt32Number temp_dest = fdp.ConsumeIntegral<cmsUInt32Number>();
    
    cmsHPROFILE bchsw_profile = cmsCreateBCHSWabstractProfile(nLUTPoints, bright, contrast, 
                                                             hue, saturation, temp_src, temp_dest);
    if (bchsw_profile != NULL) {
        cmsCloseProfile(bchsw_profile);
    }
    
    // cmsCreate_OkLabProfile (context-aware version)
    cmsHPROFILE oklab_profile = cmsCreate_OkLabProfile(context);
    if (oklab_profile != NULL) {
        cmsCloseProfile(oklab_profile);
    }
    
    // cmsCreateRGBProfile
    // Need primaries and transfer functions - create fuzzed ones
    cmsCIExyY rgb_white;
    rgb_white.x = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9);
    rgb_white.y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9);
    rgb_white.Y = 1.0;
    
    cmsCIExyYTRIPLE rgb_primaries = {
        {fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9), 
         fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9), 1.0},    // Red
        {fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9), 
         fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9), 1.0},    // Green  
        {fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9), 
         fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9), 1.0}     // Blue
    };
    
    cmsToneCurve* rgb_curves[3];
    for (int i = 0; i < 3; i++) {
        cmsFloat64Number gamma = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 3.0);
        rgb_curves[i] = cmsBuildGamma(NULL, gamma);
    }
    
    cmsHPROFILE rgb_profile = cmsCreateRGBProfile(&rgb_white, &rgb_primaries, rgb_curves);
    if (rgb_profile != NULL) {
        cmsCloseProfile(rgb_profile);
    }
    
    // Clean up RGB curves
    for (int i = 0; i < 3; i++) {
        if (rgb_curves[i] != NULL) {
            cmsFreeToneCurve(rgb_curves[i]);
        }
    }
    
    // cmsCreateGrayProfile
    cmsCIExyY gray_white;
    gray_white.x = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9);
    gray_white.y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 0.9);
    gray_white.Y = 1.0;
    
    cmsFloat64Number gray_gamma = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 3.0);
    cmsToneCurve* gray_curve = cmsBuildGamma(NULL, gray_gamma);
    cmsHPROFILE gray_profile = cmsCreateGrayProfile(&gray_white, gray_curve);
    if (gray_profile != NULL) {
        cmsCloseProfile(gray_profile);
    }
    
    if (gray_curve != NULL) {
        cmsFreeToneCurve(gray_curve);
    }
    
    // 6. Cleanup all resources
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
    
    if (source_profile != NULL) {
        cmsCloseProfile(source_profile);
    }
    
    if (dest_profile != NULL) {
        cmsCloseProfile(dest_profile);
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    return 0;
}

} // namespace harness_003
