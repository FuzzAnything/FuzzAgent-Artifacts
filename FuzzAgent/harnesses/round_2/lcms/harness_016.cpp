#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include "lcms2.h"

namespace harness_016 {
/*
 * Fuzzing harness for LittleCMS2 library
 * Targets: Alpha compositing and gamma correction APIs in cmsalpha.c and cmsgamma.c modules
 * Semantic domain: Alpha channel handling, gamma encoding/decoding, tone curve operations
 * Previous harness semantic domains covered:
 *   000-basic profile parsing/transformation
 *   001-proofing/PostScript
 *   002-virtual profiles/gamut
 *   003-CGATS/IT8 basic
 *   004-CIECAM02 (appearance model, different from basic colorimetry)
 *   005-TAC detection/sampling
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
 * This harness focuses on: Alpha channel format handling, gamma curve creation/evaluation,
 *   parametric tone curves, curve reversal/joining, and transformations with alpha channels
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
    
    // Phase 1: Gamma and tone curve operations
    
    // 1.1 Create gamma curves with different gamma values
    cmsFloat64Number gamma_value = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 5.0);
    cmsToneCurve* gamma_curve = cmsBuildGamma(context, gamma_value);
    
    // 1.2 Create parametric tone curves with different types and parameters
    cmsInt32Number param_type = fdp.ConsumeIntegralInRange<cmsInt32Number>(1, 10);
    cmsFloat64Number param_values[7] = {0};  // Max 7 parameters
    
    // Consume appropriate number of parameters based on type
    cmsUInt32Number num_params = 1;  // Default
    if (param_type == 2 || param_type == 3 || param_type == 4 || 
        param_type == 5 || param_type == 6 || param_type == 7 || param_type == 8) {
        // These types need more parameters
        num_params = (param_type == 2 || param_type == 6) ? 3 : 
                    (param_type == 3 || param_type == 4) ? 4 :
                    (param_type == 5 || param_type == 7) ? 5 : 5; // type 8 also 5
    }
    
    for (cmsUInt32Number i = 0; i < num_params; i++) {
        param_values[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-10.0, 10.0);
    }
    
    cmsToneCurve* param_curve = cmsBuildParametricToneCurve(context, param_type, param_values);
    
    // 1.3 Create tabulated tone curves (both 16-bit and float)
    cmsUInt32Number tab_entries = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 100);
    std::vector<cmsUInt16Number> tab_values_16(tab_entries);
    std::vector<cmsFloat32Number> tab_values_float(tab_entries);
    
    for (cmsUInt32Number i = 0; i < tab_entries; i++) {
        tab_values_16[i] = fdp.ConsumeIntegral<cmsUInt16Number>();
        tab_values_float[i] = fdp.ConsumeFloatingPoint<cmsFloat32Number>();
    }
    
    cmsToneCurve* tab_curve_16 = cmsBuildTabulatedToneCurve16(context, tab_entries, tab_values_16.data());
    cmsToneCurve* tab_curve_float = cmsBuildTabulatedToneCurveFloat(context, tab_entries, tab_values_float.data());
    
    // Phase 2: Evaluate tone curves with various input values
    
    if (gamma_curve != NULL) {
        // Evaluate with float and 16-bit inputs
        cmsFloat32Number float_input = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
        cmsFloat32Number float_result = cmsEvalToneCurveFloat(gamma_curve, float_input);
        
        cmsUInt16Number int_input = fdp.ConsumeIntegral<cmsUInt16Number>();
        cmsUInt16Number int_result = cmsEvalToneCurve16(gamma_curve, int_input);
        
        // Reverse the gamma curve
        cmsToneCurve* reversed_gamma = cmsReverseToneCurve(gamma_curve);
        if (reversed_gamma != NULL) {
            cmsFloat32Number reversed_result = cmsEvalToneCurveFloat(reversed_gamma, float_result);
            cmsFreeToneCurve(reversed_gamma);
        }
    }
    
    if (param_curve != NULL && gamma_curve != NULL) {
        // Join curves
        cmsToneCurve* joined_curve = cmsJoinToneCurve(context, param_curve, gamma_curve, tab_entries);
        if (joined_curve != NULL) {
            cmsFloat32Number join_test = fdp.ConsumeFloatingPoint<cmsFloat32Number>();
            cmsEvalToneCurveFloat(joined_curve, join_test);
            cmsFreeToneCurve(joined_curve);
        }
    }
    // Phase 3: Create profiles with alpha channels and test transformations
    
    // Only proceed if we have a gamma curve
    if (gamma_curve != NULL) {
        // Create primaries structure for RGB profile
        cmsCIExyYTRIPLE primaries;
        primaries.Red.x = 0.6400; primaries.Red.y = 0.3300; primaries.Red.Y = 1.0;
        primaries.Green.x = 0.3000; primaries.Green.y = 0.6000; primaries.Green.Y = 1.0;
        primaries.Blue.x = 0.1500; primaries.Blue.y = 0.0600; primaries.Blue.Y = 1.0;
        
        cmsCIExyY whitePoint;
        whitePoint.x = 0.3127; whitePoint.y = 0.3290; whitePoint.Y = 1.0;
        
        // Create transfer function array
        cmsToneCurve* transferFunctions[3] = {gamma_curve, gamma_curve, gamma_curve};
        
        // Create simple RGB profile
        cmsHPROFILE rgb_profile = cmsCreateRGBProfileTHR(context,
            &whitePoint, &primaries, transferFunctions);
        cmsUInt32Number input_format = 0;
        cmsUInt32Number output_format = 0;
        
        // Consume format flags for testing
        cmsUInt8Number format_choice = fdp.ConsumeIntegral<uint8_t>();
        
        // Test different pixel formats with alpha
        // TYPE_RGB_8 = (COLORSPACE_SH(PT_RGB)|EXTRA_SH(0)|CHANNELS_SH(3)|BYTES_SH(1))
        // TYPE_RGBA_8 = (COLORSPACE_SH(PT_RGB)|EXTRA_SH(1)|CHANNELS_SH(3)|BYTES_SH(1))
        // TYPE_ARGB_8 = (COLORSPACE_SH(PT_RGB)|EXTRA_SH(1)|CHANNELS_SH(3)|BYTES_SH(1)|FLOAT_SH(0)|EXTRA_SH(1)|SWAPFIRST_SH(1))
        
        // Basic RGB formats
        switch (format_choice % 6) {
            case 0:
                input_format = TYPE_RGB_8;
                output_format = TYPE_RGB_8;
                break;
            case 1:
                input_format = TYPE_RGBA_8;
                output_format = TYPE_RGBA_8;
                break;
            case 2:
                input_format = TYPE_RGB_16;
                output_format = TYPE_RGB_16;
                break;
            case 3:
                input_format = TYPE_RGBA_16;
                output_format = TYPE_RGBA_16;
                break;
            case 4:
                input_format = TYPE_RGB_FLT;
                output_format = TYPE_RGB_FLT;
                break;
            case 5:
                input_format = TYPE_RGBA_FLT;
                output_format = TYPE_RGBA_FLT;
                break;
        }
        
        // Create transform
        cmsHTRANSFORM transform = cmsCreateTransformTHR(context,
            rgb_profile, input_format,
            rgb_profile, output_format,
            INTENT_PERCEPTUAL, 0);
        
        if (transform != NULL) {
            // Prepare test buffers based on format
            size_t pixel_size = 0;
            cmsUInt32Number channels = T_CHANNELS(input_format);
            cmsUInt32Number extra = T_EXTRA(input_format);
            cmsUInt32Number bytes = T_BYTES(input_format);
            // For float formats: BYTES_SH(4) for float, BYTES_SH(0) for double
            if (bytes == 0 && T_FLOAT(input_format)) {
                bytes = sizeof(cmsFloat64Number);  // Double precision
            } else if (bytes == 4 && T_FLOAT(input_format)) {
                bytes = sizeof(cmsFloat32Number);  // Single precision float
            }
            
            pixel_size = (channels + extra) * bytes;
            
            if (pixel_size > 0 && pixel_size <= 64) {  // Reasonable size
                std::vector<uint8_t> input_buffer(pixel_size * 4);  // 4 pixels
                std::vector<uint8_t> output_buffer(pixel_size * 4);
                
                // Fill input buffer with fuzzed data
                std::vector<uint8_t> pixel_data = fdp.ConsumeBytes<uint8_t>(input_buffer.size());
                if (pixel_data.size() == input_buffer.size()) {
                    memcpy(input_buffer.data(), pixel_data.data(), input_buffer.size());
                    
                    // Perform transformation
                    cmsDoTransform(transform, input_buffer.data(), output_buffer.data(), 4);
                }
            }
            
            cmsDeleteTransform(transform);
        }
        
        cmsCloseProfile(rgb_profile);
    }
    
    // Phase 4: Test curve properties and analysis functions
    
    if (gamma_curve != NULL) {
        cmsBool is_linear = cmsIsToneCurveLinear(gamma_curve);
        cmsBool is_monotonic = cmsIsToneCurveMonotonic(gamma_curve);
        cmsBool is_descending = cmsIsToneCurveDescending(gamma_curve);
        cmsBool is_multisegment = cmsIsToneCurveMultisegment(gamma_curve);
        cmsInt32Number parametric_type = cmsGetToneCurveParametricType(gamma_curve);
        
        // Estimate gamma
        cmsFloat64Number precision = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.001, 0.1);
        cmsFloat64Number estimated_gamma = cmsEstimateGamma(gamma_curve, precision);
        
        // Smooth curve if it's tabulated
        if (tab_curve_16 != NULL) {
            cmsFloat64Number lambda = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 10.0);
            cmsBool smooth_result = cmsSmoothToneCurve(tab_curve_16, lambda);
            (void)smooth_result;
        }
    }
    
    // Cleanup
    if (gamma_curve != NULL) cmsFreeToneCurve(gamma_curve);
    if (param_curve != NULL) cmsFreeToneCurve(param_curve);
    if (tab_curve_16 != NULL) cmsFreeToneCurve(tab_curve_16);
    if (tab_curve_float != NULL) cmsFreeToneCurve(tab_curve_float);
    
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_016
