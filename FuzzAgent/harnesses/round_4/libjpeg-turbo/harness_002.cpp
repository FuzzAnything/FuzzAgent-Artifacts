#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "turbojpeg.h"

namespace harness_002 {
/*
 * Fuzzing harness for libjpeg-turbo targeting turbojpeg core compression and 
 * decompression operations with YUV conversion support.
 * This harness targets the main turbojpeg interface which currently has 0% coverage
 * (61.64% of APIs untouched) as identified by coverage analysis.
 * 
 * Target APIs: tjDecompress, tjCompress, tjDecompress2, tjDecompressToYUV, tjCompressFromYUV
 * Helper APIs: tjInitDecompress, tjInitCompress, tjDestroy, tjAlloc/tjFree, tjDecompressHeader*
 * 
 * Workflow: Initialize -> Parse Header -> Decompress -> Convert -> Compress -> Cleanup
 */


extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for basic JPEG header and parameters
    if (size < 200) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    /**************************************************************************
     * PART 1: DECOMPRESSION WORKFLOW
     * Initialize decompressor -> Parse header -> Decompress JPEG
     **************************************************************************/
    {
        // Create decompressor handle
        tjhandle decompress_handle = tjInitDecompress();
        if (decompress_handle == NULL) {
            // If decompressor init fails, try compressor init instead
            goto compressor_test;
        }
        
        // Use input data as JPEG buffer
        size_t remaining = fdp.remaining_bytes();
        if (remaining < 100) {
            tjDestroy(decompress_handle);
            return 0;
        }
        
        // Consume JPEG data from input
        size_t jpeg_size = fdp.ConsumeIntegralInRange<size_t>(100, remaining);
        std::vector<uint8_t> jpeg_buffer = fdp.ConsumeBytes<uint8_t>(jpeg_size);
        
        // Parse JPEG header to get dimensions and format
        int width = 0, height = 0, jpeg_subsamp = 0;
        int header_result = tjDecompressHeader2(decompress_handle, 
                                               jpeg_buffer.data(), 
                                               jpeg_buffer.size(),
                                               &width, &height, &jpeg_subsamp);
        
        // If header parsing fails, try simpler header parsing
        if (header_result != 0) {
            width = 0; height = 0;
            header_result = tjDecompressHeader(decompress_handle,
                                              jpeg_buffer.data(),
                                              jpeg_buffer.size(),
                                              &width, &height);
        }
        
        // Validate dimensions to avoid memory exhaustion
        if (header_result == 0 && width > 0 && height > 0 && 
            (uint64_t)width * height <= 1048576) { // Limit to 1MP
            
            // Consume decompression parameters from fuzzer input
            int pixel_size = fdp.ConsumeIntegralInRange<int>(3, 4); // RGB or RGBA
            int pitch = width * pixel_size;
            int flags = 0;
            
            // Consume flag combinations
            if (fdp.ConsumeBool()) flags |= TJFLAG_BOTTOMUP;
            if (fdp.ConsumeBool()) flags |= TJFLAG_FASTUPSAMPLE;
            if (fdp.ConsumeBool()) flags |= TJFLAG_FASTDCT;
            if (fdp.ConsumeBool()) flags |= TJFLAG_ACCURATEDCT;
            
            // Allocate destination buffer
            size_t dest_size = (size_t)pitch * height;
            if (dest_size > 0 && dest_size < 10 * 1024 * 1024) { // Limit to 10MB
                unsigned char *dest_buf = (unsigned char *)tjAlloc(dest_size);
                if (dest_buf != NULL) {
                    // Test tjDecompress (legacy API)
                    int decompress_result = tjDecompress(decompress_handle,
                                                        jpeg_buffer.data(),
                                                        jpeg_buffer.size(),
                                                        dest_buf,
                                                        width, pitch, height,
                                                        pixel_size, flags);
                    
                    // If legacy decompress fails, try tjDecompress2 (newer API)
                    if (decompress_result != 0) {
                        int pixel_format = (pixel_size == 3) ? TJPF_RGB : TJPF_RGBA;
                        decompress_result = tjDecompress2(decompress_handle,
                                                         jpeg_buffer.data(),
                                                         jpeg_buffer.size(),
                                                         dest_buf,
                                                         width, pitch, height,
                                                         pixel_format, flags);
                    }
                    
                    // If we successfully decompressed to RGB, try YUV conversion
                    if (decompress_result == 0 && dest_buf != NULL) {
                        // Calculate YUV buffer size
                        int align = fdp.ConsumeIntegralInRange<int>(1, 16);
                        size_t yuv_size = tjBufSizeYUV2(width, align, height, jpeg_subsamp);
                        
                        if (yuv_size > 0 && yuv_size < 10 * 1024 * 1024) {
                            unsigned char *yuv_buf = (unsigned char *)tjAlloc(yuv_size);
                            if (yuv_buf != NULL) {
                                // Test tjDecompressToYUV (legacy API)
                                int yuv_result = tjDecompressToYUV(decompress_handle,
                                                                  jpeg_buffer.data(),
                                                                  jpeg_buffer.size(),
                                                                  yuv_buf, flags);
                                
                                // If legacy YUV fails, try tjDecompressToYUV2 (newer API)
                                if (yuv_result != 0) {
                                    yuv_result = tjDecompressToYUV2(decompress_handle,
                                                                   jpeg_buffer.data(),
                                                                   jpeg_buffer.size(),
                                                                   yuv_buf,
                                                                   width, align, height,
                                                                   flags);
                                }
                                
                                // Store YUV buffer for potential compression test
                                unsigned char *yuv_buffer_for_compression = yuv_buf;
                                size_t yuv_buffer_size = yuv_size;
                                
                                // Don't free yet - might use in compression test
                                // tjFree(yuv_buf);
                            }
                        }
                    }
                    
                    // Free the destination buffer
                    tjFree(dest_buf);
                }
            }
        }
        
        // Clean up decompressor
        tjDestroy(decompress_handle);
    }
    
compressor_test:
    /**************************************************************************
     * PART 2: COMPRESSION WORKFLOW
     * Initialize compressor -> Compress data (RGB or YUV) -> Cleanup
     **************************************************************************/
    {
        // Create compressor handle
        tjhandle compress_handle = tjInitCompress();
        if (compress_handle == NULL) {
            return 0;
        }
        
        // Check if we have enough remaining data
        if (fdp.remaining_bytes() < 100) {
            tjDestroy(compress_handle);
            return 0;
        }
        
        // Consume compression parameters from fuzzer input
        int width = fdp.ConsumeIntegralInRange<int>(1, 1024);
        int height = fdp.ConsumeIntegralInRange<int>(1, 1024);
        int pixel_size = fdp.ConsumeIntegralInRange<int>(3, 4);
        int pitch = width * pixel_size;
        int jpeg_subsamp = fdp.ConsumeIntegralInRange<int>(0, TJ_NUMSAMP - 1);
        int jpeg_quality = fdp.ConsumeIntegralInRange<int>(1, 100);
        int flags = 0;
        
        // Consume flag combinations
        if (fdp.ConsumeBool()) flags |= TJFLAG_BOTTOMUP;
        if (fdp.ConsumeBool()) flags |= TJFLAG_FASTDCT;
        if (fdp.ConsumeBool()) flags |= TJFLAG_ACCURATEDCT;
        if (fdp.ConsumeBool()) flags |= TJFLAG_PROGRESSIVE;
        
        // Test compression from RGB data
        size_t src_size = (size_t)pitch * height;
        if (src_size > 0 && src_size < 10 * 1024 * 1024 && fdp.remaining_bytes() >= src_size) {
            std::vector<uint8_t> src_buffer = fdp.ConsumeBytes<uint8_t>(src_size);
            
            // Allocate destination buffer for compressed JPEG
            unsigned long jpeg_size = 0;
            unsigned char *jpeg_buf = NULL;
            
            int pixel_format = (pixel_size == 3) ? TJPF_RGB : TJPF_RGBA;
            
            // Test tjCompress2 (newer API)
            int compress_result = tjCompress2(compress_handle,
                                             src_buffer.data(),
                                             width, pitch, height,
                                             pixel_format,
                                             &jpeg_buf, &jpeg_size,
                                             jpeg_subsamp, jpeg_quality,
                                             flags);
            
            // If newer API fails, try legacy tjCompress
            if (compress_result != 0) {
                jpeg_size = tjBufSize(width, height, jpeg_subsamp);
                if (jpeg_size > 0 && jpeg_size < 10 * 1024 * 1024) {
                    jpeg_buf = (unsigned char *)tjAlloc(jpeg_size);
                    if (jpeg_buf != NULL) {
                        compress_result = tjCompress(compress_handle,
                                                    src_buffer.data(),
                                                    width, pitch, height,
                                                    pixel_size,
                                                    jpeg_buf, &jpeg_size,
                                                    jpeg_subsamp, jpeg_quality,
                                                    flags);
                    }
                }
            }
            
            // Free compressed buffer if allocated
            if (jpeg_buf != NULL) {
                tjFree(jpeg_buf);
            }
        }
        
        // Test compression from YUV data (if we have enough remaining input)
        if (fdp.remaining_bytes() >= 100) {
            int yuv_width = fdp.ConsumeIntegralInRange<int>(1, 1024);
            int yuv_height = fdp.ConsumeIntegralInRange<int>(1, 1024);
            int yuv_align = fdp.ConsumeIntegralInRange<int>(1, 16);
            int yuv_subsamp = fdp.ConsumeIntegralInRange<int>(0, TJ_NUMSAMP - 1);
            int yuv_quality = fdp.ConsumeIntegralInRange<int>(1, 100);
            int yuv_flags = 0;
            
            if (fdp.ConsumeBool()) yuv_flags |= TJFLAG_FASTDCT;
            if (fdp.ConsumeBool()) yuv_flags |= TJFLAG_ACCURATEDCT;
            
            // Calculate YUV buffer size
            size_t yuv_size = tjBufSizeYUV2(yuv_width, yuv_align, yuv_height, yuv_subsamp);
            if (yuv_size > 0 && yuv_size < 10 * 1024 * 1024 && fdp.remaining_bytes() >= yuv_size) {
                std::vector<uint8_t> yuv_buffer = fdp.ConsumeBytes<uint8_t>(yuv_size);
                
                unsigned long compressed_size = 0;
                unsigned char *compressed_buf = NULL;
                
                // Test tjCompressFromYUV
                int yuv_compress_result = tjCompressFromYUV(compress_handle,
                                                           yuv_buffer.data(),
                                                           yuv_width, yuv_align,
                                                           yuv_height, yuv_subsamp,
                                                           &compressed_buf,
                                                           &compressed_size,
                                                           yuv_quality, yuv_flags);
                
                // Free compressed buffer if allocated
                if (compressed_buf != NULL) {
                    tjFree(compressed_buf);
                }
            }
        }
        
        // Clean up compressor
        tjDestroy(compress_handle);
    }
    
    return 0;
}

} // namespace harness_002
