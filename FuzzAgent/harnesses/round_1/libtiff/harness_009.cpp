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
#include "tiff.h"

namespace harness_009 {
/*
 * Fuzzing harness for libtiff tile size calculation functions with YCbCr special case testing
 * Target APIs: TIFFVTileSize, TIFFTileSize, TIFFTileRowSize, TIFFDefaultTileSize (primary targets)
 * Required helper APIs: TIFFOpen/TIFFFdOpen, TIFFSetField, TIFFClose
 * 
 * This harness focuses specifically on tile size calculation functions that are completely uncovered:
 * 1. TIFFVTileSize - 59 undiscovered accumulated branches (highest complexity)
 * 2. TIFFTileSize - 0% covered
 * 3. TIFFTileRowSize - 0% covered
 * 4. TIFFDefaultTileSize - 0% covered
 * 
 * Semantic differentiation from previous harnesses:
 * - Focuses specifically on tile size calculation functions (not strip operations)
 * - Targets YCbCr subsampled data special case in TIFFVTileSize (lines 195-234 in tif_tile.c)
 * - Tests various YCbCr subsampling factors (1x1, 2x1, 1x2, 2x2, 4x4) and configurations
 * - Tests both 32-bit and 64-bit size function variants
 */
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various configuration parameters
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Create a temporary file for TIFF operations
    char temp_file_template[] = "/tmp/fuzz_tiff_tile_XXXXXX";
    int fd = mkstemp(temp_file_template);
    if (fd < 0) {
        return 0;
    }
    std::string temp_filename = temp_file_template;
    
    // Write minimal valid TIFF header to the temporary file
    // We need a valid TIFF file to open
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

    // Open the TIFF file
    TIFF* tif = nullptr;
    
    // Choose opening method based on fuzzed input
    uint8_t open_method = fdp.ConsumeIntegral<uint8_t>() % 2;
    std::string mode = "w"; // Write mode to allow setting fields
    
    if (open_method == 0) {
        // Standard TIFFOpen
        tif = TIFFOpen(temp_filename.c_str(), mode.c_str());
    } else {
        // TIFFFdOpen
        fd = open(temp_filename.c_str(), O_RDWR);
        if (fd >= 0) {
            tif = TIFFFdOpen(fd, temp_filename.c_str(), mode.c_str());
            // TIFFFdOpen duplicates the fd, so we can close ours
            close(fd);
        }
    }
    
    // Clean up temp file
    unlink(temp_filename.c_str());
    
    // If TIFF was not successfully opened, return early
    if (!tif) {
        return 0;
    }

    // Phase 1: Configure TIFF image parameters for tile size calculations
    // Consume configuration parameters from fuzzed input
    
    // Basic image dimensions
    uint32_t image_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint32_t image_length = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t tile_length = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    
    // Image configuration parameters
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 8; // Various photometric interpretations
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t planar_config = fdp.ConsumeIntegral<uint16_t>() % 2; // 0=CONTIG, 1=SEPARATE
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 10; // Various compression schemes
    
    // YCbCr specific parameters
    uint16_t ycbcr_subsampling_h = 1;
    uint16_t ycbcr_subsampling_v = 1;
    
    // Only set YCbCr subsampling if we're using YCbCr photometric
    if (photometric == PHOTOMETRIC_YCBCR) {
        // Valid YCbCr subsampling values: 1, 2, 4
        uint8_t subsampling_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (subsampling_choice) {
            case 0: ycbcr_subsampling_h = 1; ycbcr_subsampling_v = 1; break;
            case 1: ycbcr_subsampling_h = 2; ycbcr_subsampling_v = 1; break;
            case 2: ycbcr_subsampling_h = 1; ycbcr_subsampling_v = 2; break;
            case 3: ycbcr_subsampling_h = 2; ycbcr_subsampling_v = 2; break;
            case 4: ycbcr_subsampling_h = 4; ycbcr_subsampling_v = 4; break;
        }
        
        // For YCbCr, samples per pixel should be 3
        samples_per_pixel = 3;
        // YCbCr is typically planar config CONTIG
        planar_config = PLANARCONFIG_CONTIG;
    }
    
    // Set basic image fields
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_length);
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_length);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    // Set YCbCr specific fields if using YCbCr
    if (photometric == PHOTOMETRIC_YCBCR) {
        TIFFSetField(tif, TIFFTAG_YCBCRSUBSAMPLING, ycbcr_subsampling_h, ycbcr_subsampling_v);
    }
    
    // Phase 2: Call tile size calculation functions
    
    // 1. TIFFDefaultTileSize - compute default tile dimensions
    uint32_t default_tile_width = 0;
    uint32_t default_tile_length = 0;
    uint32_t* tw_ptr = &default_tile_width;
    uint32_t* th_ptr = &default_tile_length;
    
    // Call with different input values based on fuzzed data
    uint8_t default_size_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (default_size_choice) {
        case 0:
            // Zero values (should trigger defaults)
            *tw_ptr = 0;
            *th_ptr = 0;
            break;
        case 1:
            // Small values
            *tw_ptr = fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
            *th_ptr = fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
            break;
        case 2:
            // Large values
            *tw_ptr = fdp.ConsumeIntegralInRange<uint32_t>(1000, 4096);
            *th_ptr = fdp.ConsumeIntegralInRange<uint32_t>(1000, 4096);
            break;
    }
    
    TIFFDefaultTileSize(tif, tw_ptr, th_ptr);
    
    // 2. TIFFTileRowSize - compute bytes per tile row
    uint64_t tile_row_size_64 = TIFFTileRowSize64(tif);
    tmsize_t tile_row_size = TIFFTileRowSize(tif);
    
    // 3. TIFFTileSize - compute full tile size
    uint64_t tile_size_64 = TIFFTileSize64(tif);
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // 4. TIFFVTileSize - compute variable length tile size with different nrows values
    // Test with various nrows values to cover different code paths
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
        uint32_t nrows = 0;
        uint8_t nrows_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (nrows_choice) {
            case 0:
                // Zero rows (edge case)
                nrows = 0;
                break;
            case 1:
                // One row
                nrows = 1;
                break;
            case 2:
                // Partial tile (less than full tile length)
                if (tile_length > 1) {
                    nrows = fdp.ConsumeIntegralInRange<uint32_t>(1, tile_length - 1);
                } else {
                    // tile_length == 1, so partial tile doesn't make sense, use 0 or 1
                    nrows = fdp.ConsumeIntegral<uint8_t>() % 2; // 0 or 1
                }
                break;
            case 3:
                // Full tile or more
                if (tile_length > 0) {
                    nrows = fdp.ConsumeIntegralInRange<uint32_t>(tile_length, tile_length * 2);
                } else {
                    // Should not happen since tile_length >= 1, but handle defensively
                    nrows = 0;
                }
                break;
        }
        
        if (fdp.remaining_bytes() > 0) {
            uint64_t vtile_size_64 = TIFFVTileSize64(tif, nrows);
            tmsize_t vtile_size = TIFFVTileSize(tif, nrows);
        }
    }
    
    // Test with zero dimensions (should trigger error paths)
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, 0);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, 0);
    
    uint64_t error_tile_row_size_64 = TIFFTileRowSize64(tif);
    tmsize_t error_tile_row_size = TIFFTileRowSize(tif);
    uint64_t error_tile_size_64 = TIFFTileSize64(tif);
    tmsize_t error_tile_size = TIFFTileSize(tif);
    
    // Restore valid dimensions
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_length);
    
    // Phase 4: Test with different configurations to maximize coverage
    
    // Test a few different configurations with remaining fuzzed data
    while (fdp.remaining_bytes() > 32) {
        // Consume new configuration parameters
        uint32_t new_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 2048);
        uint32_t new_length = fdp.ConsumeIntegralInRange<uint32_t>(1, 2048);
        uint32_t new_tile_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 512);
        uint32_t new_tile_length = fdp.ConsumeIntegralInRange<uint32_t>(1, 512);
        uint16_t new_photometric = fdp.ConsumeIntegral<uint16_t>() % 8;
        uint16_t new_samples = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
        
        // Update configuration
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, new_width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, new_length);
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, new_tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, new_tile_length);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, new_photometric);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, new_samples);
        
        // Call tile size functions with new configuration
        uint64_t config_tile_row_size_64 = TIFFTileRowSize64(tif);
        uint64_t config_tile_size_64 = TIFFTileSize64(tif);
        
        // Test VTileSize with a random nrows
        uint32_t random_nrows = fdp.ConsumeIntegralInRange<uint32_t>(0, new_tile_length * 2);
        uint64_t config_vtile_size_64 = TIFFVTileSize64(tif, random_nrows);
        
        // Break if we've tested enough configurations
        if (fdp.remaining_bytes() < 64) {
            break;
        }
    }
    
    // Clean up
    TIFFClose(tif);
    
    return 0;
}

} // namespace harness_009
