#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_004 {
// Fuzzing harness for LittleCMS2 library
// Targets CIECAM02 color appearance model APIs
// harness_004.cpp - Focus on cmsCIECAM02Init, cmsCIECAM02Done, cmsCIECAM02Forward, cmsCIECAM02Reverse


// Main fuzzer entry point for CIECAM02 color appearance model
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        return 0;  // Need minimum data for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create viewing conditions from fuzzed data
    cmsViewingConditions vc;
    
    // Consume white point XYZ values
    vc.whitePoint.X = fdp.ConsumeFloatingPoint<double>();
    vc.whitePoint.Y = fdp.ConsumeFloatingPoint<double>();
    vc.whitePoint.Z = fdp.ConsumeFloatingPoint<double>();
    
    // Consume Yb (luminance of background)
    vc.Yb = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    
    // Consume La (luminance of adapting field)
    vc.La = fdp.ConsumeFloatingPointInRange<double>(0.0, 400.0);
    
    // Consume surround type (0-4, but use defined constants)
    uint32_t surround_val = fdp.ConsumeIntegralInRange<uint32_t>(0, 10);
    switch (surround_val % 5) {
        case 0: vc.surround = AVG_SURROUND; break;
        case 1: vc.surround = DIM_SURROUND; break;
        case 2: vc.surround = DARK_SURROUND; break;
        case 3: vc.surround = CUTSHEET_SURROUND; break;
        default: vc.surround = AVG_SURROUND; break;
    }
    
    // Consume D_value (degree of adaptation) or use D_CALCULATE
    if (fdp.ConsumeBool()) {
        vc.D_value = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    } else {
        vc.D_value = D_CALCULATE;
    }
    
    // Initialize CIECAM02 model
    cmsHANDLE hModel = cmsCIECAM02Init(0, &vc);
    if (hModel == NULL) {
        return 0;  // Model initialization failed
    }
    
    // Test forward transformation (XYZ to JCh)
    cmsCIEXYZ xyz_input;
    cmsJCh jch_output;
    
    // Consume XYZ input values
    xyz_input.X = fdp.ConsumeFloatingPoint<double>();
    xyz_input.Y = fdp.ConsumeFloatingPoint<double>();
    xyz_input.Z = fdp.ConsumeFloatingPoint<double>();
    
    // Perform forward transform
    cmsCIECAM02Forward(hModel, &xyz_input, &jch_output);
    
    // Test reverse transformation (JCh to XYZ)
    cmsJCh jch_input;
    cmsCIEXYZ xyz_output;
    
    // Use either the computed JCh or generate new values
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 24) {
        // Use the output from forward transform
        jch_input.J = jch_output.J;
        jch_input.C = jch_output.C;
        jch_input.h = jch_output.h;
    } else {
        // Consume new JCh values
        jch_input.J = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
        jch_input.C = fdp.ConsumeFloatingPointInRange<double>(0.0, 150.0);
        jch_input.h = fdp.ConsumeFloatingPointInRange<double>(0.0, 360.0);
    }
    
    // Perform reverse transform
    cmsCIECAM02Reverse(hModel, &jch_input, &xyz_output);
    
    // Test round-trip consistency with remaining bytes
    if (fdp.remaining_bytes() >= 24) {
        // Create another viewing condition
        cmsViewingConditions vc2;
        vc2.whitePoint.X = fdp.ConsumeFloatingPoint<double>();
        vc2.whitePoint.Y = fdp.ConsumeFloatingPoint<double>();
        vc2.whitePoint.Z = fdp.ConsumeFloatingPoint<double>();
        vc2.Yb = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
        vc2.La = fdp.ConsumeFloatingPointInRange<double>(0.0, 400.0);
        vc2.surround = fdp.ConsumeBool() ? AVG_SURROUND : DIM_SURROUND;
        vc2.D_value = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        
        cmsHANDLE hModel2 = cmsCIECAM02Init(0, &vc2);
        if (hModel2 != NULL) {
            cmsCIEXYZ xyz_test;
            cmsJCh jch_test;
            
            // Test with extreme values
            xyz_test.X = fdp.ConsumeFloatingPoint<double>();
            xyz_test.Y = fdp.ConsumeFloatingPoint<double>();
            xyz_test.Z = fdp.ConsumeFloatingPoint<double>();
            
            cmsCIECAM02Forward(hModel2, &xyz_test, &jch_test);
            cmsCIECAM02Reverse(hModel2, &jch_test, &xyz_test);
            
            cmsCIECAM02Done(hModel2);
        }
    }
    
    // Clean up
    cmsCIECAM02Done(hModel);
    
    return 0;
}

} // namespace harness_004
