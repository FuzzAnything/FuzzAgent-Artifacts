#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

namespace harness_005 {
// Fuzzing harness for Little CMS (lcms) library
// Target: CIECAM02 color appearance model functions (cmscam02.c module)
// Functions: cmsCIECAM02Init, cmsCIECAM02Forward, cmsCIECAM02Reverse, cmsCIECAM02Done
// Coverage goal: Target completely uncovered CIECAM02 module (0% line, function, and branch coverage)
// Lifecycle sequence: Create context -> Initialize CIECAM02 -> Forward transformation -> Reverse transformation -> Cleanup
// Semantically different from:
//   harness_000.cpp - profile parsing and basic transformations
//   harness_001.cpp - device link creation and virtual profiles
//   harness_002.cpp - PostScript generation
//   harness_003.cpp - CGATS parsing and manipulation
//   harness_004.cpp - Cube file device link creation and file I/O


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for viewing conditions and test data
    // We need: 3 doubles for whitePoint (24 bytes), 5 doubles for other params (40 bytes), 
    // plus at least 1 byte for surround type = 65 bytes minimum
    const size_t MIN_SIZE = sizeof(double) * 8 + sizeof(uint32_t) + 1;
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) return 0;
    
    // Step 2: Prepare viewing conditions for CIECAM02 initialization
    cmsViewingConditions vc;
    
    // Consume white point data (XYZ coordinates)
    vc.whitePoint.X = fdp.ConsumeFloatingPoint<double>();
    vc.whitePoint.Y = fdp.ConsumeFloatingPoint<double>();
    vc.whitePoint.Z = fdp.ConsumeFloatingPoint<double>();
    
    // Consume other viewing condition parameters
    vc.Yb = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);      // Background luminance factor
    vc.La = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);    // Adapting luminance
    vc.D_value = fdp.ConsumeFloatingPointInRange<double>(-1.0, 2.0);  // D can be -1 (D_CALCULATE) or 0-2
    
    // Consume surround type (valid values: AVG_SURROUND=1, DIM_SURROUND=2, DARK_SURROUND=3, CUTSHEET_SURROUND=4)
    uint32_t surround_raw = fdp.ConsumeIntegral<uint32_t>();
    // Map to valid surround types
    if (surround_raw % 4 == 0) vc.surround = AVG_SURROUND;
    else if (surround_raw % 4 == 1) vc.surround = DIM_SURROUND;
    else if (surround_raw % 4 == 2) vc.surround = DARK_SURROUND;
    else vc.surround = CUTSHEET_SURROUND;
    
    // Step 3: Initialize CIECAM02 model
    cmsHANDLE hModel = cmsCIECAM02Init(context, &vc);
    if (hModel == NULL) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Step 4: Test forward transformation (XYZ to appearance correlates)
    if (fdp.remaining_bytes() >= sizeof(double) * 3) {
        cmsCIEXYZ input_xyz;
        cmsJCh output_jch;
        
        // Consume input XYZ values
        input_xyz.X = fdp.ConsumeFloatingPoint<double>();
        input_xyz.Y = fdp.ConsumeFloatingPoint<double>();
        input_xyz.Z = fdp.ConsumeFloatingPoint<double>();
        
        // Perform forward transformation
        cmsCIECAM02Forward(hModel, &input_xyz, &output_jch);
        
        // Step 5: Test reverse transformation (appearance correlates to XYZ)
        // using the output from forward transform as input
        if (fdp.remaining_bytes() >= 1) {
            // Optionally modify the JCh values with fuzzed data
            // to test reverse transformation with varied inputs
            uint8_t modify_flags = fdp.ConsumeIntegral<uint8_t>();
            
            if (modify_flags & 0x01) {
                // Modify J (lightness) value
                output_jch.J = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
            }
            if ((modify_flags & 0x02) && fdp.remaining_bytes() >= sizeof(double)) {
                // Modify C (chroma) value
                output_jch.C = fdp.ConsumeFloatingPointInRange<double>(0.0, 150.0);
            }
            if ((modify_flags & 0x04) && fdp.remaining_bytes() >= sizeof(double)) {
                // Modify h (hue angle) value
                output_jch.h = fdp.ConsumeFloatingPointInRange<double>(0.0, 360.0);
            }
            
            cmsCIEXYZ reversed_xyz;
            
            // Perform reverse transformation
            cmsCIECAM02Reverse(hModel, &output_jch, &reversed_xyz);
            
            // Test with additional random JCh values if we have enough data
            int additional_tests = fdp.ConsumeIntegralInRange<int>(0, 3);
            for (int i = 0; i < additional_tests && fdp.remaining_bytes() >= sizeof(double) * 3; i++) {
                cmsJCh random_jch;
                random_jch.J = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
                random_jch.C = fdp.ConsumeFloatingPointInRange<double>(0.0, 150.0);
                random_jch.h = fdp.ConsumeFloatingPointInRange<double>(0.0, 360.0);
                
                cmsCIEXYZ random_output;
                cmsCIECAM02Reverse(hModel, &random_jch, &random_output);
            }
        }
    }
    
    // Step 6: Test forward transformation with random XYZ values
    int random_forward_tests = fdp.ConsumeIntegralInRange<int>(0, 5);
    for (int i = 0; i < random_forward_tests && fdp.remaining_bytes() >= sizeof(double) * 3; i++) {
        cmsCIEXYZ random_xyz;
        cmsJCh random_jch;
        
        random_xyz.X = fdp.ConsumeFloatingPoint<double>();
        random_xyz.Y = fdp.ConsumeFloatingPoint<double>();
        random_xyz.Z = fdp.ConsumeFloatingPoint<double>();
        
        cmsCIECAM02Forward(hModel, &random_xyz, &random_jch);
    }
    
    // Step 7: Cleanup - follow proper lifecycle
    cmsCIECAM02Done(hModel);
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_005
