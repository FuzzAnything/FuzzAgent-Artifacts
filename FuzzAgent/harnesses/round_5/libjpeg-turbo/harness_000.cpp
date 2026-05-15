#include "turbojpeg.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_000 {
/*
 * Fuzzing harness for libjpeg-turbo targeting TurboJPEG compression/decompression APIs
 * This harness combines patterns from existing libjpeg-turbo fuzzers with input-driven testing
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) return 0; // Need minimum input for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int width = fdp.ConsumeIntegralInRange<int>(1, 1024);
    int height = fdp.ConsumeIntegralInRange<int>(1, 1024);
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
    
    // Consume remaining data for image buffer
    size_t maxImageSize = width * height * bytesPerPixel; // Correct size for the pixel format
    size_t imageSize = fdp.ConsumeIntegralInRange<size_t>(1, maxImageSize);
    std::vector<uint8_t> srcBuffer = fdp.ConsumeBytes<uint8_t>(imageSize);
    
    // Validate buffer size based on pixel format
    size_t requiredSize = width * height * bytesPerPixel;
    if (srcBuffer.size() < requiredSize) {
        // Not enough data for the requested image size with this pixel format
        return 0;
    }
    
    // Initialize compression handle
    tjhandle compressHandle = tjInitCompress();
    if (!compressHandle) return 0;
    
    // Initialize decompression handle
    tjhandle decompressHandle = tjInitDecompress();
    if (!decompressHandle) {
        tjDestroy(compressHandle);
        return 0;
    }
    
    // Test compression
    unsigned char* jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    
    int compressResult = tjCompress2(compressHandle,
                                     srcBuffer.data(),
                                     width,
                                     width * bytesPerPixel, // pitch based on pixel format
                                     height,
                                     pixelFormat,
                                     &jpegBuf,
                                     &jpegSize,
                                     subsamp,
                                     quality,
                                     flags);

    if (compressResult == 0 && jpegBuf && jpegSize > 0) {
        // Test decompression of the compressed JPEG
        int decompWidth, decompHeight, decompSubsamp, decompColorspace;
        
        int headerResult = tjDecompressHeader3(decompressHandle,
                                              jpegBuf,
                                              jpegSize,
                                              &decompWidth,
                                              &decompHeight,
                                              &decompSubsamp,
                                              &decompColorspace);
        
        if (headerResult == 0 && decompWidth > 0 && decompHeight > 0) {
            // Limit decompression size for fuzzing efficiency
            if ((uint64_t)decompWidth * decompHeight <= 1048576) { // 1 MP limit
                size_t decompBufferSize = decompWidth * decompHeight * bytesPerPixel;
                std::vector<uint8_t> decompBuffer(decompBufferSize);
                
                int decompressResult = tjDecompress2(decompressHandle,
                                                    jpegBuf,
                                                    jpegSize,
                                                    decompBuffer.data(),
                                                    decompWidth,
                                                    decompWidth * bytesPerPixel,
                                                    decompHeight,
                                                    pixelFormat,
                                                    flags);
                // Result is ignored - we're testing for crashes
            }
        }
        
        // Test YUV compression/decompression path
        size_t yuvSize = tjBufSizeYUV(width, height, subsamp);
        if (yuvSize > 0 && yuvSize < 10 * 1024 * 1024) { // Limit to 10MB
            std::vector<uint8_t> yuvBuffer(yuvSize);
            
            int encodeResult = tjEncodeYUV2(compressHandle,
                                           srcBuffer.data(),
                                           width,
                                           width * bytesPerPixel,
                                           height,
                                           pixelFormat,
                                           yuvBuffer.data(),
                                           subsamp,
                                           flags);
            
            if (encodeResult == 0) {
                // Test YUV to JPEG compression
                unsigned char* yuvJpegBuf = nullptr;
                unsigned long yuvJpegSize = 0;
                
                int compressFromYUVResult = tjCompressFromYUV(compressHandle,
                                                             yuvBuffer.data(),
                                                             width,
                                                             4, // align
                                                             height,
                                                             subsamp,
                                                             &yuvJpegBuf,
                                                             &yuvJpegSize,
                                                             quality,
                                                             flags);
                
                if (yuvJpegBuf) {
                    tjFree(yuvJpegBuf);
                }
            }
        }
        
        // Test buffer size calculations
        unsigned long bufSize = tjBufSize(width, height, subsamp);
        unsigned long yuvBufSize = tjBufSizeYUV2(width, 4, height, subsamp);
        
        // Test scaling factors
        int numScalingFactors;
        tjscalingfactor* scalingFactors = tjGetScalingFactors(&numScalingFactors);
        
        // Test transform if we have a valid JPEG
        if (jpegSize > 0) {
            tjhandle transformHandle = tjInitTransform();
            if (transformHandle) {
                unsigned char* dstBufs[1] = {nullptr};
                unsigned long dstSizes[1] = {0};
                tjtransform transforms[1];
                
                // Simple transform
                transforms[0].r.x = 0;
                transforms[0].r.y = 0;
                transforms[0].r.w = width / 2;
                transforms[0].r.h = height / 2;
                transforms[0].op = TJXOP_NONE;
                transforms[0].options = 0;
                transforms[0].data = nullptr;
                transforms[0].customFilter = nullptr;
                
                if (width > 1 && height > 1) {
                    tjTransform(transformHandle,
                               jpegBuf,
                               jpegSize,
                               1,
                               dstBufs,
                               dstSizes,
                               transforms,
                               flags);
                    
                    if (dstBufs[0]) {
                        tjFree(dstBufs[0]);
                    }
                }
                
                tjDestroy(transformHandle);
            }
        }
    }
    
    // Cleanup
    if (jpegBuf) {
        tjFree(jpegBuf);
    }
    
    tjDestroy(compressHandle);
    tjDestroy(decompressHandle);
    
    return 0;
}

} // namespace harness_000
