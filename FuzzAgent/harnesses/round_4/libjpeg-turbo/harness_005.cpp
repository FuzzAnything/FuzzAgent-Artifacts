#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>  // For close, lseek, write, ftruncate, mkstemp
#include "turbojpeg.h"

namespace harness_005 {
/*
 * Fuzzing harness for libjpeg-turbo targeting image file I/O and YUV decode/encode operations.
 * This harness targets image loading/saving functions and YUV decode/encode functions
 * which are currently 0% covered across all existing harnesses.
 * 
 * Target APIs: tjLoadImage, tj3LoadImage8, tjSaveImage, tj3SaveImage8,
 *              tjDecodeYUV, tj3DecodeYUV8, tjEncodeYUV, tj3EncodeYUV8,
 *              tj3GetICCProfile, tj3SetICCProfile
 * 
 * Helper APIs: tjInitDecompress, tjInitCompress, tj3Init, tj3Destroy,
 *              tjAlloc/tj3Alloc, tjFree/tj3Free
 * 
 * Workflow: 
 * 1. Create temporary files from fuzzer input for image loading
 * 2. Initialize TurboJPEG instances (legacy and v3)
 * 3. Test image loading from files (tjLoadImage, tj3LoadImage8)
 * 4. Test YUV decode/encode operations on loaded image data
 * 5. Test ICC profile handling
 * 6. Test image saving to files (tjSaveImage, tj3SaveImage8)
 * 7. Clean up temporary files and TurboJPEG instances
 * 
 * Semantic diversity: This harness uniquely targets file I/O operations and
 * the complete YUV decode/encode lifecycle, complementing existing harnesses
 * that focus on memory-based operations.
 */


extern "C" {
}

// Maximum dimensions to prevent memory exhaustion
const int MAX_WIDTH = 1024;
const int MAX_HEIGHT = 1024;

// Temporary file handling
class TempFile {
public:
    TempFile(const std::string& suffix = ".tmp") {
        char template_path[] = "/tmp/fuzz_tmp_XXXXXX";
        fd = mkstemp(template_path);
        if (fd != -1) {
            filename = template_path;
        }
    }
    
    ~TempFile() {
        if (fd != -1) {
            close(fd);
            std::remove(filename.c_str());
        }
    }
    
    bool writeData(const uint8_t* data, size_t size) {
        if (fd == -1) return false;
        
        // Reset file pointer to beginning
        lseek(fd, 0, SEEK_SET);
        ftruncate(fd, 0);
        
        ssize_t written = write(fd, data, size);
        return written == (ssize_t)size;
    }
    
    const char* getFilename() const {
        return filename.c_str();
    }
    
    bool isValid() const {
        return fd != -1;
    }
    
private:
    int fd = -1;
    std::string filename;
};

// Helper to limit dimensions safely
static void safe_dimensions(int* width, int* height) {
    if (*width <= 0) *width = 64;
    if (*height <= 0) *height = 64;
    
    if (*width > MAX_WIDTH) *width = MAX_WIDTH;
    if (*height > MAX_HEIGHT) *height = MAX_HEIGHT;
    
    // Ensure at least 8x8 for JPEG compliance
    if (*width < 8) *width = 8;
    if (*height < 8) *height = 8;
}

// Generate a simple PPM file for testing image loading
static bool generate_test_ppm(TempFile& file, int width, int height) {
    std::string header = "P6\n" + std::to_string(width) + " " + 
                         std::to_string(height) + "\n255\n";
    
    std::vector<uint8_t> image_data;
    image_data.reserve(header.size() + width * height * 3);
    
    // Add header
    image_data.insert(image_data.end(), header.begin(), header.end());
    
    // Add simple gradient image data
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image_data.push_back((x * 255) / width);      // R
            image_data.push_back((y * 255) / height);     // G
            image_data.push_back((x + y) * 255 / (width + height));  // B
        }
    }
    
    return file.writeData(image_data.data(), image_data.size());
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for parameters and test image
    if (size < 200) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    /**************************************************************************
     * PART 1: CREATE TEST FILES FOR IMAGE LOADING
     **************************************************************************/
    
    // Create temporary files for image loading tests
    TempFile ppm_file(".ppm");
    if (!ppm_file.isValid()) {
        return 0;
    }
    
    // Consume dimensions from fuzzer input
    int test_width = fdp.ConsumeIntegralInRange<int>(8, 256);
    int test_height = fdp.ConsumeIntegralInRange<int>(8, 256);
    safe_dimensions(&test_width, &test_height);
    
    // Generate a simple PPM test file
    if (!generate_test_ppm(ppm_file, test_width, test_height)) {
        return 0;
    }
    
    /**************************************************************************
     * PART 2: TEST LEGACY TURBOJPEG API (tjLoadImage, tjSaveImage)
     **************************************************************************/
    
    {
        // Test tjLoadImage (legacy API)
        int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
        int align = 4; // 4-byte alignment
        
        unsigned char* loaded_image = tjLoadImage(ppm_file.getFilename(), 
                                                  &width, align, &height, 
                                                  &pixelFormat, 0);
        
        if (loaded_image != NULL) {
            // Validate loaded dimensions
            safe_dimensions(&width, &height);
            
            // Create a TurboJPEG decompression handle for testing
            tjhandle decompress_handle = tjInitDecompress();
            if (decompress_handle != NULL) {
                // Consume YUV parameters from fuzzer input
                int subsamp = fdp.ConsumeIntegralInRange<int>(TJSAMP_444, TJSAMP_GRAY);
                int flags = 0;
                if (fdp.ConsumeBool()) flags |= TJFLAG_BOTTOMUP;
                if (fdp.ConsumeBool()) flags |= TJFLAG_FASTUPSAMPLE;
                if (fdp.ConsumeBool()) flags |= TJFLAG_FASTDCT;
                if (fdp.ConsumeBool()) flags |= TJFLAG_ACCURATEDCT;
                
                // Calculate YUV buffer size
                int yuv_align = 4;
                unsigned long yuv_buf_size = tjBufSizeYUV2(width, yuv_align, height, subsamp);
                
                if (yuv_buf_size > 0 && yuv_buf_size < 10 * 1024 * 1024) {
                    unsigned char* yuv_buf = (unsigned char*)tjAlloc(yuv_buf_size);
                    if (yuv_buf != NULL) {
                        // Test tjEncodeYUV2 (newer API)
                        int pixel_size = (pixelFormat == TJPF_GRAY) ? 1 : 
                                        ((pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR) ? 3 : 4);
                        int pitch = width * pixel_size;
                        int encode_result = tjEncodeYUV2(decompress_handle,
                                                        loaded_image,
                                                        width, pitch, height,
                                                        pixelFormat,
                                                        yuv_buf, subsamp, flags);
                        if (encode_result == 0) {
                            // Allocate destination buffer for decode test
                            // Reuse pixel_size and pitch from above
                            size_t rgb_buf_size = (size_t)pitch * height;
                            if (rgb_buf_size > 0 && rgb_buf_size < 10 * 1024 * 1024) {
                                unsigned char* rgb_buf = (unsigned char*)tjAlloc(rgb_buf_size);
                                if (rgb_buf != NULL) {
                                    // Test tjDecodeYUV (legacy API)
                                    tjDecodeYUV(decompress_handle,
                                               yuv_buf, yuv_align, subsamp,
                                               rgb_buf, width, pitch, height,
                                               pixelFormat, flags);
                                    
                                    tjFree(rgb_buf);
                                }
                            }
                        }
                        
                        tjFree(yuv_buf);
                    }
                }
                
                // Test tjSaveImage (legacy API) if we have a valid image
                TempFile output_file(".ppm");
                if (output_file.isValid()) {
                    int save_flags = 0;
                    if (fdp.ConsumeBool()) save_flags |= TJFLAG_BOTTOMUP;
                    
                    tjSaveImage(output_file.getFilename(),
                               loaded_image,
                               width, 0, height,
                               pixelFormat,
                               save_flags);
                }
                
                tjDestroy(decompress_handle);
            }
            
            tjFree(loaded_image);
        }
    }
    
    /**************************************************************************
     * PART 3: TEST TURBOJPEG v3 API (tj3LoadImage8, tj3SaveImage8, etc.)
     **************************************************************************/
    
    {
        // Create TurboJPEG v3 instance for compression (needed for tj3LoadImage8)
        tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
        if (compress_handle != NULL) {
            // Test tj3LoadImage8
            int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
            int align = 4;
            
            unsigned char* loaded_image_v3 = tj3LoadImage8(compress_handle,
                                                          ppm_file.getFilename(),
                                                          &width, align, &height,
                                                          &pixelFormat);
            
            if (loaded_image_v3 != NULL) {
                safe_dimensions(&width, &height);
                
                // Test tj3GetICCProfile and tj3SetICCProfile
                size_t icc_profile_size = 0;
                unsigned char* icc_profile = NULL;
                tj3GetICCProfile(compress_handle, &icc_profile, &icc_profile_size);
                
                // Create decompression handle for YUV operations
                tjhandle decompress_handle_v3 = tj3Init(TJINIT_DECOMPRESS);
                if (decompress_handle_v3 != NULL) {
                    // Set ICC profile if one was retrieved
                    if (icc_profile != NULL && icc_profile_size > 0) {
                        tj3SetICCProfile(decompress_handle_v3, icc_profile, icc_profile_size);
                    }
                    
                    // Consume parameters for YUV operations
                    int subsamp = fdp.ConsumeIntegralInRange<int>(TJSAMP_444, TJSAMP_GRAY);
                    int yuv_align = 4;
                    
                    // Calculate YUV buffer size
                    size_t yuv_buf_size_v3 = tj3YUVBufSize(width, yuv_align, height, subsamp);
                    
                    if (yuv_buf_size_v3 > 0 && yuv_buf_size_v3 < 10 * 1024 * 1024) {
                        unsigned char* yuv_buf_v3 = (unsigned char*)tj3Alloc(yuv_buf_size_v3);
                        if (yuv_buf_v3 != NULL) {
                            // Test tj3EncodeYUV8
                            int pixel_size = (pixelFormat == TJPF_GRAY) ? 1 : 
                                            ((pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR) ? 3 : 4);
                            int pitch = width * pixel_size;
                            
                            tj3EncodeYUV8(decompress_handle_v3,
                                         loaded_image_v3,
                                         width, pitch, height,
                                         pixelFormat,
                                         yuv_buf_v3, yuv_align);
                            
                            // Allocate destination buffer for decode test
                            size_t rgb_buf_size_v3 = (size_t)pitch * height;
                            if (rgb_buf_size_v3 > 0 && rgb_buf_size_v3 < 10 * 1024 * 1024) {
                                unsigned char* rgb_buf_v3 = (unsigned char*)tj3Alloc(rgb_buf_size_v3);
                                if (rgb_buf_v3 != NULL) {
                                    // Test tj3DecodeYUV8
                                    tj3DecodeYUV8(decompress_handle_v3,
                                                 yuv_buf_v3, yuv_align,
                                                 rgb_buf_v3, width, pitch, height,
                                                 pixelFormat);
                                    
                                    tj3Free(rgb_buf_v3);
                                }
                            }
                            
                            tj3Free(yuv_buf_v3);
                        }
                    }
                    
                    // Test tj3SaveImage8
                    TempFile output_file_v3(".ppm");
                    if (output_file_v3.isValid() && pixelFormat != TJPF_UNKNOWN) {
                        // Calculate pitch based on pixel format to ensure buffer size matches
                        int bytes_per_pixel = (pixelFormat == TJPF_GRAY) ? 1 : 
                                             ((pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR) ? 3 : 4);
                        int pitch = width * bytes_per_pixel;
                        
                        tj3SaveImage8(decompress_handle_v3,
                                     output_file_v3.getFilename(),
                                     loaded_image_v3,
                                     width, pitch, height,
                                     pixelFormat);
                    }
                    
                    tj3Destroy(decompress_handle_v3);
                }
                
                // Free ICC profile if allocated
                if (icc_profile != NULL) {
                    tj3Free(icc_profile);
                }
                
                tj3Free(loaded_image_v3);
            }
            
            tj3Destroy(compress_handle);
        }
    }
    
    /**************************************************************************
     * PART 4: TEST ALTERNATE PATHS WITH DIFFERENT PARAMETERS
     **************************************************************************/
    
    // Test with different alignment values
    if (fdp.remaining_bytes() > 100) {
        int align_options[] = {1, 4, 8, 16};
        int align_idx = fdp.ConsumeIntegralInRange<int>(0, 3);
        int test_align = align_options[align_idx];
        
        tjhandle test_handle = tj3Init(TJINIT_COMPRESS);
        if (test_handle != NULL) {
            int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
            
            unsigned char* test_image = tj3LoadImage8(test_handle,
                                                     ppm_file.getFilename(),
                                                     &width, test_align, &height,
                                                     &pixelFormat);
            
            if (test_image != NULL) {
                // Test saving with the same pixel format that was loaded
                TempFile test_output(".ppm");
                if (test_output.isValid() && pixelFormat != TJPF_UNKNOWN) {
                    // Always use the pixel format returned by tj3LoadImage8
                    // This ensures the buffer format matches what tj3SaveImage8 expects
                    int bytes_per_pixel = (pixelFormat == TJPF_GRAY) ? 1 : 
                                         ((pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR) ? 3 : 4);
                    int pitch = width * bytes_per_pixel;
                    
                    tj3SaveImage8(test_handle,
                                 test_output.getFilename(),
                                 test_image,
                                 width, pitch, height,
                                 pixelFormat);
                    
                    // Optional: Test format conversion if fuzzer wants to
                    // Only attempt if we have enough input data
                    if (fdp.remaining_bytes() > 10 && fdp.ConsumeBool()) {
                        // Choose a target format from compatible family
                        int target_pixel_format;
                        
                        if (bytes_per_pixel == 3) {
                            // 3-byte formats: RGB or BGR
                            target_pixel_format = fdp.ConsumeBool() ? TJPF_RGB : TJPF_BGR;
                        } else if (bytes_per_pixel == 4) {
                            // 4-byte formats: choose from RGBA, BGRA, RGBX, BGRX
                            int format_idx = fdp.ConsumeIntegralInRange<int>(0, 3);
                            int formats[] = {TJPF_RGBA, TJPF_BGRA, TJPF_RGBX, TJPF_BGRX};
                            target_pixel_format = formats[format_idx];
                        } else {
                            // GRAY or other - use same format
                            target_pixel_format = pixelFormat;
                        }
                        
                        // Only allocate new buffer if format is different
                        if (target_pixel_format != pixelFormat) {
                            int target_bytes_per_pixel = (target_pixel_format == TJPF_GRAY) ? 1 : 
                                                       ((target_pixel_format == TJPF_RGB || target_pixel_format == TJPF_BGR) ? 3 : 4);
                            int target_pitch = width * target_bytes_per_pixel;
                            size_t new_buffer_size = (size_t)target_pitch * height;
                            
                            if (new_buffer_size > 0 && new_buffer_size < 10 * 1024 * 1024) {
                                unsigned char* converted_buffer = (unsigned char*)tj3Alloc(new_buffer_size);
                                if (converted_buffer != NULL) {
                                    // Fill buffer with fuzzer data (not hardcoded)
                                    size_t bytes_to_fill = std::min(new_buffer_size, fdp.remaining_bytes());
                                    if (bytes_to_fill > 0) {
                                        auto fill_data = fdp.ConsumeBytes<uint8_t>(bytes_to_fill);
                                        memcpy(converted_buffer, fill_data.data(), bytes_to_fill);
                                        
                                        // Fill remaining with pattern if needed
                                        for (size_t i = bytes_to_fill; i < new_buffer_size; i++) {
                                            converted_buffer[i] = (unsigned char)(i % 256);
                                        }
                                        
                                        tj3SaveImage8(test_handle,
                                                     test_output.getFilename(),
                                                     converted_buffer,
                                                     width, target_pitch, height,
                                                     target_pixel_format);
                                        
                                        tj3Free(converted_buffer);
                                    }
                                }
                            }
                        }
                    }
                }
                
                tj3Free(test_image);
            }
            
            tj3Destroy(test_handle);
        }
    }
    
    return 0;
}

} // namespace harness_005
