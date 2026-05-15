#include "turbojpeg.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_003 {
/*
 * Fuzzing harness for libjpeg-turbo targeting 12/16-bit precision JPEG operations
 * This harness specifically targets high-bit-depth JPEG compression/decompression APIs:
 * - tj3Decompress12: Decompress JPEG to 12-bit packed-pixel image
 * - tj3Compress12: Compress 12-bit packed-pixel image to JPEG
 * - tj3Decompress16: Decompress lossless JPEG to 16-bit packed-pixel image
 * - tj3Compress16: Compress 16-bit packed-pixel image to lossless JPEG
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: Focuses on 8-bit RGB compression/decompression with legacy APIs
 * - harness_001.cpp: Focuses on YUV decompression APIs
 * - harness_002.cpp: Focuses on transformation operations
 * - This harness: Focuses exclusively on high-bit-depth (12/16-bit) operations with modern tj3 APIs
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for basic parameters + some image data
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int width = fdp.ConsumeIntegralInRange<int>(1, 512);  // Conservative limit for high-bit-depth
    int height = fdp.ConsumeIntegralInRange<int>(1, 512);
    int pixelFormat = fdp.ConsumeIntegralInRange<int>(0, 12); // TJPF enum values (0-11 + 1 for invalid)
    int subsamp = fdp.ConsumeIntegralInRange<int>(0, 5); // TJSAMP enum values
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    int flags = fdp.ConsumeIntegral<int>();
    
    // Validate pixel format
    if (pixelFormat < 0 || pixelFormat >= 12) {
        pixelFormat = TJPF_RGB; // Default to RGB if out of range
    }
    
    // Get pixel size for the format
    static const int tjPixelSize[TJ_NUMPF] = {3, 3, 4, 4, 4, 4, 1, 4, 4, 4, 4, 4};
    int bytesPerPixel = tjPixelSize[pixelFormat];
    
    // Validate subsampling value
    if (subsamp < 0 || subsamp > 5) {
        subsamp = TJSAMP_420; // Default to 4:2:0
    }
    
    // Initialize compression handle for modern APIs
    tjhandle compressHandle = tj3Init(TJINIT_COMPRESS);
    if (!compressHandle) return 0;
    
    // Initialize decompression handle for modern APIs
    tjhandle decompressHandle = tj3Init(TJINIT_DECOMPRESS);
    if (!decompressHandle) {
        tj3Destroy(compressHandle);
        return 0;
    }
    
    // Set precision parameter for compression (12 or 16 bits)
    int precision = fdp.ConsumeBool() ? 12 : 16;
    tj3Set(compressHandle, TJPARAM_PRECISION, precision);
    
    // Test 1: 12-bit compression/decompression
    if (precision == 12) {
        // Calculate required buffer size for 12-bit source image
        size_t srcBufferSize = width * height * bytesPerPixel * sizeof(short); // 12-bit uses short type
        if (srcBufferSize > 10 * 1024 * 1024) { // Limit to 10MB
            tj3Destroy(compressHandle);
            tj3Destroy(decompressHandle);
            return 0;
        }
        
        // Check if we have enough data for source image
        if (fdp.remaining_bytes() < srcBufferSize) {
            tj3Destroy(compressHandle);
            tj3Destroy(decompressHandle);
            return 0;
        }
        
        // Consume source image data as 12-bit shorts
        std::vector<short> srcBuffer(srcBufferSize / sizeof(short));
        for (size_t i = 0; i < srcBuffer.size(); i++) {
            srcBuffer[i] = fdp.ConsumeIntegral<short>();
        }
        
        // Allocate JPEG buffer
        size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
        if (jpegBufSize == 0) {
            tj3Destroy(compressHandle);
            tj3Destroy(decompressHandle);
            return 0;
        }
        
        unsigned char* jpegBuf = (unsigned char*)tj3Alloc(jpegBufSize);
        if (!jpegBuf) {
            tj3Destroy(compressHandle);
            tj3Destroy(decompressHandle);
            return 0;
        }
        
        size_t jpegSize = jpegBufSize;
        
        // Test 12-bit compression
        int compressResult = tj3Compress12(compressHandle,
                                          srcBuffer.data(),
                                          width,
                                          width * bytesPerPixel, // pitch
                                          height,
                                          pixelFormat,
                                          &jpegBuf,
                                          &jpegSize);
        
        if (compressResult == 0 && jpegBuf && jpegSize > 0) {
            // Test 12-bit decompression
            // Allocate destination buffer for 12-bit decompression
            size_t dstBufferSize = width * height * bytesPerPixel * sizeof(short);
            std::vector<short> dstBuffer(dstBufferSize / sizeof(short));
            
            int decompressResult = tj3Decompress12(decompressHandle,
                                                  jpegBuf,
                                                  jpegSize,
                                                  dstBuffer.data(),
                                                  width * bytesPerPixel, // pitch
                                                  pixelFormat);
            // Result ignored - testing for crashes
        }
        
        // Free JPEG buffer
        if (jpegBuf) {
            tj3Free(jpegBuf);
        }
    }
    
    // Test 2: 16-bit lossless compression/decompression
    else if (precision == 16) {
        // Set lossless mode for 16-bit compression
        tj3Set(compressHandle, TJPARAM_LOSSLESS, 1);
        
        // Calculate required buffer size for 16-bit source image
        size_t srcBufferSize = width * height * bytesPerPixel * sizeof(unsigned short);
        if (srcBufferSize > 10 * 1024 * 1024) { // Limit to 10MB
            tj3Destroy(compressHandle);
            tj3Destroy(decompressHandle);
            return 0;
        }
        
        // Check if we have enough data for source image
        if (fdp.remaining_bytes() < srcBufferSize) {
            tj3Destroy(compressHandle);
            tj3Destroy(decompressHandle);
            return 0;
        }
        
        // Consume source image data as 16-bit unsigned shorts
        std::vector<unsigned short> srcBuffer(srcBufferSize / sizeof(unsigned short));
        for (size_t i = 0; i < srcBuffer.size(); i++) {
            srcBuffer[i] = fdp.ConsumeIntegral<unsigned short>();
        }
        
        // Allocate JPEG buffer
        size_t jpegBufSize = tj3JPEGBufSize(width, height, TJSAMP_444); // Lossless always uses 444
        if (jpegBufSize == 0) {
            tj3Destroy(compressHandle);
            tj3Destroy(decompressHandle);
            return 0;
        }
        
        unsigned char* jpegBuf = (unsigned char*)tj3Alloc(jpegBufSize);
        if (!jpegBuf) {
            tj3Destroy(compressHandle);
            tj3Destroy(decompressHandle);
            return 0;
        }
        
        size_t jpegSize = jpegBufSize;
        
        // Test 16-bit lossless compression
        int compressResult = tj3Compress16(compressHandle,
                                          srcBuffer.data(),
                                          width,
                                          width * bytesPerPixel, // pitch
                                          height,
                                          pixelFormat,
                                          &jpegBuf,
                                          &jpegSize);
        
        if (compressResult == 0 && jpegBuf && jpegSize > 0) {
            // Set decompression handle for lossless
            tj3Set(decompressHandle, TJPARAM_LOSSLESS, 1);
            
            // Allocate destination buffer for 16-bit decompression
            size_t dstBufferSize = width * height * bytesPerPixel * sizeof(unsigned short);
            std::vector<unsigned short> dstBuffer(dstBufferSize / sizeof(unsigned short));
            
            int decompressResult = tj3Decompress16(decompressHandle,
                                                  jpegBuf,
                                                  jpegSize,
                                                  dstBuffer.data(),
                                                  width * bytesPerPixel, // pitch
                                                  pixelFormat);
            // Result ignored - testing for crashes
        }
        
        // Free JPEG buffer
        if (jpegBuf) {
            tj3Free(jpegBuf);
        }
    }
    
    // Cleanup
    tj3Destroy(compressHandle);
    tj3Destroy(decompressHandle);
    
    return 0;
}

} // namespace harness_003
