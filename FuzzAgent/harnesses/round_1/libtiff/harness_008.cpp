#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tiffio.h"

namespace harness_008 {
/*
 * Fuzzing harness for libtiff RGBA strip reading operations with YCbCr color conversion
 * Target APIs: TIFFReadRGBAStrip, TIFFReadRGBAStripExt (primary targets)
 * Required helper APIs: TIFFOpen/TIFFOpenExt, TIFFSetField, TIFFClose, _TIFFmalloc, _TIFFfree
 * 
 * This harness focuses specifically on RGBA strip reading with various image configurations,
 * particularly targeting YCbCr color conversion logic which has 122+ undiscovered branches.
 * 
 * Semantic differentiation from previous harnesses: 
 * - Focuses specifically on RGBA strip reading (not tile reading)
 * - Targets YCbCr color conversion path which is complex and under-tested
 * - Uses _TIFFmalloc/_TIFFfree for proper libtiff memory management
 * - Tests various image configurations (dimensions, samples, bits, compression)
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various parameters and buffer data
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Create a temporary file for TIFF operations
    char temp_file_template[] = "/tmp/fuzz_tiff_rgba_XXXXXX";
    int fd = mkstemp(temp_file_template);
    if (fd < 0) {
        return 0;
    }
    std::string temp_filename = temp_file_template;
    
    // Write fuzzed data to the temporary file (simulating TIFF data)
    std::vector<uint8_t> file_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (write(fd, file_data.data(), file_data.size()) != (ssize_t)file_data.size()) {
        close(fd);
        unlink(temp_filename.c_str());
        return 0;
    }
    close(fd);

    // Phase 1: Open TIFF file with different methods
    TIFF* tif = nullptr;
    TIFFOpenOptions* opts = nullptr;
    
    uint8_t open_method = fdp.ConsumeIntegral<uint8_t>() % 3;
    std::string mode = "r"; // Read mode for RGBA reading
    
    switch (open_method) {
        case 0:
            // Standard TIFFOpen
            tif = TIFFOpen(temp_filename.c_str(), mode.c_str());
            break;
            
        case 1:
            // TIFFOpenExt with options
            opts = TIFFOpenOptionsAlloc();
            if (opts) {
                // Configure some options if needed
                tif = TIFFOpenExt(temp_filename.c_str(), mode.c_str(), opts);
                TIFFOpenOptionsFree(opts);
            }
            break;
            
        case 2:
            // TIFFFdOpen
            fd = open(temp_filename.c_str(), O_RDONLY);
            if (fd >= 0) {
                tif = TIFFFdOpen(fd, temp_filename.c_str(), mode.c_str());
                // TIFFFdOpen duplicates the fd, so we can close ours
                close(fd);
            }
            break;
    }
    
    // Clean up temp file
    unlink(temp_filename.c_str());
    
    // If TIFF was not successfully opened, return early
    if (!tif) {
        return 0;
    }
    
    // Phase 2: Configure TIFF image parameters for RGBA strip reading
    // We need to set up image dimensions and other parameters
    // Note: For reading existing TIFF files, these tags should already be set
    // But we'll try to reconfigure to test different scenarios
    
    // Get current image dimensions (if any)
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    // If image dimensions are not set or are invalid, set reasonable defaults
    if (width == 0 || height == 0) {
        width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
        height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
        
        // Try to set image dimensions (may fail for read-only files)
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    }
    
    // Configure photometric interpretation - focus on YCbCr for coverage
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 11; // 0-10 photometric types
    // Force YCbCr sometimes to ensure we hit that path
    if (fdp.ConsumeBool()) {
        photometric = PHOTOMETRIC_YCBCR; // 6
    }
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    
    // Configure samples per pixel
    uint16_t samples_per_pixel = 0;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    if (samples_per_pixel == 0) {
        samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    }
    
    // Configure bits per sample
    uint16_t bits_per_sample = 0;
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    if (bits_per_sample == 0) {
        bits_per_sample = fdp.ConsumeIntegral<uint16_t>() % 2 == 0 ? 8 : 16;
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    }
    
    // Configure rows per strip (critical for strip reading)
    uint32_t rows_per_strip = 0;
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    if (rows_per_strip == 0 || rows_per_strip > height) {
        rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, height > 0 ? height : 16);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    }
    
    // Configure compression
    uint16_t compression = 0;
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    if (compression == 0) {
        // Pick from common compression types
        uint16_t compressions[] = {
            COMPRESSION_NONE,
            COMPRESSION_LZW,
            COMPRESSION_PACKBITS,
            COMPRESSION_DEFLATE,
            COMPRESSION_JPEG
        };
        compression = compressions[fdp.ConsumeIntegralInRange<size_t>(0, 4)];
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    }
    
    // For YCbCr, set subsampling if needed
    if (photometric == PHOTOMETRIC_YCBCR) {
        uint16_t ycbcr_subsampling[] = {1, 1}; // Default 1x1
        if (fdp.ConsumeBool()) {
            ycbcr_subsampling[0] = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
            ycbcr_subsampling[1] = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
        }
        TIFFSetField(tif, TIFFTAG_YCBCRSUBSAMPLING, ycbcr_subsampling[0], ycbcr_subsampling[1]);
    }
    
    // Phase 3: Allocate RGBA buffer for strip reading
    uint32_t* rgba_buffer = nullptr;
    size_t buffer_size = 0;
    
    if (width > 0 && rows_per_strip > 0) {
        // Buffer size: width * rows_per_strip * 4 bytes (RGBA)
        buffer_size = width * rows_per_strip * 4;
        
        // Use _TIFFmalloc for proper libtiff memory allocation
        rgba_buffer = (uint32_t*)_TIFFmalloc(buffer_size);
    }
    
    if (rgba_buffer) {
        // Phase 4: Test RGBA strip reading operations
        uint8_t strip_op = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        // Determine valid row indices
        uint32_t valid_row = 0;
        if (height > 0) {
            valid_row = fdp.ConsumeIntegralInRange<uint32_t>(0, height - 1);
        }
        
        // Also test with invalid row indices to cover error paths
        uint32_t invalid_row = height + fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
        
        switch (strip_op) {
            case 0:
                // Normal TIFFReadRGBAStrip with valid row
                if (height > 0) {
                    TIFFReadRGBAStrip(tif, valid_row, rgba_buffer);
                }
                break;
                
            case 1:
                // TIFFReadRGBAStrip with invalid row (should handle gracefully)
                TIFFReadRGBAStrip(tif, invalid_row, rgba_buffer);
                break;
                
            case 2:
                // TIFFReadRGBAStripExt with valid row and stop_on_error flag
                if (height > 0) {
                    int stop_on_error = fdp.ConsumeBool() ? 1 : 0;
                    TIFFReadRGBAStripExt(tif, valid_row, rgba_buffer, stop_on_error);
                }
                break;
                
            case 3:
                // TIFFReadRGBAStripExt with invalid row
                int stop_on_error = fdp.ConsumeBool() ? 1 : 0;
                TIFFReadRGBAStripExt(tif, invalid_row, rgba_buffer, stop_on_error);
                break;
        }
        
        // Phase 5: Test error conditions
        // Test with NULL buffer
        if (fdp.ConsumeBool()) {
            TIFFReadRGBAStrip(tif, valid_row, nullptr);
        }
        
        // Test with zero-sized buffer (should be handled by libtiff)
        // Note: _TIFFmalloc(0) may return NULL or non-NULL, handle both
        
        // Clean up buffer
        _TIFFfree(rgba_buffer);
    }
    
    // Phase 6: Clean up
    TIFFClose(tif);
    
    return 0;
}

} // namespace harness_008
