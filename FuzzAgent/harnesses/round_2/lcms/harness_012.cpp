#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "lcms2.h"
#include "lcms2_plugin.h"

namespace harness_012 {
/*
 * Harness for testing MD5 profile ID computation and half-precision float conversion APIs
 * Targets: cmsMD5computeID, _cmsFloat2Half, _cmsHalf2Float
 * Source modules: cmsmd5.c and cmshalf.c (0% coverage)
 * Semantic domain: Cryptographic hashing and numeric conversion
 */


// Include Little CMS headers

// Declare internal functions that we want to test
// These are defined in the library but not in public headers
extern "C" {
// Half-precision conversion functions from cmshalf.c
cmsFloat32Number _cmsHalf2Float(cmsUInt16Number h);
cmsUInt16Number _cmsFloat2Half(cmsFloat32Number flt);

// MD5 helper functions from cmsmd5.c (used by cmsMD5computeID)
cmsHANDLE cmsMD5alloc(cmsContext ContextID);
void cmsMD5add(cmsHANDLE Handle, const cmsUInt8Number* buf, cmsUInt32Number len);
void cmsMD5finish(cmsProfileID* ProfileID, cmsHANDLE Handle);
}

// Safe conversion macro to avoid AFL instrumentation issues with values > 0x7fffffff
#define SAFE_U32(val) ((val) & 0x7fffffff)

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Minimum input size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Test half-precision float conversion APIs
    // Consume various float values to test conversion in both directions
    
    // Test 1: Basic float to half conversion with random floats
    int num_conversions = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    for (int i = 0; i < num_conversions && fdp.remaining_bytes() >= sizeof(float); i++) {
        // Generate random float values for testing
        float float_value = fdp.ConsumeFloatingPoint<float>();
        
        // Convert float to half-precision
        cmsUInt16Number half_value = _cmsFloat2Half(float_value);
        
        // Convert back to float
        cmsFloat32Number recovered_float = _cmsHalf2Float(half_value);
        
        // Note: We don't assert equality because half-precision has less precision
        // The conversion should at least not crash
        (void)recovered_float;  // Prevent unused variable warning
    }
    
    // Test 2: Special float values (infinity, NaN, zero, denormals)
    // Create a test set of special float values
    float special_floats[] = {
        0.0f,
        -0.0f,
        1.0f,
        -1.0f,
        2.0f,
        0.5f,
        -0.5f,
        // Generate some extreme values
        fdp.ConsumeFloatingPointInRange<float>(-1e10f, 1e10f),
        fdp.ConsumeFloatingPointInRange<float>(-1e-10f, 1e-10f)
    };
    
    for (size_t j = 0; j < sizeof(special_floats)/sizeof(special_floats[0]); j++) {
        cmsUInt16Number half_val = _cmsFloat2Half(special_floats[j]);
        cmsFloat32Number recovered = _cmsHalf2Float(half_val);
        (void)recovered;  // Prevent unused variable warning
    }
    
    // Phase 2: Test MD5 computation APIs
    // Create a simple ICC profile for MD5 computation testing
    
    // Create a context for the profile
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Create a simple sRGB profile for testing (simpler than custom RGB profile)
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(context);
    
    if (profile == NULL) {
        // Try creating a NULL profile if sRGB fails
        profile = cmsCreateNULLProfileTHR(context);
    }
    
    if (profile == NULL) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Test 1: Basic MD5 computation on the profile
    cmsBool md5_result = cmsMD5computeID(profile);
    (void)md5_result;  // Result can be TRUE or FALSE, both are valid
    
    // Test 2: Test MD5 APIs directly with random data
    // Create MD5 context
    cmsHANDLE md5_handle = cmsMD5alloc(context);
    if (md5_handle != NULL) {
        // Add random data to MD5
        size_t md5_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        if (md5_data_size > fdp.remaining_bytes()) {
            md5_data_size = fdp.remaining_bytes();
        }
        
        std::vector<uint8_t> md5_data = fdp.ConsumeBytes<uint8_t>(md5_data_size);
        if (!md5_data.empty()) {
            cmsMD5add(md5_handle, md5_data.data(), md5_data.size());
        }
        
        // Also add some fixed test data
        const char* test_string = "Test data for MD5";
        cmsMD5add(md5_handle, (const cmsUInt8Number*)test_string, strlen(test_string));
        
        // Finish MD5 computation
        cmsProfileID profile_id;
        cmsMD5finish(&profile_id, md5_handle);
        
        // The profile_id is now populated with MD5 hash
        (void)profile_id;  // Prevent unused variable warning
    }
    
    // Test 3: Test with different profile configurations
    // Modify profile header flags and recompute MD5
    cmsSetProfileVersion(profile, 4.4);  // ICC 4.4
    md5_result = cmsMD5computeID(profile);
    (void)md5_result;
    
    // Clean up
    cmsCloseProfile(profile);
    cmsDeleteContext(context);
    
    // Phase 3: Combined test - Use fuzzed data for both operations
    // Test half-precision conversion on values derived from fuzzed data
    if (fdp.remaining_bytes() >= sizeof(float) * 10) {
        for (int k = 0; k < 10; k++) {
            float test_float = fdp.ConsumeFloatingPoint<float>();
            cmsUInt16Number half_result = _cmsFloat2Half(test_float);
            cmsFloat32Number float_result = _cmsHalf2Float(half_result);
            (void)float_result;
        }
    }
    
    // Test edge cases for half-precision
    // Half-precision has limited range: approximately 6.1e-5 to 6.5e4
    // Test values near and beyond these limits
    float edge_cases[] = {
        65504.0f,  // Maximum half-precision value
        0.00006103515625f,  // Minimum positive normal half-precision value
        -65504.0f,
        -0.00006103515625f,
        fdp.ConsumeFloatingPointInRange<float>(-70000.0f, 70000.0f),  // Beyond range
        fdp.ConsumeFloatingPointInRange<float>(-0.00001f, 0.00001f)   // Subnormal range
    };
    
    for (size_t m = 0; m < sizeof(edge_cases)/sizeof(edge_cases[0]); m++) {
        cmsUInt16Number edge_half = _cmsFloat2Half(edge_cases[m]);
        cmsFloat32Number edge_recovered = _cmsHalf2Float(edge_half);
        (void)edge_recovered;
    }
    
    return 0;
}

} // namespace harness_012
