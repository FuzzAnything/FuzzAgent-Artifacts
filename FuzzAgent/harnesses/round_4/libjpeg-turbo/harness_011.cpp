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

namespace harness_011 {
/*
 * Fuzzing harness for libjpeg-turbo targeting comprehensive YUV decompression pipeline
 * as identified by coverage analysis. Coverage analysis shows critical gap in 
 * YUV decompression pipeline in turbojpeg.c with 557+ undiscovered branches.
 * 
 * Target APIs (77.4% uncovered):
 * - tjDecompressToYUV: Decompress JPEG to planar YUV (legacy API)
 * - tjDecompressToYUV2: Decompress JPEG to planar YUV with alignment control
 * - tj3DecompressToYUV8: Decompress JPEG to planar YUV (v3 API with alignment)
 * - tjSaveImage: Save image buffer to file
 * - tjLoadImage: Load image from file to buffer  
 * - tjDecodeYUVPlanes: Decode separate YUV planes to RGB
 * 
 * Supporting APIs:
 * - tjInitDecompress, tjInitCompress, tjDestroy, tjAlloc, tjFree
 * - tj3InitVersion, tj3Destroy, tj3Alloc, tj3Free
 * - tjDecompressHeader3, tj3Get, tj3Set
 * - tjBufSizeYUV2, tj3YUVBufSize
 * 
 * Workflow for YUV decompression pipeline:
 * 1. Initialize decompressor instances (legacy and v3)
 * 2. Parse JPEG header to get dimensions and subsampling
 * 3. Decompress JPEG to YUV using various APIs and alignments
 * 4. Optionally decode YUV back to RGB using tjDecodeYUVPlanes
 * 5. Test file I/O operations with YUV/RGB buffers
 * 6. Test edge cases: different subsampling, alignments, dimensions
 * 
 * Semantic diversity: This harness uniquely focuses on the complete YUV decompression
 * pipeline with comprehensive testing of:
 * - All YUV subsampling modes (4:4:4, 4:2:2, 4:2:0, 4:4:0, 4:1:1, grayscale)
 * - Multiple alignment values (1, 4, 8, 16, 32)
 * - Both legacy (tj) and v3 (tj3) APIs
 * - Planar vs unified YUV buffer formats
 * - File I/O integration with YUV operations
 * - Error handling and validation paths
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
    
    // Ensure at least 8x8 for YUV compliance
    if (*width < 8) *width = 8;
    if (*height < 8) *height = 8;
}

// Helper to get valid subsampling from fuzzer input
static int get_valid_subsamp(FuzzedDataProvider& fdp) {
    int subsamp = fdp.ConsumeIntegralInRange<int>(0, TJ_NUMSAMP - 1);
    // Map to valid subsampling values
    switch (subsamp) {
        case TJSAMP_444:
        case TJSAMP_422:
        case TJSAMP_420:
        case TJSAMP_GRAY:
        case TJSAMP_440:
        case TJSAMP_411:
            return subsamp;
        default:
            return TJSAMP_420; // Default to 4:2:0
    }
}

// Helper to get valid pixel format
static int get_valid_pixel_format(FuzzedDataProvider& fdp) {
    int pf = fdp.ConsumeIntegralInRange<int>(0, 12);
    switch (pf) {
        case TJPF_RGB:
        case TJPF_BGR:
        case TJPF_RGBX:
        case TJPF_BGRX:
        case TJPF_XRGB:
        case TJPF_XBGR:
        case TJPF_GRAY:
        case TJPF_RGBA:
        case TJPF_BGRA:
        case TJPF_ABGR:
        case TJPF_ARGB:
        case TJPF_CMYK:
            return pf;
        default:
            return TJPF_RGB;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for JPEG header and parameters
    if (size < 300) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    /**************************************************************************
     * PART 1: LEGACY TURBOJPEG API - COMPREHENSIVE YUV DECOMPRESSION
     * Test tjDecompressToYUV, tjDecompressToYUV2 with various configurations
     **************************************************************************/
    {
        // Initialize legacy decompressor
        tjhandle legacy_decompress = tjInitDecompress();
        if (legacy_decompress == NULL) {
            // Skip if initialization fails
            goto v3_api_test;
        }
        
        // Consume JPEG data from input (first 200 bytes or remaining)
        size_t jpeg_data_size = fdp.ConsumeIntegralInRange<size_t>(200, fdp.remaining_bytes());
        std::vector<uint8_t> jpeg_buffer = fdp.ConsumeBytes<uint8_t>(jpeg_data_size);
        
        if (jpeg_buffer.size() < 100) {
            tjDestroy(legacy_decompress);
            goto v3_api_test;
        }
        
        // Parse JPEG header to get dimensions and subsampling
        int width = 0, height = 0, jpeg_subsamp = 0, jpeg_colorspace = 0;
        int header_result = tjDecompressHeader3(legacy_decompress,
                                               jpeg_buffer.data(),
                                               jpeg_buffer.size(),
                                               &width, &height,
                                               &jpeg_subsamp,
                                               &jpeg_colorspace);
        // If header parsing fails, try with estimated dimensions
        if (header_result != 0) {
            // Use fuzzer-provided dimensions as fallback
            width = fdp.ConsumeIntegralInRange<int>(8, MAX_WIDTH);
            height = fdp.ConsumeIntegralInRange<int>(8, MAX_HEIGHT);
            safe_dimensions(&width, &height);
            jpeg_subsamp = get_valid_subsamp(fdp);
        }
        
        // Test YUV decompression with various alignments
        for (int align : {1, 4, 8, 16, 32}) {
            // Check if align is power of 2 manually
            if (align < 1 || (align & (align - 1)) != 0) continue; // Alignment must be power of 2
            
            // Calculate YUV buffer size
            unsigned long yuv_size = tjBufSizeYUV2(width, align, height, jpeg_subsamp);
            if (yuv_size == 0 || yuv_size > 10 * 1024 * 1024) {
                continue; // Skip if size is invalid or too large
            }
            
            // Allocate YUV buffer
            unsigned char* yuv_buf = (unsigned char*)tjAlloc(yuv_size);
            if (yuv_buf == NULL) {
                continue;
            }
            
            // Test tjDecompressToYUV2 (more flexible API)
            int yuv_result = tjDecompressToYUV2(legacy_decompress,
                                               jpeg_buffer.data(),
                                               jpeg_buffer.size(),
                                               yuv_buf,
                                               width, align, height,
                                               0); // flags
            
            // If tjDecompressToYUV2 succeeds, also test tjDecompressToYUV
            if (yuv_result == 0) {
                // Test tjDecompressToYUV (simpler API, uses default alignment 4)
                unsigned long yuv_size_simple = tjBufSizeYUV2(width, 4, height, jpeg_subsamp);
                if (yuv_size_simple > 0 && yuv_size_simple <= 10 * 1024 * 1024) {
                    unsigned char* yuv_buf_simple = (unsigned char*)tjAlloc(yuv_size_simple);
                    if (yuv_buf_simple != NULL) {
                        int simple_result = tjDecompressToYUV(legacy_decompress,
                                                             jpeg_buffer.data(),
                                                             jpeg_buffer.size(),
                                                             yuv_buf_simple,
                                                             0); // flags
                        // Result is not checked - we're testing API coverage
                        tjFree(yuv_buf_simple);
                    }
                }
                
                // Test tjDecodeYUVPlanes to convert YUV back to RGB
                // First need to extract separate planes from unified buffer
                int plane_width = width;
                int plane_height = height;
                if (jpeg_subsamp != TJSAMP_GRAY && jpeg_subsamp != TJSAMP_444) {
                    // Calculate plane dimensions based on subsampling
                    plane_width = tjPlaneWidth(0, width, jpeg_subsamp);
                    plane_height = tjPlaneHeight(0, height, jpeg_subsamp);
                }
                
                // Allocate RGB buffer for decoding
                int pixel_format = get_valid_pixel_format(fdp);
                int pixel_size = 3; // Default for RGB
                if (pixel_format == TJPF_GRAY) pixel_size = 1;
                else if (pixel_format == TJPF_CMYK) pixel_size = 4;
                else if (pixel_format >= TJPF_RGBX && pixel_format <= TJPF_ARGB) pixel_size = 4;
                
                size_t rgb_size = (size_t)width * height * pixel_size;
                if (rgb_size > 0 && rgb_size < 10 * 1024 * 1024) {
                    unsigned char* rgb_buf = (unsigned char*)tjAlloc(rgb_size);
                    if (rgb_buf != NULL) {
                        // For unified buffer, we need to create plane pointers
                        // Calculate plane offsets in unified buffer
                        size_t y_plane_size = (size_t)plane_width * plane_height;
                        size_t uv_plane_size = 0;
                        
                        if (jpeg_subsamp != TJSAMP_GRAY) {
                            uv_plane_size = tjPlaneSizeYUV(1, width, plane_width, height, jpeg_subsamp);
                        }
                        
                        if (y_plane_size + 2 * uv_plane_size <= yuv_size) {
                            const unsigned char* planes[3] = {NULL, NULL, NULL};
                            int strides[3] = {0, 0, 0};
                            
                            planes[0] = yuv_buf; // Y plane
                            strides[0] = plane_width;
                            
                            if (jpeg_subsamp != TJSAMP_GRAY && uv_plane_size > 0) {
                                planes[1] = yuv_buf + y_plane_size; // U/Cb plane
                                planes[2] = planes[1] + uv_plane_size; // V/Cr plane
                                strides[1] = strides[2] = plane_width / 2;
                            }
                            
                            int decode_result = tjDecodeYUVPlanes(legacy_decompress,
                                                                 planes,
                                                                 strides,
                                                                 jpeg_subsamp,
                                                                 rgb_buf,
                                                                 width, width * pixel_size, height,
                                                                 pixel_format,
                                                                 0); // flags
                            // Result not checked - testing API coverage
                        }
                        
                        tjFree(rgb_buf);
                    }
                }
            }
            
            tjFree(yuv_buf);
        }
        
        // Clean up legacy decompressor
        tjDestroy(legacy_decompress);
    }

v3_api_test:
    /**************************************************************************
     * PART 2: TURBOJPEG V3 API - MODERN YUV DECOMPRESSION
     * Test tj3DecompressToYUV8 with parameter configuration
     **************************************************************************/
    {
        // Initialize v3 decompressor
        tjhandle v3_decompress = tj3InitVersion(TJINIT_DECOMPRESS, TURBOJPEG_VERSION_NUMBER);
        if (v3_decompress == NULL) {
            goto file_io_test;
        }
        
        // Consume more JPEG data for v3 testing
        if (fdp.remaining_bytes() < 200) {
            tj3Destroy(v3_decompress);
            goto file_io_test;
        }
        
        size_t jpeg_data_size_v3 = fdp.ConsumeIntegralInRange<size_t>(200, fdp.remaining_bytes());
        std::vector<uint8_t> jpeg_buffer_v3 = fdp.ConsumeBytes<uint8_t>(jpeg_data_size_v3);
        
        if (jpeg_buffer_v3.size() < 100) {
            tj3Destroy(v3_decompress);
            goto file_io_test;
        }
        
        // Set parameters for v3 decompressor
        tj3Set(v3_decompress, TJPARAM_STOPONWARNING, fdp.ConsumeBool() ? 1 : 0);
        
        // Get JPEG info using v3 API
        if (tj3DecompressHeader(v3_decompress, jpeg_buffer_v3.data(), jpeg_buffer_v3.size()) == 0) {
            int width = tj3Get(v3_decompress, TJPARAM_JPEGWIDTH);
            int height = tj3Get(v3_decompress, TJPARAM_JPEGHEIGHT);
            int subsamp = tj3Get(v3_decompress, TJPARAM_SUBSAMP);
            
            safe_dimensions(&width, &height);
            // Test tj3DecompressToYUV8 with various alignments
            for (int align : {1, 4, 8, 16, 32}) {
                // Check if align is power of 2 manually
                if (align < 1 || (align & (align - 1)) != 0) continue;
                
                // Calculate YUV buffer size using v3 API
                size_t yuv_size_v3 = tj3YUVBufSize(width, align, height, subsamp);
                if (yuv_size_v3 == 0 || yuv_size_v3 > 10 * 1024 * 1024) {
                    continue;
                }
                
                // Allocate YUV buffer using v3 allocator
                unsigned char* yuv_buf_v3 = (unsigned char*)tj3Alloc(yuv_size_v3);
                if (yuv_buf_v3 == NULL) {
                    continue;
                }
                
                // Test tj3DecompressToYUV8
                int yuv_result_v3 = tj3DecompressToYUV8(v3_decompress,
                                                       jpeg_buffer_v3.data(),
                                                       jpeg_buffer_v3.size(),
                                                       yuv_buf_v3,
                                                       align);
                
                // If successful, test file I/O with the YUV buffer
                if (yuv_result_v3 == 0 && fdp.ConsumeBool()) {
                    // Create a temporary file for testing
                    TempFile yuv_file(".yuv");
                    if (yuv_file.isValid()) {
                        // Write YUV data to file
                        if (yuv_file.writeData(yuv_buf_v3, yuv_size_v3)) {
                            // Test could reload and verify, but minimal test for now
                        }
                    }
                }
                
                tj3Free(yuv_buf_v3);
            }
        }
        tj3Destroy(v3_decompress);
    }

file_io_test:
    /**************************************************************************
     * PART 3: FILE I/O OPERATIONS WITH YUV/RGB BUFFERS
     * Test tjLoadImage and tjSaveImage with synthetic image data
     **************************************************************************/
    if (fdp.remaining_bytes() > 500) {
        // Create synthetic image data for file I/O testing
        int width = fdp.ConsumeIntegralInRange<int>(16, 256);
        int height = fdp.ConsumeIntegralInRange<int>(16, 256);
        safe_dimensions(&width, &height);
        
        int pixel_format = get_valid_pixel_format(fdp);
        int pixel_size = 3;
        if (pixel_format == TJPF_GRAY) pixel_size = 1;
        else if (pixel_format == TJPF_CMYK) pixel_size = 4;
        else if (pixel_format >= TJPF_RGBX && pixel_format <= TJPF_ARGB) pixel_size = 4;
        
        size_t image_size = (size_t)width * height * pixel_size;
        if (image_size > 0 && image_size < 5 * 1024 * 1024 && 
            fdp.remaining_bytes() >= image_size) {
            
            std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
            
            // Create temporary file
            TempFile image_file(".raw");
            if (image_file.isValid() && image_file.writeData(image_data.data(), image_data.size())) {
                // Test tjLoadImage
                int loaded_width = 0, loaded_height = 0, loaded_pixel_format = 0;
                unsigned char* loaded_buffer = tjLoadImage(image_file.getFilename(),
                                                          &loaded_width, 4, &loaded_height,
                                                          &loaded_pixel_format,
                                                          0); // flags
                
                if (loaded_buffer != NULL) {
                    // Test tjSaveImage with loaded buffer
                    TempFile saved_file(".saved");
                    if (saved_file.isValid()) {
                        int save_result = tjSaveImage(saved_file.getFilename(),
                                                     loaded_buffer,
                                                     loaded_width,
                                                     loaded_width * pixel_size,
                                                     loaded_height,
                                                     loaded_pixel_format,
                                                     0); // flags
                        // Result not checked - testing API coverage
                    }
                    
                    tjFree(loaded_buffer);
                }
            }
        }
    }

    /**************************************************************************
     * PART 4: ADDITIONAL YUV PIPELINE TESTS
     * Test edge cases and error conditions
     **************************************************************************/
    if (fdp.remaining_bytes() > 100) {
        // Test with minimal valid JPEG data
        size_t min_jpeg_size = fdp.ConsumeIntegralInRange<size_t>(100, fdp.remaining_bytes());
        std::vector<uint8_t> min_jpeg = fdp.ConsumeBytes<uint8_t>(min_jpeg_size);
        
        if (min_jpeg.size() >= 100) {
            // Quick test with legacy API
            tjhandle quick_decompress = tjInitDecompress();
            if (quick_decompress != NULL) {
                int quick_width = 64, quick_height = 64;
                safe_dimensions(&quick_width, &quick_height);
                
                int quick_align = 4;
                unsigned long quick_yuv_size = tjBufSizeYUV2(quick_width, quick_align, quick_height, TJSAMP_420);
                if (quick_yuv_size > 0 && quick_yuv_size < 1024 * 1024) {
                    unsigned char* quick_yuv = (unsigned char*)tjAlloc(quick_yuv_size);
                    if (quick_yuv != NULL) {
                        // Try decompression (may fail, but tests error paths)
                        tjDecompressToYUV2(quick_decompress,
                                          min_jpeg.data(),
                                          min_jpeg.size(),
                                          quick_yuv,
                                          quick_width, quick_align, quick_height,
                                          0);
                        tjFree(quick_yuv);
                    }
                }
                tjDestroy(quick_decompress);
            }
        }
    }

    return 0;
}

} // namespace harness_011
