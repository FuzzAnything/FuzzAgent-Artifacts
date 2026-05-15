#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>

namespace harness_006 {
// Fuzzing harness for Little CMS 2 (lcms) library - CIECAM02 color appearance model operations
// Targets CIECAM02 APIs with 0% coverage per coverage guidance (42+ branches undiscovered)
// Primary target APIs: cmsCIECAM02Init, cmsCIECAM02Forward, cmsCIECAM02Reverse, cmsCIECAM02Done
// Helper APIs: cmsCreateXYZProfile or cmsD50_XYZ for test data
// This harness focuses on complete CIECAM02 color appearance modeling lifecycle,
// differentiating from previous harnesses by targeting advanced color appearance calculations.
// Invocation sequence: Init → Forward with test XYZ → Reverse with appearance attributes → Done


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        return 0;  // Need minimum input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input for multiple operations
    // Consume fixed-size data first for viewing condition parameters
    uint8_t use_context = fdp.ConsumeBool();  // Whether to use thread-safe context
    uint8_t surround_type = fdp.ConsumeIntegral<uint8_t>() % 5;  // 0-4 for surround types
    uint8_t use_d_calculate = fdp.ConsumeBool();  // Whether to use D_CALCULATE
    
    // Create context if requested
    cmsContext context = NULL;
    if (use_context) {
        context = cmsCreateContext(NULL, NULL);
        if (context == NULL) {
            return 0;  // Context creation failed
        }
    }
    
    // Set up viewing conditions structure from fuzzer input
    cmsViewingConditions vc;
    
    // Consume white point coordinates (XYZ)
    vc.whitePoint.X = fdp.ConsumeFloatingPoint<double>();
    vc.whitePoint.Y = fdp.ConsumeFloatingPoint<double>();
    vc.whitePoint.Z = fdp.ConsumeFloatingPoint<double>();
    
    // Consume Yb (background luminance factor)
    vc.Yb = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    
    // Consume La (adapting field luminance)
    vc.La = fdp.ConsumeFloatingPointInRange<double>(0.0, 1000.0);
    
    // Set surround type based on consumed value
    switch (surround_type) {
        case 0: vc.surround = AVG_SURROUND; break;
        case 1: vc.surround = DIM_SURROUND; break;
        case 2: vc.surround = DARK_SURROUND; break;
        case 3: vc.surround = CUTSHEET_SURROUND; break;
        default: vc.surround = AVG_SURROUND; break;
    }
    
    // Set D_value (degree of adaptation) - either calculated or from input
    if (use_d_calculate) {
        vc.D_value = D_CALCULATE;
    } else {
        vc.D_value = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    }
    
    // Step 1: Initialize CIECAM02 model
    cmsHANDLE hModel = cmsCIECAM02Init(context, &vc);
    if (hModel == NULL) {
        if (use_context) cmsDeleteContext(context);
        return 0;  // CIECAM02 initialization failed
    }
    
    // Prepare test XYZ input for forward transform
    cmsCIEXYZ xyz_in;
    xyz_in.X = fdp.ConsumeFloatingPoint<double>();
    xyz_in.Y = fdp.ConsumeFloatingPoint<double>();
    xyz_in.Z = fdp.ConsumeFloatingPoint<double>();
    
    // Step 2: Perform forward transform (XYZ -> appearance attributes)
    cmsJCh jch_out;
    cmsCIECAM02Forward(hModel, &xyz_in, &jch_out);
    
    // Prepare test JCh input for reverse transform (using output from forward)
    cmsJCh jch_in;
    jch_in.J = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    jch_in.C = fdp.ConsumeFloatingPointInRange<double>(0.0, 150.0);
    jch_in.h = fdp.ConsumeFloatingPointInRange<double>(0.0, 360.0);
    
    // Step 3: Perform reverse transform (appearance attributes -> XYZ)
    cmsCIEXYZ xyz_out;
    cmsCIECAM02Reverse(hModel, &jch_in, &xyz_out);
    
    // Optional: Test with D50 white point reference
    const cmsCIEXYZ* d50_xyz = cmsD50_XYZ();
    if (d50_xyz != NULL) {
        // Create alternative viewing conditions with D50 white point
        cmsViewingConditions vc_d50 = vc;
        vc_d50.whitePoint = *d50_xyz;
        
        // Initialize second model with D50 white point
        cmsHANDLE hModelD50 = cmsCIECAM02Init(context, &vc_d50);
        if (hModelD50 != NULL) {
            // Test forward transform with D50
            cmsJCh jch_d50;
            cmsCIECAM02Forward(hModelD50, &xyz_in, &jch_d50);
            
            // Test reverse transform with D50
            cmsCIEXYZ xyz_d50_out;
            cmsCIECAM02Reverse(hModelD50, &jch_in, &xyz_d50_out);
            
            // Clean up D50 model
            cmsCIECAM02Done(hModelD50);
        }
    }
    
    // Optional: Test with XYZ profile creation
    cmsHPROFILE xyz_profile = cmsCreateXYZProfile();
    if (xyz_profile != NULL) {
        // Profile could be used for additional testing if needed
        cmsCloseProfile(xyz_profile);
    }
    
    // Step 4: Clean up CIECAM02 model
    cmsCIECAM02Done(hModel);
    
    // Clean up context if created
    if (use_context) {
        cmsDeleteContext(context);
    }
    
    return 0;
}

} // namespace harness_006
