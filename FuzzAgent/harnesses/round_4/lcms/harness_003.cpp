#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>

namespace harness_003 {
// Fuzzing harness for Little CMS 2 (lcms) library - Virtual Profile Creation Functions
// Targets virtual profile creation APIs: cmsCreateRGBProfile, cmsCreateGrayProfile, cmsCreateLab2Profile, 
// cmsCreateNULLProfile, cmsCreateBCHSWabstractProfile, cmsCreateLinearizationDeviceLink, 
// cmsCreateInkLimitingDeviceLink (1100+ undiscovered complexity in cmsvirt.c)
// Helper APIs: cmsBuildParametricToneCurve, cmsD50_xyY, cmsCloseProfile
// This harness focuses on virtual profile creation lifecycle, differentiating from previous harnesses:
// - harness_000.cpp: Basic profile parsing and transformations
// - harness_001.cpp: PostScript color resource generation
// - harness_002.cpp: IT8/CGATS file handling and cube file device links


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for parametric curve parameters and profile creation
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input for multiple operations
    // Consume fixed-size data first for parametric curve parameters
    int32_t curve_type = fdp.ConsumeIntegralInRange<int32_t>(1, 10);  // Valid parametric curve types
    std::vector<double> curve_params;
    
    // Generate parameters for parametric tone curve based on curve type
    // Different curve types require different numbers of parameters
    size_t num_params = 5;  // Reasonable default
    if (curve_type == 1 || curve_type == 2) num_params = 5;
    else if (curve_type == 3) num_params = 6;
    else if (curve_type == 4) num_params = 7;
    else if (curve_type >= 5 && curve_type <= 10) num_params = 10;
    
    // Consume parameters for the tone curve
    for (size_t i = 0; i < num_params; i++) {
        if (fdp.remaining_bytes() < sizeof(double)) break;
        curve_params.push_back(fdp.ConsumeFloatingPoint<double>());
    }
    
    // Create tone curves for RGB channels
    cmsToneCurve* tone_curves[3] = {NULL, NULL, NULL};
    for (int i = 0; i < 3; i++) {
        if (curve_params.size() >= num_params) {
            tone_curves[i] = cmsBuildParametricToneCurve(NULL, curve_type, curve_params.data());
        }
    }
    
    // Create a single tone curve for gray profile
    cmsToneCurve* gray_tone_curve = NULL;
    if (curve_params.size() >= num_params) {
        gray_tone_curve = cmsBuildParametricToneCurve(NULL, curve_type, curve_params.data());
    }
    
    // Get standard white point
    const cmsCIExyY* white_point = cmsD50_xyY();
    
    // Test 1: cmsCreateRGBProfile - RGB color profile creation
    cmsHPROFILE hRGBProfile = NULL;
    if (white_point != NULL && tone_curves[0] != NULL && tone_curves[1] != NULL && tone_curves[2] != NULL) {
        // Generate primaries from fuzzer input within valid chromaticity bounds
        cmsCIExyYTRIPLE primaries;
        if (fdp.remaining_bytes() >= 9 * sizeof(double)) {
            // Red primaries (x, y should be between 0-1, Y=1.0)
            primaries.Red.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.8);
            primaries.Red.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.8);
            primaries.Red.Y = 1.0;
            
            // Green primaries
            primaries.Green.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.8);
            primaries.Green.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.8);
            primaries.Green.Y = 1.0;
            
            // Blue primaries
            primaries.Blue.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.8);
            primaries.Blue.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.8);
            primaries.Blue.Y = 1.0;
            
            hRGBProfile = cmsCreateRGBProfile(white_point, &primaries, tone_curves);
        }
    }
    
    // Test 2: cmsCreateGrayProfile - Gray color profile creation
    cmsHPROFILE hGrayProfile = NULL;
    if (white_point != NULL && gray_tone_curve != NULL) {
        hGrayProfile = cmsCreateGrayProfile(white_point, gray_tone_curve);
    }
    
    // Test 3: cmsCreateLab2Profile - Lab color profile creation
    cmsHPROFILE hLabProfile = NULL;
    if (white_point != NULL) {
        hLabProfile = cmsCreateLab2Profile(white_point);
    }
    
    // Test 4: cmsCreateNULLProfile - NULL profile creation
    cmsHPROFILE hNULLProfile = cmsCreateNULLProfile();
    
    // Test 5: cmsCreateBCHSWabstractProfile - Brightness/Contrast/Hue/Saturation profile creation
    cmsHPROFILE hBCHSWProfile = NULL;
    if (fdp.remaining_bytes() >= 7 * sizeof(double) + 2 * sizeof(uint32_t)) {
        uint32_t nLUTPoints = fdp.ConsumeIntegralInRange<uint32_t>(2, 256);
        double bright = fdp.ConsumeFloatingPointInRange<double>(-100.0, 100.0);
        double contrast = fdp.ConsumeFloatingPointInRange<double>(-100.0, 100.0);
        double hue = fdp.ConsumeFloatingPointInRange<double>(-180.0, 180.0);
        double saturation = fdp.ConsumeFloatingPointInRange<double>(-100.0, 100.0);
        uint32_t tempSrc = fdp.ConsumeIntegral<uint32_t>();
        uint32_t tempDest = fdp.ConsumeIntegral<uint32_t>();
        
        hBCHSWProfile = cmsCreateBCHSWabstractProfile(nLUTPoints, bright, contrast, hue, saturation, tempSrc, tempDest);
    }
    
    // Test 6: cmsCreateLinearizationDeviceLink - Linearization device link creation
    cmsHPROFILE hLinearizationLink = NULL;
    if (tone_curves[0] != NULL && tone_curves[1] != NULL && tone_curves[2] != NULL) {
        hLinearizationLink = cmsCreateLinearizationDeviceLink(cmsSigRgbData, tone_curves);
    }
    
    // Test 7: cmsCreateInkLimitingDeviceLink - Ink limiting device link creation
    cmsHPROFILE hInkLimitingLink = NULL;
    if (fdp.remaining_bytes() >= sizeof(double)) {
        double ink_limit = fdp.ConsumeFloatingPointInRange<double>(0.0, 400.0);
        hInkLimitingLink = cmsCreateInkLimitingDeviceLink(cmsSigCmykData, ink_limit);
    }
    
    // Clean up all created profiles
    if (hRGBProfile != NULL) {
        cmsCloseProfile(hRGBProfile);
    }
    if (hGrayProfile != NULL) {
        cmsCloseProfile(hGrayProfile);
    }
    if (hLabProfile != NULL) {
        cmsCloseProfile(hLabProfile);
    }
    if (hNULLProfile != NULL) {
        cmsCloseProfile(hNULLProfile);
    }
    if (hBCHSWProfile != NULL) {
        cmsCloseProfile(hBCHSWProfile);
    }
    if (hLinearizationLink != NULL) {
        cmsCloseProfile(hLinearizationLink);
    }
    if (hInkLimitingLink != NULL) {
        cmsCloseProfile(hInkLimitingLink);
    }
    
    // Free tone curves
    for (int i = 0; i < 3; i++) {
        if (tone_curves[i] != NULL) {
            cmsFreeToneCurve(tone_curves[i]);
        }
    }
    if (gray_tone_curve != NULL) {
        cmsFreeToneCurve(gray_tone_curve);
    }
    
    return 0;
}

} // namespace harness_003
