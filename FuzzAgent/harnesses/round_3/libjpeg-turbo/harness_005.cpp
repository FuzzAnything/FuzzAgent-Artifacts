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

namespace harness_005 {
/*
 * Fuzzing harness for libjpeg-turbo 12/16-bit image I/O operations and ICC profile handling
 * This harness targets uncovered APIs: tj3SaveImage12, tj3SaveImage16, tj3LoadImage12, tj3LoadImage16, tj3GetICCProfile, tjEncodeYUV
 * Differentiates from harness_003.cpp by focusing on image save/load operations and ICC profile extraction
 * Implements: Load high-bit-depth images -> extract ICC profiles -> save high-bit-depth images -> YUV encoding
 */

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for image I/O operations and temporary files
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize handles for different operations
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    tjhandle legacy_handle = tjInitCompress(); // For tjEncodeYUV
    
    if (!compress_handle || !decompress_handle || !legacy_handle) {
        if (compress_handle) tj3Destroy(compress_handle);
        if (decompress_handle) tj3Destroy(decompress_handle);
        if (legacy_handle) tjDestroy(legacy_handle);
        return 0;
    }
    
    // Consume image dimensions from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(8, 128);
    int height = fdp.ConsumeIntegralInRange<int>(8, 128);
    // Choose pixel format for high-bit-depth operations
    int pixel_format_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJPF pixel_formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_GRAY, TJPF_CMYK
    };
    int pixelFormat = pixel_formats[pixel_format_idx % 6];
    int pixelSize = tjPixelSize[pixelFormat];
    
    // Choose subsampling for YUV operations
    int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJSAMP subsampling[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420, 
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    enum TJSAMP subsamp = subsampling[subsamp_idx % 6];
    
    // Choose quality for compression
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Choose row alignment (must be power of 2)
    int align_options[] = {1, 2, 4, 8, 16};
    int align = align_options[fdp.ConsumeIntegralInRange<int>(0, 4) % 5];
    
    // Choose precision (12-bit or 16-bit)
    int precision_choice = fdp.ConsumeIntegralInRange<int>(0, 1);
    int precision = (precision_choice == 0) ? 12 : 16;
    
    // Set precision parameter
    tj3Set(compress_handle, TJPARAM_PRECISION, precision);
    tj3Set(decompress_handle, TJPARAM_PRECISION, precision);
    
    // Enable ICC profile handling for save/load operations
    tj3Set(compress_handle, TJPARAM_SAVEMARKERS, fdp.ConsumeIntegralInRange<int>(0, 4));
    tj3Set(decompress_handle, TJPARAM_SAVEMARKERS, fdp.ConsumeIntegralInRange<int>(0, 4));
    
    // Calculate buffer sizes
    size_t src_sample_count = (size_t)width * height * pixelSize;
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
    size_t yuv_buf_size = tjBufSizeYUV2(width, align, height, subsamp);
    
    if (src_sample_count == 0 || jpeg_buf_size == 0 || yuv_buf_size == 0) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        tjDestroy(legacy_handle);
        return 0;
    }
    
    // Create temporary file for image save/load operations
    char temp_filename[] = "/tmp/libjpeg_fuzz_XXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd < 0) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        tjDestroy(legacy_handle);
        return 0;
    }
    close(temp_fd);
    
    // ========== PHASE 1: Create high-bit-depth image data ==========
    
    if (precision == 12) {
        // 12-bit precision uses short buffers
        std::vector<short> src_buf_12(src_sample_count);
        if (src_buf_12.empty()) {
            unlink(temp_filename);
            tj3Destroy(compress_handle);
            tj3Destroy(decompress_handle);
            tjDestroy(legacy_handle);
            return 0;
        }
        
        // Fill with valid fuzzer data for 12-bit precision (0-4095)
        size_t bytes_needed = src_sample_count * sizeof(short);
        if (fdp.remaining_bytes() < bytes_needed) {
            unlink(temp_filename);
            tj3Destroy(compress_handle);
            tj3Destroy(decompress_handle);
            tjDestroy(legacy_handle);
            return 0;
        }
        
        for (size_t i = 0; i < src_sample_count; i++) {
            src_buf_12[i] = fdp.ConsumeIntegralInRange<short>(0, 4095);
        }
        
        // ========== TEST 1: Save 12-bit image ==========
        int save_result = tj3SaveImage12(compress_handle, temp_filename,
                                        src_buf_12.data(), width,
                                        width * pixelSize, height,
                                        pixelFormat);
        
        // ========== TEST 2: Load 12-bit image ==========
        if (save_result == 0) {
            int loaded_width, loaded_height, loaded_pixelFormat;
            short* loaded_buf = tj3LoadImage12(decompress_handle, temp_filename,
                                              &loaded_width, align,
                                              &loaded_height, &loaded_pixelFormat);
            
            if (loaded_buf) {
                // ========== TEST 3: Get ICC profile (if any) ==========
                unsigned char* icc_buf = NULL;
                size_t icc_size = 0;
                tj3GetICCProfile(decompress_handle, &icc_buf, &icc_size);
                
                if (icc_buf) {
                    // ICC profile extracted, free it
                    tj3Free(icc_buf);
                }
                
                // Free loaded buffer
                tj3Free(loaded_buf);
            }
        }
        
    } else { // 16-bit precision
        // 16-bit precision uses unsigned short buffers
        std::vector<unsigned short> src_buf_16(src_sample_count);
        if (src_buf_16.empty()) {
            unlink(temp_filename);
            tj3Destroy(compress_handle);
            tj3Destroy(decompress_handle);
            tjDestroy(legacy_handle);
            return 0;
        }
        
        // Fill with valid fuzzer data for 16-bit precision (0-65535)
        size_t bytes_needed = src_sample_count * sizeof(unsigned short);
        if (fdp.remaining_bytes() < bytes_needed) {
            unlink(temp_filename);
            tj3Destroy(compress_handle);
            tj3Destroy(decompress_handle);
            tjDestroy(legacy_handle);
            return 0;
        }
        
        for (size_t i = 0; i < src_sample_count; i++) {
            src_buf_16[i] = fdp.ConsumeIntegralInRange<unsigned short>(0, 65535);
        }
        
        // ========== TEST 1: Save 16-bit image ==========
        int save_result = tj3SaveImage16(compress_handle, temp_filename,
                                        src_buf_16.data(), width,
                                        width * pixelSize, height,
                                        pixelFormat);
        
        // ========== TEST 2: Load 16-bit image ==========
        if (save_result == 0) {
            int loaded_width, loaded_height, loaded_pixelFormat;
            unsigned short* loaded_buf = tj3LoadImage16(decompress_handle, temp_filename,
                                                       &loaded_width, align,
                                                       &loaded_height, &loaded_pixelFormat);
            
            if (loaded_buf) {
                // ========== TEST 3: Get ICC profile (if any) ==========
                unsigned char* icc_buf = NULL;
                size_t icc_size = 0;
                tj3GetICCProfile(decompress_handle, &icc_buf, &icc_size);
                
                if (icc_buf) {
                    // ICC profile extracted, free it
                    tj3Free(icc_buf);
                }
                
                // Free loaded buffer
                tj3Free(loaded_buf);
            }
        }
    }
    
    // ========== PHASE 2: YUV encoding operations ==========
    
    // Create source buffer for YUV encoding (8-bit)
    size_t src_image_size = (size_t)width * height * pixelSize;
    std::vector<unsigned char> src_buf(src_image_size);
    if (!src_buf.empty()) {
        auto src_data = fdp.ConsumeBytes<unsigned char>(std::min(src_image_size, fdp.remaining_bytes()));
        if (src_data.size() > 0) {
            memcpy(src_buf.data(), src_data.data(), src_data.size());
        }
        
        // Allocate YUV buffer
        std::vector<unsigned char> yuv_buf(yuv_buf_size);
        if (!yuv_buf.empty()) {
            // ========== TEST 4: tjEncodeYUV3 (replaces tjEncodeYUV and tjEncodeYUV2) ==========
            // Note: tjEncodeYUV3 accepts alignment parameter, fixing the alignment mismatch bug
            int flags = 0;
            if (fdp.ConsumeBool()) flags |= TJFLAG_FASTDCT;
            if (fdp.ConsumeBool()) flags |= TJFLAG_BOTTOMUP;
            if (fdp.ConsumeBool()) flags |= TJFLAG_FASTUPSAMPLE;
            if (fdp.ConsumeBool()) flags |= TJFLAG_NOREALLOC;
            
            int encode_result = tjEncodeYUV3(legacy_handle,
                                          src_buf.data(),
                                          width,
                                          width * pixelSize, // pitch
                                          height,
                                          pixelFormat,
                                          yuv_buf.data(),
                                          align,  // Use the same alignment as tjBufSizeYUV2
                                          subsamp,
                                          flags);
            // Also test with different flags
            if (encode_result == 0) {
                int flags2 = 0;
                if (fdp.ConsumeBool()) flags2 |= TJFLAG_BOTTOMUP;
                if (fdp.ConsumeBool()) flags2 |= TJFLAG_FASTUPSAMPLE;
                if (fdp.ConsumeBool()) flags2 |= TJFLAG_NOREALLOC;
                
                tjEncodeYUV3(legacy_handle,
                           src_buf.data(),
                           width,
                           width * pixelSize,
                           height,
                           pixelFormat,
                           yuv_buf.data(),
                           align,  // Use the same alignment as tjBufSizeYUV2
                           subsamp,
                           flags2);
            }
        }
    }
    
    // ========== PHASE 3: Test ICC profile extraction with compression ==========
    
    // Create a simple JPEG to test ICC profile extraction
    if (fdp.remaining_bytes() > 100) {
        // Create source buffer for compression
        size_t small_width = 16;
        size_t small_height = 16;
        size_t small_sample_count = small_width * small_height * pixelSize;
        std::vector<unsigned char> small_src_buf(small_sample_count);
        
        auto small_data = fdp.ConsumeBytes<unsigned char>(std::min(small_sample_count, fdp.remaining_bytes()));
        if (small_data.size() > 0) {
            memcpy(small_src_buf.data(), small_data.data(), small_data.size());
            
            // Allocate JPEG buffer
            size_t small_jpeg_size = tj3JPEGBufSize(small_width, small_height, subsamp);
            if (small_jpeg_size > 0) {
                unsigned char* small_jpeg_buf = (unsigned char*)tj3Alloc(small_jpeg_size);
                if (small_jpeg_buf) {
                    size_t compressed_size = small_jpeg_size;
                    
                    // Compress
                    int compress_result = tj3Compress8(compress_handle,
                                                     small_src_buf.data(),
                                                     small_width,
                                                     small_width * pixelSize,
                                                     small_height,
                                                     pixelFormat,
                                                     &small_jpeg_buf,
                                                     &compressed_size);
                    
                    // Decompress to extract ICC profile
                    if (compress_result == 0 && compressed_size > 0) {
                        tj3DecompressHeader(decompress_handle, small_jpeg_buf, compressed_size);
                        
                        // Get ICC profile
                        unsigned char* icc_buf = NULL;
                        size_t icc_size = 0;
                        tj3GetICCProfile(decompress_handle, &icc_buf, &icc_size);
                        
                        if (icc_buf) {
                            tj3Free(icc_buf);
                        }
                    }
                    
                    tj3Free(small_jpeg_buf);
                }
            }
        }
    }
    
    // Clean up
    unlink(temp_filename);
    tj3Destroy(compress_handle);
    tj3Destroy(decompress_handle);
    tjDestroy(legacy_handle);
    
    return 0;
}

} // namespace harness_005
