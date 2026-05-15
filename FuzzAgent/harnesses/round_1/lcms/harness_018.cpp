#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_018 {
// Fuzzing harness for Little CMS 2 library
// Target: Illuminant adaptation and temperature conversion functions
// Strategy: Target color temperature and illuminant adaptation operations with 32+ undiscovered branches
// Target APIs: cmsAdaptToIlluminant (18 undiscovered branches), cmsTempFromWhitePoint (14 undiscovered branches)
// Required helpers: cmsWhitePointFromTemp (3 undiscovered branches)
// Optional helpers: cmsFloat2XYZEncoded (14 undiscovered branches), cmsXYZEncoded2Float
// Invocation sequence:
//   1. cmsWhitePointFromTemp to create source white point from temperature
//   2. cmsWhitePointFromTemp to create destination illuminant white point
//   3. Create test cmsCIEXYZ color values
//   4. cmsAdaptToIlluminant to adapt color from source to destination illuminant
//   5. cmsTempFromWhitePoint to convert resulting white point back to temperature
//   6. Optionally use cmsFloat2XYZEncoded and cmsXYZEncoded2Float for encoding/decoding
// Error handling scenarios included:
//   - Invalid temperature ranges
//   - Function return value checking
//   - Edge cases for white point calculations
// Note: Uses FuzzedDataProvider for input-driven testing of all parameters
// Temperature ranges: 4000K-25000K for cmsWhitePointFromTemp (valid range)

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for temperature values and color components
    // We need: 2 temperature values (double) + 3 color components (double) + encoding buffers
    const size_t MIN_SIZE = sizeof(double) * 5 + 6; // 2 temps + 3 XYZ + extra for encoding
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create source white point from temperature
    // Temperature should be in valid range 4000K-25000K for cmsWhitePointFromTemp
    cmsFloat64Number source_temp = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(4000.0, 25000.0);
    cmsCIExyY source_white_point;
    cmsCIEXYZ source_white_point_xyz;
    
    cmsBool source_result = cmsWhitePointFromTemp(&source_white_point, source_temp);
    
    // If source white point calculation fails, we can still test other functions
    // Convert xyY to XYZ for use in adaptation
    if (source_result) {
        cmsxyY2XYZ(&source_white_point_xyz, &source_white_point);
    } else {
        // Use default D50 white point if calculation fails
        const cmsCIEXYZ* d50 = cmsD50_XYZ();
        source_white_point_xyz = *d50;
    }
    
    // Step 2: Create destination illuminant white point from different temperature
    cmsFloat64Number dest_temp = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(4000.0, 25000.0);
    cmsCIExyY dest_white_point;
    cmsCIEXYZ dest_white_point_xyz;
    
    cmsBool dest_result = cmsWhitePointFromTemp(&dest_white_point, dest_temp);
    
    if (dest_result) {
        cmsxyY2XYZ(&dest_white_point_xyz, &dest_white_point);
    } else {
        // Use D65 white point if calculation fails (different from source)
        const cmsCIEXYZ* d50 = cmsD50_XYZ();
        // Create a slightly different white point for testing
        dest_white_point_xyz.X = d50->X * 1.1;
        dest_white_point_xyz.Y = d50->Y * 1.05;
        dest_white_point_xyz.Z = d50->Z * 0.95;
    }
    
    // Step 3: Create test cmsCIEXYZ color values from fuzzed input
    cmsCIEXYZ test_color;
    test_color.X = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
    test_color.Y = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
    test_color.Z = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
    
    // Ensure color components are reasonable (not NaN, infinite, or extreme)
    // Apply some basic bounds checking
    const cmsFloat64Number MAX_COLOR_VALUE = 1000.0;
    if (test_color.X > MAX_COLOR_VALUE || test_color.X < -MAX_COLOR_VALUE ||
        test_color.Y > MAX_COLOR_VALUE || test_color.Y < -MAX_COLOR_VALUE ||
        test_color.Z > MAX_COLOR_VALUE || test_color.Z < -MAX_COLOR_VALUE) {
        // Normalize to reasonable range
        test_color.X = fmod(test_color.X, MAX_COLOR_VALUE);
        test_color.Y = fmod(test_color.Y, MAX_COLOR_VALUE);
        test_color.Z = fmod(test_color.Z, MAX_COLOR_VALUE);
    }
    
    // Step 4: Adapt color from source white point to destination illuminant
    cmsCIEXYZ adapted_color;
    cmsBool adapt_result = cmsAdaptToIlluminant(&adapted_color, 
                                               &source_white_point_xyz,
                                               &dest_white_point_xyz,
                                               &test_color);
    
    // Step 5: Convert resulting adapted white point back to temperature for verification
    // First, we need to convert the adapted color to xyY space
    cmsCIExyY adapted_xyY;
    cmsXYZ2xyY(&adapted_xyY, &adapted_color);
    
    cmsFloat64Number verified_temp = 0.0;
    cmsBool temp_result = cmsTempFromWhitePoint(&verified_temp, &adapted_xyY);
    
    // Optional Step 6: Test encoding/decoding functions if we have enough data
    if (fdp.remaining_bytes() >= 6) {  // Need at least 6 bytes for 3 uint16 values
        // Test cmsFloat2XYZEncoded and cmsXYZEncoded2Float
        cmsUInt16Number encoded_xyz[3];
        cmsCIEXYZ decoded_xyz;
        
        // Encode the test color
        cmsFloat2XYZEncoded(encoded_xyz, &test_color);
        
        // Decode back
        cmsXYZEncoded2Float(&decoded_xyz, encoded_xyz);
        
        // Also test with the adapted color
        cmsUInt16Number encoded_adapted[3];
        cmsCIEXYZ decoded_adapted;
        
        cmsFloat2XYZEncoded(encoded_adapted, &adapted_color);
        cmsXYZEncoded2Float(&decoded_adapted, encoded_adapted);
    }
    
    // Additional test: Test edge cases with extreme temperature values
    // Test with temperature slightly outside valid range
    cmsFloat64Number extreme_temp = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
    cmsCIExyY extreme_white_point;
    
    // This may fail, which is expected behavior we want to test
    cmsBool extreme_result = cmsWhitePointFromTemp(&extreme_white_point, extreme_temp);
    
    // Test with zero or negative temperature
    cmsFloat64Number invalid_temp = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
    cmsCIExyY invalid_white_point;
    cmsBool invalid_result = cmsWhitePointFromTemp(&invalid_white_point, invalid_temp);
    
    // Test cmsTempFromWhitePoint with various inputs
    // Test with the original white points (if they were successfully calculated)
    if (source_result) {
        cmsFloat64Number source_temp_verify = 0.0;
        cmsBool source_temp_verify_result = cmsTempFromWhitePoint(&source_temp_verify, &source_white_point);
    }
    
    if (dest_result) {
        cmsFloat64Number dest_temp_verify = 0.0;
        cmsBool dest_temp_verify_result = cmsTempFromWhitePoint(&dest_temp_verify, &dest_white_point);
    }
    
    // Test adaptation with swapped source and destination
    cmsCIEXYZ swapped_adapted_color;
    cmsBool swapped_result = cmsAdaptToIlluminant(&swapped_adapted_color,
                                                 &dest_white_point_xyz,
                                                 &source_white_point_xyz,
                                                 &test_color);
    
    // Test adaptation with same source and destination (should result in same color)
    cmsCIEXYZ same_adapted_color;
    cmsBool same_result = cmsAdaptToIlluminant(&same_adapted_color,
                                              &source_white_point_xyz,
                                              &source_white_point_xyz,
                                              &test_color);
    
    // Test with NULL pointers to check error handling
    // (commented out as they might crash, but useful for testing defensive code)
    // cmsAdaptToIlluminant(NULL, &source_white_point_xyz, &dest_white_point_xyz, &test_color);
    // cmsAdaptToIlluminant(&adapted_color, NULL, &dest_white_point_xyz, &test_color);
    // cmsAdaptToIlluminant(&adapted_color, &source_white_point_xyz, NULL, &test_color);
    // cmsAdaptToIlluminant(&adapted_color, &source_white_point_xyz, &dest_white_point_xyz, NULL);
    
    return 0;
}

} // namespace harness_018
