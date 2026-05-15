#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_004 {
// Fuzzing harness for Little CMS 2 library
// Target: Device link profile creation APIs with 665+ undiscovered branches
// Strategy: Target non-THR versions of device link creation APIs and BCHSWabstractProfile
// Focus APIs: cmsCreateInkLimitingDeviceLink (206 undiscovered branches),
//             cmsCreateLinearizationDeviceLink (163 undiscovered branches),
//             cmsCreateBCHSWabstractProfile (148 undiscovered branches),
//             cmsCreateBCHSWabstractProfileTHR (148 undiscovered branches)
// Semantic uniqueness: Uses non-THR API versions (different from harness_003),
//                      includes BCHSWabstractProfile creation,
//                      follows exact guidance sequence: context -> profiles -> device links -> cleanup
// Input processing: Uses FuzzedDataProvider to consume all parameters from fuzzer input


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for various parameters
    // We need: color space (4 bytes), limit (8 bytes), nLUTPoints (4 bytes), 
    // Bright/Contrast/Hue/Saturation (4*8=32 bytes), TempSrc/TempDest (2*4=8 bytes)
    // Plus some extra for safety
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize - no context needed for non-THR functions
    // Non-THR functions use NULL context internally
    
    // Step 2: Create source and destination profiles as per guidance
    // Create XYZ profile (source) and Lab2 profile (destination)
    cmsHPROFILE src_profile = cmsCreateXYZProfile();
    cmsHPROFILE dst_profile = cmsCreateLab2Profile(NULL); // NULL white point uses D50
    
    if (src_profile == NULL || dst_profile == NULL) {
        if (src_profile) cmsCloseProfile(src_profile);
        if (dst_profile) cmsCloseProfile(dst_profile);
        return 0;
    }
    
    // Also create sRGB profile for additional testing
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfile();
    
    // Step 3: Consume parameters for device link creation
    
    // 3.1: Consume color space signature for device links
    // Use a subset of common color spaces
    uint32_t color_space_choice = fdp.ConsumeIntegralInRange<uint32_t>(0, 5);
    cmsColorSpaceSignature color_space;
    switch (color_space_choice) {
        case 0: color_space = cmsSigRgbData; break;      // RGB
        case 1: color_space = cmsSigCmykData; break;     // CMYK  
        case 2: color_space = cmsSigGrayData; break;     // Gray
        case 3: color_space = cmsSigLabData; break;      // Lab
        case 4: color_space = cmsSigXYZData; break;      // XYZ
        case 5: color_space = cmsSigYCbCrData; break;    // YCbCr
        default: color_space = cmsSigRgbData; break;
    }
    
    // 3.2: Create ink-limiting device link (206 undiscovered branches)
    // Note: cmsCreateInkLimitingDeviceLink only supports CMYK color space
    if (color_space == cmsSigCmykData) {
        cmsFloat64Number ink_limit = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(1.0, 400.0);
        cmsHPROFILE ink_limiting_link = cmsCreateInkLimitingDeviceLink(color_space, ink_limit);
        
        // Clean up if created
        if (ink_limiting_link != NULL) {
            cmsCloseProfile(ink_limiting_link);
        }
    }
    // 3.3: Create linearization device link (163 undiscovered branches)
    // Need tone curves for linearization. Create simple gamma curves.
    // For fuzzing, we can create a context to build tone curves
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context != NULL) {
        // Create a gamma curve with fuzzed gamma value
        cmsFloat64Number gamma = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 5.0);
        cmsToneCurve* gamma_curve = cmsBuildGamma(context, gamma);
        
        if (gamma_curve != NULL) {
            // Get the number of channels for the current color space
            int nChannels = cmsChannelsOfColorSpace(color_space);
            if (nChannels > 0 && nChannels <= 16) { // Reasonable upper bound for color space channels
                // Dynamically allocate transfer functions array based on actual channel count
                cmsToneCurve** transfer_functions = new cmsToneCurve*[nChannels];
                for (int i = 0; i < nChannels; i++) {
                    transfer_functions[i] = gamma_curve;
                }
                
                cmsHPROFILE linearization_link = cmsCreateLinearizationDeviceLink(color_space,
                                                                                  transfer_functions);
                
                // Clean up if created
                if (linearization_link != NULL) {
                    cmsCloseProfile(linearization_link);
                }
                
                delete[] transfer_functions;
            }
            
            cmsFreeToneCurve(gamma_curve);
        }
        // 3.4: Create BCHSWabstractProfileTHR (148 undiscovered branches) - needs context
        cmsUInt32Number nLUTPoints = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 256);
        cmsFloat64Number Bright = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
        cmsFloat64Number Contrast = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
        cmsFloat64Number Hue = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-180.0, 180.0);
        cmsFloat64Number Saturation = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
        cmsUInt32Number TempSrc = fdp.ConsumeIntegral<cmsUInt32Number>();
        cmsUInt32Number TempDest = fdp.ConsumeIntegral<cmsUInt32Number>();
        
        cmsHPROFILE bchsw_profile_thr = cmsCreateBCHSWabstractProfileTHR(context,
                                                                        nLUTPoints,
                                                                        Bright,
                                                                        Contrast,
                                                                        Hue,
                                                                        Saturation,
                                                                        TempSrc,
                                                                        TempDest);
        
        // Clean up if created
        if (bchsw_profile_thr != NULL) {
            cmsCloseProfile(bchsw_profile_thr);
        }
        
        cmsDeleteContext(context);
    }
    
    // 3.5: Create BCHSWabstractProfile (148 undiscovered branches) - non-THR version
    // Need to consume more parameters for second call
    if (fdp.remaining_bytes() >= 44) { // 4 + 8*4 + 4*2 = 44 bytes
        cmsUInt32Number nLUTPoints2 = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 256);
        cmsFloat64Number Bright2 = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
        cmsFloat64Number Contrast2 = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
        cmsFloat64Number Hue2 = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-180.0, 180.0);
        cmsFloat64Number Saturation2 = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
        cmsUInt32Number TempSrc2 = fdp.ConsumeIntegral<cmsUInt32Number>();
        cmsUInt32Number TempDest2 = fdp.ConsumeIntegral<cmsUInt32Number>();
        
        cmsHPROFILE bchsw_profile = cmsCreateBCHSWabstractProfile(nLUTPoints2,
                                                                 Bright2,
                                                                 Contrast2,
                                                                 Hue2,
                                                                 Saturation2,
                                                                 TempSrc2,
                                                                 TempDest2);
        
        // Clean up if created
        if (bchsw_profile != NULL) {
            cmsCloseProfile(bchsw_profile);
        }
    }
    
    // Step 4: Cleanup (as per guidance sequence)
    cmsCloseProfile(src_profile);
    cmsCloseProfile(dst_profile);
    if (srgb_profile != NULL) {
        cmsCloseProfile(srgb_profile);
    }
    
    return 0;
}

} // namespace harness_004
