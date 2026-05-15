#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace harness_004 {
/*
 * Fuzzing harness for libjpeg-turbo YUV/image processing APIs
 * This harness targets uncovered YUV compression/decompression to/from planes and image save/load operations
 * Differentiates from harness_002.cpp by focusing on specific APIs:
 * - tjCompressFromYUV: Compress from planar YUV buffer
 * - tjDecompressToYUVPlanes: Decompress to separate YUV planes  
 * - tjCompressFromYUVPlanes: Compress from separate YUV planes
 * - tjSaveImage: Save image to file (requires temporary file)
 * - tjLoadImage: Load image from file (requires temporary file)
 * Implements: YUV plane buffer allocation -> compression/decompression to/from YUV planes -> image save/load -> cleanup
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for YUV plane operations and file I/O
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize handles for compression and decompression
    tjhandle compress_handle = tjInitCompress();
    tjhandle decompress_handle = tjInitDecompress();
    
    if (!compress_handle || !decompress_handle) {
        if (compress_handle) tjDestroy(compress_handle);
        if (decompress_handle) tjDestroy(decompress_handle);
        return 0;
    }
    
    // Consume image dimensions from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(16, 256);
    int height = fdp.ConsumeIntegralInRange<int>(16, 256);
    
    // Choose pixel format for RGB operations
    int pixel_format_idx = fdp.ConsumeIntegralInRange<int>(0, 11);
    enum TJPF pixel_formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XBGR, TJPF_XRGB,
        TJPF_GRAY, TJPF_RGBA, TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK
    };
    int pixelFormat = pixel_formats[pixel_format_idx % 12];
    int pixelSize = pixelFormat == TJPF_GRAY ? 1 : 
                   (pixelFormat == TJPF_CMYK ? 4 : 
                    (pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR ? 3 : 4));
    
    // Choose subsampling for YUV operations
    int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJSAMP subsampling[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420, 
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    int subsamp = subsampling[subsamp_idx % 6];
    
    // Choose quality for compression
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Choose row alignment (must be power of 2)
    int align_options[] = {1, 2, 4, 8, 16};
    int align = align_options[fdp.ConsumeIntegralInRange<int>(0, 4) % 5];
    
    // Choose flags
    int flags = 0;
    if (fdp.ConsumeBool()) flags |= TJFLAG_FASTDCT;
    if (fdp.ConsumeBool()) flags |= TJFLAG_ACCURATEDCT;
    if (fdp.ConsumeBool()) flags |= TJFLAG_BOTTOMUP;
    if (fdp.ConsumeBool()) flags |= TJFLAG_FASTUPSAMPLE;
    if (fdp.ConsumeBool()) flags |= TJFLAG_NOREALLOC;
    if (fdp.ConsumeBool()) flags |= TJFLAG_STOPONWARNING;
    if (fdp.ConsumeBool()) flags |= TJFLAG_PROGRESSIVE;
    
    // Calculate buffer sizes
    unsigned long src_image_size = (unsigned long)width * height * pixelSize;
    unsigned long yuv_buf_size = tjBufSizeYUV2(width, align, height, subsamp);
    unsigned long jpeg_buf_size = tjBufSize(width, height, subsamp);
    
    if (src_image_size == 0 || yuv_buf_size == 0 || jpeg_buf_size == 0) {
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        return 0;
    }
    
    // ========== TEST 1: Create source RGB image ==========
    unsigned char* src_buf = tjAlloc(src_image_size);
    if (!src_buf) {
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        return 0;
    }
    
    // Fill source buffer with fuzzer data
    auto src_data = fdp.ConsumeBytes<unsigned char>(std::min(src_image_size, fdp.remaining_bytes()));
    if (src_data.size() > 0) {
        memcpy(src_buf, src_data.data(), src_data.size());
    }
    
    // ========== TEST 2: Compress to JPEG first (to use for decompression tests) ==========
    unsigned char* jpeg_buf = tjAlloc(jpeg_buf_size);
    if (!jpeg_buf) {
        tjFree(src_buf);
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        return 0;
    }
    
    unsigned long jpeg_size = jpeg_buf_size;
    int ret = tjCompress2(compress_handle, src_buf, width, 0, height, pixelFormat,
                         &jpeg_buf, &jpeg_size, subsamp, quality, flags);
    if (ret != 0) {
        // Compression failed, but continue with other tests if possible
        jpeg_size = 0;
    }
    
    // ========== TEST 3: tjCompressFromYUV ==========
    // First encode to YUV
    unsigned char* yuv_buf = tjAlloc(yuv_buf_size);
    if (yuv_buf) {
        ret = tjEncodeYUV3(compress_handle, src_buf, width, 0, height, pixelFormat,
                          yuv_buf, align, subsamp, flags);
        
        if (ret == 0) {
            // Now compress from YUV
            unsigned char* jpeg_from_yuv = NULL;
            unsigned long jpeg_from_yuv_size = 0;
            
            tjCompressFromYUV(compress_handle, yuv_buf, width, align, height, subsamp,
                             &jpeg_from_yuv, &jpeg_from_yuv_size, quality, flags);
            
            if (jpeg_from_yuv) {
                tjFree(jpeg_from_yuv);
            }
        }
        tjFree(yuv_buf);
    }
    
    // ========== TEST 4: tjCompressFromYUVPlanes ==========
    if (subsamp != TJSAMP_GRAY) {
        // Calculate plane dimensions
        int pw0 = tjPlaneWidth(0, width, subsamp);
        int ph0 = tjPlaneHeight(0, height, subsamp);
        int pw1 = tjPlaneWidth(1, width, subsamp);
        int ph1 = tjPlaneHeight(1, height, subsamp);
        int pw2 = tjPlaneWidth(2, width, subsamp);
        int ph2 = tjPlaneHeight(2, height, subsamp);
        
        // Allocate planes with proper strides
        int strides[3] = {0};
        strides[0] = pw0;
        strides[1] = pw1;
        strides[2] = pw2;
        
        size_t plane0_size = (size_t)strides[0] * ph0;
        size_t plane1_size = (size_t)strides[1] * ph1;
        size_t plane2_size = (size_t)strides[2] * ph2;
        
        unsigned char* plane0 = NULL;
        unsigned char* plane1 = NULL;
        unsigned char* plane2 = NULL;
        
        if (plane0_size > 0) plane0 = tjAlloc(plane0_size);
        if (plane1_size > 0) plane1 = tjAlloc(plane1_size);
        if (plane2_size > 0) plane2 = tjAlloc(plane2_size);
        
        if (plane0 && plane1 && plane2) {
            // Fill planes with some data
            auto plane_data = fdp.ConsumeBytes<unsigned char>(std::min(plane0_size + plane1_size + plane2_size, fdp.remaining_bytes()));
            if (plane_data.size() > 0) {
                size_t copy_size = std::min(plane0_size, plane_data.size());
                if (copy_size > 0) memcpy(plane0, plane_data.data(), copy_size);
                
                if (plane_data.size() > plane0_size) {
                    copy_size = std::min(plane1_size, plane_data.size() - plane0_size);
                    if (copy_size > 0) memcpy(plane1, plane_data.data() + plane0_size, copy_size);
                }
                
                if (plane_data.size() > plane0_size + plane1_size) {
                    copy_size = std::min(plane2_size, plane_data.size() - plane0_size - plane1_size);
                    if (copy_size > 0) memcpy(plane2, plane_data.data() + plane0_size + plane1_size, copy_size);
                }
            }
            
            const unsigned char* src_planes[3] = {plane0, plane1, plane2};
            
            unsigned char* jpeg_from_planes = NULL;
            unsigned long jpeg_from_planes_size = 0;
            
            tjCompressFromYUVPlanes(compress_handle, src_planes, width, strides, height, subsamp,
                                   &jpeg_from_planes, &jpeg_from_planes_size, quality, flags);
            
            if (jpeg_from_planes) {
                tjFree(jpeg_from_planes);
            }
        }
        
        if (plane0) tjFree(plane0);
        if (plane1) tjFree(plane1);
        if (plane2) tjFree(plane2);
    }
    
    // ========== TEST 5: tjDecompressToYUVPlanes ==========
    if (jpeg_size > 0) {
        // Allocate destination planes
        int pw0 = tjPlaneWidth(0, width, subsamp);
        int ph0 = tjPlaneHeight(0, height, subsamp);
        int pw1 = tjPlaneWidth(1, width, subsamp);
        int ph1 = tjPlaneHeight(1, height, subsamp);
        int pw2 = tjPlaneWidth(2, width, subsamp);
        int ph2 = tjPlaneHeight(2, height, subsamp);
        
        int dst_strides[3] = {0};
        dst_strides[0] = pw0;
        dst_strides[1] = pw1;
        dst_strides[2] = pw2;
        
        size_t dst_plane0_size = (size_t)dst_strides[0] * ph0;
        size_t dst_plane1_size = (size_t)dst_strides[1] * ph1;
        size_t dst_plane2_size = (size_t)dst_strides[2] * ph2;
        
        unsigned char* dst_plane0 = NULL;
        unsigned char* dst_plane1 = NULL;
        unsigned char* dst_plane2 = NULL;
        
        if (dst_plane0_size > 0) dst_plane0 = tjAlloc(dst_plane0_size);
        if (dst_plane1_size > 0) dst_plane1 = tjAlloc(dst_plane1_size);
        if (dst_plane2_size > 0) dst_plane2 = tjAlloc(dst_plane2_size);
        
        if (dst_plane0 && dst_plane1 && dst_plane2) {
            unsigned char* dst_planes[3] = {dst_plane0, dst_plane1, dst_plane2};
            
            tjDecompressToYUVPlanes(decompress_handle, jpeg_buf, jpeg_size,
                                   dst_planes, width, dst_strides, height, flags);
        }
        
        if (dst_plane0) tjFree(dst_plane0);
        if (dst_plane1) tjFree(dst_plane1);
        if (dst_plane2) tjFree(dst_plane2);
    }
    
    // ========== TEST 6: tjSaveImage and tjLoadImage ==========
    // Create a temporary filename for image I/O tests
    char temp_filename[] = "/tmp/libjpeg_fuzz_XXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd >= 0) {
        close(temp_fd);
        
        // Test tjSaveImage
        int pitch = width * pixelSize;
        tjSaveImage(temp_filename, src_buf, width, pitch, height, pixelFormat, flags);
        
        // Test tjLoadImage
        int loaded_width = 0, loaded_height = 0, loaded_pixelFormat = 0;
        unsigned char* loaded_buf = tjLoadImage(temp_filename, &loaded_width, align, 
                                               &loaded_height, &loaded_pixelFormat, flags);
        
        if (loaded_buf) {
            tjFree(loaded_buf);
        }
        
        // Clean up temporary file
        unlink(temp_filename);
    }
    
    // ========== CLEANUP ==========
    if (src_buf) tjFree(src_buf);
    if (jpeg_buf) tjFree(jpeg_buf);
    tjDestroy(compress_handle);
    tjDestroy(decompress_handle);
    
    return 0;
}

} // namespace harness_004
