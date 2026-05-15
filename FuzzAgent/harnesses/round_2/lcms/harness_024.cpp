#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include "lcms2.h"
#include "lcms2_plugin.h"

namespace harness_024 {
/*
 * Fuzzing harness for LittleCMS2 library
 * Targets: Half-float color transformations in cmshalf.c module
 * harness_024.cpp - Focus on IEEE 754-2008 half-precision floating point support
 * Current coverage: cmshalf.c has 0% coverage (0/20 lines, 0/2 functions)
 * Semantic differentiation from existing harnesses:
 *   Existing harnesses focus on standard data types (8-bit, 16-bit integer, 32-bit float)
 *   This harness specifically targets specialized half-float format (16-bit floating point)
 *   Used in GPU computing and HDR imaging, memory efficient, requires specialized conversion
 */


// Include Little CMS headers

// Helper function to generate random half-float values
static uint16_t GenerateHalfFloat(FuzzedDataProvider& fdp) {
    // Generate random 16-bit value that represents a half-float
    return fdp.ConsumeIntegral<uint16_t>();
}

// Helper function to generate half-float buffer
static std::vector<uint16_t> GenerateHalfFloatBuffer(FuzzedDataProvider& fdp, size_t num_elements) {
    std::vector<uint16_t> buffer(num_elements);
    for (size_t i = 0; i < num_elements; i++) {
        buffer[i] = GenerateHalfFloat(fdp);
    }
    return buffer;
}

// Test half-float transformations
static void TestHalfFloatTransforms(FuzzedDataProvider& fdp, cmsContext ctx) {
    // Create Lab profile (using double precision)
    cmsHPROFILE hLab = cmsCreateLab4ProfileTHR(ctx, NULL);
    if (!hLab) return;
    
    // Create sRGB profile
    cmsHPROFILE hsRGB = cmsCreate_sRGBProfileTHR(ctx);
    if (!hsRGB) {
        cmsCloseProfile(hLab);
        return;
    }
    
    // Get random intent and flags from fuzzer input
    cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
    cmsUInt32Number flags = fdp.ConsumeIntegral<cmsUInt32Number>();
    
    // Test 1: TYPE_Lab_DBL -> TYPE_RGB_HALF_FLT transformation
    cmsHTRANSFORM xform1 = cmsCreateTransformTHR(ctx, hLab, TYPE_Lab_DBL, 
                                                 hsRGB, TYPE_RGB_HALF_FLT, 
                                                 intent, flags);
    if (xform1) {
        // Allocate buffers
        size_t num_pixels = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        std::vector<cmsFloat64Number> lab_input(num_pixels * 3);
        std::vector<uint16_t> half_output(num_pixels * 3);
        
        // Generate random Lab values (L: 0-100, a,b: -128 to 127)
        for (size_t i = 0; i < lab_input.size(); i++) {
            if (i % 3 == 0) { // L channel
                lab_input[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 100.0);
            } else { // a or b channel
                lab_input[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-128.0, 127.0);
            }
        }
        
        // Execute transformation
        cmsDoTransform(xform1, lab_input.data(), half_output.data(), num_pixels);
        
        cmsDeleteTransform(xform1);
    }
    
    // Test 2: TYPE_RGB_HALF_FLT -> TYPE_Lab_DBL transformation
    cmsHTRANSFORM xform2 = cmsCreateTransformTHR(ctx, hsRGB, TYPE_RGB_HALF_FLT,
                                                 hLab, TYPE_Lab_DBL,
                                                 intent, flags);
    if (xform2) {
        size_t num_pixels = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        std::vector<uint16_t> half_input(num_pixels * 3);
        std::vector<cmsFloat64Number> lab_output(num_pixels * 3);
        
        // Generate random half-float RGB values
        half_input = GenerateHalfFloatBuffer(fdp, num_pixels * 3);
        
        // Execute transformation
        cmsDoTransform(xform2, half_input.data(), lab_output.data(), num_pixels);
        
        cmsDeleteTransform(xform2);
    }
    
    // Test 3: TYPE_RGB_HALF_FLT -> TYPE_CMYK_HALF_FLT transformation
    // Create CMYK profile
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(ctx, 256, NULL);
    cmsHPROFILE hCMYK = NULL;
    if (curve) {
        cmsToneCurve* curves[4] = { curve, curve, curve, curve };
        hCMYK = cmsCreateLinearizationDeviceLinkTHR(ctx, cmsSigCmykData, curves);
        cmsFreeToneCurve(curve);
    }
    
    if (hCMYK) {
        cmsHTRANSFORM xform3 = cmsCreateTransformTHR(ctx, hsRGB, TYPE_RGB_HALF_FLT,
                                                     hCMYK, TYPE_CMYK_HALF_FLT,
                                                     intent, flags);
        if (xform3) {
            size_t num_pixels = fdp.ConsumeIntegralInRange<size_t>(1, 8);
            std::vector<uint16_t> rgb_half_input(num_pixels * 3);
            std::vector<uint16_t> cmyk_half_output(num_pixels * 4);
            
            // Generate random half-float RGB values
            rgb_half_input = GenerateHalfFloatBuffer(fdp, num_pixels * 3);
            
            // Execute transformation
            cmsDoTransform(xform3, rgb_half_input.data(), cmyk_half_output.data(), num_pixels);
            
            cmsDeleteTransform(xform3);
        }
        cmsCloseProfile(hCMYK);
    }
    
    // Test 4: TYPE_GRAY_HALF_FLT transformations
    cmsHPROFILE hGray = cmsCreateGrayProfileTHR(ctx, NULL, NULL);
    if (hGray) {
        // Gray -> RGB half-float
        cmsHTRANSFORM xform4 = cmsCreateTransformTHR(ctx, hGray, TYPE_GRAY_HALF_FLT,
                                                     hsRGB, TYPE_RGB_HALF_FLT,
                                                     intent, flags);
        if (xform4) {
            size_t num_pixels = fdp.ConsumeIntegralInRange<size_t>(1, 16);
            std::vector<uint16_t> gray_half_input(num_pixels);
            std::vector<uint16_t> rgb_half_output(num_pixels * 3);
            
            gray_half_input = GenerateHalfFloatBuffer(fdp, num_pixels);
            
            cmsDoTransform(xform4, gray_half_input.data(), rgb_half_output.data(), num_pixels);
            
            cmsDeleteTransform(xform4);
        }
        
        // RGB half-float -> Gray half-float
        cmsHTRANSFORM xform5 = cmsCreateTransformTHR(ctx, hsRGB, TYPE_RGB_HALF_FLT,
                                                     hGray, TYPE_GRAY_HALF_FLT,
                                                     intent, flags);
        if (xform5) {
            size_t num_pixels = fdp.ConsumeIntegralInRange<size_t>(1, 16);
            std::vector<uint16_t> rgb_half_input2(num_pixels * 3);
            std::vector<uint16_t> gray_half_output(num_pixels);
            
            rgb_half_input2 = GenerateHalfFloatBuffer(fdp, num_pixels * 3);
            
            cmsDoTransform(xform5, rgb_half_input2.data(), gray_half_output.data(), num_pixels);
            
            cmsDeleteTransform(xform5);
        }
        
        cmsCloseProfile(hGray);
    }
    
    // Cleanup
    cmsCloseProfile(hLab);
    cmsCloseProfile(hsRGB);
}

// Test extended transforms with half-float types
static void TestExtendedHalfFloatTransforms(FuzzedDataProvider& fdp, cmsContext ctx) {
    // Create profiles
    cmsHPROFILE hLab = cmsCreateLab4ProfileTHR(ctx, NULL);
    cmsHPROFILE hsRGB = cmsCreate_sRGBProfileTHR(ctx);
    
    if (!hLab || !hsRGB) {
        if (hLab) cmsCloseProfile(hLab);
        if (hsRGB) cmsCloseProfile(hsRGB);
        return;
    }
    
    cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
    cmsUInt32Number flags = fdp.ConsumeIntegral<cmsUInt32Number>();
    
    // Create array of profiles for multi-profile transform
    cmsHPROFILE profiles[] = { hLab, hsRGB };
    
    // Test multi-profile transform: Lab -> sRGB with half-float output
    cmsHTRANSFORM xform = cmsCreateMultiprofileTransformTHR(ctx, profiles, 2,
                                                           TYPE_Lab_DBL, TYPE_RGB_HALF_FLT,
                                                           intent, flags);
    if (xform) {
        size_t num_pixels = fdp.ConsumeIntegralInRange<size_t>(1, 8);
        std::vector<cmsFloat64Number> lab_input(num_pixels * 3);
        std::vector<uint16_t> half_output(num_pixels * 3);
        
        // Generate random Lab values
        for (size_t i = 0; i < lab_input.size(); i++) {
            if (i % 3 == 0) {
                lab_input[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 100.0);
            } else {
                lab_input[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-128.0, 127.0);
            }
        }
        
        // Execute transformation
        cmsDoTransform(xform, lab_input.data(), half_output.data(), num_pixels);
        
        cmsDeleteTransform(xform);
    }
    
    cmsCloseProfile(hLab);
    cmsCloseProfile(hsRGB);
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) {
        return 0;  // Need minimum data for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create context for this test
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }
    
    // Test half-float transformations
    TestHalfFloatTransforms(fdp, ctx);
    
    // Test extended transforms if we have enough input
    if (fdp.remaining_bytes() > 64) {
        TestExtendedHalfFloatTransforms(fdp, ctx);
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}

} // namespace harness_024
