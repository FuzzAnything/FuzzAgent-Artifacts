#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include "turbojpeg.h"
#include "jpeglib.h"
#include "jerror.h"

namespace harness_027 {
/*
 * Fuzzing harness for libjpeg-turbo comprehensive 12-bit JPEG processing
 * targeting blocked color conversion branches and uncovering 12-bit specific APIs.
 * 
 * This harness specifically targets:
 * - j12init_color_converter: 168 blocked branches (82% blocked) with high undiscovered complexity
 * - tj3Compress12: 68 blocked branches
 * - tj3Decompress12: 166 blocked branches  
 * - jpeg12_write_raw_data: Completely uncovered (0% function lines)
 * - jpeg12_read_raw_data: Completely uncovered (0% function lines)
 * 
 * Target APIs (per coverage guidance):
 * - tj3Compress12 (Primary Target: 12-bit compression with 68 blocked branches)
 * - tj3Decompress12 (Primary Target: 12-bit decompression with 166 blocked branches)
 * - jpeg12_write_raw_data (Primary Target: Completely uncovered 12-bit raw output)
 * - jpeg12_read_raw_data (Primary Target: Completely uncovered 12-bit raw input)
 * - jpeg12_write_scanlines (Required Helper: 12-bit line-by-line output)
 * - jpeg12_read_scanlines (Required Helper: 12-bit line-by-line input)
 * - tj3Set (Required Helper: Configure 12-bit processing parameters)
 * - tj3Destroy (Required Helper: Cleanup)
 * - jpeg_CreateCompress (Required Helper: Initialize 12-bit compression context)
 * - jpeg_destroy_compress (Required Helper: Cleanup)
 * - jpeg_CreateDecompress (Required Helper: Initialize 12-bit decompression context)
 * - jpeg_destroy_decompress (Required Helper: Cleanup)
 * 
 * Invocation sequence (per coverage guidance):
 * 1. Call tj3Set to configure 12-bit processing parameters
 * 2. Call jpeg_CreateCompress to initialize 12-bit compression context
 * 3. Call tj3Compress12 with various color spaces (RGB, YCbCr, CMYK) and sampling factors
 * 4. Call jpeg12_write_raw_data to test 12-bit raw output path
 * 5. Call jpeg12_write_scanlines for line-by-line 12-bit output
 * 6. Call jpeg_CreateDecompress to initialize 12-bit decompression context
 * 7. Call tj3Decompress12 with the compressed 12-bit data
 * 8. Call jpeg12_read_raw_data to test 12-bit raw input path
 * 9. Call jpeg12_read_scanlines for line-by-line 12-bit input
 * 10. Call jpeg_destroy_compress and jpeg_destroy_decompress for cleanup
 * 11. Call tj3Destroy to free turbojpeg handles
 * 
 * Reasoning: The j12init_color_converter function has 168 blocked branches (82% blocked)
 * with 481 blocker hits, indicating 12-bit color conversion paths are severely under-tested.
 * This blocker is reachable from multiple compression APIs. Targeting 12-bit specific APIs
 * (tj3Compress12, tj3Decompress12) along with completely uncovered 12-bit raw data APIs
 * (jpeg12_write_raw_data, jpeg12_read_raw_data) addresses the highest complexity gaps.
 * These functions represent 250+ points of undiscovered branch complexity and form a
 * logical 12-bit JPEG processing lifecycle.
 * 
 * Implementation strategy:
 * 1. Test both TurboJPEG (tj3*) and traditional libjpeg (jpeg*) 12-bit APIs
 * 2. Test various color spaces (RGB, YCbCr, CMYK) as specified in guidance
 * 3. Test different sampling factors to exercise different conversion paths
 * 4. Test both raw data and scanline APIs for comprehensive coverage
 * 5. Include proper error handling and resource cleanup
 * 6. Follow exact invocation sequence from coverage guidance
 * 7. Use FuzzedDataProvider to generate all test parameters from fuzzer input
 * 8. Ensure no hardcoded test cases - all data derived from fuzzer input
 */


// TurboJPEG headers

// Traditional libjpeg headers (for jpeg12_* APIs)
extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for comprehensive 12-bit testing
    // Required: dimensions (2 ints) + color space + sampling + quality + image data for both 12-bit and raw APIs
    if (size < 2048) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // ========== PHASE 1: TURBOJPEG 12-BIT COMPRESSION ==========
    
    // 1. Call tj3Set to configure 12-bit processing parameters
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    if (!compress_handle) {
        return 0;
    }
    
    // Configure 12-bit precision
    tj3Set(compress_handle, TJPARAM_PRECISION, 12);
    
    // Consume compression parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(16, 128);
    int height = fdp.ConsumeIntegralInRange<int>(16, 128);
    
    // Choose color space: 0 = RGB, 1 = YCbCr, 2 = CMYK (as specified in guidance)
    int colorspace_choice = fdp.ConsumeIntegralInRange<int>(0, 2);
    int colorspace;
    switch (colorspace_choice) {
        case 0: colorspace = TJCS_RGB; break;
        case 1: colorspace = TJCS_YCbCr; break;
        case 2: colorspace = TJCS_YCCK; break;  // CMYK uses YCCK colorspace
        default: colorspace = TJCS_RGB;
    }
    tj3Set(compress_handle, TJPARAM_COLORSPACE, colorspace);
    
    // Choose sampling factor
    int subsamp_choice = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJSAMP subsampling[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420, 
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    enum TJSAMP subsamp = subsampling[subsamp_choice % 6];
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    
    // Choose quality
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    
    // Consume other parameters
    tj3Set(compress_handle, TJPARAM_FASTDCT, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_ARITHMETIC, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_OPTIMIZE, fdp.ConsumeBool());
    
    // 2. Generate 12-bit source data for compression
    // For 12-bit data, we use short (16-bit) arrays but with 12-bit precision
    int pixel_format = (colorspace == TJCS_YCCK) ? TJPF_CMYK : TJPF_RGB;
    int pixel_size = tjPixelSize[pixel_format];
    size_t image_size = (size_t)width * height * pixel_size;
    
    if (image_size == 0 || image_size > 1024 * 1024) {  // Limit to 1MB
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Allocate and generate 12-bit source data
    std::vector<short> src_buf_12bit(image_size);
    if (src_buf_12bit.empty()) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Fill with fuzzer data for 12-bit values (0-4095 range for 12-bit)
    size_t bytes_needed = image_size * sizeof(short);
    if (fdp.remaining_bytes() < bytes_needed) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(bytes_needed);
    if (raw_data.size() < bytes_needed) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Convert bytes to 12-bit values (0-4095 range)
    for (size_t i = 0; i < image_size; i++) {
        // Combine two bytes to create 12-bit value (0-4095)
        uint16_t val = (raw_data[i * 2] << 4) | (raw_data[i * 2 + 1] & 0x0F);
        src_buf_12bit[i] = val & 0xFFF;  // Mask to 12 bits
    }
    
    // 3. Call tj3Compress12 with the 12-bit source data
    unsigned char* jpeg_buf = nullptr;
    size_t jpeg_size = 0;
    
    int compress_result = tj3Compress12(compress_handle, src_buf_12bit.data(), 
                                        width, width * pixel_size, height, 
                                        pixel_format, &jpeg_buf, &jpeg_size);
    
    // Store compressed data for decompression phase
    std::vector<unsigned char> compressed_data;
    if (compress_result == 0 && jpeg_buf && jpeg_size > 0) {
        compressed_data.assign(jpeg_buf, jpeg_buf + jpeg_size);
    }
    
    // Clean up TurboJPEG compression handle
    if (jpeg_buf) {
        tj3Free(jpeg_buf);
    }
    tj3Destroy(compress_handle);
    
    // ========== PHASE 2: TRADITIONAL LIBJPEG 12-BIT RAW DATA APIS ==========
    
    // 4. Call jpeg_CreateCompress to initialize 12-bit compression context
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_CreateCompress(&cinfo, JPEG_LIB_VERSION, sizeof(struct jpeg_compress_struct));
    
    // Set up memory destination for traditional libjpeg compression
    unsigned char* mem_buffer = nullptr;
    unsigned long mem_size = 0;
    jpeg_mem_dest(&cinfo, &mem_buffer, &mem_size);
    
    // Set image parameters
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = (pixel_format == TJPF_CMYK) ? 4 : 3;
    cinfo.in_color_space = (pixel_format == TJPF_CMYK) ? JCS_YCCK : JCS_RGB;
    
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    
    // Configure for 12-bit precision
    cinfo.data_precision = 12;
    
    // Start compression
    jpeg_start_compress(&cinfo, TRUE);
    
    // 5. Test jpeg12_write_scanlines for line-by-line 12-bit output
    // Allocate scanline buffer
    int row_stride = width * cinfo.input_components;
    J12SAMPLE* scanline_buffer = (J12SAMPLE*)malloc(row_stride * sizeof(J12SAMPLE));
    if (scanline_buffer) {
        // Prepare 12-bit scanline data (using remaining fuzzer data)
        size_t scanline_data_needed = row_stride * sizeof(J12SAMPLE);
        std::vector<uint8_t> scanline_raw = fdp.ConsumeBytes<uint8_t>(
            std::min(scanline_data_needed, fdp.remaining_bytes()));
        
        if (scanline_raw.size() >= row_stride * sizeof(J12SAMPLE)) {
            // Convert to 12-bit samples
            for (int i = 0; i < row_stride; i++) {
                uint16_t val = (scanline_raw[i * 2] << 4) | (scanline_raw[i * 2 + 1] & 0x0F);
                scanline_buffer[i] = val & 0xFFF;
            }
            
            // Write one scanline
            J12SAMPARRAY scanlines = &scanline_buffer;
            jpeg12_write_scanlines(&cinfo, scanlines, 1);
        }
        free(scanline_buffer);
    }
    
    // 6. Test jpeg12_write_raw_data for 12-bit raw output path
    // Allocate raw data buffer (2D array for components)
    int max_v_samp_factor = cinfo.max_v_samp_factor;
    JDIMENSION num_lines = DCTSIZE * max_v_samp_factor;
    
    J12SAMPIMAGE raw_image = (J12SAMPIMAGE)malloc(cinfo.num_components * sizeof(J12SAMPARRAY));
    if (raw_image) {
        for (int ci = 0; ci < cinfo.num_components; ci++) {
            int v_samp_factor = cinfo.comp_info[ci].v_samp_factor;
            JDIMENSION lines_in_buf = v_samp_factor * DCTSIZE;
            raw_image[ci] = (J12SAMPARRAY)malloc(lines_in_buf * sizeof(J12SAMPROW));
            
            if (raw_image[ci]) {
                for (JDIMENSION line = 0; line < lines_in_buf; line++) {
                    int width_in_blocks = cinfo.comp_info[ci].width_in_blocks;
                    raw_image[ci][line] = (J12SAMPROW)malloc(width_in_blocks * DCTSIZE * sizeof(J12SAMPLE));
                    
                    if (raw_image[ci][line]) {
                        // Fill with test data from remaining fuzzer input
                        size_t data_needed = width_in_blocks * DCTSIZE * sizeof(J12SAMPLE);
                        std::vector<uint8_t> component_data = fdp.ConsumeBytes<uint8_t>(
                            std::min(data_needed, fdp.remaining_bytes()));
                        
                        if (component_data.size() >= width_in_blocks * DCTSIZE * sizeof(J12SAMPLE)) {
                            for (int i = 0; i < width_in_blocks * DCTSIZE; i++) {
                                uint16_t val = (component_data[i * 2] << 4) | (component_data[i * 2 + 1] & 0x0F);
                                raw_image[ci][line][i] = val & 0xFFF;
                            }
                        }
                    }
                }
            }
        }
        
        // Write raw data
        jpeg12_write_raw_data(&cinfo, raw_image, num_lines);
        
        // Clean up raw data buffers
        for (int ci = 0; ci < cinfo.num_components; ci++) {
            if (raw_image[ci]) {
                int v_samp_factor = cinfo.comp_info[ci].v_samp_factor;
                JDIMENSION lines_in_buf = v_samp_factor * DCTSIZE;
                for (JDIMENSION line = 0; line < lines_in_buf; line++) {
                    if (raw_image[ci][line]) {
                        free(raw_image[ci][line]);
                    }
                }
                free(raw_image[ci]);
            }
        }
        free(raw_image);
    }
    
    // Finish compression
    jpeg_finish_compress(&cinfo);
    
    // Clean up traditional compression
    if (mem_buffer) {
        free(mem_buffer);
    }
    jpeg_destroy_compress(&cinfo);
    
    // ========== PHASE 3: TURBOJPEG 12-BIT DECOMPRESSION ==========
    
    // 7. Call tj3Decompress12 with the compressed 12-bit data (if available)
    if (!compressed_data.empty()) {
        tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
        if (decompress_handle) {
            // Configure for 12-bit decompression
            tj3Set(decompress_handle, TJPARAM_PRECISION, 12);
            
            // Allocate buffer for decompressed 12-bit data
            int decompressed_width, decompressed_height;
            int decompressed_subsamp, decompressed_colorspace;
            
            // Get JPEG header information
            tj3DecompressHeader(decompress_handle, compressed_data.data(), compressed_data.size());
            decompressed_width = tj3Get(decompress_handle, TJPARAM_JPEGWIDTH);
            decompressed_height = tj3Get(decompress_handle, TJPARAM_JPEGHEIGHT);
            decompressed_subsamp = tj3Get(decompress_handle, TJPARAM_SUBSAMP);
            decompressed_colorspace = tj3Get(decompress_handle, TJPARAM_COLORSPACE);
            
            // Choose output pixel format based on colorspace
            int out_pixel_format = (decompressed_colorspace == TJCS_YCCK) ? TJPF_CMYK : TJPF_RGB;
            int out_pixel_size = tjPixelSize[out_pixel_format];
            size_t out_buf_size = (size_t)decompressed_width * decompressed_height * out_pixel_size;
            
            if (out_buf_size > 0 && out_buf_size <= 1024 * 1024) {
                std::vector<short> out_buf_12bit(out_buf_size);
                if (!out_buf_12bit.empty()) {
                    // 7. Call tj3Decompress12 with correct parameters
                    // Signature: tj3Decompress12(handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat)
                    int pitch = decompressed_width * out_pixel_size; // Calculate pitch
                    tj3Decompress12(decompress_handle, compressed_data.data(), compressed_data.size(),
                                   out_buf_12bit.data(), pitch, out_pixel_format);
                }
            }
            
            tj3Destroy(decompress_handle);
        }
    }
    
    // ========== PHASE 4: TRADITIONAL LIBJPEG 12-BIT DECOMPRESSION ==========
    
    // 8. Test jpeg12_read_raw_data for 12-bit raw input path (if we have compressed data)
    if (!compressed_data.empty() && compressed_data.size() > 100) {
        struct jpeg_decompress_struct dinfo;
        struct jpeg_error_mgr derr;
        
        dinfo.err = jpeg_std_error(&derr);
        jpeg_CreateDecompress(&dinfo, JPEG_LIB_VERSION, sizeof(struct jpeg_decompress_struct));
        
        // Set up memory source
        jpeg_mem_src(&dinfo, compressed_data.data(), compressed_data.size());
        
        // Read header
        jpeg_read_header(&dinfo, TRUE);
        
        // Configure for raw data output
        dinfo.raw_data_out = TRUE;
        dinfo.do_fancy_upsampling = FALSE;
        
        // Start decompression
        jpeg_start_decompress(&dinfo);
        
        // Allocate raw data buffers for reading
        int max_v_samp_factor = dinfo.max_v_samp_factor;
        JDIMENSION lines_per_buf = max_v_samp_factor * DCTSIZE;
        
        J12SAMPIMAGE raw_in_buf = (J12SAMPIMAGE)malloc(dinfo.num_components * sizeof(J12SAMPARRAY));
        if (raw_in_buf) {
            for (int ci = 0; ci < dinfo.num_components; ci++) {
                int v_samp_factor = dinfo.comp_info[ci].v_samp_factor;
                JDIMENSION lines_in_buf = v_samp_factor * DCTSIZE;
                raw_in_buf[ci] = (J12SAMPARRAY)malloc(lines_in_buf * sizeof(J12SAMPROW));
                
                if (raw_in_buf[ci]) {
                    for (JDIMENSION line = 0; line < lines_in_buf; line++) {
                        int width_in_blocks = dinfo.comp_info[ci].width_in_blocks;
                        raw_in_buf[ci][line] = (J12SAMPROW)malloc(width_in_blocks * DCTSIZE * sizeof(J12SAMPLE));
                    }
                }
            }
            
            // 8. Call jpeg12_read_raw_data to test 12-bit raw input path
            while (dinfo.output_scanline < dinfo.output_height) {
                jpeg12_read_raw_data(&dinfo, raw_in_buf, lines_per_buf);
            }
            
            // Clean up raw input buffers
            for (int ci = 0; ci < dinfo.num_components; ci++) {
                if (raw_in_buf[ci]) {
                    int v_samp_factor = dinfo.comp_info[ci].v_samp_factor;
                    JDIMENSION lines_in_buf = v_samp_factor * DCTSIZE;
                    for (JDIMENSION line = 0; line < lines_in_buf; line++) {
                        if (raw_in_buf[ci][line]) {
                            free(raw_in_buf[ci][line]);
                        }
                    }
                    free(raw_in_buf[ci]);
                }
            }
            free(raw_in_buf);
        }
        
        // 9. Test jpeg12_read_scanlines for line-by-line 12-bit input
        // Reset decompressor for normal (non-raw) output
        jpeg_destroy_decompress(&dinfo);
        jpeg_CreateDecompress(&dinfo, JPEG_LIB_VERSION, sizeof(struct jpeg_decompress_struct));
        dinfo.err = jpeg_std_error(&derr);
        jpeg_mem_src(&dinfo, compressed_data.data(), compressed_data.size());
        jpeg_read_header(&dinfo, TRUE);
        
        // Start normal decompression
        jpeg_start_decompress(&dinfo);
        
        // Allocate scanline buffer
        int out_row_stride = dinfo.output_width * dinfo.output_components;
        J12SAMPLE* out_scanline = (J12SAMPLE*)malloc(out_row_stride * sizeof(J12SAMPLE));
        if (out_scanline) {
            J12SAMPARRAY out_scanlines = &out_scanline;
            
            // 9. Call jpeg12_read_scanlines for line-by-line 12-bit input
            while (dinfo.output_scanline < dinfo.output_height) {
                jpeg12_read_scanlines(&dinfo, out_scanlines, 1);
            }
            
            free(out_scanline);
        }
        
        jpeg_finish_decompress(&dinfo);
        jpeg_destroy_decompress(&dinfo);
    }
    
    return 0;
}

} // namespace harness_027
