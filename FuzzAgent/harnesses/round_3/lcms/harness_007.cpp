#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_007 {
// Fuzzing harness for LittleCMS (lcms) CIECAM02 color appearance model functions
// This harness specifically targets CIECAM02 color appearance model functions from cmscam02.c module
// that are currently 0% covered according to coverage analysis
// Target APIs: cmsCIECAM02Init, cmsCIECAM02Forward, cmsCIECAM02Reverse, cmsCIECAM02Done
// MODULE: cmscam02.c [current coverage: 0%]
// STRATEGY: Target CIECAM02 color appearance modeling transformation workflow
// DIFFERENTIATION: Unlike existing harnesses focused on transforms, PostScript, virtual profiles, I/O operations, IT8 handling, and dictionary management, target advanced color appearance modeling
// API SEQUENCE: Create context -> Initialize CIECAM02 -> Forward transform (XYZ to appearance) -> Reverse transform (appearance to XYZ) -> Cleanup CIECAM02 -> Delete context


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful CIECAM02 operations
    // Minimum: context + viewing conditions (cmsViewingConditions) + XYZ input + JCh input
    // cmsViewingConditions: whitePoint (3 doubles) + Yb (double) + La (double) + surround (uint32) + D_value (double) = 5*8 + 4 = 44 bytes
    // cmsCIEXYZ: 3 doubles = 24 bytes
    // cmsJCh: 3 doubles = 24 bytes
    // Total minimum: 44 + 24 + 24 = 92 bytes
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create Context: Create a working context for CIECAM02 operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0; // Cannot proceed without context
    }
    
    // 2. Prepare Viewing Conditions: Consume input for cmsViewingConditions structure
    cmsViewingConditions vc;
    
    // Consume white point (cmsCIEXYZ: X, Y, Z as doubles)
    vc.whitePoint.X = fdp.ConsumeFloatingPoint<double>();
    vc.whitePoint.Y = fdp.ConsumeFloatingPoint<double>();
    vc.whitePoint.Z = fdp.ConsumeFloatingPoint<double>();
    
    // Consume Yb (luminance factor of background)
    vc.Yb = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    
    // Consume La (adapting field luminance)
    vc.La = fdp.ConsumeFloatingPointInRange<double>(0.0, 1000.0);
    
    // Consume surround type (1-4: AVG_SURROUND, DIM_SURROUND, DARK_SURROUND, CUTSHEET_SURROUND)
    vc.surround = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 4);
    
    // Consume D_value (degree of adaptation) or use D_CALCULATE
    // Use either calculated or provided value based on input
    if (fdp.ConsumeBool()) {
        vc.D_value = D_CALCULATE; // Let library calculate
    } else {
        vc.D_value = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    }
    
    // 3. Initialize CIECAM02 Model
    cmsHANDLE hModel = cmsCIECAM02Init(context, &vc);
    if (hModel == NULL) {
        cmsDeleteContext(context);
        return 0; // Cannot initialize CIECAM02 model
    }
    
    // 4. Forward Transform: XYZ to JCh (appearance correlates)
    // Prepare input XYZ color
    cmsCIEXYZ xyz_input;
    xyz_input.X = fdp.ConsumeFloatingPoint<double>();
    xyz_input.Y = fdp.ConsumeFloatingPoint<double>();
    xyz_input.Z = fdp.ConsumeFloatingPoint<double>();
    
    // Output for forward transform
    cmsJCh jch_output;
    
    // Perform forward transform
    cmsCIECAM02Forward(hModel, &xyz_input, &jch_output);
    
    // 5. Reverse Transform: JCh back to XYZ
    // Use the output from forward transform as input for reverse
    // But also consume some fuzzer data to potentially modify values
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(double) * 3) {
        // Optionally modify JCh values with fuzzer data
        jch_output.J = fdp.ConsumeFloatingPoint<double>();
        jch_output.C = fdp.ConsumeFloatingPoint<double>();
        jch_output.h = fdp.ConsumeFloatingPoint<double>();
    }
    
    // Output for reverse transform
    cmsCIEXYZ xyz_output;
    
    // Perform reverse transform
    cmsCIECAM02Reverse(hModel, &jch_output, &xyz_output);
    
    // 6. Test with multiple colors if enough data remains
    size_t additional_tests = fdp.ConsumeIntegralInRange<size_t>(0, 3);
    for (size_t i = 0; i < additional_tests && fdp.remaining_bytes() >= sizeof(double) * 6; i++) {
        // Consume new XYZ input
        cmsCIEXYZ xyz_test;
        xyz_test.X = fdp.ConsumeFloatingPoint<double>();
        xyz_test.Y = fdp.ConsumeFloatingPoint<double>();
        xyz_test.Z = fdp.ConsumeFloatingPoint<double>();
        
        cmsJCh jch_test;
        
        // Forward transform
        cmsCIECAM02Forward(hModel, &xyz_test, &jch_test);
        
        // Reverse transform
        cmsCIEXYZ xyz_back;
        cmsCIECAM02Reverse(hModel, &jch_test, &xyz_back);
    }
    
    // 7. Cleanup CIECAM02 Model
    cmsCIECAM02Done(hModel);
    
    // 8. Cleanup Context
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_007
