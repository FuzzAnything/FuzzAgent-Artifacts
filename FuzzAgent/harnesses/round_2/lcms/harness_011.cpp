#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_011 {
// Fuzzing harness for LittleCMS2 library
// Targets PostScript color resource generation APIs in cmsps2.c module
// harness_011.cpp - Focus on cmsGetPostScriptColorResource and related functions
// Note: harness_001 already targeted cmsGetPostScriptCSA/cmsGetPostScriptCRD, 
// so this harness focuses on the unified cmsGetPostScriptColorResource API with cmsIOHANDLER


// Safe conversion macro to avoid AFL instrumentation issues with values > 0x7fffffff
#define SAFE_U32(val) ((val) & 0x7fffffff)

// Helper function to read all tags from a profile
// Uses uint32_t for tag signatures to avoid AFL instrumentation issues with values > 0x7fffffff
static void ReadAllTags(cmsHPROFILE h) {
    cmsInt32Number i, n;
    uint32_t sig_u32;

    n = cmsGetTagCount(h);
    for (i = 0; i < n; i++) {
        // Get tag signature and mask to keep within safe range for AFL instrumentation
        sig_u32 = SAFE_U32((uint32_t)cmsGetTagSignature(h, i));
        // Cast to cmsTagSignature for API call (safe after masking)
        cmsTagSignature sig = (cmsTagSignature)sig_u32;
        if (cmsReadTag(h, sig) == NULL) return;
    }
}

// Helper function to read all raw tags from a profile
// Uses uint32_t for tag signatures to avoid AFL instrumentation issues with values > 0x7fffffff
static void ReadAllRAWTags(cmsHPROFILE h) {
    cmsInt32Number i, n;
    uint32_t sig_u32;
    cmsInt32Number len;

    n = cmsGetTagCount(h);
    for (i = 0; i < n; i++) {
        // Get tag signature and mask to keep within safe range for AFL instrumentation
        sig_u32 = SAFE_U32((uint32_t)cmsGetTagSignature(h, i));
        // Cast to cmsTagSignature for API call (safe after masking)
        cmsTagSignature sig = (cmsTagSignature)sig_u32;
        len = cmsReadRawTag(h, sig, NULL, 0);
        (void)len; // Use the value to avoid unused variable warning
    }
}

// Generate PostScript color resource using cmsGetPostScriptColorResource with cmsIOHANDLER
static void GeneratePostScriptResource(cmsContext ContextID, 
                                      cmsPSResourceType Type,
                                      cmsHPROFILE hProfile, 
                                      cmsUInt32Number Intent, 
                                      cmsUInt32Number dwFlags,
                                      void* Buffer, 
                                      cmsUInt32Number dwBufferLen) {
    cmsIOHANDLER* mem;
    cmsUInt32Number dwBytesUsed;

    // Set up the serialization engine
    if (Buffer == NULL)
        mem = cmsOpenIOhandlerFromNULL(ContextID);
    else
        mem = cmsOpenIOhandlerFromMem(ContextID, Buffer, dwBufferLen, "w");

    if (!mem) return;

    // Use the unified color resource API
    dwBytesUsed = cmsGetPostScriptColorResource(ContextID, Type, hProfile, Intent, dwFlags, mem);

    // Clean up memory stream
    cmsCloseIOhandler(mem);

    // The return value dwBytesUsed is ignored in fuzzing context
    (void)dwBytesUsed;
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // We need at least: 256 bytes for profile + 32 bytes for parameters = 288 bytes
    if (size < 288) {
        return 0;  // Insufficient input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input: first part for profile data
    size_t max_profile_size = size / 2;
    if (max_profile_size < 256) {
        return 0;  // Not enough data for profile
    }
    size_t profile_size = fdp.ConsumeIntegralInRange<size_t>(256, max_profile_size);
    std::vector<uint8_t> profile_data = fdp.ConsumeBytes<uint8_t>(profile_size);
    
    // Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(profile_data.data(), (cmsUInt32Number)profile_data.size());
    if (hProfile == NULL) {
        return 0;  // Invalid profile, nothing to test
    }
    
    // Read profile information to increase coverage
    ReadAllTags(hProfile);
    ReadAllRAWTags(hProfile);
    
    // Consume parameters for PostScript resource generation
    uint8_t resource_type_sel = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0=CSA, 1=CRD
    uint8_t intent = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0-3 for different intents
    uint32_t flags = fdp.ConsumeIntegral<uint32_t>();
    
    // Convert selection to actual resource type
    cmsPSResourceType resource_type = (resource_type_sel == 0) ? cmsPS_RESOURCE_CSA : cmsPS_RESOURCE_CRD;
    
    // Check remaining bytes for buffer allocation
    size_t remaining = fdp.remaining_bytes();
    if (remaining < 64) {
        cmsCloseProfile(hProfile);
        return 0;
    }
    
    // Allocate buffer for resource output
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, remaining);
    std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    
    // Test 1: Generate resource with buffer (using cmsOpenIOhandlerFromMem)
    GeneratePostScriptResource(0, resource_type, hProfile, intent, flags, 
                              buffer.data(), (cmsUInt32Number)buffer.size());
    
    // Test 2: Generate resource without buffer (using cmsOpenIOhandlerFromNULL)
    // This tests the case where Buffer is NULL and size calculation mode is used
    GeneratePostScriptResource(0, resource_type, hProfile, intent, flags, NULL, 0);
    
    // Test 3: Test with different combinations of flags
    // Try some common flag combinations for PostScript generation
    uint32_t test_flags[] = {
        0,  // No flags
        cmsFLAGS_NODEFAULTRESOURCEDEF,
        cmsFLAGS_BLACKPOINTCOMPENSATION,
        cmsFLAGS_NODEFAULTRESOURCEDEF | cmsFLAGS_BLACKPOINTCOMPENSATION
    };
    
    for (size_t i = 0; i < sizeof(test_flags)/sizeof(test_flags[0]); i++) {
        if (fdp.remaining_bytes() > 32) {
            // Use small buffer for this test
            size_t small_buffer_size = fdp.ConsumeIntegralInRange<size_t>(32, 128);
            std::vector<uint8_t> small_buffer = fdp.ConsumeBytes<uint8_t>(small_buffer_size);
            
            GeneratePostScriptResource(0, resource_type, hProfile, intent, test_flags[i],
                                      small_buffer.data(), (cmsUInt32Number)small_buffer.size());
        }
    }
    
    // Test 4: Test the other resource type as well
    cmsPSResourceType other_type = (resource_type == cmsPS_RESOURCE_CSA) ? 
                                   cmsPS_RESOURCE_CRD : cmsPS_RESOURCE_CSA;
    
    if (fdp.remaining_bytes() > 32) {
        size_t final_buffer_size = fdp.ConsumeIntegralInRange<size_t>(32, fdp.remaining_bytes());
        std::vector<uint8_t> final_buffer = fdp.ConsumeBytes<uint8_t>(final_buffer_size);
        
        GeneratePostScriptResource(0, other_type, hProfile, intent, flags,
                                  final_buffer.data(), (cmsUInt32Number)final_buffer.size());
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}

} // namespace harness_011
