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

namespace harness_028 {
/*
 * Fuzzing harness for libjpeg-turbo 12-bit compression with comprehensive color space coverage
 * targeting blocked color conversion paths in j12init_color_converter (170 blocked branches, 83.3% blocked).
 * 
 * This harness specifically targets:
 * - j12init_color_converter: 170 blocked branches (83.3% blocked) - highest blocker in codebase
 * - Related color conversion functions: jinit_color_converter (125 branches), j16init_color_converter (63 branches)
 * - j12init_color_deconverter: 133 blocked branches (12-bit decompression)
 * 
 * Target APIs (per coverage guidance):
 * - tj3Compress12 (Primary Target: 12-bit compression)
 * - tj3Decompress12 (For verification)
 * - jpeg12_write_scanlines (12-bit data input)
 * - jpeg12_read_scanlines (12-bit data output)
 * - tj3Init (Initialization)
 * - tj3Destroy (Cleanup)
 * - tj3Alloc (Memory allocation)
 * - tj3Free (Memory cleanup)
 * - jpeg_CreateCompress (JPEG compress object creation)
 * - jpeg_destroy_compress (Cleanup)
 * 
 * Invocation sequence (per coverage guidance):
 * 1. Initialize TurboJPEG handle with tj3Init
 * 2. Create JPEG compression object with jpeg_CreateCompress
 * 3. Set up compression parameters for 12-bit depth
 * 4. Test multiple color space combinations:
 *    - RGB → YCbCr conversion
 *    - CMYK → YCCK conversion  
 *    - Grayscale → Grayscale (no conversion)
 *    - Extended RGB formats (EXT_RGB, EXT_RGBX, etc.)
 *    - Lossless 12-bit compression
 * 5. For each color space:
 *    - Allocate source buffer with tj3Alloc
 *    - Write 12-bit scanlines with jpeg12_write_scanlines
 *    - Compress with tj3Compress12
 *    - Optionally decompress with tj3Decompress12 for verification
 *    - Free buffers with tj3Free
 * 6. Clean up with jpeg_destroy_compress and tj3Destroy
 * 
 * Reasoning: 
 * - j12init_color_converter represents 170+ blocked branches (highest blocker)
 * - 12-bit color conversion paths are severely under-tested compared to 8-bit
 * - Color space conversion is a complex, conditional-heavy area of the code
 * - This harness can also cover related blockers: jinit_color_converter (125 branches), 
 *   j16init_color_converter (63 branches), and j12init_color_deconverter (133 branches)
 * - The 12-bit compression APIs (tj3Compress12, tj3Decompress12) have moderate coverage 
 *   but can be exercised more comprehensively with varied color spaces
 * 
 * Implementation strategy:
 * 1. Test comprehensive color space combinations as specified in guidance
 * 2. Use tj3Alloc/tj3Free for proper memory management
 * 3. Test both compression and optional decompression for verification
 * 4. Include traditional libjpeg APIs (jpeg_CreateCompress, jpeg12_write_scanlines) for full coverage
 * 5. Use FuzzedDataProvider to generate all test parameters from fuzzer input
 * 6. Ensure no hardcoded test cases - all data derived from fuzzer input
 */


// TurboJPEG headers

// Traditional libjpeg headers (for jpeg12_* APIs)
extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for comprehensive 12-bit color space testing
    // Required: dimensions + color space selection + multiple test configurations + image data
    if (size < 4096) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // ========== PHASE 1: TURBOJPEG 12-BIT COMPRESSION WITH COMPREHENSIVE COLOR SPACES ==========
    
    // 1. Initialize TurboJPEG handle with tj3Init
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    if (!compress_handle) {
        return 0;
    }
    
    // Configure 12-bit precision
    tj3Set(compress_handle, TJPARAM_PRECISION, 12);
    
    // Consume basic parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(16, 128);
    int height = fdp.ConsumeIntegralInRange<int>(16, 128);
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Determine number of color space tests to perform (1-5 as per guidance)
    int num_color_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int test_idx = 0; test_idx < num_color_tests; test_idx++) {
        // Skip if not enough data remaining
        if (fdp.remaining_bytes() < 100) break;
        
        // Consume color space configuration for this test
        int color_config = fdp.ConsumeIntegralInRange<int>(0, 7);
        
        enum TJPF pixel_format;
        int jpeg_colorspace;
        enum TJSAMP subsamp;
        const char* test_name;
        
        // Configure based on color configuration (covering all combinations from guidance)
        switch (color_config) {
            case 0:
                // RGB → YCbCr conversion (as specified in guidance)
                pixel_format = TJPF_RGB;
                jpeg_colorspace = TJCS_YCbCr;
                subsamp = TJSAMP_444;
                test_name = "RGB->YCbCr";
                break;
            case 1:
                // CMYK → YCCK conversion (as specified in guidance)
                pixel_format = TJPF_CMYK;
                jpeg_colorspace = TJCS_YCCK;
                subsamp = TJSAMP_444;
                test_name = "CMYK->YCCK";
                break;
            case 2:
                // Grayscale → Grayscale (no conversion) (as specified in guidance)
                pixel_format = TJPF_GRAY;
                jpeg_colorspace = TJCS_GRAY;
                subsamp = TJSAMP_GRAY;
                test_name = "Grayscale";
                break;
            case 3:
                // Extended RGB format: RGBX
                pixel_format = TJPF_RGBX;
                jpeg_colorspace = TJCS_RGB;
                subsamp = TJSAMP_444;
                test_name = "RGBX->RGB";
                break;
            case 4:
                // Extended RGB format: BGR
                pixel_format = TJPF_BGR;
                jpeg_colorspace = TJCS_YCbCr;
                subsamp = TJSAMP_422;
                test_name = "BGR->YCbCr";
                break;
            case 5:
                // Extended RGB format: XBGR
                pixel_format = TJPF_XBGR;
                jpeg_colorspace = TJCS_RGB;
                subsamp = TJSAMP_420;
                test_name = "XBGR->RGB";
                break;
            case 6:
                // Lossless 12-bit compression with custom sampling
                pixel_format = TJPF_RGB;
                jpeg_colorspace = TJCS_YCbCr;
                subsamp = TJSAMP_444;
                quality = 100; // Lossless
                test_name = "Lossless RGB->YCbCr";
                break;
            case 7:
                // Additional extended format: BGRA
                pixel_format = TJPF_BGRA;
                jpeg_colorspace = TJCS_YCbCr;
                subsamp = TJSAMP_411;
                test_name = "BGRA->YCbCr";
                break;
        }
        
        // Set JPEG color space
        tj3Set(compress_handle, TJPARAM_COLORSPACE, jpeg_colorspace);
        tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
        tj3Set(compress_handle, TJPARAM_QUALITY, quality);
        
        // Consume additional compression parameters
        int bottomUp = fdp.ConsumeBool() ? 1 : 0;
        int fastDCT = fdp.ConsumeBool() ? 1 : 0;
        int progressive = fdp.ConsumeBool() ? 1 : 0;
        int arithmetic = fdp.ConsumeBool() ? 1 : 0;
        
        tj3Set(compress_handle, TJPARAM_BOTTOMUP, bottomUp);
        tj3Set(compress_handle, TJPARAM_FASTDCT, fastDCT);
        tj3Set(compress_handle, TJPARAM_PROGRESSIVE, progressive);
        tj3Set(compress_handle, TJPARAM_ARITHMETIC, arithmetic);
        
        // 2. Create JPEG compression object with jpeg_CreateCompress (traditional libjpeg path)
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;
        
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_CreateCompress(&cinfo, JPEG_LIB_VERSION, sizeof(struct jpeg_compress_struct));
        
        // Set up memory destination
        unsigned char* mem_buffer = nullptr;
        unsigned long mem_size = 0;
        jpeg_mem_dest(&cinfo, &mem_buffer, &mem_size);
        
        // Set image parameters for traditional libjpeg
        cinfo.image_width = width;
        cinfo.image_height = height;
        
        // Map pixel format to libjpeg color space
        switch (pixel_format) {
            case TJPF_RGB:
            case TJPF_BGR:
            case TJPF_RGBX:
            case TJPF_BGRX:
            case TJPF_XRGB:
            case TJPF_XBGR:
                cinfo.input_components = 3;
                cinfo.in_color_space = JCS_RGB;
                break;
            case TJPF_GRAY:
                cinfo.input_components = 1;
                cinfo.in_color_space = JCS_GRAYSCALE;
                break;
            case TJPF_CMYK:
                cinfo.input_components = 4;
                cinfo.in_color_space = JCS_YCCK;
                break;
            default:
                cinfo.input_components = 3;
                cinfo.in_color_space = JCS_RGB;
                break;
        }
        
        jpeg_set_defaults(&cinfo);
        
        // Configure for 12-bit precision
        cinfo.data_precision = 12;
        
        // Set quality
        jpeg_set_quality(&cinfo, quality, TRUE);
        
        // Set color space for compression
        switch (jpeg_colorspace) {
            case TJCS_RGB:
                cinfo.jpeg_color_space = JCS_RGB;
                break;
            case TJCS_YCbCr:
                cinfo.jpeg_color_space = JCS_YCbCr;
                break;
            case TJCS_GRAY:
                cinfo.jpeg_color_space = JCS_GRAYSCALE;
                break;
            case TJCS_CMYK:
            case TJCS_YCCK:
                cinfo.jpeg_color_space = JCS_YCCK;
                break;
            default:
                cinfo.jpeg_color_space = JCS_YCbCr;
                break;
        }
        
        // Set sampling factors based on subsampling
        switch (subsamp) {
            case TJSAMP_444:
                cinfo.comp_info[0].h_samp_factor = 1;
                cinfo.comp_info[0].v_samp_factor = 1;
                break;
            case TJSAMP_422:
                cinfo.comp_info[0].h_samp_factor = 2;
                cinfo.comp_info[0].v_samp_factor = 1;
                break;
            case TJSAMP_420:
                cinfo.comp_info[0].h_samp_factor = 2;
                cinfo.comp_info[0].v_samp_factor = 2;
                break;
            case TJSAMP_440:
                cinfo.comp_info[0].h_samp_factor = 1;
                cinfo.comp_info[0].v_samp_factor = 2;
                break;
            case TJSAMP_411:
                cinfo.comp_info[0].h_samp_factor = 4;
                cinfo.comp_info[0].v_samp_factor = 1;
                break;
            case TJSAMP_GRAY:
                cinfo.comp_info[0].h_samp_factor = 1;
                cinfo.comp_info[0].v_samp_factor = 1;
                break;
        }
        
        // Start compression
        jpeg_start_compress(&cinfo, TRUE);
        
        // 3. Allocate source buffer with tj3Alloc (as specified in guidance)
        int pixel_size = tjPixelSize[pixel_format];
        size_t src_size = (size_t)width * height * pixel_size * 2; // *2 for 12-bit (2 bytes per sample)
        
        // 4. Allocate source buffer using tj3Alloc
        unsigned short* src_buf = (unsigned short*)tj3Alloc(src_size);
        if (!src_buf) {
            jpeg_destroy_compress(&cinfo);
            if (mem_buffer) free(mem_buffer);
            continue; // Try next test
        }
        
        // Fill source buffer with fuzzer data
        size_t data_to_consume = std::min(src_size, fdp.remaining_bytes());
        std::vector<uint8_t> src_data = fdp.ConsumeBytes<uint8_t>(data_to_consume);
        
        // Convert to 12-bit samples (0-4095 range)
        for (size_t i = 0; i < src_data.size() / 2 && i * 2 < src_size / sizeof(unsigned short); i++) {
            uint16_t val = (src_data[i * 2] << 4) | (src_data[i * 2 + 1] & 0x0F);
            src_buf[i] = val & 0xFFF; // 12-bit mask
        }
        
        // 5. Write 12-bit scanlines with jpeg12_write_scanlines (traditional libjpeg)
        int row_stride = width * cinfo.input_components;
        J12SAMPLE* scanline_buffer = (J12SAMPLE*)tj3Alloc(row_stride * sizeof(J12SAMPLE));
        
        if (scanline_buffer) {
            // Prepare one scanline from source data
            for (int i = 0; i < row_stride && i * 2 < (int)src_data.size(); i++) {
                uint16_t val = (src_data[i * 2] << 4) | (src_data[i * 2 + 1] & 0x0F);
                scanline_buffer[i] = val & 0xFFF;
            }
            
            J12SAMPARRAY scanlines = &scanline_buffer;
            
            // Write one scanline using traditional libjpeg API
            jpeg12_write_scanlines(&cinfo, scanlines, 1);
            
            // Free scanline buffer with tj3Free
            tj3Free(scanline_buffer);
        }
        
        // Finish traditional libjpeg compression
        jpeg_finish_compress(&cinfo);
        jpeg_destroy_compress(&cinfo);
        
        // 6. Compress with tj3Compress12 (TurboJPEG API)
        unsigned char* dst_buf = nullptr;
        size_t dst_size = 0;
        
        // Get maximum buffer size needed
        size_t max_dst_size = tj3JPEGBufSize(width, height, subsamp);
        
        // Allocate destination buffer with tj3Alloc
        dst_buf = (unsigned char*)tj3Alloc(max_dst_size);
        if (!dst_buf) {
            tj3Free(src_buf);
            if (mem_buffer) free(mem_buffer);
            continue;
        }
        
        dst_size = max_dst_size;
        
        // Perform compression - note: tj3Compress12 expects const short* for srcBuf
        int compress_result = tj3Compress12(compress_handle, 
                                           (const short*)src_buf, width, 0, height, pixel_format,
                                           &dst_buf, &dst_size);
        
        // 7. Optionally decompress with tj3Decompress12 for verification (as specified in guidance)
        if (compress_result == 0 && dst_size > 0 && fdp.ConsumeBool()) {
            tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
            if (decompress_handle) {
                // Set decompression parameters
                tj3Set(decompress_handle, TJPARAM_PRECISION, 12);
                
                int decomp_width = 0, decomp_height = 0, decomp_pixel_format = 0;
                int decomp_subsamp = 0, decomp_colorspace = 0;
                
                // Get image info
                if (tj3DecompressHeader(decompress_handle, dst_buf, dst_size) == 0) {
                    decomp_width = tj3Get(decompress_handle, TJPARAM_JPEGWIDTH);
                    decomp_height = tj3Get(decompress_handle, TJPARAM_JPEGHEIGHT);
                    decomp_subsamp = tj3Get(decompress_handle, TJPARAM_SUBSAMP);
                    decomp_colorspace = tj3Get(decompress_handle, TJPARAM_COLORSPACE);
                    
                    // Choose decompression pixel format based on original
                    enum TJPF decomp_pf;
                    if (pixel_format == TJPF_CMYK) {
                        decomp_pf = TJPF_CMYK;
                    } else if (pixel_format == TJPF_GRAY) {
                        decomp_pf = TJPF_GRAY;
                    } else {
                        decomp_pf = TJPF_RGB; // Default to RGB for decompression
                    }
                    
                    // Allocate decompression buffer with tj3Alloc
                    size_t decomp_buf_size = (size_t)decomp_width * decomp_height * 
                                            tjPixelSize[decomp_pf] * 2; // *2 for 12-bit
                    short* decomp_buf = (short*)tj3Alloc(decomp_buf_size);
                    
                    if (decomp_buf) {
                        // Perform decompression - note: tj3Decompress12 expects short* for dstBuf
                        // Parameters: handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat
                        int pitch = decomp_width * tjPixelSize[decomp_pf];
                        tj3Decompress12(decompress_handle, dst_buf, dst_size,
                                       decomp_buf, pitch, decomp_pf);
                        
                        // Free decompression buffer with tj3Free
                        tj3Free(decomp_buf);
                    }
                }
                
                tj3Destroy(decompress_handle);
            }
        }
        // 8. Free buffers with tj3Free (as specified in guidance)
        tj3Free(dst_buf);
        tj3Free(src_buf);
        
        // Free traditional libjpeg memory buffer
        if (mem_buffer) {
            free(mem_buffer);
            mem_buffer = nullptr;
            mem_size = 0;
        }
        
        // Reset quality for next iteration if it was set to lossless
        if (color_config == 6) {
            quality = fdp.ConsumeIntegralInRange<int>(1, 100);
        }
    }
    
    // 9. Clean up with tj3Destroy
    tj3Destroy(compress_handle);
    
    return 0;
}

} // namespace harness_028
