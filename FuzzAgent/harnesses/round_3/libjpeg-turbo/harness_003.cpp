#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

namespace harness_003 {
/*
 * Fuzzing harness for libjpeg-turbo 12-bit and 16-bit precision APIs
 * This harness targets uncovered high-bit-depth JPEG operations in turbojpeg-mp.c module
 * Differentiates from existing harnesses by focusing on 12-bit and 16-bit precision operations
 * Implements: tj3Compress12, tj3Decompress12, tj3Compress16, tj3Decompress16
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for high-bit-depth operations
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize handles for compression and decompression
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!compress_handle || !decompress_handle) {
        if (compress_handle) tj3Destroy(compress_handle);
        if (decompress_handle) tj3Destroy(decompress_handle);
        return 0;
    }
    
    // Consume image dimensions from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(8, 128);
    int height = fdp.ConsumeIntegralInRange<int>(8, 128);
    
    // Choose pixel format (limited set suitable for high-bit-depth)
    int pixel_format_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJPF pixel_formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_GRAY, TJPF_CMYK
    };
    int pixelFormat = pixel_formats[pixel_format_idx % 6];
    int pixelSize = tjPixelSize[pixelFormat];
    
    // Choose subsampling (some may not be valid for all bit depths)
    int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJSAMP subsampling[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420, 
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    enum TJSAMP subsamp = subsampling[subsamp_idx % 6];
    
    // Choose quality for lossy compression
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Choose precision (12-bit or 16-bit)
    int precision_choice = fdp.ConsumeIntegralInRange<int>(0, 1);
    int precision = (precision_choice == 0) ? 12 : 16;
    
    // Set precision parameter
    tj3Set(compress_handle, TJPARAM_PRECISION, precision);
    tj3Set(decompress_handle, TJPARAM_PRECISION, precision);
    
    // Set other compression parameters from fuzzer input
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    tj3Set(compress_handle, TJPARAM_FASTDCT, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_OPTIMIZE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_ARITHMETIC, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_LOSSLESS, fdp.ConsumeBool());
    
    // Calculate buffer sizes
    size_t src_image_size = (size_t)width * height * pixelSize;
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
    
    if (src_image_size == 0 || jpeg_buf_size == 0) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    // Allocate and fill source buffers based on precision
    if (precision == 12) {
        // 12-bit precision uses short buffers
        size_t src_sample_count = (size_t)width * height * pixelSize;
        std::vector<short> src_buf_12(src_sample_count);
        if (src_buf_12.empty()) {
            tj3Destroy(compress_handle);
            tj3Destroy(decompress_handle);
            return 0;
        }
        
        // Fill with fuzzer data (convert bytes to shorts)
        auto byte_data = fdp.ConsumeBytes<uint8_t>(std::min(src_sample_count * sizeof(short), fdp.remaining_bytes()));
        if (byte_data.size() > 0) {
            size_t copy_size = std::min(byte_data.size(), src_buf_12.size() * sizeof(short));
            memcpy(src_buf_12.data(), byte_data.data(), copy_size);
        }
        
        // Allocate JPEG buffer
        unsigned char* jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
        if (!jpeg_buf) {
            tj3Destroy(compress_handle);
            tj3Destroy(decompress_handle);
            return 0;
        }
        
        size_t compressed_size = jpeg_buf_size;
        
        // Test tj3Compress12
        int compress_result = tj3Compress12(compress_handle, 
                                          src_buf_12.data(), 
                                          width, 
                                          width * pixelSize, 
                                          height, 
                                          pixelFormat, 
                                          &jpeg_buf, 
                                          &compressed_size);
        
        if (compress_result == 0 && compressed_size > 0) {
            // Allocate decompression buffer for 12-bit
            std::vector<short> dst_buf_12(src_sample_count);
            
            // Test tj3Decompress12
            tj3Decompress12(decompress_handle, 
                           jpeg_buf, 
                           compressed_size, 
                           dst_buf_12.data(), 
                           width * pixelSize, 
                           pixelFormat);
        }
        
        tj3Free(jpeg_buf);
        
    } else { // 16-bit precision
        // 16-bit precision uses unsigned short buffers
        size_t src_sample_count = (size_t)width * height * pixelSize;
        std::vector<unsigned short> src_buf_16(src_sample_count);
        if (src_buf_16.empty()) {
            tj3Destroy(compress_handle);
            tj3Destroy(decompress_handle);
            return 0;
        }
        
        // Fill with fuzzer data (convert bytes to unsigned shorts)
        auto byte_data = fdp.ConsumeBytes<uint8_t>(std::min(src_sample_count * sizeof(unsigned short), fdp.remaining_bytes()));
        if (byte_data.size() > 0) {
            size_t copy_size = std::min(byte_data.size(), src_buf_16.size() * sizeof(unsigned short));
            memcpy(src_buf_16.data(), byte_data.data(), copy_size);
        }
        
        // Allocate JPEG buffer
        unsigned char* jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
        if (!jpeg_buf) {
            tj3Destroy(compress_handle);
            tj3Destroy(decompress_handle);
            return 0;
        }
        
        size_t compressed_size = jpeg_buf_size;
        
        // Test tj3Compress16
        int compress_result = tj3Compress16(compress_handle, 
                                          src_buf_16.data(), 
                                          width, 
                                          width * pixelSize, 
                                          height, 
                                          pixelFormat, 
                                          &jpeg_buf, 
                                          &compressed_size);
        
        if (compress_result == 0 && compressed_size > 0) {
            // Allocate decompression buffer for 16-bit
            std::vector<unsigned short> dst_buf_16(src_sample_count);
            
            // Test tj3Decompress16
            tj3Decompress16(decompress_handle, 
                           jpeg_buf, 
                           compressed_size, 
                           dst_buf_16.data(), 
                           width * pixelSize, 
                           pixelFormat);
        }
        
        tj3Free(jpeg_buf);
    }
    
    // Clean up
    tj3Destroy(compress_handle);
    tj3Destroy(decompress_handle);
    
    return 0;
}

} // namespace harness_003
