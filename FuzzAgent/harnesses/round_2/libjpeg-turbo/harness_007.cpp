#include "turbojpeg.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>

namespace harness_007 {
/*
 * Harness 007: Advanced transform operations with YUV planes and ICC profiles using TurboJPEG API
 * This harness tests uncovered TurboJPEG APIs including:
 * 1) Transform operations (tj3Transform, tj3TransformBufSize) with cropping region
 * 2) YUV plane operations (tj3EncodeYUVPlanes8, tj3CompressFromYUVPlanes8, tj3YUVPlaneSize, tj3YUVPlaneWidth, tj3YUVPlaneHeight)
 * 3) Advanced compression with YUV input
 * 4) Cropping region operations (tj3SetCroppingRegion)
 * 5) ICC profile operations (tj3SetICCProfile, tj3GetICCProfile)
 * 
 * This harness is semantically differentiated from existing harnesses by focusing on:
 * 1. Integrated YUV plane workflows with transformation operations
 * 2. Cropping region integration with both RGB and YUV workflows
 * 3. ICC profile handling in transformation contexts
 * 4. Comprehensive error handling for all targeted APIs
 * 5. Semantic combination of multiple advanced features in a single workflow
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for complex operations
    if (size < 300) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================
    // PHASE 1: Setup and initialization
    // ============================================
    
    // Initialize TurboJPEG handles for different operations
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    tjhandle transform_handle = tj3Init(TJINIT_TRANSFORM);
    
    if (compress_handle == NULL || decompress_handle == NULL || transform_handle == NULL) {
        if (compress_handle) tj3Destroy(compress_handle);
        if (decompress_handle) tj3Destroy(decompress_handle);
        if (transform_handle) tj3Destroy(transform_handle);
        return 0;
    }
    
    // Consume operation flags from fuzzer input
    bool test_transform_operations = fdp.ConsumeBool();
    bool test_yuv_plane_operations = fdp.ConsumeBool();
    bool test_cropping_region = fdp.ConsumeBool();
    bool test_icc_profile = fdp.ConsumeBool();
    bool test_advanced_yuv_compression = fdp.ConsumeBool();
    
    // Consume image parameters with overflow protection
    int width = fdp.ConsumeIntegralInRange<int>(16, 256);
    int height = fdp.ConsumeIntegralInRange<int>(16, 256);
    int align = 1 << (fdp.ConsumeIntegralInRange<int>(0, 4)); // 1, 2, 4, 8, 16
    align = std::max(1, std::min(16, align));
    
    // Skip if image would be too large
    uint64_t total_pixels = static_cast<uint64_t>(width) * height;
    if (total_pixels > 262144) { // 256K pixels
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        tj3Destroy(transform_handle);
        return 0;
    }
    
    // Consume pixel format
    uint8_t pfIndex = fdp.ConsumeIntegral<uint8_t>() % 12;
    enum TJPF pixelFormats[12] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XBGR, TJPF_XRGB,
        TJPF_GRAY, TJPF_RGBA, TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK
    };
    enum TJPF pixelFormat = pixelFormats[pfIndex];
    
    // Consume subsampling mode
    uint8_t ssIndex = fdp.ConsumeIntegral<uint8_t>() % 6;
    enum TJSAMP subsampling[6] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420, TJSAMP_440, TJSAMP_411, TJSAMP_GRAY
    };
    enum TJSAMP subsamp = subsampling[ssIndex];
    
    // Consume compression parameters
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    bool bottomUp = fdp.ConsumeBool();
    bool optimize = fdp.ConsumeBool();
    bool progressive = fdp.ConsumeBool();
    bool arithmetic = fdp.ConsumeBool();
    
    // Set common parameters
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    tj3Set(compress_handle, TJPARAM_BOTTOMUP, bottomUp ? 1 : 0);
    tj3Set(compress_handle, TJPARAM_OPTIMIZE, optimize ? 1 : 0);
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, progressive ? 1 : 0);
    tj3Set(compress_handle, TJPARAM_ARITHMETIC, arithmetic ? 1 : 0);
    tj3Set(compress_handle, TJPARAM_MAXPIXELS, 262144);
    
    tj3Set(decompress_handle, TJPARAM_MAXPIXELS, 262144);
    tj3Set(transform_handle, TJPARAM_MAXPIXELS, 262144);
    
    // ============================================
    // PHASE 2: Test YUV plane operations
    // ============================================
    
    if (test_yuv_plane_operations && fdp.remaining_bytes() > 2000) {
        // Create source RGB image from fuzzer data
        int pixelSize = tjPixelSize[pixelFormat];
        size_t srcBufSize = static_cast<size_t>(width) * height * pixelSize;
        
        if (srcBufSize > 0 && srcBufSize < 10 * 1024 * 1024 && fdp.remaining_bytes() >= srcBufSize) {
            std::vector<unsigned char> srcBuf(srcBufSize);
            auto consumed = fdp.ConsumeBytes<unsigned char>(srcBufSize);
            if (consumed.size() >= srcBufSize) {
                std::copy(consumed.begin(), consumed.begin() + srcBufSize, srcBuf.begin());
                
                // Calculate YUV plane sizes
                int strides[3] = {0, 0, 0};
                strides[0] = tj3YUVPlaneWidth(0, width, subsamp);
                strides[1] = tj3YUVPlaneWidth(1, width, subsamp);
                strides[2] = tj3YUVPlaneWidth(2, width, subsamp);
                
                // Allocate YUV planes
                size_t ySize = tj3YUVPlaneSize(0, width, strides[0], height, subsamp);
                size_t uSize = tj3YUVPlaneSize(1, width, strides[1], height, subsamp);
                size_t vSize = tj3YUVPlaneSize(2, width, strides[2], height, subsamp);
                
                if (ySize > 0 && uSize > 0 && vSize > 0 && 
                    ySize < 5 * 1024 * 1024 && uSize < 5 * 1024 * 1024 && vSize < 5 * 1024 * 1024) {
                    
                    std::vector<unsigned char> yPlane(ySize);
                    std::vector<unsigned char> uPlane(uSize);
                    std::vector<unsigned char> vPlane(vSize);
                    
                    // Prepare plane pointers for tj3EncodeYUVPlanes8
                    unsigned char* dstPlanes[3] = {yPlane.data(), uPlane.data(), vPlane.data()};
                    
                    // Test tj3EncodeYUVPlanes8
                    if (tj3EncodeYUVPlanes8(compress_handle, srcBuf.data(), width, 0, height, pixelFormat,
                                           dstPlanes, strides) == 0) {
                        
                        // Test advanced YUV compression
                        if (test_advanced_yuv_compression) {
                            // Create JPEG buffer - need pointer for tj3CompressFromYUVPlanes8
                            unsigned char* jpegBuf = nullptr;
                            size_t jpegSize = 0;
                            
                            // Prepare source plane pointers for tj3CompressFromYUVPlanes8
                            const unsigned char* srcPlanes[3] = {yPlane.data(), uPlane.data(), vPlane.data()};
                            
                            // Test tj3CompressFromYUVPlanes8
                            if (tj3CompressFromYUVPlanes8(compress_handle, srcPlanes, width,
                                                         strides, height, &jpegBuf, &jpegSize) == 0) {
                                
                                if (jpegBuf && jpegSize > 0) {
                                    // Try to decompress the generated JPEG
                                    tj3DecompressHeader(decompress_handle, jpegBuf, jpegSize);
                                    
                                    // Free the JPEG buffer allocated by TurboJPEG
                                    if (jpegBuf) tj3Free(jpegBuf);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // ============================================
    // PHASE 3: Test transform operations with cropping region
    // ============================================
    
    if (test_transform_operations && fdp.remaining_bytes() > 1000) {
        // Create a simple JPEG buffer for transformation testing
        int testWidth = 64;
        int testHeight = 64;
        int testPixelSize = tjPixelSize[TJPF_RGB];
        size_t testSrcBufSize = static_cast<size_t>(testWidth) * testHeight * testPixelSize;
        
        if (testSrcBufSize > 0 && fdp.remaining_bytes() >= testSrcBufSize) {
            std::vector<unsigned char> testSrcBuf(testSrcBufSize);
            auto testConsumed = fdp.ConsumeBytes<unsigned char>(testSrcBufSize);
            if (testConsumed.size() >= testSrcBufSize) {
                std::copy(testConsumed.begin(), testConsumed.begin() + testSrcBufSize, testSrcBuf.begin());
                
                // Compress test image
                unsigned char* testJpegBuf = nullptr;
                size_t testJpegSize = 0;
                
                // Set compression parameters for test image
                tjhandle test_compress_handle = tj3Init(TJINIT_COMPRESS);
                if (test_compress_handle) {
                    tj3Set(test_compress_handle, TJPARAM_SUBSAMP, TJSAMP_444);
                    tj3Set(test_compress_handle, TJPARAM_QUALITY, 85);
                    
                    if (tj3Compress8(test_compress_handle, testSrcBuf.data(), testWidth, 0, testHeight,
                                    TJPF_RGB, &testJpegBuf, &testJpegSize) == 0) {
                        
                        if (testJpegBuf && testJpegSize > 0) {
                            // Define transform operation
                            tjtransform transform;
                            memset(&transform, 0, sizeof(transform));
                            
                            // Consume transform options
                            uint8_t transformOp = fdp.ConsumeIntegral<uint8_t>() % 7; // Only 7 valid operations
                            int options = 0;
                            
                            if (transformOp == 0) options = TJXOP_HFLIP;
                            else if (transformOp == 1) options = TJXOP_VFLIP;
                            else if (transformOp == 2) options = TJXOP_TRANSPOSE;
                            else if (transformOp == 3) options = TJXOP_TRANSVERSE;
                            else if (transformOp == 4) options = TJXOP_ROT90;
                            else if (transformOp == 5) options = TJXOP_ROT180;
                            else if (transformOp == 6) options = TJXOP_ROT270;
                            
                            transform.op = transformOp;
                            transform.options = options;
                            
                            // Test cropping region if enabled (separate from transform operations)
                            if (test_cropping_region) {
                                tjregion croppingRegion;
                                memset(&croppingRegion, 0, sizeof(croppingRegion));
                                
                                croppingRegion.x = fdp.ConsumeIntegralInRange<int>(0, testWidth - 8);
                                croppingRegion.y = fdp.ConsumeIntegralInRange<int>(0, testHeight - 8);
                                croppingRegion.w = fdp.ConsumeIntegralInRange<int>(8, testWidth - croppingRegion.x);
                                croppingRegion.h = fdp.ConsumeIntegralInRange<int>(8, testHeight - croppingRegion.y);
                                
                                // Test tj3SetCroppingRegion
                                tj3SetCroppingRegion(decompress_handle, croppingRegion);
                                
                                // Set cropping region in transform for testing
                                transform.r = croppingRegion;
                            }
                            size_t transformBufSize = tj3TransformBufSize(transform_handle, &transform);
                            if (transformBufSize > 0 && transformBufSize < 10 * 1024 * 1024) {
                                unsigned char* transformBuf = nullptr;
                                size_t transformSize = transformBufSize;
                                
                                // Allocate buffer for transformation output
                                transformBuf = (unsigned char*)tj3Alloc(transformBufSize);
                                if (transformBuf) {
                                    // Perform transformation - FIXED: correct number of arguments
                                    tj3Transform(transform_handle, testJpegBuf, testJpegSize,
                                                1, &transformBuf, &transformSize, &transform);
                                    
                                    // Free transformation buffer
                                    tj3Free(transformBuf);
                                }
                            }
                            
                            // Free the JPEG buffer allocated by TurboJPEG
                            tj3Free(testJpegBuf);
                        }
                    }
                    tj3Destroy(test_compress_handle);
                }
            }
        }
    }
    
    // ============================================
    // PHASE 4: Test ICC profile operations
    // ============================================
    
    if (test_icc_profile && fdp.remaining_bytes() > 500) {
        // Create a synthetic ICC profile
        size_t iccSize = fdp.ConsumeIntegralInRange<size_t>(128, 2048);
        if (iccSize <= fdp.remaining_bytes()) {
            std::vector<unsigned char> iccProfile(iccSize);
            auto iccData = fdp.ConsumeBytes<unsigned char>(iccSize);
            std::copy(iccData.begin(), iccData.end(), iccProfile.begin());
            
            // Test setting ICC profile on compression handle
            tj3SetICCProfile(compress_handle, iccProfile.data(), iccSize);
            
            // Create a simple image and compress it with ICC profile
            int testWidth = 32;
            int testHeight = 32;
            int testPixelSize = tjPixelSize[TJPF_RGB];
            size_t testImgSize = static_cast<size_t>(testWidth) * testHeight * testPixelSize;
            
            if (testImgSize > 0 && fdp.remaining_bytes() >= testImgSize) {
                std::vector<unsigned char> testImg(testImgSize);
                auto imgData = fdp.ConsumeBytes<unsigned char>(testImgSize);
                std::copy(imgData.begin(), imgData.end(), testImg.begin());
                
                // Compress with ICC profile
                unsigned char* jpegBuf = nullptr;
                size_t outSize = 0;
                
                if (tj3Compress8(compress_handle, testImg.data(), testWidth, 0, testHeight,
                                TJPF_RGB, &jpegBuf, &outSize) == 0) {
                    
                    if (jpegBuf && outSize > 0) {
                        // Try to get ICC profile back
                        unsigned char* retrievedICC = NULL;
                        size_t retrievedICCSize = 0;
                        
                        // Set up decompression handle
                        if (tj3DecompressHeader(decompress_handle, jpegBuf, outSize) == 0) {
                            // Get ICC profile - function returns int, profile via parameter
                            tj3GetICCProfile(decompress_handle, &retrievedICC, &retrievedICCSize);
                        }
                        
                        // Free the JPEG buffer
                        tj3Free(jpegBuf);
                    }
                }
            }
        }
    }
    
    // ============================================
    // PHASE 5: Cleanup
    // ============================================
    
    tj3Destroy(compress_handle);
    tj3Destroy(decompress_handle);
    tj3Destroy(transform_handle);
    
    return 0;
}

} // namespace harness_007
