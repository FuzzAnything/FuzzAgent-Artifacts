#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include "lcms2.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_005 {
// Fuzzing harness for LittleCMS2 library
// Targets Total Area Coverage detection and space sampling operations
// harness_005.cpp - Focus on cmsDetectTAC (440 undiscovered branches) and 3D grid sampling functions
// Semantic diversity: harness_000 (basic profile parsing), harness_001 (proofing/PostScript), 
// harness_002 (virtual profiles/gamut), harness_003 (CGATS/IT8), harness_004 (CIECAM02)
// This harness targets Total Area Coverage detection and 3D grid sampling


// Custom sampler for CLUT sampling - 16-bit version
static int CustomSampler16(const cmsUInt16Number In[],
                           cmsUInt16Number Out[],
                           void* Cargo) {
    // Simple pass-through sampler for testing
    // In real scenarios, this would implement custom color processing
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(Cargo);
    
    // Use fuzzed data to modify output
    for (int i = 0; i < 3 && i < 16; i++) {
        // Generate output based on input and fuzzed data
        if (fdp->remaining_bytes() > 0) {
            // Simple transformation using XOR with fuzzed data
            uint16_t modifier = fdp->ConsumeIntegral<uint16_t>();
            Out[i] = In[i] ^ modifier;
        } else {
            Out[i] = In[i];
        }
    }
    return 1;
}

// Custom sampler for CLUT sampling - float version
static int CustomSamplerFloat(const cmsFloat32Number In[],
                              cmsFloat32Number Out[],
                              void* Cargo) {
    // Simple pass-through sampler for testing
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(Cargo);
    
    // Use fuzzed data to modify output
    for (int i = 0; i < 3 && i < 16; i++) {
        // Generate output based on input and fuzzed data
        if (fdp->remaining_bytes() > 0) {
            float adjustment = fdp->ConsumeFloatingPoint<float>();
            Out[i] = In[i] * (0.5f + fabs(adjustment));
        } else {
            Out[i] = In[i];
        }
    }
    return 1;
}

// Main fuzzer entry point for TAC detection and space sampling
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 200) {
        return 0;  // Need minimum data for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input: first part for profile data, second part for sampling parameters
    size_t profile_size = fdp.ConsumeIntegralInRange<size_t>(128, size - 72);
    std::vector<uint8_t> profile_data = fdp.ConsumeBytes<uint8_t>(profile_size);
    
    // Try to open profile from memory for TAC detection
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(profile_data.data(), (cmsUInt32Number)profile_data.size());
    if (hProfile == NULL) {
        // If we can't open a profile, test with built-in profiles
        hProfile = cmsCreate_sRGBProfile();
        if (hProfile == NULL) {
            return 0;  // Can't create profile, nothing to test
        }
    }
    
    // TEST 1: Total Area Coverage detection - PRIMARY TARGET (440 undiscovered branches)
    cmsFloat64Number tac_value = cmsDetectTAC(hProfile);
    // tac_value could be -1 on error, or a positive value representing TAC percentage
    
    // TEST 2: Create CLUT stages for sampling
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Create a 3D CLUT stage (16-bit) for sampling
    cmsUInt32Number gridPoints = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 17);
    cmsUInt32Number inputChannels = 3;  // RGB
    cmsUInt32Number outputChannels = 3; // RGB
    
    // Allocate CLUT stage with random table data
    std::vector<cmsUInt16Number> table16(gridPoints * gridPoints * gridPoints * outputChannels);
    for (size_t i = 0; i < table16.size() && fdp.remaining_bytes() > 0; i++) {
        table16[i] = fdp.ConsumeIntegral<uint16_t>();
    }
    
    cmsStage* clut16 = cmsStageAllocCLut16bit(context, gridPoints, inputChannels, outputChannels, table16.data());
    
    if (clut16 != NULL) {
        // Sample the CLUT using custom sampler
        cmsStageSampleCLut16bit(clut16, CustomSampler16, &fdp, 0);
        
        // Test with inspect flag
        if (fdp.ConsumeBool()) {
            cmsStageSampleCLut16bit(clut16, CustomSampler16, &fdp, SAMPLER_INSPECT);
        }
        
        cmsStageFree(clut16);
    }
    
    // Create a 3D CLUT stage (float) for sampling
    std::vector<cmsFloat32Number> tableFloat(gridPoints * gridPoints * gridPoints * outputChannels);
    for (size_t i = 0; i < tableFloat.size() && fdp.remaining_bytes() > 0; i++) {
        tableFloat[i] = fdp.ConsumeFloatingPoint<float>();
    }
    
    cmsStage* clutFloat = cmsStageAllocCLutFloat(context, gridPoints, inputChannels, outputChannels, tableFloat.data());
    
    if (clutFloat != NULL) {
        // Sample the CLUT using custom float sampler
        cmsStageSampleCLutFloat(clutFloat, CustomSamplerFloat, &fdp, 0);
        
        // Test with inspect flag
        if (fdp.ConsumeBool()) {
            cmsStageSampleCLutFloat(clutFloat, CustomSamplerFloat, &fdp, SAMPLER_INSPECT);
        }
        
        cmsStageFree(clutFloat);
    }
    
    // TEST 3: Test slice space functions (alternative sampling approach)
    cmsUInt32Number clutPoints[3] = {gridPoints, gridPoints, gridPoints};
    
    if (fdp.remaining_bytes() > 100) {
        cmsSliceSpace16(3, clutPoints, CustomSampler16, &fdp);
    }
    
    if (fdp.remaining_bytes() > 100) {
        cmsSliceSpaceFloat(3, clutPoints, CustomSamplerFloat, &fdp);
    }
    
    // TEST 4: Test with granular CLUT (non-uniform grid points)
    if (fdp.remaining_bytes() > 50) {
        cmsUInt32Number granularPoints[3];
        granularPoints[0] = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 9);
        granularPoints[1] = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 9);
        granularPoints[2] = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 9);
        
        std::vector<cmsUInt16Number> granularTable(granularPoints[0] * granularPoints[1] * granularPoints[2] * outputChannels);
        for (size_t i = 0; i < granularTable.size() && fdp.remaining_bytes() > 0; i++) {
            granularTable[i] = fdp.ConsumeIntegral<uint16_t>();
        }
        
        cmsStage* granularClut = cmsStageAllocCLut16bitGranular(context, granularPoints, inputChannels, outputChannels, granularTable.data());
        if (granularClut != NULL) {
            cmsStageFree(granularClut);
        }
    }
    
    // TEST 5: Additional TAC-related tests with different profile types
    if (fdp.ConsumeBool()) {
        // Test with Lab profile (Lab2 version, NULL for D50 white point)
        cmsHPROFILE hLab = cmsCreateLab2Profile(NULL);
        if (hLab != NULL) {
            cmsFloat64Number tac_lab = cmsDetectTAC(hLab);
            cmsCloseProfile(hLab);
        }
    }
    
    if (fdp.ConsumeBool()) {
        // Test with XYZ profile
        cmsHPROFILE hXYZ = cmsCreateXYZProfile();
        if (hXYZ != NULL) {
            cmsFloat64Number tac_xyz = cmsDetectTAC(hXYZ);
            cmsCloseProfile(hXYZ);
        }
    }
    
    // TEST 6: Test black point detection (related to TAC)
    cmsCIEXYZ blackPoint;
    if (fdp.ConsumeBool()) {
        cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
        cmsDetectBlackPoint(&blackPoint, hProfile, intent, 0);
    }
    
    if (fdp.ConsumeBool()) {
        cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
        cmsDetectDestinationBlackPoint(&blackPoint, hProfile, intent, 0);
    }
    
    // Cleanup
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
    
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_005
