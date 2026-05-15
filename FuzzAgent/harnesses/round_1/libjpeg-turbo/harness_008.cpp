#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "turbojpeg.h"

namespace harness_008 {
/*
 * libjpeg-turbo fuzzing harness targeting uncovered JPEG header reading APIs
 * This harness specifically targets tjDecompressHeader (0% coverage, 72 undiscovered branches) 
 * and tjDecompressHeader2 (0% coverage, 72 undiscovered branches) as identified by coverage analysis.
 * 
 * Target APIs: tjDecompressHeader, tjDecompressHeader2
 * Required Helper Functions: tjInitDecompress (handle creation), tjDestroy (cleanup)
 * 
 * This harness implements:
 * 1. Exclusive focus on header reading/parsing operations before any decompression
 * 2. Testing both tjDecompressHeader (basic width/height extraction) and tjDecompressHeader2 (extended with subsampling)
 * 3. Using fuzzer input as compressed JPEG data to test various header formats and corruptions
 * 4. Semantic diversity from existing harnesses - focuses on header parsing only, not full decompression
 * 
 * This harness is semantically differentiated from existing harnesses by:
 * 1. Targeting tjDecompressHeader and tjDecompressHeader2 specifically (0% coverage APIs)
 * 2. Not performing any actual decompression - only header reading operations
 * 3. Testing the foundational JPEG header parsing code paths before any pixel processing
 * 4. Focusing on error handling in header validation and parsing logic
 */


extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum input size check - need at least some data for JPEG header testing
    if (size < 4) return 0;  // Need at least minimal data for testing
    
    FuzzedDataProvider fdp(data, size);
    
    /* Initialize decompression handle */
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    /* Strategy: Use the fuzzer input as potential JPEG compressed data
     * We'll test various header reading operations on this data */
    
    /* Consume a portion of the input for JPEG data testing */
    size_t jpeg_test_size = fdp.ConsumeIntegralInRange<size_t>(4, size);
    std::vector<uint8_t> jpeg_test_data = fdp.ConsumeBytes<uint8_t>(jpeg_test_size);
    
    if (jpeg_test_data.empty()) {
        tjDestroy(handle);
        return 0;
    }
    
    /* Variables to store header information */
    int width = 0, height = 0, jpegSubsamp = 0;
    
    /* TEST 1: tjDecompressHeader - Basic header reading (width and height only) */
    /* This API reads only width and height from JPEG header */
    int result1 = tjDecompressHeader(handle, 
                                     jpeg_test_data.data(), 
                                     (unsigned long)jpeg_test_data.size(),
                                     &width, 
                                     &height);
    
    /* We don't check the result - we want to test both success and failure paths
     * The fuzzer input may be valid or invalid JPEG data, and we want to cover
     * error handling in the header parsing code */
    
    /* Log the results for coverage tracking (no-op in production) */
    (void)result1;  // Suppress unused variable warning
    (void)width;    // Suppress unused variable warning  
    (void)height;   // Suppress unused variable warning
    
    /* TEST 2: tjDecompressHeader2 - Extended header reading (width, height, subsampling) */
    /* Reset variables for second test */
    width = 0;
    height = 0;
    jpegSubsamp = 0;
    
    /* This API reads width, height, and JPEG subsampling type */
    int result2 = tjDecompressHeader2(handle,
                                      jpeg_test_data.data(),
                                      (unsigned long)jpeg_test_data.size(),
                                      &width,
                                      &height,
                                      &jpegSubsamp);
    
    /* Again, we don't check the result - we're testing the API calls themselves
     * to ensure coverage of the 72 undiscovered branches in each function */
    (void)result2;     // Suppress unused variable warning
    (void)jpegSubsamp; // Suppress unused variable warning
    
    /* TEST 3: Additional variations with different input sizes and offsets */
    /* Test with different slices of the input data to exercise boundary conditions */
    if (jpeg_test_data.size() > 8) {
        /* Test with first half of data */
        size_t half_size = jpeg_test_data.size() / 2;
        int width3 = 0, height3 = 0, subsamp3 = 0;
        
        tjDecompressHeader(handle, 
                          jpeg_test_data.data(), 
                          (unsigned long)half_size,
                          &width3, 
                          &height3);
        
        /* Test with second half of data */
        if (half_size > 4) {
            tjDecompressHeader2(handle,
                               jpeg_test_data.data() + half_size,
                               (unsigned long)(jpeg_test_data.size() - half_size),
                               &width3,
                               &height3,
                               &subsamp3);
        }
    }
    
    /* TEST 4: Test with minimum viable input sizes */
    /* Some JPEG headers might be very small or malformed */
    if (jpeg_test_data.size() >= 4) {
        int width4 = 0, height4 = 0;
        
        /* Test with just 4 bytes (minimum for some header checks) */
        tjDecompressHeader(handle,
                          jpeg_test_data.data(),
                          4,
                          &width4,
                          &height4);
        
        if (jpeg_test_data.size() >= 8) {
            int subsamp4 = 0;
            tjDecompressHeader2(handle,
                               jpeg_test_data.data(),
                               8,
                               &width4,
                               &height4,
                               &subsamp4);
        }
    }
    
    /* TEST 5: Test with NULL pointers for output parameters (should be handled gracefully) */
    /* Some implementations might handle NULL pointers differently */
    if (jpeg_test_data.size() >= 16) {
        /* Call with NULL width/height pointers */
        tjDecompressHeader(handle,
                          jpeg_test_data.data(),
                          (unsigned long)jpeg_test_data.size(),
                          NULL,
                          NULL);
        
        /* Call with NULL pointers for all outputs */
        tjDecompressHeader2(handle,
                           jpeg_test_data.data(),
                           (unsigned long)jpeg_test_data.size(),
                           NULL,
                           NULL,
                           NULL);
    }
    
    /* Cleanup */
    tjDestroy(handle);
    
    return 0;
}

} // namespace harness_008
