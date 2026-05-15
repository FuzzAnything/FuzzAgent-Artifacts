#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_000 {
/*
 * Fuzzing harness for libjpeg-turbo core compression and decompression APIs
 * This harness tests the basic JPEG processing pipeline using TurboJPEG API
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize handles for compression and decompression
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!compress_handle || !decompress_handle) {
        if (compress_handle) tj3Destroy(compress_handle);
        if (decompress_handle) tj3Destroy(decompress_handle);
        return 0;
    }
    
    // Consume parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(8, 256);
    int height = fdp.ConsumeIntegralInRange<int>(8, 256);
    
    // Choose pixel format from available options
    int pixel_format_idx = fdp.ConsumeIntegralInRange<int>(0, 11);
    enum TJPF pixel_formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XBGR, TJPF_XRGB,
        TJPF_GRAY, TJPF_RGBA, TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK
    };
    int pixelFormat = pixel_formats[pixel_format_idx % 12];
    
    // Choose subsampling
    int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJSAMP subsampling[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420, 
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    enum TJSAMP subsamp = subsampling[subsamp_idx % 6];
    
    // Choose quality
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Generate synthetic image data
    size_t image_size = (size_t)width * height * tjPixelSize[pixelFormat];
    if (image_size == 0) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    // Allocate source image buffer
    std::vector<unsigned char> src_buf(image_size);
    if (src_buf.empty()) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    // Fill with fuzzer data (use remaining bytes)
    auto remaining = fdp.ConsumeRemainingBytes<unsigned char>();
    size_t fill_size = std::min(image_size, remaining.size());
    if (fill_size > 0) {
        memcpy(src_buf.data(), remaining.data(), fill_size);
    }
    
    // Set compression parameters
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    tj3Set(compress_handle, TJPARAM_FASTDCT, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_OPTIMIZE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, fdp.ConsumeBool());
    
    // Calculate JPEG buffer size
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
    if (jpeg_buf_size == 0) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    // Allocate JPEG buffer
    unsigned char* jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
    if (!jpeg_buf) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    size_t compressed_size = jpeg_buf_size;
    
    // Perform compression
    int compress_result = tj3Compress8(compress_handle, 
                                       src_buf.data(), 
                                       width, 
                                       0,  // pitch (0 means width * pixelSize)
                                       height, 
                                       pixelFormat, 
                                       &jpeg_buf, 
                                       &compressed_size);
    
    // If compression succeeded, try decompression
    if (compress_result == 0 && compressed_size > 0) {
        // Set decompression parameters
        tj3Set(decompress_handle, TJPARAM_BOTTOMUP, fdp.ConsumeBool());
        tj3Set(decompress_handle, TJPARAM_FASTUPSAMPLE, fdp.ConsumeBool());
        tj3Set(decompress_handle, TJPARAM_FASTDCT, fdp.ConsumeBool());
        
        // Try to get JPEG header
        tj3DecompressHeader(decompress_handle, jpeg_buf, compressed_size);
        
        int jpeg_width = tj3Get(decompress_handle, TJPARAM_JPEGWIDTH);
        int jpeg_height = tj3Get(decompress_handle, TJPARAM_JPEGHEIGHT);
        
        // Only attempt decompression if we got valid dimensions
        if (jpeg_width > 0 && jpeg_height > 0 && 
            (uint64_t)jpeg_width * jpeg_height <= 1048576) {
            
            // Choose a different pixel format for decompression
            int decomp_pf_idx = fdp.ConsumeIntegralInRange<int>(0, 11);
            int decomp_pixelFormat = pixel_formats[decomp_pf_idx % 12];
            
            size_t decomp_buf_size = (size_t)jpeg_width * jpeg_height * 
                                     tjPixelSize[decomp_pixelFormat];
            
            if (decomp_buf_size > 0) {
                unsigned char* decomp_buf = (unsigned char*)tj3Alloc(decomp_buf_size);
                if (decomp_buf) {
                    // Perform decompression (ignore result for fuzzing)
                    tj3Decompress8(decompress_handle, 
                                  jpeg_buf, 
                                  compressed_size, 
                                  decomp_buf, 
                                  0,  // pitch
                                  decomp_pixelFormat);
                    
                    tj3Free(decomp_buf);
                }
            }
        }
    }
    // Cleanup
    tj3Free(jpeg_buf);
    tj3Destroy(compress_handle);
    tj3Destroy(decompress_handle);
    
    return 0;
}

} // namespace harness_000
