#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_019 {
// Fuzzing harness for Little CMS 2 library
// Target: Color difference (deltaE) calculation functions
// Strategy: Target all deltaE formulas with 24+ undiscovered branches
// Target APIs: 
//   - cmsCMCdeltaE (Target) - 12 undiscovered branches
//   - cmsCIE2000DeltaE (Target) - 8 undiscovered branches
//   - cmsCIE94DeltaE (Target) - 2 undiscovered branches
//   - cmsBFDdeltaE (Target) - 2 undiscovered branches
//   - cmsDeltaE (Target) - 0% coverage
// Required Helpers:
//   - cmsLabEncoded2Float (Color conversion)
//   - cmsFloat2LabEncoded (Color conversion)
//   - cmsLab2LCh (Color space conversion)
//   - cmsLCh2Lab (Color space conversion)
//   - cmsCreateContext (Initialization)
//   - cmsDeleteContext (Cleanup)
// Invocation sequence:
//   1. Call cmsCreateContext to initialize context.
//   2. Generate or encode Lab color values using helper functions.
//   3. Call cmsLabEncoded2Float/cmsFloat2LabEncoded to convert between encoded and float representations.
//   4. Call cmsLab2LCh/cmsLCh2Lab for LCh conversions if needed.
//   5. Compute color differences using the target deltaE functions with different color pairs.
//   6. Call cmsDeleteContext to clean up.
// Error handling scenarios included:
//   - Invalid color component ranges
//   - Edge cases for parameter values
//   - Function return value checking
// Note: Uses FuzzedDataProvider for input-driven testing of all parameters
// Lab value ranges: L [0,100], a,b [-128,128] typical ranges


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for Lab values, parameters, and encoded arrays
    // We need: 6 Lab components (double) + 5 deltaE parameters (double) + 6 encoded values (uint16) + extra
    const size_t MIN_SIZE = sizeof(double) * 11 + sizeof(uint16_t) * 6 + 16;
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create context (though deltaE functions may not need it)
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        // Context creation failed, but we can still test some functions
        // Some functions might work without context, but we'll continue
    }
    
    // Step 2: Generate Lab color values from fuzzed input
    // Lab color 1
    cmsCIELab lab1;
    // L typically 0-100, a,b typically -128 to 128
    lab1.L = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 100.0);
    lab1.a = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-128.0, 128.0);
    lab1.b = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-128.0, 128.0);
    
    // Lab color 2
    cmsCIELab lab2;
    lab2.L = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 100.0);
    lab2.a = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-128.0, 128.0);
    lab2.b = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-128.0, 128.0);
    
    // Step 3: Convert between encoded and float representations
    // First, encode Lab values to uint16 arrays
    cmsUInt16Number encoded_lab1[3], encoded_lab2[3];
    cmsFloat2LabEncoded(encoded_lab1, &lab1);
    cmsFloat2LabEncoded(encoded_lab2, &lab2);
    
    // Then decode back to float to test both directions
    cmsCIELab decoded_lab1, decoded_lab2;
    cmsLabEncoded2Float(&decoded_lab1, encoded_lab1);
    cmsLabEncoded2Float(&decoded_lab2, encoded_lab2);
    
    // Also test V2 versions if available
    cmsUInt16Number encoded_lab1_v2[3], encoded_lab2_v2[3];
    cmsFloat2LabEncodedV2(encoded_lab1_v2, &lab1);
    cmsFloat2LabEncodedV2(encoded_lab2_v2, &lab2);
    
    cmsCIELab decoded_lab1_v2, decoded_lab2_v2;
    cmsLabEncoded2FloatV2(&decoded_lab1_v2, encoded_lab1_v2);
    cmsLabEncoded2FloatV2(&decoded_lab2_v2, encoded_lab2_v2);
    
    // Step 4: LCh conversions
    cmsCIELCh lch1, lch2;
    cmsLab2LCh(&lch1, &lab1);
    cmsLab2LCh(&lch2, &lab2);
    
    // Convert back to Lab
    cmsCIELab lab_from_lch1, lab_from_lch2;
    cmsLCh2Lab(&lab_from_lch1, &lch1);
    cmsLCh2Lab(&lab_from_lch2, &lch2);
    
    // Step 5: Compute color differences using all deltaE functions
    
    // 5.1 Basic deltaE (CIE76)
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
    
    // 5.2 CIE94 deltaE
    cmsFloat64Number cie94_deltaE = cmsCIE94DeltaE(&lab1, &lab2);
    
    // 5.3 BFD deltaE
    cmsFloat64Number bfd_deltaE = cmsBFDdeltaE(&lab1, &lab2);
    
    // 5.4 CMC deltaE - requires l and c parameters
    // l: lightness weighting factor (typically 1.0-2.0)
    // c: chroma weighting factor (typically 1.0)
    cmsFloat64Number l_param = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 3.0);
    cmsFloat64Number c_param = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 3.0);
    cmsFloat64Number cmc_deltaE = cmsCMCdeltaE(&lab1, &lab2, l_param, c_param);
    
    // 5.5 CIE2000 deltaE - requires Kl, Kc, Kh parameters
    // Kl: lightness weighting (typically 1.0)
    // Kc: chroma weighting (typically 1.0)  
    // Kh: hue weighting (typically 1.0)
    cmsFloat64Number Kl = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 2.0);
    cmsFloat64Number Kc = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 2.0);
    cmsFloat64Number Kh = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 2.0);
    cmsFloat64Number cie2000_deltaE = cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
    
    // Also test with the decoded Lab values from encoded conversion
    cmsFloat64Number deltaE_decoded = cmsDeltaE(&decoded_lab1, &decoded_lab2);
    cmsFloat64Number cie94_deltaE_decoded = cmsCIE94DeltaE(&decoded_lab1, &decoded_lab2);
    cmsFloat64Number bfd_deltaE_decoded = cmsBFDdeltaE(&decoded_lab1, &decoded_lab2);
    cmsFloat64Number cmc_deltaE_decoded = cmsCMCdeltaE(&decoded_lab1, &decoded_lab2, l_param, c_param);
    cmsFloat64Number cie2000_deltaE_decoded = cmsCIE2000DeltaE(&decoded_lab1, &decoded_lab2, Kl, Kc, Kh);
    
    // Test with Lab values from LCh conversion
    cmsFloat64Number deltaE_lch = cmsDeltaE(&lab_from_lch1, &lab_from_lch2);
    cmsFloat64Number cie94_deltaE_lch = cmsCIE94DeltaE(&lab_from_lch1, &lab_from_lch2);
    cmsFloat64Number bfd_deltaE_lch = cmsBFDdeltaE(&lab_from_lch1, &lab_from_lch2);
    
    // Step 6: Test edge cases
    // Test with identical colors (deltaE should be 0 or very small)
    cmsFloat64Number deltaE_same = cmsDeltaE(&lab1, &lab1);
    cmsFloat64Number cie94_deltaE_same = cmsCIE94DeltaE(&lab1, &lab1);
    cmsFloat64Number bfd_deltaE_same = cmsBFDdeltaE(&lab1, &lab1);
    cmsFloat64Number cmc_deltaE_same = cmsCMCdeltaE(&lab1, &lab1, 1.0, 1.0);
    cmsFloat64Number cie2000_deltaE_same = cmsCIE2000DeltaE(&lab1, &lab1, 1.0, 1.0, 1.0);
    
    // Test with extreme color values (still derived from fuzzer input)
    // Use remaining bytes to create extreme values
    // Initialize extreme deltaE values with defaults
    cmsFloat64Number deltaE_extreme = 0.0;
    cmsFloat64Number cie94_deltaE_extreme = 0.0;
    cmsFloat64Number bfd_deltaE_extreme = 0.0;
    cmsFloat64Number cmc_deltaE_extreme = 0.0;
    cmsFloat64Number cie2000_deltaE_extreme = 0.0;
    
    if (fdp.remaining_bytes() >= sizeof(double) * 6) {
        cmsCIELab extreme_lab1, extreme_lab2;
        extreme_lab1.L = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 10.0);  // Very dark
        extreme_lab1.a = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-128.0, -100.0);  // Extreme negative
        extreme_lab1.b = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-128.0, -100.0);  // Extreme negative
        
        extreme_lab2.L = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(90.0, 100.0);  // Very light
        extreme_lab2.a = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(100.0, 128.0);  // Extreme positive
        extreme_lab2.b = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(100.0, 128.0);  // Extreme positive
        
        deltaE_extreme = cmsDeltaE(&extreme_lab1, &extreme_lab2);
        cie94_deltaE_extreme = cmsCIE94DeltaE(&extreme_lab1, &extreme_lab2);
        bfd_deltaE_extreme = cmsBFDdeltaE(&extreme_lab1, &extreme_lab2);
        cmc_deltaE_extreme = cmsCMCdeltaE(&extreme_lab1, &extreme_lab2, 1.0, 1.0);
        cie2000_deltaE_extreme = cmsCIE2000DeltaE(&extreme_lab1, &extreme_lab2, 1.0, 1.0, 1.0);
    }
    
    // Test with potentially problematic values (NaN/inf) from remaining input
    if (fdp.remaining_bytes() >= sizeof(double) * 3) {
        cmsCIELab potential_bad_lab;
        potential_bad_lab.L = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
        potential_bad_lab.a = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
        potential_bad_lab.b = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
        
        // These might produce NaN results, but that's OK for fuzzing
        (void)cmsDeltaE(&lab1, &potential_bad_lab);
        (void)cmsCIE94DeltaE(&lab1, &potential_bad_lab);
        (void)cmsBFDdeltaE(&lab1, &potential_bad_lab);
        (void)cmsCMCdeltaE(&lab1, &potential_bad_lab, 1.0, 1.0);
        (void)cmsCIE2000DeltaE(&lab1, &potential_bad_lab, 1.0, 1.0, 1.0);
    }
    
    // Step 7: Clean up
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
    
    // Use the results to avoid compiler optimization removing calls
    // This ensures all function calls have side effects
    volatile double results[] = {
        deltaE, cie94_deltaE, bfd_deltaE, cmc_deltaE, cie2000_deltaE,
        deltaE_decoded, cie94_deltaE_decoded, bfd_deltaE_decoded,
        cmc_deltaE_decoded, cie2000_deltaE_decoded,
        deltaE_lch, cie94_deltaE_lch, bfd_deltaE_lch,
        deltaE_same, cie94_deltaE_same, bfd_deltaE_same,
        cmc_deltaE_same, cie2000_deltaE_same,
        deltaE_extreme, cie94_deltaE_extreme, bfd_deltaE_extreme,
        cmc_deltaE_extreme, cie2000_deltaE_extreme
    };
    (void)results;
    
    return 0;
}

} // namespace harness_019
