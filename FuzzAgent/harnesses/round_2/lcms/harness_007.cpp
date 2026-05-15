#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include "lcms2.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_007 {
// Fuzzing harness for LittleCMS2 library
// Targets BCHSW abstract profile functions (240 branches, 108 undiscovered)
// harness_007.cpp - Focus on BCHSW abstract profile creation and management
// Semantic diversity: harness_000 (basic profile parsing), harness_001 (proofing/PostScript), 
// harness_002 (virtual profiles/gamut), harness_003 (CGATS/IT8), harness_004 (CIECAM02), 
// harness_005 (TAC detection/sampling), harness_006 (device link creation/I/O)
// This harness targets BCHSW abstract profile functions with proper lifecycle management


// Main fuzzer entry point for BCHSW abstract profile functions
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 50) {
        return 0;  // Need minimum data for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters for BCHSW abstract profile creation
    cmsUInt32Number nLUTPoints = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 100);
    cmsFloat64Number Bright = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
    cmsFloat64Number Contrast = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 10.0);
    cmsFloat64Number Hue = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-180.0, 180.0);
    cmsFloat64Number Saturation = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
    cmsUInt32Number TempSrc = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2000, 10000);
    cmsUInt32Number TempDest = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2000, 10000);
    
    // Consume remaining data for transform testing
    size_t transform_data_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
    std::vector<uint8_t> transform_data = fdp.ConsumeBytes<uint8_t>(transform_data_size);
    
    // Create a context for testing THR variant
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // TEST PHASE 1: Create BCHSW abstract profile using THR variant with context
    cmsHPROFILE hProfile1 = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, Bright, Contrast, 
                                                              Hue, Saturation, TempSrc, TempDest);
    
    if (hProfile1 != NULL) {
        // Extract and verify profile properties
        cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile1);
        cmsColorSpaceSignature pcs = cmsGetPCS(hProfile1);
        cmsProfileClassSignature deviceClass = cmsGetDeviceClass(hProfile1);
        
        // TEST PHASE 2: Create transform using the BCHSW profile
        // Create a simple sRGB profile as source
        cmsHPROFILE hSRGB = cmsCreate_sRGBProfileTHR(ctx);
        
        if (hSRGB != NULL) {
            // Create transform from sRGB to BCHSW abstract profile
            cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
            cmsUInt32Number flags = fdp.ConsumeIntegral<uint32_t>();
            
            cmsHTRANSFORM hTransform = cmsCreateTransformTHR(ctx, hSRGB, 
                                                              TYPE_RGB_8, 
                                                              hProfile1, 
                                                              TYPE_RGB_8, 
                                                              intent, flags);
            
            if (hTransform != NULL) {
                // Test the transform with some fuzzed data
                if (transform_data.size() >= 3) {
                    uint8_t rgb_in[3];
                    uint8_t rgb_out[3];
                    
                    // Use first 3 bytes for RGB input
                    rgb_in[0] = transform_data.size() > 0 ? transform_data[0] : 0;
                    rgb_in[1] = transform_data.size() > 1 ? transform_data[1] : 0;
                    rgb_in[2] = transform_data.size() > 2 ? transform_data[2] : 0;
                    
                    cmsDoTransform(hTransform, rgb_in, rgb_out, 1);
                    
                    // Test with different data types
                    if (transform_data.size() >= 12) {
                        uint16_t rgb16_in[3];
                        uint16_t rgb16_out[3];
                        
                        for (int i = 0; i < 3 && i*2+1 < transform_data.size(); i++) {
                            rgb16_in[i] = (transform_data[i*2] << 8) | transform_data[i*2+1];
                        }
                        
                        // Create another transform for 16-bit data
                        cmsHTRANSFORM hTransform16 = cmsCreateTransformTHR(ctx, hSRGB, 
                                                                           TYPE_RGB_16, 
                                                                           hProfile1, 
                                                                           TYPE_RGB_16, 
                                                                           intent, flags);
                        if (hTransform16 != NULL) {
                            cmsDoTransform(hTransform16, rgb16_in, rgb16_out, 1);
                            cmsDeleteTransform(hTransform16);
                        }
                    }
                }
                
                cmsDeleteTransform(hTransform);
            }
            
            cmsCloseProfile(hSRGB);
        }
        
        // TEST PHASE 3: Test profile saving and loading
        if (transform_data.size() > 128) {
            // Try to save profile to memory
            cmsUInt32Number savedSize = 0;
            if (cmsSaveProfileToMem(hProfile1, NULL, &savedSize)) {
                if (savedSize > 0 && savedSize < 10*1024*1024) {  // Reasonable size limit
                    std::vector<uint8_t> buffer(savedSize);
                    if (cmsSaveProfileToMem(hProfile1, buffer.data(), &savedSize)) {
                        // Try to reload the saved profile
                        cmsHPROFILE hReloaded = cmsOpenProfileFromMemTHR(ctx, buffer.data(), savedSize);
                        if (hReloaded != NULL) {
                            cmsCloseProfile(hReloaded);
                        }
                    }
                }
            }
        }
        
        cmsCloseProfile(hProfile1);
    }
    
    // TEST PHASE 4: Create BCHSW abstract profile using non-THR variant (NULL context)
    cmsHPROFILE hProfile2 = cmsCreateBCHSWabstractProfile(nLUTPoints, Bright, Contrast, 
                                                          Hue, Saturation, TempSrc, TempDest);
    
    if (hProfile2 != NULL) {
        // Test different parameter combinations
        // Vary parameters slightly for additional coverage
        cmsFloat64Number altBright = Bright * 0.5;
        cmsFloat64Number altContrast = Contrast * 2.0;
        cmsFloat64Number altHue = Hue + 30.0;
        cmsFloat64Number altSaturation = Saturation * 0.8;
        cmsUInt32Number altTempSrc = TempSrc + 500;
        cmsUInt32Number altTempDest = TempDest - 500;
        
        // Create another profile with modified parameters
        cmsHPROFILE hProfile3 = cmsCreateBCHSWabstractProfileTHR(ctx, 
                                                                 nLUTPoints + 1,
                                                                 altBright,
                                                                 altContrast,
                                                                 altHue,
                                                                 altSaturation,
                                                                 altTempSrc,
                                                                 altTempDest);
        
        if (hProfile3 != NULL) {
            // Compare the two profiles
            cmsUInt32Number size1 = 0, size2 = 0;
            cmsSaveProfileToMem(hProfile2, NULL, &size1);
            cmsSaveProfileToMem(hProfile3, NULL, &size2);
            
            cmsCloseProfile(hProfile3);
        }
        
        // TEST PHASE 5: Test profile tags
        cmsMLU* description = (cmsMLU*)cmsReadTag(hProfile2, cmsSigProfileDescriptionTag);
        if (description != NULL) {
            wchar_t buffer[256];
            cmsMLUgetWide(description, "en", "US", buffer, 256);
        }
        
        // Read white point tag
        cmsCIEXYZ* whitePoint = (cmsCIEXYZ*)cmsReadTag(hProfile2, cmsSigMediaWhitePointTag);
        
        // Read AToB0 tag (should contain the CLUT for BCHSW)
        cmsPipeline* pipeline = (cmsPipeline*)cmsReadTag(hProfile2, cmsSigAToB0Tag);
        
        cmsCloseProfile(hProfile2);
    }
    
    // TEST PHASE 6: Test edge cases
    // Test with extreme values
    if (fdp.remaining_bytes() > 20) {
        cmsUInt32Number extremeLUT = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 1000);
        cmsFloat64Number extremeBright = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
        cmsFloat64Number extremeContrast = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
        cmsFloat64Number extremeHue = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
        cmsFloat64Number extremeSaturation = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
        
        cmsHPROFILE hExtreme = cmsCreateBCHSWabstractProfileTHR(ctx, 
                                                                extremeLUT,
                                                                extremeBright,
                                                                extremeContrast,
                                                                extremeHue,
                                                                extremeSaturation,
                                                                TempSrc,
                                                                TempDest);
        
        if (hExtreme != NULL) {
            cmsCloseProfile(hExtreme);
        }
    }
    
    // TEST PHASE 7: Test with same source and dest temperatures (no white point adjustment)
    cmsUInt32Number sameTemp = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2000, 10000);
    cmsHPROFILE hNoWPAdjust = cmsCreateBCHSWabstractProfileTHR(ctx, 
                                                               nLUTPoints,
                                                               Bright,
                                                               Contrast,
                                                               Hue,
                                                               Saturation,
                                                               sameTemp,
                                                               sameTemp);
    
    if (hNoWPAdjust != NULL) {
        cmsCloseProfile(hNoWPAdjust);
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}

} // namespace harness_007
