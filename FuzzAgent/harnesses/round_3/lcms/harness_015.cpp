#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_015 {
// Fuzzing harness for LittleCMS (lcms) Color Science functions focusing on gamut clamping and color difference
// This harness specifically targets cmsDesaturateLab and related color science functions identified as critical coverage gap
// Target APIs: cmsDesaturateLab, cmsDeltaE, cmsCIE94DeltaE, cmsBFDdeltaE, cmsCMCdeltaE, cmsCIE2000DeltaE, cmsLab2LCh, cmsLCh2Lab
// MODULE: cmsgmt.c - Gamut mapping and color difference functions
// STRATEGY: Comprehensive testing of gamut clamping with diverse Lab values and boundary conditions
// DIFFERENTIATION: Focus on gamut desaturation and color difference calculations which have high undiscovered branch complexity
// API SEQUENCE: Generate Lab values -> Apply gamut clamping -> Calculate color differences -> Test color space conversions


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful color science testing
    // Minimum: multiple Lab values (each 3 doubles = 24 bytes), clamping bounds, operation type
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Generate clamping bounds from fuzzer input
    // These define the gamut boundaries for desaturation
    double amax = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
    double amin = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
    double bmax = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
    double bmin = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
    
    // Ensure amin < amax and bmin < bmax for valid bounds
    if (amin > amax) std::swap(amin, amax);
    if (bmin > bmax) std::swap(bmin, bmax);
    
    // 2. Determine number of Lab points to test
    // Each Lab point requires 3 doubles (L, a, b) = 24 bytes
    size_t max_points = fdp.remaining_bytes() / (3 * sizeof(double));
    if (max_points > 20) max_points = 20; // Cap for performance
    if (max_points < 2) max_points = 2; // Need at least 2 for color difference calculations
    
    size_t num_lab_points = fdp.ConsumeIntegralInRange<size_t>(2, max_points);
    
    std::vector<cmsCIELab> lab_points;
    
    // 3. Generate Lab points for testing
    for (size_t i = 0; i < num_lab_points; i++) {
        if (fdp.remaining_bytes() < 3 * sizeof(double)) {
            break; // Not enough data for another Lab point
        }
        
        cmsCIELab lab;
        // L*: 0-100 typical range, but allow extended for edge cases
        lab.L = fdp.ConsumeFloatingPointInRange<double>(-10.0, 110.0);
        // a*, b*: typical range -128 to 128
        lab.a = fdp.ConsumeFloatingPointInRange<double>(-150.0, 150.0);
        lab.b = fdp.ConsumeFloatingPointInRange<double>(-150.0, 150.0);
        
        lab_points.push_back(lab);
    }
    
    if (lab_points.size() < 2) {
        return 0; // Need at least 2 points for meaningful testing
    }
    
    // 4. Test cmsDesaturateLab on each point
    for (size_t i = 0; i < lab_points.size(); i++) {
        cmsCIELab lab_copy = lab_points[i]; // Make a copy for desaturation
        
        // Apply gamut desaturation
        cmsBool desaturate_result = cmsDesaturateLab(&lab_copy, amax, amin, bmax, bmin);
        
        // Desaturation may fail for certain conditions (e.g., invalid angle)
        // Don't fail on return value - both TRUE and FALSE are valid outcomes
        
        // After desaturation, the point should be within gamut bounds
        // or have been set to (0,0,0) if L < 0
    }
    
    // 5. Test color difference calculations between pairs of points
    // Use fuzzer to determine which color difference formula to test
    uint8_t diff_method = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    for (size_t i = 0; i < lab_points.size() - 1; i++) {
        for (size_t j = i + 1; j < lab_points.size(); j++) {
            double deltaE = 0.0;
            
            switch (diff_method) {
                case 0:
                    // Simple DeltaE (CIE 1976)
                    deltaE = cmsDeltaE(&lab_points[i], &lab_points[j]);
                    break;
                case 1:
                    // CIE94 DeltaE
                    deltaE = cmsCIE94DeltaE(&lab_points[i], &lab_points[j]);
                    break;
                case 2:
                    // BFD DeltaE
                    deltaE = cmsBFDdeltaE(&lab_points[i], &lab_points[j]);
                    break;
                case 3:
                    // CMC DeltaE - requires additional parameters
                    {
                        double l = fdp.ConsumeFloatingPointInRange<double>(0.5, 2.0);
                        double c = fdp.ConsumeFloatingPointInRange<double>(0.5, 2.0);
                        deltaE = cmsCMCdeltaE(&lab_points[i], &lab_points[j], l, c);
                    }
                    break;
                case 4:
                    // CIE2000 DeltaE - requires additional parameters
                    {
                        double Kl = fdp.ConsumeFloatingPointInRange<double>(0.5, 2.0);
                        double Kc = fdp.ConsumeFloatingPointInRange<double>(0.5, 2.0);
                        double Kh = fdp.ConsumeFloatingPointInRange<double>(0.5, 2.0);
                        deltaE = cmsCIE2000DeltaE(&lab_points[i], &lab_points[j], Kl, Kc, Kh);
                    }
                    break;
            }
            
            // deltaE should be non-negative
            // Don't fail on result - valid calculation
        }
    }
    
    // 6. Test color space conversions (Lab <-> LCh)
    for (size_t i = 0; i < lab_points.size(); i++) {
        cmsCIELCh lch;
        
        // Convert Lab to LCh
        cmsLab2LCh(&lch, &lab_points[i]);
        
        // Convert back to Lab
        cmsCIELab lab_back;
        cmsLCh2Lab(&lab_back, &lch);
        
        // The conversion should be reversible (within floating point precision)
        // Don't fail on conversion - both functions should handle all inputs
    }
    
    // 7. Additional test: Apply desaturation then check if within bounds
    // Test edge case scenarios
    
    // Create test cases with extreme values
    cmsCIELab extreme_labs[] = {
        { -1.0, 0.0, 0.0 },      // L < 0 should result in (0,0,0)
        { 150.0, 200.0, 200.0 }, // L > 100 should be clamped to 100
        { 50.0, 0.0, 0.0 },      // a = 0 edge case (hue exactly 90)
        { 50.0, amin - 10.0, bmin - 10.0 }, // Outside bounds
        { 50.0, amax + 10.0, bmax + 10.0 }  // Outside bounds
    };
    
    for (size_t i = 0; i < 5; i++) {
        cmsCIELab test_lab = extreme_labs[i];
        cmsBool result = cmsDesaturateLab(&test_lab, amax, amin, bmax, bmin);
        // Don't fail on result
    }
    
    // 8. Test with different hue angles to exercise all 4 zones in cmsDesaturateLab
    // The function has 4 zones based on hue angle: [0,45), [45,135), [135,225), [225,315), [315,360]
    
    // Create test points with specific hue angles
    std::vector<double> test_hues = {0.0, 22.5, 45.0, 90.0, 135.0, 180.0, 225.0, 270.0, 315.0, 337.5, 360.0};
    
    for (double hue : test_hues) {
        if (fdp.remaining_bytes() < 2 * sizeof(double)) {
            break;
        }
        
        // Create Lab point with specific hue
        cmsCIELab hue_lab;
        hue_lab.L = fdp.ConsumeFloatingPointInRange<double>(30.0, 70.0);
        double chroma = fdp.ConsumeFloatingPointInRange<double>(10.0, 50.0);
        
        // Convert polar (LCh) to Cartesian (Lab)
        hue_lab.a = chroma * cos(hue * M_PI / 180.0);
        hue_lab.b = chroma * sin(hue * M_PI / 180.0);
        
        cmsBool desat_result = cmsDesaturateLab(&hue_lab, amax, amin, bmax, bmin);
        // Don't fail on result
    }
    
    return 0;
}

} // namespace harness_015
