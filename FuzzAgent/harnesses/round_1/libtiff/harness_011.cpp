#include <fuzzer/FuzzedDataProvider.h>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tiffio.h"
#include "tiff.h"

namespace harness_011 {
/*
 * Fuzzing harness for libtiff strip size calculation functions with YCbCr special case testing
 * Target APIs: TIFFRawStripSize, TIFFRawStripSize64, TIFFStripSize, TIFFStripSize64, 
 *              TIFFVStripSize, TIFFVStripSize64, TIFFComputeStrip, TIFFNumberOfStrips (primary targets)
 * Required helper APIs: TIFFOpen/TIFFOpenExt, TIFFSetField, TIFFClose, TIFFGetStrileByteCount
 * 
 * This harness focuses specifically on strip size calculation functions that are completely uncovered:
 * 1. TIFFRawStripSize - 0% covered (105+ undiscovered branches)
 * 2. TIFFRawStripSize64 - 0% covered 
 * 3. TIFFStripSize - 0% covered
 * 4. TIFFStripSize64 - 0% covered
 * 5. TIFFVStripSize - 0% covered
 * 6. TIFFVStripSize64 - 0% covered
 * 7. TIFFComputeStrip - 0% covered
 * 8. TIFFNumberOfStrips - 0% covered
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_009 focuses on tile size calculation functions
 * - harness_010 focuses on tile reading operations
 * - This harness focuses specifically on strip size calculation functions (not tile operations)
 * - Targets YCbCr subsampled data special case in TIFFVStripSize64 (lines 88-141 in tif_strip.c)
 * - Tests various strip configurations (rows per strip, planar vs contiguous)
 * - Tests both valid and invalid strip indices for robustness
 * - Tests edge cases for strip size calculations
 */



// Error handler to suppress warnings during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various configuration parameters
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Set error handlers to suppress warnings
    TIFFSetErrorHandler(NULL);
    TIFFSetWarningHandler(NULL);

    // Create a temporary file for TIFF operations
    char temp_file_template[] = "/tmp/fuzz_tiff_strip_XXXXXX";
    int fd = mkstemp(temp_file_template);
    if (fd < 0) {
        return 0;
    }
    std::string temp_filename = temp_file_template;
    
    // Write minimal valid TIFF header to the temporary file
    uint8_t tiff_header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, classic TIFF
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
        0x00, 0x00,              // Number of directory entries (0 = end)
        0x00, 0x00, 0x00, 0x00   // Next IFD offset (0 = no more)
    };
    
    if (write(fd, tiff_header, sizeof(tiff_header)) != (ssize_t)sizeof(tiff_header)) {
        close(fd);
        unlink(temp_filename.c_str());
        return 0;
    }
    close(fd);

    // Open the TIFF file for writing to set up test configuration
    TIFF* tif = nullptr;
    TIFFOpenOptions* opts = nullptr;
    
    // Choose opening method based on fuzzed input
    uint8_t open_method = fdp.ConsumeIntegral<uint8_t>() % 3;
    std::string mode = "w"; // Write mode to allow setting fields
    
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
            fd = open(temp_filename.c_str(), O_RDWR);
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

    // Phase 1: Configure TIFF image parameters for strip size calculations
    // Consume configuration parameters from fuzzed input
    
    // Basic image dimensions
    uint32_t image_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint32_t image_length = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    
    // Rows per strip configuration
    uint32_t rows_per_strip = fdp.ConsumeIntegral<uint32_t>() % 3;
    switch (rows_per_strip) {
        case 0:
            rows_per_strip = 1;  // Minimum valid value
            break;
        case 1:
            rows_per_strip = 16; // Typical value
            break;
        case 2:
            rows_per_strip = (uint32_t)-1; // Single strip for entire image
            break;
    }
    
    // Image configuration parameters
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 8; // Various photometric interpretations
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegral<uint16_t>() % 5;
    switch (bits_per_sample) {
        case 0: bits_per_sample = 1; break;
        case 1: bits_per_sample = 8; break;
        case 2: bits_per_sample = 16; break;
        case 3: bits_per_sample = 32; break;
        case 4: bits_per_sample = 64; break;
    }
    
    // Planar configuration (contiguous or separate)
    uint16_t planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    
    // Compression scheme
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 10;
    
    // YCbCr subsampling factors (critical for YCbCr path in TIFFVStripSize64)
    uint16_t ycbcr_subsampling_h = 1;
    uint16_t ycbcr_subsampling_v = 1;
    
    // Special case: Configure for YCbCr if photometric is YCbCr and samples_per_pixel == 3
    if (photometric == PHOTOMETRIC_YCBCR && samples_per_pixel == 3) {
        uint8_t subsample_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (subsample_choice) {
            case 0: ycbcr_subsampling_h = 1; ycbcr_subsampling_v = 1; break; // 1x1
            case 1: ycbcr_subsampling_h = 2; ycbcr_subsampling_v = 1; break; // 2x1
            case 2: ycbcr_subsampling_h = 1; ycbcr_subsampling_v = 2; break; // 1x2
            case 3: ycbcr_subsampling_h = 2; ycbcr_subsampling_v = 2; break; // 2x2
            case 4: ycbcr_subsampling_h = 4; ycbcr_subsampling_v = 4; break; // 4x4
        }
    }
    
    // Set TIFF fields for configuration
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_length);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    // Set YCbCr subsampling if applicable
    if (photometric == PHOTOMETRIC_YCBCR && samples_per_pixel == 3) {
        TIFFSetField(tif, TIFFTAG_YCBCRSUBSAMPLING, ycbcr_subsampling_h, ycbcr_subsampling_v);
    }
    
    // Phase 2: Test strip size calculation functions
    
    // 1. Test TIFFNumberOfStrips - calculate total number of strips
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    
    // 2. Test TIFFStripSize and TIFFStripSize64 - size of a strip
    uint64_t strip_size_64 = TIFFStripSize64(tif);
    tmsize_t strip_size = TIFFStripSize(tif);
    
    // 3. Test TIFFVStripSize and TIFFVStripSize64 with various row counts
    // Test with different numbers of rows
    uint32_t test_rows[] = {1, 16, 64, 128, (uint32_t)-1};
    for (int i = 0; i < 5; i++) {
        uint32_t nrows = test_rows[i];
        if (nrows == (uint32_t)-1 || nrows <= image_length) {
            uint64_t vstrip_size_64 = TIFFVStripSize64(tif, nrows);
            tmsize_t vstrip_size = TIFFVStripSize(tif, nrows);
        }
    }
    
    // 4. Test TIFFRawStripSize and TIFFRawStripSize64 with various strip indices
    if (num_strips > 0) {
        // Test with valid strip indices
        uint32_t max_test_strips = (num_strips < 5U) ? num_strips : 5U;
        for (uint32_t strip_idx = 0; strip_idx < max_test_strips; strip_idx++) {
            tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip_idx);
            
            // Also test TIFFGetStrileByteCount which is used internally
            uint64_t strile_byte_count = TIFFGetStrileByteCount(tif, strip_idx);
        }
        
        // Test with some potentially invalid strip indices
        uint32_t invalid_strip_indices[] = {num_strips, num_strips + 1, (uint32_t)-1};
        for (int i = 0; i < 3; i++) {
            uint32_t invalid_strip = invalid_strip_indices[i];
            uint64_t raw_strip_size_64 = TIFFRawStripSize64(tif, invalid_strip);
            tmsize_t raw_strip_size = TIFFRawStripSize(tif, invalid_strip);
        }
    }
    
    // 5. Test TIFFComputeStrip with various row and sample combinations
    // Test compute strip for various rows
    uint32_t max_rows_test = (image_length < 10U) ? image_length : 10U;
    uint32_t row_step = (image_length / 10 > 1U) ? (image_length / 10) : 1U;
    for (uint32_t row = 0; row < max_rows_test; row += row_step) {
        uint16_t max_samples_test = (samples_per_pixel < 3U) ? samples_per_pixel : 3U;
        for (uint16_t sample = 0; sample < max_samples_test; sample++) {
            uint32_t computed_strip = TIFFComputeStrip(tif, row, sample);
        }
    }
    
    // Test edge cases for TIFFComputeStrip
    if (image_length > 0) {
        // Last row
        uint32_t last_row = image_length - 1;
        uint16_t max_samples_test2 = (samples_per_pixel < 2U) ? samples_per_pixel : 2U;
        for (uint16_t sample = 0; sample < max_samples_test2; sample++) {
            uint32_t computed_strip = TIFFComputeStrip(tif, last_row, sample);
        }
        
        // Row beyond image length
        uint32_t beyond_row = image_length + 1;
        uint32_t computed_strip = TIFFComputeStrip(tif, beyond_row, 0);
        
        // Sample beyond samples per pixel
        uint16_t beyond_sample = samples_per_pixel + 1;
        computed_strip = TIFFComputeStrip(tif, 0, beyond_sample);
    }
    
    // 6. Test with rows_per_strip = 0 (edge case)
    // Temporarily set rows_per_strip to 0 to test error handling
    uint32_t saved_rows_per_strip = rows_per_strip;
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, (uint32_t)0);
    
    // These should handle the error case
    uint32_t num_strips_zero = TIFFNumberOfStrips(tif);
    uint64_t strip_size_64_zero = TIFFStripSize64(tif);
    
    // Restore original rows_per_strip
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, saved_rows_per_strip);
    
    // 7. Test with extremely large dimensions (overflow testing)
    if (fdp.ConsumeBool()) {
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)0xFFFFFFFF);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)0xFFFFFFFF);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 64);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 4);
        
        // Test size calculations with extreme values
        uint64_t extreme_strip_size_64 = TIFFStripSize64(tif);
        tmsize_t extreme_strip_size = TIFFStripSize(tif);
        
        // Restore reasonable dimensions
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_length);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    }
    
    // Clean up
    TIFFClose(tif);
    
    return 0;
}

} // namespace harness_011
