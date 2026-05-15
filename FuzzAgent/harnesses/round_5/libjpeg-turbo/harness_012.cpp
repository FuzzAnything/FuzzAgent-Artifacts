#include "turbojpeg.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <memory>

namespace harness_012 {
/*
 * Fuzzing harness for libjpeg-turbo targeting ICC Profile operations
 * This harness specifically targets ICC Profile metadata handling with high coverage impact:
 * - tj3SetICCProfile: 14 undiscovered branches, 0% coverage (reached but not executed)
 * - tj3GetICCProfile: 20 undiscovered branches, 0% coverage, unreached
 * 
 * Target ICC Profile APIs:
 * 1. tj3SetICCProfile - Embed ICC color management profile in JPEG images
 * 2. tj3GetICCProfile - Retrieve embedded ICC profile from JPEG images
 * 
 * Required Helper APIs for lifecycle:
 * - tj3InitVersion: Initialize TurboJPEG instance with version checking
 * - tj3Destroy: Cleanup TurboJPEG instance
 * 
 * Alternative paths to explore:
 * - Combine with compression (tj3Compress8) to create JPEG with embedded ICC
 * - Combine with decompression (tj3Decompress8) to read JPEG with ICC profile
 * - Test with NULL/empty ICC profiles for removal functionality
 * - Test with various ICC profile sizes and content
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: RGB compression/decompression with legacy APIs
 * - harness_001.cpp: YUV decompression APIs  
 * - harness_002.cpp: Transformation operations
 * - harness_003.cpp: 12/16-bit precision compression/decompression
 * - harness_004.cpp: Image file I/O operations
 * - harness_011.cpp: 12-bit JPEG operations with classic libjpeg API
 * - This harness: Focuses EXCLUSIVELY on ICC Profile metadata handling in TurboJPEG
 * 
 * Coverage strategy:
 * 1. Basic set/get ICC profile lifecycle
 * 2. ICC profile with compression operations
 * 3. ICC profile with decompression operations  
 * 4. NULL/empty profile handling (profile removal)
 * 5. Memory allocation and cleanup paths in ICC profile handling
 * 6. Error handling paths for invalid operations
 * 
 * Expected Coverage Gain:
 * - Direct coverage of 2 completely uncovered APIs
 * - 34+ undiscovered branches
 * - Exploration of JPEG metadata path distinct from pixel data processing
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for basic parameters + ICC profile data
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int initType = fdp.ConsumeIntegralInRange<int>(TJINIT_COMPRESS, TJINIT_DECOMPRESS);
    int apiVersion = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 3); // 0: set/get only, 1: with compression, 2: with decompression, 3: NULL profile
    
    // Consume image parameters for compression/decompression paths
    int width = fdp.ConsumeIntegralInRange<int>(1, 256);
    int height = fdp.ConsumeIntegralInRange<int>(1, 256);
    int pixelFormat = fdp.ConsumeIntegralInRange<int>(0, 12); // TJPF enum values
    int subsamp = fdp.ConsumeIntegralInRange<int>(0, 5); // TJSAMP enum values
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Validate pixel format
    if (pixelFormat < 0 || pixelFormat >= 12) {
        pixelFormat = TJPF_RGB; // Default to RGB if out of range
    }
    
    // Get pixel size for the format
    static const int tjPixelSize[TJ_NUMPF] = {3, 3, 4, 4, 4, 4, 1, 4, 4, 4, 4, 4};
    int bytesPerPixel = tjPixelSize[pixelFormat];
    
    // Initialize TurboJPEG instance
    tjhandle handle = tj3InitVersion(initType, apiVersion);
    if (!handle) {
        // Try with default API version if custom version fails
        handle = tj3Init(initType);
        if (!handle) return 0;
    }
    
    // Test different ICC profile scenarios based on operation type
    unsigned char* iccBuf = nullptr;
    size_t iccSize = 0;
    unsigned char* retrievedIccBuf = nullptr;
    size_t retrievedIccSize = 0;
    
    int result = 0;
    
    switch (operation) {
        case 0: // Basic set/get ICC profile lifecycle
        case 1: // ICC profile with compression
        case 2: // ICC profile with decompression
        {
            // Consume ICC profile data from fuzzer input
            size_t maxIccSize = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
            if (fdp.remaining_bytes() < maxIccSize) {
                maxIccSize = fdp.remaining_bytes();
            }
            
            if (maxIccSize > 0) {
                std::vector<uint8_t> iccData = fdp.ConsumeBytes<uint8_t>(maxIccSize);
                iccBuf = iccData.data();
                iccSize = iccData.size();
                
                // Set ICC profile
                result = tj3SetICCProfile(handle, iccBuf, iccSize);
                if (result != 0) {
                    // Error setting ICC profile, but continue with cleanup
                }
                
                // Get ICC profile back
                result = tj3GetICCProfile(handle, &retrievedIccBuf, &retrievedIccSize);
                if (result == 0 && retrievedIccBuf && retrievedIccSize > 0) {
                    // Verify we got something back (may not match exactly due to internal copying)
                    // Free the retrieved buffer
                    tj3Free(retrievedIccBuf);
                    retrievedIccBuf = nullptr;
                }
                
                // Test getting just the size without buffer
                size_t sizeOnly = 0;
                result = tj3GetICCProfile(handle, nullptr, &sizeOnly);
                
                if (operation == 1 && initType == TJINIT_COMPRESS) {
                    // ICC profile with compression path
                    // Create a simple image buffer
                    size_t imageSize = width * height * bytesPerPixel;
                    if (fdp.remaining_bytes() >= imageSize) {
                        std::vector<uint8_t> srcBuffer = fdp.ConsumeBytes<uint8_t>(imageSize);
                        
                        unsigned char* jpegBuf = nullptr;
                        size_t jpegSize = 0;
                        
                        // Compress with ICC profile embedded
                        result = tj3Compress8(handle, 
                                            srcBuffer.data(),
                                            width,
                                            width * bytesPerPixel, // pitch
                                            height,
                                            pixelFormat,
                                            &jpegBuf,
                                            &jpegSize);
                        
                        if (result == 0 && jpegBuf && jpegSize > 0) {
                            // Free the compressed buffer
                            tj3Free(jpegBuf);
                        }
                    }
                }
                else if (operation == 2 && initType == TJINIT_DECOMPRESS) {
                    // ICC profile with decompression path
                    // First we need a JPEG to decompress - for simplicity, 
                    // use remaining bytes as JPEG data
                    if (fdp.remaining_bytes() > 0) {
                        std::vector<uint8_t> jpegData = fdp.ConsumeRemainingBytes<uint8_t>();
                        
                        // Try to decompress header to see if it's valid JPEG
                        result = tj3DecompressHeader(handle,
                                                    jpegData.data(),
                                                    jpegData.size());
                        
                        if (result == 0) {
                            // Header decompression successful
                            // Try to get ICC profile from the decompressed JPEG
                            // (this would only work if the JPEG actually contains an ICC profile)
                            unsigned char* decompIccBuf = nullptr;
                            size_t decompIccSize = 0;
                            result = tj3GetICCProfile(handle, &decompIccBuf, &decompIccSize);
                            
                            if (result == 0 && decompIccBuf && decompIccSize > 0) {
                                tj3Free(decompIccBuf);
                            }
                        }
                    }
                }
            }
            break;
        }
        
        case 3: // NULL/empty ICC profile handling (profile removal)
        {
            // First set a dummy ICC profile
            uint8_t dummyProfile[] = {0x00, 0x01, 0x02, 0x03, 0x04};
            result = tj3SetICCProfile(handle, dummyProfile, sizeof(dummyProfile));
            
            // Remove ICC profile by setting NULL/0
            result = tj3SetICCProfile(handle, nullptr, 0);
            
            // Verify profile is removed
            result = tj3GetICCProfile(handle, &retrievedIccBuf, &retrievedIccSize);
            if (result == 0 && retrievedIccSize == 0) {
                // Profile successfully removed
            }
            
            // Also test with NULL buffer but non-zero size (should remove)
            result = tj3SetICCProfile(handle, nullptr, 100);
            
            // Test with valid buffer but zero size (should remove)
            uint8_t dummyProfile2[] = {0x05, 0x06, 0x07};
            result = tj3SetICCProfile(handle, dummyProfile2, 0);
            break;
        }
    }
    
    // Test error handling: try to get ICC profile without setting one first
    // (when operation is NULL profile case, we already tested this)
    if (operation != 3) {
        // Create a new handle to test uninitialized state
        tjhandle handle2 = tj3Init(initType);
        if (handle2) {
            unsigned char* testIccBuf = nullptr;
            size_t testIccSize = 0;
            result = tj3GetICCProfile(handle2, &testIccBuf, &testIccSize);
            // Result may be -1 (error) or 0 with NULL buffer and 0 size
            if (testIccBuf) {
                tj3Free(testIccBuf);
            }
            tj3Destroy(handle2);
        }
    }
    
    // Test setting ICC profile on decompression-only handle (should fail)
    if (initType == TJINIT_DECOMPRESS) {
        uint8_t testProfile[] = {0x10, 0x20, 0x30};
        // This should fail or be ignored since handle is for decompression
        result = tj3SetICCProfile(handle, testProfile, sizeof(testProfile));
    }
    
    // Test getting ICC profile from compression-only handle without setting (should return NULL/0)
    if (initType == TJINIT_COMPRESS) {
        unsigned char* testGetBuf = nullptr;
        size_t testGetSize = 0;
        result = tj3GetICCProfile(handle, &testGetBuf, &testGetSize);
        if (testGetBuf) {
            tj3Free(testGetBuf);
        }
    }
    
    // Cleanup
    tj3Destroy(handle);
    
    return 0;
}

} // namespace harness_012
