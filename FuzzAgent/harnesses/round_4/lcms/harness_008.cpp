#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include "lcms2.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_008 {
/*
 * Harness 008 - Total Area Coverage (TAC) Detection Fuzzer for Little CMS 2
 * 
 * Target APIs:
 * - cmsDetectTAC (438 undiscovered branches, 0% coverage) - Primary target
 * - cmsSliceSpace16 (10 undiscovered branches) - Called internally by cmsDetectTAC
 * - cmsDesaturateLab (37 undiscovered branches) - Related gamut function
 * 
 * Helper APIs:
 * - cmsCreate_sRGBProfile/cmsCreateGrayProfile/cmsCreateRGBProfile - Profile creation
 * - cmsCloseProfile - Cleanup
 * 
 * Strategy:
 * 1. Create various profile types using fuzzer input to select profile type
 * 2. Call cmsDetectTAC on created profiles to compute Total Area Coverage
 * 3. Optionally test cmsDesaturateLab with Lab coordinates derived from fuzzer input
 * 4. Ensure proper cleanup and resource management
 * 
 * This harness specifically targets the largest coverage gap in Little CMS 2.
 */



// Simple sampler function for cmsSliceSpace16 (if needed)
static cmsInt32Number dummy_sampler16(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo) {
    // Just copy input to output
    (void)Cargo;
    for (int i = 0; i < 3; i++) {  // Assume up to 3 channels
        Out[i] = In[i];
    }
    return 1;
}

// Helper function to create different profile types based on fuzzer input
static cmsHPROFILE create_profile_by_type(FuzzedDataProvider& fdp, int profile_type) {
    cmsHPROFILE profile = NULL;
    
    switch (profile_type % 4) {  // Reduced to 4 types to simplify
        case 0:
            // sRGB profile
            profile = cmsCreate_sRGBProfile();
            break;
        case 1:
            // Gray profile with gamma derived from fuzzer input
            {
                cmsFloat64Number gamma = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 5.0);
                cmsCIExyY white_point = {0.3127, 0.3290, 1.0}; // D65
                cmsToneCurve* transfer_function = cmsBuildGamma(NULL, gamma);
                if (transfer_function) {
                    profile = cmsCreateGrayProfile(&white_point, transfer_function);
                    cmsFreeToneCurve(transfer_function);
                }
            }
            break;
        case 2:
            // RGB profile with custom primaries and gamma
            {
                cmsFloat64Number gamma = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 5.0);
                cmsCIExyYTRIPLE primaries = {
                    {0.6400, 0.3300, 1.0}, // Red
                    {0.3000, 0.6000, 1.0}, // Green  
                    {0.1500, 0.0600, 1.0}  // Blue
                };
                cmsCIExyY white_point = {0.3127, 0.3290, 1.0}; // D65
                
                // Create tone curve for all channels
                cmsToneCurve* transfer_function = cmsBuildGamma(NULL, gamma);
                if (transfer_function) {
                    cmsToneCurve* transfer_functions[3] = {transfer_function, transfer_function, transfer_function};
                    profile = cmsCreateRGBProfile(&white_point, &primaries, transfer_functions);
                    cmsFreeToneCurve(transfer_function);
                }
            }
            break;
        case 3:
            // Lab profile
            {
                cmsCIExyY white_point = {0.3127, 0.3290, 1.0}; // D65
                profile = cmsCreateLab2Profile(&white_point);
            }
            break;
        default:
            profile = cmsCreate_sRGBProfile();
            break;
    }
    
    return profile;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    int profile_type = fdp.ConsumeIntegralInRange<int>(0, 10);
    bool test_desaturate = fdp.ConsumeBool();
    int num_profiles = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    std::vector<cmsHPROFILE> profiles;
    std::vector<cmsFloat64Number> tac_results;
    
    // Create and test multiple profiles
    for (int i = 0; i < num_profiles && fdp.remaining_bytes() > 10; i++) {
        cmsHPROFILE profile = create_profile_by_type(fdp, profile_type + i);
        
        if (profile != NULL) {
            profiles.push_back(profile);
            
            // MAIN TARGET: Call cmsDetectTAC to compute Total Area Coverage
            // This targets 438 undiscovered branches with 0% coverage
            cmsFloat64Number tac = cmsDetectTAC(profile);
            tac_results.push_back(tac);
            
            // Optionally test cmsDesaturateLab if we have enough input
            if (test_desaturate && fdp.remaining_bytes() >= sizeof(cmsCIELab) + 16) {
                cmsCIELab lab;
                lab.L = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
                lab.a = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                lab.b = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                
                double amax = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                double amin = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                double bmax = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                double bmin = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                
                // Ensure amax >= amin and bmax >= bmin for valid ranges
                if (amax < amin) std::swap(amax, amin);
                if (bmax < bmin) std::swap(bmax, bmin);
                
                // Call cmsDesaturateLab to target its 37 undiscovered branches
                cmsBool desaturate_result = cmsDesaturateLab(&lab, amax, amin, bmax, bmin);
                (void)desaturate_result;
                
                // Test with the desaturated Lab values
                cmsFloat64Number tac_after = cmsDetectTAC(profile);
                (void)tac_after;
            }
        }
    }
    
    // Additional tests with remaining input
    while (fdp.remaining_bytes() > 50 && profiles.size() > 0) {
        // Select a random profile from created ones
        size_t profile_idx = fdp.ConsumeIntegralInRange<size_t>(0, profiles.size() - 1);
        
        // Test TAC detection again with different context
        cmsFloat64Number tac_again = cmsDetectTAC(profiles[profile_idx]);
        (void)tac_again;
        
        // Consume some input to vary test patterns
        if (fdp.ConsumeBool()) {
            // Test with Lab desaturation using different parameters
            if (fdp.remaining_bytes() >= sizeof(cmsCIELab) + 16) {
                cmsCIELab lab2;
                lab2.L = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
                lab2.a = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                lab2.b = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                
                double amax2 = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                double amin2 = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                double bmax2 = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                double bmin2 = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
                
                if (amax2 < amin2) std::swap(amax2, amin2);
                if (bmax2 < bmin2) std::swap(bmax2, bmin2);
                
                cmsBool desat2 = cmsDesaturateLab(&lab2, amax2, amin2, bmax2, bmin2);
                (void)desat2;
            }
        }
    }
    
    // Cleanup all created profiles
    for (cmsHPROFILE profile : profiles) {
        if (profile != NULL) {
            cmsCloseProfile(profile);
        }
    }
    
    return 0;
}

} // namespace harness_008
