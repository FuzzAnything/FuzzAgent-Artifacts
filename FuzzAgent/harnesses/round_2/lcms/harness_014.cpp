#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include "lcms2.h"

namespace harness_014 {
/*
 * Fuzzing harness for LittleCMS2 library
 * Targets: Colorimetric conversions, DeltaE metrics, and color science algorithms
 * Semantic domain: Color science algorithms and metrics (untested in previous harnesses)
 * Previous harness semantic domains covered:
 *   000-basic profile parsing/transformation
 *   001-proofing/PostScript
 *   002-virtual profiles/gamut
 *   003-CGATS/IT8 basic
 *   004-CIECAM02 (appearance model, different from basic colorimetry)
 *   005-TAC detection/sampling
 *   006-device link creation/I/O
 *   007-BCHSW abstract profiles
 *   008-advanced CGATS/IT8
 *   009-cube file device link creation/stream I/O
 *   010-pipeline/MPE operations
 *   011-PostScript color resources
 *   012-MD5/half-precision
 *   013-context management/plugin system
 * This harness focuses on: DeltaE metrics, basic color space conversions,
 *   temperature conversions, chromatic adaptation, and encoding/decoding
 */


// Include Little CMS headers

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) {
        return 0;  // Minimum input size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Test basic color space conversions
    
    // Create color values from fuzzed data
    cmsCIEXYZ xyz1, xyz2, xyz_result;
    cmsCIELab lab1, lab2, lab_result;
    cmsCIELCh lch1, lch2;
    cmsCIExyY xyY1, xyY2;
    
    // Consume XYZ values for testing
    xyz1.X = fdp.ConsumeFloatingPoint<double>();
    xyz1.Y = fdp.ConsumeFloatingPoint<double>();
    xyz1.Z = fdp.ConsumeFloatingPoint<double>();
    
    xyz2.X = fdp.ConsumeFloatingPoint<double>();
    xyz2.Y = fdp.ConsumeFloatingPoint<double>();
    xyz2.Z = fdp.ConsumeFloatingPoint<double>();
    
    // Get standard white points for conversions
    const cmsCIEXYZ* d50_xyz = cmsD50_XYZ();
    const cmsCIExyY* d50_xyY = cmsD50_xyY();
    
    if (d50_xyz == NULL || d50_xyY == NULL) {
        return 0;  // Failed to get standard white points
    }
    
    // Test 1: XYZ to Lab and back conversion
    cmsXYZ2Lab(d50_xyz, &lab1, &xyz1);
    cmsLab2XYZ(d50_xyz, &xyz_result, &lab1);
    
    // Test 2: Lab to LCh and back conversion
    cmsLab2LCh(&lch1, &lab1);
    cmsLCh2Lab(&lab_result, &lch1);
    
    // Test 3: XYZ to xyY and back conversion
    cmsXYZ2xyY(&xyY1, &xyz1);
    cmsxyY2XYZ(&xyz_result, &xyY1);
    
    // Phase 2: Test DeltaE metrics (color difference formulas)
    
    // Consume Lab values for DeltaE calculations
    lab1.L = fdp.ConsumeFloatingPoint<double>();
    lab1.a = fdp.ConsumeFloatingPoint<double>();
    lab1.b = fdp.ConsumeFloatingPoint<double>();
    
    lab2.L = fdp.ConsumeFloatingPoint<double>();
    lab2.a = fdp.ConsumeFloatingPoint<double>();
    lab2.b = fdp.ConsumeFloatingPoint<double>();
    
    // Test all DeltaE formulas
    double deltaE = cmsDeltaE(&lab1, &lab2);
    double cie94DeltaE = cmsCIE94DeltaE(&lab1, &lab2);
    double bfdDeltaE = cmsBFDdeltaE(&lab1, &lab2);
    
    // Consume parameters for CMC and CIE2000 DeltaE
    double l_param = fdp.ConsumeFloatingPointInRange<double>(0.5, 2.0);
    double c_param = fdp.ConsumeFloatingPointInRange<double>(0.5, 2.0);
    double cmcDeltaE = cmsCMCdeltaE(&lab1, &lab2, l_param, c_param);
    
    double Kl = fdp.ConsumeFloatingPointInRange<double>(0.5, 2.0);
    double Kc = fdp.ConsumeFloatingPointInRange<double>(0.5, 2.0);
    double Kh = fdp.ConsumeFloatingPointInRange<double>(0.5, 2.0);
    double cie2000DeltaE = cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
    
    // Prevent unused variable warnings
    (void)deltaE;
    (void)cie94DeltaE;
    (void)bfdDeltaE;
    (void)cmcDeltaE;
    (void)cie2000DeltaE;
    
    // Phase 3: Test encoding/decoding functions
    
    if (fdp.remaining_bytes() >= 12) {
        cmsUInt16Number encoded_lab[3], encoded_xyz[3];
        
        // Encode Lab values
        encoded_lab[0] = fdp.ConsumeIntegral<uint16_t>();
        encoded_lab[1] = fdp.ConsumeIntegral<uint16_t>();
        encoded_lab[2] = fdp.ConsumeIntegral<uint16_t>();
        
        // Decode Lab values
        cmsCIELab decoded_lab;
        cmsLabEncoded2Float(&decoded_lab, encoded_lab);
        
        // Encode XYZ values
        encoded_xyz[0] = fdp.ConsumeIntegral<uint16_t>();
        encoded_xyz[1] = fdp.ConsumeIntegral<uint16_t>();
        encoded_xyz[2] = fdp.ConsumeIntegral<uint16_t>();
        
        // Decode XYZ values
        cmsCIEXYZ decoded_xyz;
        cmsXYZEncoded2Float(&decoded_xyz, encoded_xyz);
        
        // Test V2 versions if we have enough data
        if (fdp.remaining_bytes() >= 6) {
            cmsUInt16Number encoded_lab_v2[3];
            encoded_lab_v2[0] = fdp.ConsumeIntegral<uint16_t>();
            encoded_lab_v2[1] = fdp.ConsumeIntegral<uint16_t>();
            encoded_lab_v2[2] = fdp.ConsumeIntegral<uint16_t>();
            
            cmsCIELab decoded_lab_v2;
            cmsLabEncoded2FloatV2(&decoded_lab_v2, encoded_lab_v2);
        }
    }
    
    // Phase 4: Test temperature conversions
    
    if (fdp.remaining_bytes() >= 8) {
        cmsCIExyY white_point;
        double temp_k;
        
        // Test temperature to white point conversion
        double input_temp = fdp.ConsumeFloatingPointInRange<double>(1000.0, 25000.0);
        cmsBool temp_success = cmsWhitePointFromTemp(&white_point, input_temp);
        
        if (temp_success && fdp.remaining_bytes() >= 24) {
            // Test white point to temperature conversion
            double output_temp;
            cmsBool reverse_success = cmsTempFromWhitePoint(&output_temp, &white_point);
            (void)reverse_success;
        }
    }
    
    // Phase 5: Test chromatic adaptation
    
    if (fdp.remaining_bytes() >= 36) {
        cmsCIEXYZ source_white, illuminant, value, result;
        
        // Consume source white point
        source_white.X = fdp.ConsumeFloatingPoint<double>();
        source_white.Y = fdp.ConsumeFloatingPoint<double>();
        source_white.Z = fdp.ConsumeFloatingPoint<double>();
        
        // Consume illuminant
        illuminant.X = fdp.ConsumeFloatingPoint<double>();
        illuminant.Y = fdp.ConsumeFloatingPoint<double>();
        illuminant.Z = fdp.ConsumeFloatingPoint<double>();
        
        // Consume value to adapt
        value.X = fdp.ConsumeFloatingPoint<double>();
        value.Y = fdp.ConsumeFloatingPoint<double>();
        value.Z = fdp.ConsumeFloatingPoint<double>();
        
        // Perform chromatic adaptation
        cmsBool adapt_success = cmsAdaptToIlluminant(&result, &source_white, &illuminant, &value);
        (void)adapt_success;
    }
    
    // Phase 6: Test float to encoded conversions
    
    if (fdp.remaining_bytes() >= 12) {
        cmsCIELab float_lab;
        cmsCIEXYZ float_xyz;
        
        // Consume float values
        float_lab.L = fdp.ConsumeFloatingPoint<double>();
        float_lab.a = fdp.ConsumeFloatingPoint<double>();
        float_lab.b = fdp.ConsumeFloatingPoint<double>();
        
        float_xyz.X = fdp.ConsumeFloatingPoint<double>();
        float_xyz.Y = fdp.ConsumeFloatingPoint<double>();
        float_xyz.Z = fdp.ConsumeFloatingPoint<double>();
        
        // Convert to encoded format
        cmsUInt16Number encoded_lab_out[3], encoded_xyz_out[3];
        cmsFloat2LabEncoded(encoded_lab_out, &float_lab);
        cmsFloat2XYZEncoded(encoded_xyz_out, &float_xyz);
        
        // Test V2 version if we have enough data
        if (fdp.remaining_bytes() >= 12) {
            cmsUInt16Number encoded_lab_v2_out[3];
            cmsFloat2LabEncodedV2(encoded_lab_v2_out, &float_lab);
        }
    }
    
    // Phase 7: Test with random remaining data
    
    while (fdp.remaining_bytes() >= 24) {
        // Create random color values from remaining data
        cmsCIEXYZ random_xyz;
        cmsCIELab random_lab;
        
        random_xyz.X = fdp.ConsumeFloatingPoint<double>();
        random_xyz.Y = fdp.ConsumeFloatingPoint<double>();
        random_xyz.Z = fdp.ConsumeFloatingPoint<double>();
        
        random_lab.L = fdp.ConsumeFloatingPoint<double>();
        random_lab.a = fdp.ConsumeFloatingPoint<double>();
        random_lab.b = fdp.ConsumeFloatingPoint<double>();
        
        // Test a random combination of conversions
        uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (test_type) {
            case 0: {
                // XYZ to Lab
                cmsCIELab converted_lab;
                cmsXYZ2Lab(d50_xyz, &converted_lab, &random_xyz);
                break;
            }
            case 1: {
                // Lab to XYZ
                cmsCIEXYZ converted_xyz;
                cmsLab2XYZ(d50_xyz, &converted_xyz, &random_lab);
                break;
            }
            case 2: {
                // Lab to LCh and back
                cmsCIELCh converted_lch;
                cmsCIELab back_lab;
                cmsLab2LCh(&converted_lch, &random_lab);
                cmsLCh2Lab(&back_lab, &converted_lch);
                break;
            }
            case 3: {
                // DeltaE between random colors
                cmsCIELab another_lab;
                another_lab.L = fdp.ConsumeFloatingPoint<double>();
                another_lab.a = fdp.ConsumeFloatingPoint<double>();
                another_lab.b = fdp.ConsumeFloatingPoint<double>();
                
                double random_deltaE = cmsDeltaE(&random_lab, &another_lab);
                (void)random_deltaE;
                break;
            }
        }
    }
    
    return 0;
}

} // namespace harness_014
