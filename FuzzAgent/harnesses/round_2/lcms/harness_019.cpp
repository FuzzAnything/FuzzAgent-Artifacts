#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <cmath>
#include "lcms2.h"

namespace harness_019 {
/*
 * Fuzzing harness for LittleCMS2 library
 * Targets: RGB profile gamma detection and related gamut/TAC operations in cmsgmt.c module
 * Semantic domain: RGB profile gamma space estimation, gamut boundary operations
 * Previous harness semantic domains covered:
 *   000-basic profile parsing/transformation
 *   001-proofing/PostScript
 *   002-virtual profiles/gamut (includes cmsDesaturateLab)
 *   003-CGATS/IT8 basic
 *   004-CIECAM02 (appearance model)
 *   005-TAC detection/sampling (includes cmsDetectTAC)
 *   006-device link creation/I/O
 *   007-BCHSW abstract profiles
 *   008-advanced CGATS/IT8
 *   009-cube file device link creation/stream I/O
 *   010-pipeline/MPE operations
 *   011-PostScript color resources
 *   012-MD5/half-precision
 *   013-context management/plugin system
 *   014-color science algorithms and metrics
 *   015-named color list management
 *   016-alpha compositing/gamma correction
 *   017-optimization/smoothing/white point
 *   018-basic profile parsing (replica)
 * This harness focuses on: RGB profile gamma detection (cmsDetectRGBProfileGamma), 
 *   additional gamut/TAC operations, and RGB color space validation
 */


// Include Little CMS headers

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 256) {
        return 0;  // Minimum input size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Create a context for the operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Phase 1: Test with various RGB profiles and threshold values
    
    // Split input: part for profile data, part for testing parameters
    size_t profile_data_size = fdp.ConsumeIntegralInRange<size_t>(128, size - 128);
    std::vector<uint8_t> profile_data = fdp.ConsumeBytes<uint8_t>(profile_data_size);
    
    // Try to open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(profile_data.data(), (cmsUInt32Number)profile_data.size());
    cmsBool profile_from_mem = (hProfile != NULL);
    
    // If we couldn't open from memory, create standard profiles
    if (!profile_from_mem) {
        // Randomly choose between different built-in RGB profiles
        uint8_t profile_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (profile_choice) {
            case 0:
                hProfile = cmsCreate_sRGBProfile();
                break;
            case 1:
                hProfile = cmsCreateLab4Profile(NULL);
                break;
            case 2:
                hProfile = cmsCreateXYZProfile();
                break;
            case 3:
                hProfile = cmsCreateBCHSWabstractProfile(
                    256,  // nLUTPoints
                    fdp.ConsumeFloatingPoint<double>(),  // Bright
                    fdp.ConsumeFloatingPoint<double>(),  // Contrast
                    fdp.ConsumeFloatingPoint<double>(),  // Hue
                    fdp.ConsumeFloatingPoint<double>(),  // Saturation
                    fdp.ConsumeIntegral<cmsUInt32Number>(),  // TempSrc
                    fdp.ConsumeIntegral<cmsUInt32Number>()   // TempDest
                );
                break;
            default:
                // Fallback to sRGB profile for unexpected values
                hProfile = cmsCreate_sRGBProfile();
                break;
        }
        
        if (hProfile == NULL) {
            cmsDeleteContext(context);
            return 0;  // Can't create any profile
        }
    }
    
    // Phase 2: Primary target - cmsDetectRGBProfileGamma
    
    // Consume threshold value (should be positive)
    cmsFloat64Number threshold = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.001, 1.0);
    
    // Call the primary target API - cmsDetectRGBProfileGamma
    cmsFloat64Number detected_gamma = cmsDetectRGBProfileGamma(hProfile, threshold);
    
    // Gamma detection returns -1 on error, or positive gamma value
    // We don't need to validate the result for fuzzing purposes
    
    // Phase 3: Additional RGB profile validation and operations
    
    // Check if profile is RGB
    cmsColorSpaceSignature color_space = cmsGetColorSpace(hProfile);
    cmsBool is_rgb_profile = (color_space == cmsSigRgbData);
    
    // Check device class
    cmsProfileClassSignature device_class = cmsGetDeviceClass(hProfile);
    
    // If it's an RGB profile, test additional operations
    if (is_rgb_profile) {
        // Get profile version
        cmsFloat64Number profile_version = cmsGetProfileVersion(hProfile);
        
        // Get rendering intent
        cmsUInt32Number rendering_intent = cmsGetHeaderRenderingIntent(hProfile);
        
        // Test profile connection space
        cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);
        
        // Test gamut boundary operations if profile is output class
        if (device_class == cmsSigOutputClass) {
            // Test with different threshold values
            for (int i = 0; i < 3 && fdp.remaining_bytes() > sizeof(cmsFloat64Number); i++) {
                cmsFloat64Number alt_threshold = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
                // Ensure threshold is positive
                alt_threshold = fabs(alt_threshold);
                if (alt_threshold < 0.001) alt_threshold = 0.001;
                
                cmsFloat64Number alt_gamma = cmsDetectRGBProfileGamma(hProfile, alt_threshold);
                (void)alt_gamma;  // Use result to avoid unused variable warning
            }
            
            // Also test TAC detection on RGB output profiles
            cmsFloat64Number tac_value = cmsDetectTAC(hProfile);
            (void)tac_value;
        }
    }
    
    // Phase 4: Test with transformed profiles
    
    // Create a simple transform to modify the profile
    if (is_rgb_profile && fdp.remaining_bytes() > 100) {
        // Create XYZ profile for transform
        cmsHPROFILE hXYZ = cmsCreateXYZProfileTHR(context);
        if (hXYZ != NULL) {
            // Create transform from RGB to XYZ
            cmsHTRANSFORM xform = cmsCreateTransformTHR(context, 
                hProfile, TYPE_RGB_16,
                hXYZ, TYPE_XYZ_DBL,
                fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3),  // Intent
                fdp.ConsumeIntegral<cmsUInt32Number>()  // Flags
            );
            
            if (xform != NULL) {
                // Test gamma detection on XYZ profile (should return -1 as it's not RGB)
                cmsFloat64Number xyz_gamma = cmsDetectRGBProfileGamma(hXYZ, threshold);
                (void)xyz_gamma;
                
                // Clean up transform
                cmsDeleteTransform(xform);
            }
            
            // Clean up XYZ profile
            cmsCloseProfile(hXYZ);
        }
    }
    
    // Phase 5: Test gamma detection on Lab profiles (should return -1)
    
    if (fdp.remaining_bytes() > 50) {
        cmsHPROFILE hLab = cmsCreateLab4ProfileTHR(context, NULL);
        if (hLab != NULL) {
            cmsFloat64Number lab_gamma = cmsDetectRGBProfileGamma(hLab, threshold);
            (void)lab_gamma;  // Should be -1 (not an RGB profile)
            
            // Test TAC on Lab profile (should also return -1 or error)
            cmsFloat64Number lab_tac = cmsDetectTAC(hLab);
            (void)lab_tac;
            
            cmsCloseProfile(hLab);
        }
    }
    // Phase 6: Test with different profile types
    
    if (fdp.remaining_bytes() > 100) {
        // Create a NULL profile and test gamma detection (should return -1)
        cmsHPROFILE hNull = cmsCreateNULLProfile();
        if (hNull != NULL) {
            cmsFloat64Number null_gamma = cmsDetectRGBProfileGamma(hNull, threshold);
            (void)null_gamma;
            
            cmsCloseProfile(hNull);
        }
    }
    
    // Phase 7: Additional gamut operations
    
    // Test Lab desaturation if we have enough data
    if (fdp.remaining_bytes() >= 4 * sizeof(double)) {
        cmsCIELab lab;
        lab.L = fdp.ConsumeFloatingPoint<double>();
        lab.a = fdp.ConsumeFloatingPoint<double>();
        lab.b = fdp.ConsumeFloatingPoint<double>();
        
        double amax = fdp.ConsumeFloatingPoint<double>();
        double amin = fdp.ConsumeFloatingPoint<double>();
        double bmax = fdp.ConsumeFloatingPoint<double>();
        double bmin = fdp.ConsumeFloatingPoint<double>();
        
        // Ensure valid ranges
        amax = fabs(amax);
        amin = -fabs(amin);
        bmax = fabs(bmax);
        bmin = -fabs(bmin);
        
        cmsBool desat_result = cmsDesaturateLab(&lab, amax, amin, bmax, bmin);
        (void)desat_result;
    }
    
    // Cleanup
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
    
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_019
