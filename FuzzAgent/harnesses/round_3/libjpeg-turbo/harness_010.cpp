#include <fuzzer/FuzzedDataProvider.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include "jpeglib.h"
#include "jerror.h"

namespace harness_010 {
/*
 * Fuzzing harness for libjpeg-turbo traditional file-based I/O APIs
 * This harness targets completely uncovered traditional libjpeg file I/O operations:
 * - jpeg_stdio_dest: File-based output destination (0% coverage, 4 undiscovered branches)
 * - jpeg_stdio_src: File-based input source (0% coverage, 4 undiscovered branches)
 * - jpeg_write_tables: Quantization table writing (0% coverage, 2 undiscovered branches)
 * - jpeg_resync_to_restart: Error recovery (0% coverage, 24 undiscovered branches)
 * 
 * Required Helper APIs:
 * - jpeg_CreateCompress/jpeg_CreateDecompress: Object initialization
 * - jpeg_destroy_compress/jpeg_destroy_decompress: Cleanup
 * - jpeg_set_defaults: Default configuration
 * - jpeg_set_quality: Quality setting
 * - jpeg_start_compress/jpeg_start_decompress: Operation start
 * - jpeg_finish_compress/jpeg_finish_decompress: Operation completion
 * 
 * Differentiates from existing harnesses (000-009):
 * - harness_000.cpp: Modern tj3* TurboJPEG APIs
 * - harness_001.cpp: Traditional tj* TurboJPEG APIs  
 * - harness_002.cpp: Modern tj3* YUV APIs
 * - harness_003.cpp: 12/16-bit precision APIs
 * - harness_004.cpp: Traditional tj* YUV operations
 * - harness_005.cpp: High-bit-depth image I/O
 * - harness_006.cpp: Traditional YUV encoding
 * - harness_007.cpp: Traditional libjpeg memory I/O APIs (jpeg_mem_src/jpeg_mem_dest)
 * - harness_008.cpp: Traditional libjpeg 12-bit APIs
 * - harness_009.cpp: Old turbojpeg YUV decompression APIs
 * 
 * Strategy: Implement full traditional libjpeg file-based compression/decompression lifecycle
 * with temporary files, error injection, quantization table writing, and restart marker recovery
 * to exercise foundational file I/O modules currently at 0% coverage.
 * 
 * Expected Coverage Gain: 34+ undiscovered branches in critical file I/O path
 */


// Include traditional libjpeg headers
extern "C" {
}

// Create temporary file and return FILE* handle
static FILE* create_temp_file(const char* prefix, char** temp_path) {
    char template_path[] = "/tmp/jpeg_fuzz_XXXXXX";
    int fd = mkstemp(template_path);
    if (fd < 0) {
        return nullptr;
    }
    
    FILE* fp = fdopen(fd, "w+b");
    if (!fp) {
        close(fd);
        unlink(template_path);
        return nullptr;
    }
    
    *temp_path = strdup(template_path);
    return fp;
}

// Clean up temporary file
static void cleanup_temp_file(FILE* fp, const char* temp_path) {
    if (fp) {
        fclose(fp);
    }
    if (temp_path) {
        unlink(temp_path);
        free((void*)temp_path);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful JPEG operations
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // ========== PHASE 1: COMPRESSION WITH FILE DESTINATION ==========
    
    struct jpeg_compress_struct cinfo_compress;
    struct jpeg_error_mgr jerr_compress;
    
    // Initialize compression object
    cinfo_compress.err = jpeg_std_error(&jerr_compress);
    jpeg_CreateCompress(&cinfo_compress, JPEG_LIB_VERSION, sizeof(struct jpeg_compress_struct));
    
    // Consume compression parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(8, 256);
    int height = fdp.ConsumeIntegralInRange<int>(8, 256);
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    boolean progressive = fdp.ConsumeBool();
    boolean write_tables = fdp.ConsumeBool();
    int color_space_idx = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Set image dimensions
    cinfo_compress.image_width = width;
    cinfo_compress.image_height = height;
    cinfo_compress.input_components = 3; // RGB
    cinfo_compress.in_color_space = JCS_RGB;
    
    // Set default parameters
    jpeg_set_defaults(&cinfo_compress);
    
    // Set quality
    jpeg_set_quality(&cinfo_compress, quality, TRUE /* force_baseline */);
    
    // Set color space based on fuzzer input
    J_COLOR_SPACE color_spaces[] = {JCS_RGB, JCS_GRAYSCALE, JCS_YCbCr, JCS_CMYK, JCS_YCCK};
    cinfo_compress.in_color_space = color_spaces[color_space_idx % 5];
    
    // Adjust input components based on color space
    if (cinfo_compress.in_color_space == JCS_GRAYSCALE) {
        cinfo_compress.input_components = 1;
    } else if (cinfo_compress.in_color_space == JCS_CMYK || cinfo_compress.in_color_space == JCS_YCCK) {
        cinfo_compress.input_components = 4;
    }
    
    // Set progressive mode if requested
    if (progressive) {
        jpeg_simple_progression(&cinfo_compress);
    }
    
    // Create temporary output file for compression
    char* temp_output_path = nullptr;
    FILE* temp_output_file = create_temp_file("jpeg_out", &temp_output_path);
    if (!temp_output_file) {
        jpeg_destroy_compress(&cinfo_compress);
        return 0;
    }
    
    // Setup file destination using jpeg_stdio_dest
    jpeg_stdio_dest(&cinfo_compress, temp_output_file);
    
    // Write quantization tables if requested (using jpeg_write_tables)
    if (write_tables) {
        // Note: jpeg_write_tables writes tables to the current destination
        // This is typically used for creating separate table files
        jpeg_write_tables(&cinfo_compress);
        
        // After writing tables, we need to reset for actual image compression
        // We'll close and reopen the file for simplicity
        fclose(temp_output_file);
        temp_output_file = fopen(temp_output_path, "ab");  // Append binary mode
        if (!temp_output_file) {
            free(temp_output_path);
            jpeg_destroy_compress(&cinfo_compress);
            return 0;
        }
        
        // Re-setup destination
        jpeg_stdio_dest(&cinfo_compress, temp_output_file);
    }
    
    // Start compression
    jpeg_start_compress(&cinfo_compress, TRUE);
    
    // Generate synthetic scanline data from fuzzer input
    size_t scanline_size = (size_t)width * cinfo_compress.input_components;
    if (scanline_size == 0) {
        cleanup_temp_file(temp_output_file, temp_output_path);
        jpeg_destroy_compress(&cinfo_compress);
        return 0;
    }
    
    std::vector<JSAMPLE> scanline(scanline_size);
    
    // Write scanlines using remaining fuzzer data
    size_t scanlines_to_write = std::min((size_t)height, fdp.remaining_bytes() / scanline_size);
    for (size_t i = 0; i < scanlines_to_write; i++) {
        auto scanline_data = fdp.ConsumeBytes<JSAMPLE>(scanline_size);
        if (scanline_data.size() == scanline_size) {
            JSAMPROW row_pointer[1];
            row_pointer[0] = scanline_data.data();
            jpeg_write_scanlines(&cinfo_compress, row_pointer, 1);
        } else {
            // Use zeros for remaining scanlines
            memset(scanline.data(), 0, scanline_size * sizeof(JSAMPLE));
            JSAMPROW row_pointer[1];
            row_pointer[0] = scanline.data();
            jpeg_write_scanlines(&cinfo_compress, row_pointer, 1);
        }
    }
    
    // Finish compression
    jpeg_finish_compress(&cinfo_compress);
    
    // Close output file and get file size
    long compressed_size = ftell(temp_output_file);
    fclose(temp_output_file);
    temp_output_file = nullptr;
    
    // Clean up compression object
    jpeg_destroy_compress(&cinfo_compress);
    
    // ========== PHASE 2: DECOMPRESSION WITH FILE SOURCE ==========
    
    // Reopen the compressed file for reading
    FILE* temp_input_file = fopen(temp_output_path, "rb");
    if (!temp_input_file || compressed_size <= 0) {
        cleanup_temp_file(nullptr, temp_output_path);
        return 0;
    }
    
    struct jpeg_decompress_struct cinfo_decompress;
    struct jpeg_error_mgr jerr_decompress;
    
    // Initialize decompression object
    cinfo_decompress.err = jpeg_std_error(&jerr_decompress);
    jpeg_CreateDecompress(&cinfo_decompress, JPEG_LIB_VERSION, sizeof(struct jpeg_decompress_struct));
    
    // Setup file source using jpeg_stdio_src
    jpeg_stdio_src(&cinfo_decompress, temp_input_file);
    
    // Read header
    int ret = jpeg_read_header(&cinfo_decompress, TRUE);
    if (ret != JPEG_HEADER_OK) {
        cleanup_temp_file(temp_input_file, temp_output_path);
        jpeg_destroy_decompress(&cinfo_decompress);
        return 0;
    }
    
    // Start decompression
    jpeg_start_decompress(&cinfo_decompress);
    
    // Allocate buffer for decompressed scanlines
    size_t output_scanline_size = (size_t)cinfo_decompress.output_width * cinfo_decompress.output_components;
    if (output_scanline_size == 0) {
        cleanup_temp_file(temp_input_file, temp_output_path);
        jpeg_destroy_decompress(&cinfo_decompress);
        return 0;
    }
    
    std::vector<JSAMPLE> output_buffer(output_scanline_size);
    
    // Read scanlines (simulate reading but don't store)
    while (cinfo_decompress.output_scanline < cinfo_decompress.output_height) {
        JSAMPROW row_pointer[1];
        row_pointer[0] = output_buffer.data();
        jpeg_read_scanlines(&cinfo_decompress, row_pointer, 1);
    }
    
    // Finish decompression
    jpeg_finish_decompress(&cinfo_decompress);
    
    // Test error recovery with jpeg_resync_to_restart
    // This function is called internally during error recovery
    // We can test it by attempting to read corrupted data
    
    // Create a corrupted version of the file
    char* temp_corrupted_path = nullptr;
    FILE* temp_corrupted_file = create_temp_file("jpeg_corrupt", &temp_corrupted_path);
    if (temp_corrupted_file) {
        // Write original compressed data
        fseek(temp_input_file, 0, SEEK_SET);
        char buffer[4096];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), temp_input_file)) > 0) {
            fwrite(buffer, 1, bytes_read, temp_corrupted_file);
        }
        
        // Corrupt some bytes in the middle
        fseek(temp_corrupted_file, compressed_size / 2, SEEK_SET);
        uint8_t corrupt_byte = fdp.ConsumeIntegral<uint8_t>();
        fputc(corrupt_byte, temp_corrupted_file);
        fflush(temp_corrupted_file);
        
        // Try to decompress corrupted file
        fclose(temp_corrupted_file);
        temp_corrupted_file = fopen(temp_corrupted_path, "rb");
        
        if (temp_corrupted_file) {
            struct jpeg_decompress_struct cinfo_corrupt;
            struct jpeg_error_mgr jerr_corrupt;
            
            cinfo_corrupt.err = jpeg_std_error(&jerr_corrupt);
            jpeg_CreateDecompress(&cinfo_corrupt, JPEG_LIB_VERSION, sizeof(struct jpeg_decompress_struct));
            
            jpeg_stdio_src(&cinfo_corrupt, temp_corrupted_file);
            
            // Try to read header (may fail or succeed depending on corruption)
            jpeg_read_header(&cinfo_corrupt, TRUE);
            
            // Try to start decompression (will trigger error recovery if corrupted)
            // jpeg_resync_to_restart is called internally during error recovery
            jpeg_start_decompress(&cinfo_corrupt);
            
            // Attempt to read some scanlines (may trigger resync)
            std::vector<JSAMPLE> corrupt_buffer(output_scanline_size);
            int max_attempts = std::min(10, (int)cinfo_corrupt.output_height);
            for (int i = 0; i < max_attempts && cinfo_corrupt.output_scanline < cinfo_corrupt.output_height; i++) {
                JSAMPROW row_pointer[1];
                row_pointer[0] = corrupt_buffer.data();
                jpeg_read_scanlines(&cinfo_corrupt, row_pointer, 1);
            }
            
            // Clean up
            jpeg_destroy_decompress(&cinfo_corrupt);
            fclose(temp_corrupted_file);
            cleanup_temp_file(nullptr, temp_corrupted_path);
        }
    }
    
    // ========== PHASE 3: CLEANUP ==========
    
    cleanup_temp_file(temp_input_file, temp_output_path);
    jpeg_destroy_decompress(&cinfo_decompress);
    
    return 0;
}

} // namespace harness_010
