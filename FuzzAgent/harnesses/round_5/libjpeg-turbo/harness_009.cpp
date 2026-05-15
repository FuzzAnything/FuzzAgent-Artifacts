#include "turbojpeg.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_009 {
/*
 * Fuzzing harness for libjpeg-turbo targeting EXCLUSIVELY 12-bit precision JPEG operations
 * This harness specifically targets 12-bit high-bit-depth JPEG compression/decompression APIs:
 * - tj3Decompress12: Decompress JPEG to 12-bit packed-pixel image
 * - tj3Compress12: Compress 12-bit packed-pixel image to JPEG
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_003.cpp: Covers both 12-bit and 16-bit operations with random selection
 * - This harness: Focuses EXCLUSIVELY on 12-bit precision JPEG workflows
 * 
 * Coverage strategy:
 * 1. Comprehensive 12-bit compression/decompression cycles
 * 2. Varied pixel formats suitable for 12-bit operations
 * 3. Different subsampling modes for 12-bit compression
 * 4. Quality parameter variations
 * 5. Edge cases in pitch calculation for 12-bit buffers
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for parameters + some 12-bit image data
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first for 12-bit operations
    int width = fdp.ConsumeIntegralInRange<int>(1, 256);  // Conservative for 12-bit processing
    int height = fdp.ConsumeIntegralInRange<int>(1, 256);
    
    // Only pixel formats suitable for 12-bit operations
    // TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XBGR, TJPF_XRGB, TJPF_GRAY, TJPF_RGBA, TJPF_BGRA, TJPF_ABGR, TJPF_ARGB
    int pixelFormatChoice = fdp.ConsumeIntegralInRange<int>(0, 10);
    int pixelFormat;
    switch (pixelFormatChoice) {
        case 0: pixelFormat = TJPF_RGB; break;
        case 1: pixelFormat = TJPF_BGR; break;
        case 2: pixelFormat = TJPF_RGBX; break;
        case 3: pixelFormat = TJPF_BGRX; break;
        case 4: pixelFormat = TJPF_XBGR; break;
        case 5: pixelFormat = TJPF_XRGB; break;
        case 6: pixelFormat = TJPF_GRAY; break;
        case 7: pixelFormat = TJPF_RGBA; break;
        case 8: pixelFormat = TJPF_BGRA; break;
        case 9: pixelFormat = TJPF_ABGR; break;
        case 10: pixelFormat = TJPF_ARGB; break;
        default: pixelFormat = TJPF_RGB; break;
    }
    
    // Subsamp options for 12-bit compression
    int subsamp = fdp.ConsumeIntegralInRange<int>(0, 5); // TJSAMP enum values
    if (subsamp < 0 || subsamp > 5) {
        subsamp = TJSAMP_420; // Default to 4:2:0
    }
    
    // Quality for 12-bit compression
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Flags for compression/decompression
    int flags = fdp.ConsumeIntegral<int>();
    
    // Get pixel size for the format
    static const int tjPixelSize[TJ_NUMPF] = {3, 3, 4, 4, 4, 4, 1, 4, 4, 4, 4, 4};
    int bytesPerPixel = tjPixelSize[pixelFormat];
    
    // Initialize compression handle for 12-bit operations
    tjhandle compressHandle = tj3Init(TJINIT_COMPRESS);
    if (!compressHandle) return 0;
    
    // Initialize decompression handle for 12-bit operations
    tjhandle decompressHandle = tj3Init(TJINIT_DECOMPRESS);
    if (!decompressHandle) {
        tj3Destroy(compressHandle);
        return 0;
    }
    
    // EXCLUSIVE 12-bit mode: Force 12-bit precision
    tj3Set(compressHandle, TJPARAM_PRECISION, 12);
    
    // Also set for decompression to ensure consistent 12-bit pipeline
    tj3Set(decompressHandle, TJPARAM_PRECISION, 12);
    
    // Calculate required buffer size for 12-bit source image
    // 12-bit uses short type (2 bytes per sample)
    size_t srcBufferSize = width * height * bytesPerPixel * sizeof(short);
    
    // Memory limit for 12-bit operations (more conservative than 8-bit)
    if (srcBufferSize > 5 * 1024 * 1024) { // Limit to 5MB for 12-bit
        tj3Destroy(compressHandle);
        tj3Destroy(decompressHandle);
        return 0;
    }
    
    // Check if we have enough data for 12-bit source image
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
    
    // Allocate JPEG buffer for 12-bit compression
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
    
    // Test 1: 12-bit compression with consumed parameters
    int compressResult = tj3Compress12(compressHandle,
                                      srcBuffer.data(),
                                      width,
                                      width * bytesPerPixel, // pitch
                                      height,
                                      pixelFormat,
                                      &jpegBuf,
                                      &jpegSize);
    
    if (compressResult == 0 && jpegBuf && jpegSize > 0) {
        // Test 1A: Direct 12-bit decompression
        size_t dstBufferSize = width * height * bytesPerPixel * sizeof(short);
        std::vector<short> dstBuffer(dstBufferSize / sizeof(short));
        
        int decompressResult = tj3Decompress12(decompressHandle,
                                              jpegBuf,
                                              jpegSize,
                                              dstBuffer.data(),
                                              width * bytesPerPixel, // pitch
                                              pixelFormat);
        // Result ignored - testing for crashes
        
        // Test 1B: Decompress with different pitch (edge case)
        if (height > 1 && width > 1) {
            std::vector<short> dstBuffer2(dstBufferSize / sizeof(short));
            int alternatePitch = (width * bytesPerPixel) + fdp.ConsumeIntegralInRange<int>(0, 16);
            
            int decompressResult2 = tj3Decompress12(decompressHandle,
                                                   jpegBuf,
                                                   jpegSize,
                                                   dstBuffer2.data(),
                                                   alternatePitch,
                                                   pixelFormat);
        }
    }
    
    // Test 2: Recompress with different quality (if we have a valid JPEG)
    if (jpegBuf && jpegSize > 0 && fdp.remaining_bytes() > 100) {
        // Create new compression handle for recompression test
        tjhandle recompressHandle = tj3Init(TJINIT_COMPRESS);
        if (recompressHandle) {
            tj3Set(recompressHandle, TJPARAM_PRECISION, 12);
            
            // Try different quality for recompression
            int newQuality = fdp.ConsumeIntegralInRange<int>(1, 100);
            
            // Decompress first to get source data
            size_t tempBufferSize = width * height * bytesPerPixel * sizeof(short);
            std::vector<short> tempBuffer(tempBufferSize / sizeof(short));
            
            tjhandle tempDecompressHandle = tj3Init(TJINIT_DECOMPRESS);
            if (tempDecompressHandle) {
                tj3Set(tempDecompressHandle, TJPARAM_PRECISION, 12);
                
                int tempDecompressResult = tj3Decompress12(tempDecompressHandle,
                                                          jpegBuf,
                                                          jpegSize,
                                                          tempBuffer.data(),
                                                          width * bytesPerPixel,
                                                          pixelFormat);
                
                if (tempDecompressResult == 0) {
                    // Now recompress with new quality
                    unsigned char* recompressBuf = (unsigned char*)tj3Alloc(jpegBufSize);
                    size_t recompressSize = jpegBufSize;
                    
                    if (recompressBuf) {
                        tj3Set(recompressHandle, TJPARAM_QUALITY, newQuality);
                        
                        int recompressResult = tj3Compress12(recompressHandle,
                                                            tempBuffer.data(),
                                                            width,
                                                            width * bytesPerPixel,
                                                            height,
                                                            pixelFormat,
                                                            &recompressBuf,
                                                            &recompressSize);
                        
                        tj3Free(recompressBuf);
                    }
                }
                
                tj3Destroy(tempDecompressHandle);
            }
            
            tj3Destroy(recompressHandle);
        }
    }
    
    // Test 3: Edge case - empty or minimal JPEG buffer (should handle gracefully)
    if (fdp.remaining_bytes() > 10) {
        size_t smallJpegSize = fdp.ConsumeIntegralInRange<size_t>(0, 100);
        std::vector<unsigned char> smallJpegBuf(smallJpegSize);
        for (size_t i = 0; i < smallJpegSize && fdp.remaining_bytes() > 0; i++) {
            smallJpegBuf[i] = fdp.ConsumeIntegral<unsigned char>();
        }
        
        if (smallJpegSize > 0) {
            size_t edgeDstBufferSize = 100 * 100 * bytesPerPixel * sizeof(short);
            if (edgeDstBufferSize > 0 && edgeDstBufferSize < 100000) {
                std::vector<short> edgeDstBuffer(edgeDstBufferSize / sizeof(short));
                
                // This may fail, but should not crash
                tj3Decompress12(decompressHandle,
                               smallJpegBuf.data(),
                               smallJpegSize,
                               edgeDstBuffer.data(),
                               100 * bytesPerPixel,
                               pixelFormat);
            }
        }
    }
    
    // Test 4: Test with different pixel format on decompression
    if (jpegBuf && jpegSize > 0 && fdp.remaining_bytes() > 50) {
        int altPixelFormatChoice = fdp.ConsumeIntegralInRange<int>(0, 10);
        int altPixelFormat;
        switch (altPixelFormatChoice) {
            case 0: altPixelFormat = TJPF_RGB; break;
            case 1: altPixelFormat = TJPF_BGR; break;
            case 2: altPixelFormat = TJPF_RGBX; break;
            case 3: altPixelFormat = TJPF_BGRX; break;
            case 4: altPixelFormat = TJPF_XBGR; break;
            case 5: altPixelFormat = TJPF_XRGB; break;
            case 6: altPixelFormat = TJPF_GRAY; break;
            case 7: altPixelFormat = TJPF_RGBA; break;
            case 8: altPixelFormat = TJPF_BGRA; break;
            case 9: altPixelFormat = TJPF_ABGR; break;
            case 10: altPixelFormat = TJPF_ARGB; break;
            default: altPixelFormat = TJPF_RGB; break;
        }
        
        size_t altDstBufferSize = width * height * tjPixelSize[altPixelFormat] * sizeof(short);
        if (altDstBufferSize > 0 && altDstBufferSize < 5 * 1024 * 1024) {
            std::vector<short> altDstBuffer(altDstBufferSize / sizeof(short));
            
            // Try decompression with alternative pixel format
            tj3Decompress12(decompressHandle,
                           jpegBuf,
                           jpegSize,
                           altDstBuffer.data(),
                           width * tjPixelSize[altPixelFormat],
                           altPixelFormat);
        }
    }
    
    // Cleanup
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    
    tj3Destroy(compressHandle);
    tj3Destroy(decompressHandle);
    
    return 0;
}

} // namespace harness_009
