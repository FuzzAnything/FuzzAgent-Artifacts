#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include "lcms2.h"
#include "lcms2_plugin.h"

namespace harness_023 {
/*
 * Fuzzing harness for LittleCMS2 library
 * Targets: Comprehensive PostScript color resource generation APIs in cmsps2.c module
 * harness_023.cpp - Focus on cmsGetPostScriptColorResource, cmsGetPostScriptCSA, cmsGetPostScriptCRD
 * with semantic diversity from previous harnesses:
 *   harness_001: Basic proofing/PostScript CRD generation
 *   harness_011: Unified cmsGetPostScriptColorResource with cmsIOHANDLER
 * This harness provides comprehensive testing with different profile types,
 * intent combinations, flag variations, and edge cases
 */


// Include Little CMS headers

// Helper function to read all tags from a profile
static void ReadAllTags(cmsHPROFILE h) {
    cmsInt32Number i, n;
    cmsTagSignature sig;
    uint32_t sig_u32;

    n = cmsGetTagCount(h);
    for (i = 0; i < n; i++) {
        sig = cmsGetTagSignature(h, i);
        // Store signature in uint32_t to avoid AFL instrumentation issues with values > 0x7fffffff
        sig_u32 = (uint32_t)sig & 0x7fffffff;
        (void)sig_u32; // Use the value to avoid unused variable warning
        if (cmsReadTag(h, sig) == NULL) return;
    }
}

// Helper function to read all raw tags from a profile
static void ReadAllRAWTags(cmsHPROFILE h) {
    cmsInt32Number i, n;
    cmsTagSignature sig;
    uint32_t sig_u32;
    cmsInt32Number len;

    n = cmsGetTagCount(h);
    for (i = 0; i < n; i++) {
        sig = cmsGetTagSignature(h, i);
        // Store signature in uint32_t to avoid AFL instrumentation issues with values > 0x7fffffff
        sig_u32 = (uint32_t)sig & 0x7fffffff;
        (void)sig_u32; // Use the value to avoid unused variable warning
        len = cmsReadRawTag(h, sig, NULL, 0);
        (void)len;
    }
}

// Generate PostScript resource using the unified API with cmsIOHANDLER
static void TestUnifiedResourceAPI(cmsContext ContextID, 
                                  cmsPSResourceType Type,
                                  cmsHPROFILE hProfile, 
                                  cmsUInt32Number Intent, 
                                  cmsUInt32Number dwFlags,
                                  const std::vector<uint8_t>& buffer) {
    cmsIOHANDLER* mem;
    cmsUInt32Number dwBytesUsed;

    // Set up the serialization engine
    if (buffer.empty()) {
        mem = cmsOpenIOhandlerFromNULL(ContextID);
    } else {
        mem = cmsOpenIOhandlerFromMem(ContextID, (void*)buffer.data(), 
                                     (cmsUInt32Number)buffer.size(), "w");
    }

    if (!mem) return;

    // Use the unified color resource API
    dwBytesUsed = cmsGetPostScriptColorResource(ContextID, Type, hProfile, 
                                               Intent, dwFlags, mem);

    // Clean up memory stream
    cmsCloseIOhandler(mem);
    (void)dwBytesUsed;
}

// Test CSA generation with direct buffer API
static void TestDirectCSA(cmsContext ContextID,
                         cmsHPROFILE hProfile,
                         cmsUInt32Number Intent,
                         cmsUInt32Number dwFlags,
                         const std::vector<uint8_t>& buffer) {
    cmsUInt32Number n;
    char* allocated_buffer = NULL;

    // First call to get size
    n = cmsGetPostScriptCSA(ContextID, hProfile, Intent, dwFlags, NULL, 0);
    if (n == 0) return;

    // Allocate buffer or use provided buffer
    if (buffer.empty()) {
        allocated_buffer = (char*)_cmsMalloc(ContextID, n);
        if (allocated_buffer) {
            cmsGetPostScriptCSA(ContextID, hProfile, Intent, dwFlags, 
                               allocated_buffer, n);
            _cmsFree(ContextID, allocated_buffer);
        }
    } else if (buffer.size() >= n) {
        // Use provided buffer if large enough
        cmsGetPostScriptCSA(ContextID, hProfile, Intent, dwFlags, 
                           (void*)buffer.data(), (cmsUInt32Number)buffer.size());
    }
}

// Test CRD generation with direct buffer API
static void TestDirectCRD(cmsContext ContextID,
                         cmsHPROFILE hProfile,
                         cmsUInt32Number Intent,
                         cmsUInt32Number dwFlags,
                         const std::vector<uint8_t>& buffer) {
    cmsUInt32Number n;
    char* allocated_buffer = NULL;

    // First call to get size
    n = cmsGetPostScriptCRD(ContextID, hProfile, Intent, dwFlags, NULL, 0);
    if (n == 0) return;

    // Allocate buffer or use provided buffer
    if (buffer.empty()) {
        allocated_buffer = (char*)_cmsMalloc(ContextID, n);
        if (allocated_buffer) {
            cmsGetPostScriptCRD(ContextID, hProfile, Intent, dwFlags, 
                               allocated_buffer, n);
            _cmsFree(ContextID, allocated_buffer);
        }
    } else if (buffer.size() >= n) {
        // Use provided buffer if large enough
        cmsGetPostScriptCRD(ContextID, hProfile, Intent, dwFlags, 
                           (void*)buffer.data(), (cmsUInt32Number)buffer.size());
    }
}

// Create different types of profiles for testing
static cmsHPROFILE CreateTestProfile(cmsContext ContextID, 
                                     cmsColorSpaceSignature colorSpace,
                                     FuzzedDataProvider& fdp) {
    switch (colorSpace) {
        case cmsSigRgbData:
            return cmsCreate_sRGBProfileTHR(ContextID);
        
        case cmsSigCmykData:
            // CMYK profile creation is not available in public API, use NULL profile instead
            return cmsCreateNULLProfileTHR(ContextID);
        
        case cmsSigGrayData: {
            // Create a simple gamma curve for grayscale
            cmsToneCurve* gammaCurve = cmsBuildGamma(ContextID, 2.2);
            cmsHPROFILE profile = cmsCreateGrayProfileTHR(ContextID, 
                                                         cmsD50_xyY(), gammaCurve);
            if (gammaCurve) cmsFreeToneCurve(gammaCurve);
            return profile;
        }
        
        case cmsSigLabData:
            return cmsCreateLab2ProfileTHR(ContextID, NULL);
        
        default:
            return NULL;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 512) {
        return 0;  // Minimum input size for comprehensive testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Create a context for operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Test scenario selector
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (scenario) {
        case 0: {
            // Scenario 1: Test with memory-based ICC profiles
            size_t profile_size = fdp.ConsumeIntegralInRange<size_t>(256, size/2);
            std::vector<uint8_t> profile_data = fdp.ConsumeBytes<uint8_t>(profile_size);
            
            cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(context, 
                                                           profile_data.data(), 
                                                           (cmsUInt32Number)profile_data.size());
            if (hProfile == NULL) {
                cmsDeleteContext(context);
                return 0;
            }
            
            // Read profile information
            ReadAllTags(hProfile);
            ReadAllRAWTags(hProfile);
            
            // Consume test parameters
            uint8_t resource_type_sel = fdp.ConsumeIntegral<uint8_t>() % 2;
            uint8_t intent = fdp.ConsumeIntegral<uint8_t>() % 4;
            uint32_t flags = fdp.ConsumeIntegral<uint32_t>();
            
            cmsPSResourceType resource_type = (resource_type_sel == 0) ? 
                                              cmsPS_RESOURCE_CSA : cmsPS_RESOURCE_CRD;
            
            // Test buffer for resource generation
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, fdp.remaining_bytes());
            std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            // Test all three APIs
            TestUnifiedResourceAPI(context, resource_type, hProfile, intent, flags, buffer);
            TestDirectCSA(context, hProfile, intent, flags, buffer);
            TestDirectCRD(context, hProfile, intent, flags, buffer);
            
            // Test with NULL buffer
            std::vector<uint8_t> empty_buffer;
            TestUnifiedResourceAPI(context, resource_type, hProfile, intent, flags, empty_buffer);
            TestDirectCSA(context, hProfile, intent, flags, empty_buffer);
            TestDirectCRD(context, hProfile, intent, flags, empty_buffer);
            
            cmsCloseProfile(hProfile);
            break;
        }
        
        case 1: {
            // Scenario 2: Test with different built-in profile types
            uint8_t profile_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            cmsColorSpaceSignature colorSpaces[] = {
                cmsSigRgbData, cmsSigCmykData, cmsSigGrayData, cmsSigLabData
            };
            
            cmsHPROFILE hProfile = CreateTestProfile(context, colorSpaces[profile_type], fdp);
            if (hProfile == NULL) {
                cmsDeleteContext(context);
                return 0;
            }
            
            // Test various flag combinations
            uint32_t flag_combinations[] = {
                0,  // No flags
                cmsFLAGS_NODEFAULTRESOURCEDEF,
                cmsFLAGS_BLACKPOINTCOMPENSATION,
                cmsFLAGS_NODEFAULTRESOURCEDEF | cmsFLAGS_BLACKPOINTCOMPENSATION,
                cmsFLAGS_GRIDPOINTS(5),
                cmsFLAGS_GRIDPOINTS(17) | cmsFLAGS_BLACKPOINTCOMPENSATION
            };
            
            for (size_t i = 0; i < sizeof(flag_combinations)/sizeof(flag_combinations[0]); i++) {
                if (fdp.remaining_bytes() < 64) break;
                
                uint8_t intent = fdp.ConsumeIntegral<uint8_t>() % 4;
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(32, 128);
                std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
                
                // Test both CSA and CRD
                TestUnifiedResourceAPI(context, cmsPS_RESOURCE_CSA, hProfile, 
                                      intent, flag_combinations[i], buffer);
                TestUnifiedResourceAPI(context, cmsPS_RESOURCE_CRD, hProfile, 
                                      intent, flag_combinations[i], buffer);
            }
            
            cmsCloseProfile(hProfile);
            break;
        }
        
        case 2: {
            // Scenario 3: Test edge cases and special conditions
            // Create an sRGB profile for consistent testing
            cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(context);
            if (hProfile == NULL) {
                cmsDeleteContext(context);
                return 0;
            }
            
            // Test with various buffer sizes (including very small and very large)
            size_t buffer_sizes[] = {0, 1, 10, 100, 1000, 10000};
            
            for (size_t i = 0; i < sizeof(buffer_sizes)/sizeof(buffer_sizes[0]); i++) {
                if (fdp.remaining_bytes() < buffer_sizes[i]) continue;
                
                std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_sizes[i]);
                uint8_t intent = fdp.ConsumeIntegral<uint8_t>() % 4;
                uint32_t flags = fdp.ConsumeIntegral<uint32_t>();
                
                // Test all APIs with this buffer
                TestUnifiedResourceAPI(context, cmsPS_RESOURCE_CSA, hProfile, 
                                      intent, flags, buffer);
                TestUnifiedResourceAPI(context, cmsPS_RESOURCE_CRD, hProfile, 
                                      intent, flags, buffer);
                
                if (!buffer.empty()) {
                    TestDirectCSA(context, hProfile, intent, flags, buffer);
                    TestDirectCRD(context, hProfile, intent, flags, buffer);
                }
            }
            
            // Test invalid intent values
            for (int invalid_intent = 4; invalid_intent < 10; invalid_intent++) {
                if (fdp.remaining_bytes() < 32) break;
                
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(32, 64);
                std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
                
                TestUnifiedResourceAPI(context, cmsPS_RESOURCE_CSA, hProfile, 
                                      invalid_intent, 0, buffer);
            }
            
            cmsCloseProfile(hProfile);
            break;
        }
        
        case 3: {
            // Scenario 4: Test with multiple profiles and mixed operations
            // Create several different profile types
            cmsHPROFILE profiles[3] = {NULL, NULL, NULL};
            cmsColorSpaceSignature colorSpaces[] = {
                cmsSigRgbData, cmsSigCmykData, cmsSigGrayData
            };
            
            for (int i = 0; i < 3; i++) {
                profiles[i] = CreateTestProfile(context, colorSpaces[i], fdp);
                if (profiles[i] == NULL) {
                    // Clean up any created profiles
                    for (int j = 0; j < i; j++) {
                        cmsCloseProfile(profiles[j]);
                    }
                    cmsDeleteContext(context);
                    return 0;
                }
            }
            
            // Test mixed operations across profiles
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 64; i++) {
                uint8_t intent = fdp.ConsumeIntegral<uint8_t>() % 4;
                uint32_t flags = fdp.ConsumeIntegral<uint32_t>();
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 256);
                std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
                
                // Alternate between CSA and CRD
                cmsPSResourceType resource_type = (i % 2 == 0) ? 
                                                  cmsPS_RESOURCE_CSA : cmsPS_RESOURCE_CRD;
                
                TestUnifiedResourceAPI(context, resource_type, profiles[i], 
                                      intent, flags, buffer);
                TestDirectCSA(context, profiles[i], intent, flags, buffer);
                TestDirectCRD(context, profiles[i], intent, flags, buffer);
            }
            
            // Clean up profiles
            for (int i = 0; i < 3; i++) {
                if (profiles[i]) cmsCloseProfile(profiles[i]);
            }
            break;
        }
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_023
