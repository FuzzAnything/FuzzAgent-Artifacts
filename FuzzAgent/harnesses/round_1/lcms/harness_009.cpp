#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_009 {
// Fuzzing harness for Little CMS 2 library
// Target: CIECAM02 color appearance model operations with 0% coverage
// Strategy: Follow the exact workflow from coverage guidance:
//           1. cmsWhitePointFromTemp for viewing conditions
//           2. cmsD50_XYZ for reference white point  
//           3. cmsCIECAM02Init to initialize context
//           4. cmsCIECAM02Forward to convert CIEXYZ to perceptual attributes
//           5. cmsCIECAM02Reverse to convert back to CIEXYZ
//           6. cmsCIECAM02Done to cleanup
// Focus APIs: cmsCIECAM02Init, cmsCIECAM02Forward, cmsCIECAM02Reverse, 
//             cmsCIECAM02Done, cmsWhitePointFromTemp, cmsD50_XYZ
// Semantic uniqueness: First harness targeting CIECAM02 color appearance model,
//                      tests full forward/reverse perceptual transformation workflow
//                      under different viewing conditions and surround types.


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for:
    // - temperature (double): 8 bytes
    // - viewing conditions: whitePoint (cmsCIExyY: 3 doubles = 24 bytes), Yb, La, D_value (3 doubles = 24 bytes), surround (uint32 = 4 bytes)
    // - XYZ input for forward conversion (3 doubles = 24 bytes)
    // Total minimum: 84 bytes
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Generate white point from temperature for viewing conditions
    // Temperature range: reasonable for white points (e.g., 2000K to 15000K)
    cmsFloat64Number tempK = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(2000.0, 15000.0);
    cmsCIExyY whitePoint_xyY;
    cmsCIEXYZ whitePoint_xyz;
    bool use_temp_white = false;
    
    // cmsWhitePointFromTemp returns cmsBool (true on success)
    if (cmsWhitePointFromTemp(&whitePoint_xyY, tempK)) {
        // Successfully generated white point from temperature
        use_temp_white = true;
        // Convert xyY to XYZ for viewing conditions
        cmsxyY2XYZ(&whitePoint_xyz, &whitePoint_xyY);
    } else {
        // If temperature is invalid, use D50 as fallback
        use_temp_white = false;
    }
    
    // Step 2: Get D50 reference white point (always available)
    const cmsCIEXYZ* d50_xyz = cmsD50_XYZ();
    
    // Step 3: Prepare viewing conditions
    cmsViewingConditions vc;
    
    // Use the white point from temperature if available, otherwise use D50
    if (use_temp_white) {
        vc.whitePoint.X = whitePoint_xyz.X;
        vc.whitePoint.Y = whitePoint_xyz.Y;
        vc.whitePoint.Z = whitePoint_xyz.Z;
    } else {
        vc.whitePoint.X = d50_xyz->X;
        vc.whitePoint.Y = d50_xyz->Y;
        vc.whitePoint.Z = d50_xyz->Z;
    }
    
    // Consume viewing condition parameters from fuzzer input
    // Yb: background luminance factor (0..100 typical)
    vc.Yb = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 100.0);
    
    // La: adapting field luminance (cd/m², typical range 0..1000)
    vc.La = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1000.0);
    
    // surround: one of AVG_SURROUND, DIM_SURROUND, DARK_SURROUND, CUTSHEET_SURROUND
    cmsUInt32Number surround_type = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 4);
    vc.surround = surround_type;
    
    // D_value: degree of adaptation (-1 for D_CALCULATE, or 0..1)
    // Consume a value: -1 for D_CALCULATE, or 0..1 for fixed adaptation
    cmsFloat64Number d_choice = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-1.0, 1.0);
    if (d_choice < 0.0) {
        vc.D_value = D_CALCULATE;
    } else {
        vc.D_value = d_choice;
    }
    
    // Step 4: Initialize CIECAM02 context
    cmsHANDLE cam02 = cmsCIECAM02Init(NULL, &vc);
    if (cam02 == NULL) {
        // Initialization failed, return early
        return 0;
    }
    
    // Step 5: Test forward conversion (CIEXYZ to perceptual attributes)
    cmsCIEXYZ input_xyz;
    cmsJCh output_jch;
    
    // Consume XYZ values from fuzzer input
    // XYZ values typically in range 0..100 for Y, but can be higher
    input_xyz.X = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 200.0);
    input_xyz.Y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 200.0);
    input_xyz.Z = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 200.0);
    
    // Perform forward conversion
    cmsCIECAM02Forward(cam02, &input_xyz, &output_jch);
    
    // Step 6: Test reverse conversion (perceptual attributes back to CIEXYZ)
    cmsCIEXYZ reversed_xyz;
    cmsCIECAM02Reverse(cam02, &output_jch, &reversed_xyz);
    
    // Optional: Test with different surround types to explore more code paths
    // We can create another viewing condition with a different surround
    if (fdp.remaining_bytes() > 50) {
        cmsViewingConditions vc2;
        vc2.whitePoint.X = d50_xyz->X;
        vc2.whitePoint.Y = d50_xyz->Y;
        vc2.whitePoint.Z = d50_xyz->Z;
        vc2.Yb = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 100.0);
        vc2.La = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1000.0);
        
        // Try a different surround type
        cmsUInt32Number surround_type2 = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 4);
        vc2.surround = surround_type2;
        vc2.D_value = D_CALCULATE;
        
        cmsHANDLE cam02_2 = cmsCIECAM02Init(NULL, &vc2);
        if (cam02_2 != NULL) {
            // Test another forward conversion
            cmsCIEXYZ input_xyz2;
            cmsJCh output_jch2;
            
            input_xyz2.X = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 200.0);
            input_xyz2.Y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 200.0);
            input_xyz2.Z = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 200.0);
            
            cmsCIECAM02Forward(cam02_2, &input_xyz2, &output_jch2);
            
            // Clean up second context
            cmsCIECAM02Done(cam02_2);
        }
    }
    
    // Step 7: Cleanup
    cmsCIECAM02Done(cam02);
    
    return 0;
}

} // namespace harness_009
