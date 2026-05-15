#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_003 {
// Fuzzing harness for Little CMS 2 library
// Target: Device link profile creation operations
// Strategy: Follow the workflow: create profiles → create transform → convert to device link 
//           → create specialized device links → cleanup
// Focus APIs: cmsTransform2DeviceLink (395 undiscovered branches), 
//             cmsCreateInkLimitingDeviceLinkTHR (209 undiscovered),
//             cmsCreateLinearizationDeviceLinkTHR (170 undiscovered)
// Semantic uniqueness: Specifically targets device link profile creation workflow,
//                      which is currently 0% covered in cmsvirt.c module


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for various parameters
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create context for THR functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) return 0;
    
    // Step 2: Create source and destination profiles (as per guidance)
    cmsHPROFILE src_profile = cmsCreate_sRGBProfileTHR(context);
    cmsHPROFILE dst_profile = cmsCreateXYZProfileTHR(context);
    
    if (src_profile == NULL || dst_profile == NULL) {
        if (src_profile) cmsCloseProfile(src_profile);
        if (dst_profile) cmsCloseProfile(dst_profile);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Step 3: Consume parameters from fuzzer input for transform creation
    cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
    cmsUInt32Number flags = fdp.ConsumeIntegral<cmsUInt32Number>();
    
    // Map intent to Little CMS intents
    cmsUInt32Number cms_intent;
    switch (intent) {
        case 0: cms_intent = INTENT_PERCEPTUAL; break;
        case 1: cms_intent = INTENT_RELATIVE_COLORIMETRIC; break;
        case 2: cms_intent = INTENT_SATURATION; break;
        case 3: cms_intent = INTENT_ABSOLUTE_COLORIMETRIC; break;
        default: cms_intent = INTENT_PERCEPTUAL; break;
    }
    
    // Step 4: Create transform between profiles
    cmsHTRANSFORM transform = cmsCreateTransformTHR(context,
                                                    src_profile,
                                                    TYPE_RGB_8,      // Input format: RGB 8-bit
                                                    dst_profile,
                                                    TYPE_XYZ_DBL,    // Output format: XYZ double
                                                    cms_intent,
                                                    flags);
    
    if (transform == NULL) {
        cmsCloseProfile(src_profile);
        cmsCloseProfile(dst_profile);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Step 5: Convert transform to device link profile (main target - 395 undiscovered branches)
    cmsFloat64Number version = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(2.0, 4.0);
    cmsUInt32Number devicelink_flags = fdp.ConsumeIntegral<cmsUInt32Number>();
    
    cmsHPROFILE device_link = cmsTransform2DeviceLink(transform, version, devicelink_flags);
    
    // Clean up device link if created
    if (device_link != NULL) {
        cmsCloseProfile(device_link);
    }
    
    // Step 6: Create ink-limiting device link (209 undiscovered branches)
    cmsFloat64Number ink_limit = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    cmsHPROFILE ink_limiting_link = cmsCreateInkLimitingDeviceLinkTHR(context,
                                                                      cmsSigRgbData,  // RGB color space
                                                                      ink_limit);
    
    // Clean up ink-limiting device link if created
    if (ink_limiting_link != NULL) {
        cmsCloseProfile(ink_limiting_link);
    }
    
    // Step 7: Create linearization device link (170 undiscovered branches)
    // We need tone curves for linearization. Create simple gamma curves.
    cmsToneCurve* gamma_curve = cmsBuildGamma(context, 2.2); // Gamma 2.2 curve
    cmsToneCurve* transfer_functions[3] = {gamma_curve, gamma_curve, gamma_curve};
    
    cmsHPROFILE linearization_link = cmsCreateLinearizationDeviceLinkTHR(context,
                                                                         cmsSigRgbData,
                                                                         transfer_functions);
    
    // Clean up linearization device link and tone curves if created
    if (linearization_link != NULL) {
        cmsCloseProfile(linearization_link);
    }
    if (gamma_curve != NULL) {
        cmsFreeToneCurve(gamma_curve);
    }
    
    // Step 8: Cleanup (as per guidance sequence)
    cmsDeleteTransform(transform);
    cmsCloseProfile(src_profile);
    cmsCloseProfile(dst_profile);
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_003
