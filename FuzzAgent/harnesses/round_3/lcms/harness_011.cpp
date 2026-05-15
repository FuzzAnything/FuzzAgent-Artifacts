#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_011 {
// Fuzzing harness for LittleCMS (lcms) formatter/packing functions
// This harness specifically targets formatter/packing functions from cmspack.c module
// that are currently 0% covered according to coverage analysis
// Target APIs: cmsFormatterForPCSOfProfile [0% covered, 80 undiscovered branches], 
// cmsFormatterForColorspaceOfProfile [partially covered, 77 undiscovered branches]
// MODULE: cmspack.c [current coverage: 10.75% region]
// STRATEGY: Target formatter/packing operations workflow for color transformations
// DIFFERENTIATION: Unlike existing harnesses, focus on low-level data packing/unpacking formatters used in color transformations
// API SEQUENCE: Create source and destination profiles -> Create transform -> Get PCS formatter -> Get colorspace formatter -> Execute transform with various input formats -> Cleanup


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful testing of formatter workflow
    // Minimum: parameters for profiles, intents, formats, and transform data
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create source and destination profiles as specified in guidance
    // Source: sRGB profile (commonly used source color space)
    cmsHPROFILE src_profile = cmsCreate_sRGBProfile();
    if (src_profile == NULL) {
        return 0; // Cannot proceed without source profile
    }
    
    // Destination: Lab4 profile (commonly used destination color space for PCS)
    // Use D50 white point for Lab profile
    cmsCIExyY d50;
    d50.x = 0.3457;
    d50.y = 0.3585;
    d50.Y = 1.0;
    
    cmsHPROFILE dst_profile = cmsCreateLab4Profile(&d50);
    if (dst_profile == NULL) {
        cmsCloseProfile(src_profile);
        return 0; // Cannot proceed without destination profile
    }
    
    // 2. Consume parameters for transform creation
    cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3); // 0-3 valid intents
    cmsUInt32Number flags = 0;
    
    // Randomly set some transform flags from fuzzer input
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_BLACKPOINTCOMPENSATION;
    }
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_HIGHRESPRECALC;
    }
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_LOWRESPRECALC;
    }
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_GRIDPOINTS(2); // Use reasonable grid points
    }
    
    // 3. Choose input and output formats based on consumed data
    // Format selection: 8-bit or 16-bit, floating point or integer
    uint8_t format_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    cmsUInt32Number inputFormat, outputFormat;
    
    switch (format_choice) {
        case 0:
            inputFormat = TYPE_RGB_8;      // 8-bit RGB
            outputFormat = TYPE_Lab_8;     // 8-bit Lab
            break;
        case 1:
            inputFormat = TYPE_RGB_16;     // 16-bit RGB
            outputFormat = TYPE_Lab_16;    // 16-bit Lab
            break;
        case 2:
            inputFormat = TYPE_RGB_FLT;    // Floating point RGB
            outputFormat = TYPE_Lab_FLT;   // Floating point Lab
            break;
        case 3:
            inputFormat = TYPE_RGB_DBL;    // Double precision RGB
            outputFormat = TYPE_Lab_DBL;   // Double precision Lab
            break;
        default:
            inputFormat = TYPE_RGB_8;
            outputFormat = TYPE_Lab_8;
            break;
    }
    
    // 4. Create transform between profiles
    cmsHTRANSFORM hTransform = cmsCreateTransform(src_profile, inputFormat,
                                                   dst_profile, outputFormat,
                                                   intent, flags);
    
    if (hTransform == NULL) {
        // If transform creation fails, still test formatter APIs
        // Continue to test formatter functions without transform
    } else {
        // 5. Execute transform with fuzzer-provided data if transform exists
        // Prepare input buffer based on format
        size_t pixel_count = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        size_t input_buffer_size = 0;
        
        // Calculate buffer size based on format
        // Check if format is floating point (single or double precision)
        cmsUInt32Number bytes_per_channel = T_BYTES(inputFormat);
        bool is_float = T_FLOAT(inputFormat) ? true : false;
        
        if (is_float) {
            if (bytes_per_channel == 0) {
                // Double precision: 8 bytes per channel
                input_buffer_size = pixel_count * 3 * sizeof(double);
            } else {
                // Single precision: 4 bytes per channel (bytes_per_channel should be 4)
                input_buffer_size = pixel_count * 3 * sizeof(float);
            }
        } else {
            // Integer formats
            input_buffer_size = pixel_count * 3 * bytes_per_channel;
        }
        
        // Ensure we have enough remaining data
        if (fdp.remaining_bytes() >= input_buffer_size && input_buffer_size > 0) {
            std::vector<uint8_t> input_buffer(input_buffer_size);
            std::vector<uint8_t> output_buffer(input_buffer_size * 2); // Extra space for output
            
            // Fill input buffer with fuzzer data
            auto input_bytes = fdp.ConsumeBytes<uint8_t>(input_buffer_size);
            if (input_bytes.size() == input_buffer_size) {
                memcpy(input_buffer.data(), input_bytes.data(), input_buffer_size);
                
                // Perform the transformation
                cmsDoTransform(hTransform, input_buffer.data(), output_buffer.data(), pixel_count);
            }
        }
    }
    
    // 6. PRIMARY TARGET: Get formatters for profiles (main coverage goal)
    // Consume parameters for formatter functions
    cmsUInt32Number nBytes = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 2); // 1=8-bit, 2=16-bit
    cmsBool lIsFloat = fdp.ConsumeBool() ? TRUE : FALSE;
    
    // Target API 1: cmsFormatterForPCSOfProfile (0% covered)
    cmsUInt32Number pcs_formatter = cmsFormatterForPCSOfProfile(src_profile, nBytes, lIsFloat);
    
    // Target API 2: cmsFormatterForColorspaceOfProfile (partially covered)
    cmsUInt32Number colorspace_formatter = cmsFormatterForColorspaceOfProfile(src_profile, nBytes, lIsFloat);
    
    // Also test with destination profile
    cmsUInt32Number dst_pcs_formatter = cmsFormatterForPCSOfProfile(dst_profile, nBytes, lIsFloat);
    cmsUInt32Number dst_colorspace_formatter = cmsFormatterForColorspaceOfProfile(dst_profile, nBytes, lIsFloat);
    
    // 7. Test formatter properties and utilities if available
    // Check if formatters are valid (non-zero indicates valid formatter)
    bool pcs_formatter_valid = (pcs_formatter != 0);
    bool colorspace_formatter_valid = (colorspace_formatter != 0);
    
    // Test with different nBytes values
    for (int i = 1; i <= 2; i++) {
        cmsFormatterForPCSOfProfile(src_profile, i, lIsFloat);
        cmsFormatterForColorspaceOfProfile(src_profile, i, lIsFloat);
        
        cmsFormatterForPCSOfProfile(dst_profile, i, !lIsFloat); // Test opposite float flag
        cmsFormatterForColorspaceOfProfile(dst_profile, i, !lIsFloat);
    }
    
    // 8. Additional testing: Get profile information that might relate to formatters
    cmsColorSpaceSignature src_space = cmsGetColorSpace(src_profile);
    cmsColorSpaceSignature dst_space = cmsGetColorSpace(dst_profile);
    
    cmsColorSpaceSignature src_pcs = cmsGetPCS(src_profile);
    cmsColorSpaceSignature dst_pcs = cmsGetPCS(dst_profile);
    
    // Get channel counts (used internally by formatter functions)
    cmsInt32Number src_channels = cmsChannelsOfColorSpace(src_space);
    cmsInt32Number dst_channels = cmsChannelsOfColorSpace(dst_space);
    
    // 9. Cleanup as specified in guidance
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
    
    cmsCloseProfile(dst_profile);
    cmsCloseProfile(src_profile);
    
    return 0;
}

} // namespace harness_011
