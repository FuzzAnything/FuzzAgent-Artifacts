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

namespace harness_010 {
/*
 * Fuzzing harness for libtiff tile reading operations with coordinate-based tile selection
 * Target APIs: TIFFReadTile (primary target), TIFFCheckTile, TIFFComputeTile (secondary targets)
 * Required helper APIs: TIFFOpen/TIFFOpenExt, TIFFSetField, TIFFGetField, TIFFClose, _TIFFmalloc, _TIFFfree
 * 
 * This harness focuses specifically on tile reading operations that are completely uncovered:
 * 1. TIFFReadTile - 500+ undiscovered accumulated branches (highest complexity)
 * 2. TIFFCheckTile - Tile coordinate validation logic
 * 3. TIFFComputeTile - Mathematical tile coordinate calculation
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_009 focuses on tile size calculation functions (TIFFVTileSize, TIFFTileSize, etc.)
 * - This harness focuses on actual tile reading operations with coordinate-based selection
 * - Tests TIFFReadTile's coordinate-to-tile-index conversion and validation logic
 * - Tests various image configurations (tiled vs untiled, planar vs contiguous)
 * - Tests tile reading with both valid and invalid coordinate ranges
 */



// Error handler to suppress warnings during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various configuration parameters and tile coordinates
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Set error handlers to suppress warnings
    TIFFSetErrorHandler(NULL);
    TIFFSetWarningHandler(NULL);

    // Create a temporary file for TIFF operations
    char temp_file_template[] = "/tmp/fuzz_tiff_readtile_XXXXXX";
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
    
    // Phase 1: Configure TIFF image parameters for tile reading
    // Consume configuration parameters from fuzzed input
    
    // Basic image dimensions
    uint32_t image_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint32_t image_length = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t tile_length = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t image_depth = fdp.ConsumeIntegralInRange<uint32_t>(1, 16);
    
    // Image configuration parameters
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 8; // Various photometric interpretations
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t planar_config = fdp.ConsumeIntegral<uint16_t>() % 2; // PLANARCONFIG_CONTIG or PLANARCONFIG_SEPARATE
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 10; // Various compression types
    
    // Set basic image parameters
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_length);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    // Determine if we should configure as tiled or untiled
    bool use_tiles = fdp.ConsumeBool();
    
    if (use_tiles) {
        // Configure as tiled image
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_length);
        TIFFSetField(tif, TIFFTAG_IMAGEDEPTH, image_depth);
    } else {
        // Configure as untiled (stripped) image
        uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 256);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    }
    
    // For YCbCr photometric, set subsampling if needed
    if (photometric == PHOTOMETRIC_YCBCR && samples_per_pixel == 3) {
        uint16_t ycbcr_subsampling_h = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
        uint16_t ycbcr_subsampling_v = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
        TIFFSetField(tif, TIFFTAG_YCBCRSUBSAMPLING, ycbcr_subsampling_h, ycbcr_subsampling_v);
    }
    
    // Phase 2: Test TIFFComputeTile function
    // Consume tile coordinates from fuzzed input
    uint32_t tile_x = fdp.ConsumeIntegral<uint32_t>();
    uint32_t tile_y = fdp.ConsumeIntegral<uint32_t>();
    uint32_t tile_z = fdp.ConsumeIntegralInRange<uint32_t>(0, image_depth - 1);
    uint16_t tile_sample = fdp.ConsumeIntegralInRange<uint16_t>(0, samples_per_pixel - 1);
    
    // Compute tile index from coordinates (even if not tiled, this tests the calculation)
    uint32_t computed_tile = TIFFComputeTile(tif, tile_x, tile_y, tile_z, tile_sample);
    (void)computed_tile; // Use result to avoid compiler warnings
    
    // Phase 3: Test TIFFCheckTile function
    // This validates if coordinates are within image bounds
    int tile_check_result = TIFFCheckTile(tif, tile_x, tile_y, tile_z, tile_sample);
    (void)tile_check_result; // Use result to avoid compiler warnings
    
    // Phase 4: Test TIFFReadTile function with various scenarios
    // Only test TIFFReadTile if image is tiled
    if (use_tiles) {
        // Calculate tile size for buffer allocation
        tmsize_t tile_size = TIFFTileSize(tif);
        
        if (tile_size > 0 && tile_size < 1024 * 1024) { // Limit buffer size
            // Allocate buffer for tile data
            void* tile_buffer = _TIFFmalloc(tile_size);
            if (tile_buffer) {
                // Test TIFFReadTile with valid and potentially invalid coordinates
                // We'll test a few different coordinate combinations
                
                // Test 1: Normal coordinates (may be valid or invalid)
                tmsize_t read_result1 = TIFFReadTile(tif, tile_buffer, tile_x, tile_y, tile_z, tile_sample);
                (void)read_result1;
                
                // Test 2: Edge case coordinates (near boundaries)
                uint32_t edge_x = fdp.ConsumeIntegralInRange<uint32_t>(0, image_width);
                uint32_t edge_y = fdp.ConsumeIntegralInRange<uint32_t>(0, image_length);
                uint32_t edge_z = fdp.ConsumeIntegralInRange<uint32_t>(0, image_depth);
                uint16_t edge_sample = fdp.ConsumeIntegralInRange<uint16_t>(0, samples_per_pixel);
                
                tmsize_t read_result2 = TIFFReadTile(tif, tile_buffer, edge_x, edge_y, edge_z, edge_sample);
                (void)read_result2;
                
                // Test 3: Out-of-bounds coordinates (should fail validation)
                uint32_t out_of_bounds_x = image_width + fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
                uint32_t out_of_bounds_y = image_length + fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
                
                tmsize_t read_result3 = TIFFReadTile(tif, tile_buffer, out_of_bounds_x, out_of_bounds_y, 0, 0);
                (void)read_result3;
                
                // Test 4: Zero coordinates (edge case)
                tmsize_t read_result4 = TIFFReadTile(tif, tile_buffer, 0, 0, 0, 0);
                (void)read_result4;
                
                // Free tile buffer
                _TIFFfree(tile_buffer);
            }
        }
        
        // Phase 5: Test related tile reading functions
        // Test TIFFReadEncodedTile with computed tile index
        if (computed_tile < 1000) { // Reasonable limit for testing
            tmsize_t tile_buffer_size = TIFFTileSize(tif);
            if (tile_buffer_size > 0 && tile_buffer_size < 1024 * 1024) {
                void* encoded_buffer = _TIFFmalloc(tile_buffer_size);
                if (encoded_buffer) {
                    tmsize_t encoded_result = TIFFReadEncodedTile(tif, computed_tile, encoded_buffer, tile_buffer_size);
                    (void)encoded_result;
                    _TIFFfree(encoded_buffer);
                }
            }
        }
        
        // Test TIFFReadRawTile if compression is NONE
        if (compression == COMPRESSION_NONE) {
            tmsize_t raw_tile_size = TIFFTileSize(tif);
            if (raw_tile_size > 0 && raw_tile_size < 1024 * 1024) {
                void* raw_buffer = _TIFFmalloc(raw_tile_size);
                if (raw_buffer) {
                    tmsize_t raw_result = TIFFReadRawTile(tif, computed_tile, raw_buffer, raw_tile_size);
                    (void)raw_result;
                    _TIFFfree(raw_buffer);
                }
            }
        }
    } else {
        // For untiled images, test that TIFFReadTile returns error appropriately
        // Allocate a small buffer to test the error path
        void* test_buffer = _TIFFmalloc(1024);
        if (test_buffer) {
            tmsize_t read_result = TIFFReadTile(tif, test_buffer, 0, 0, 0, 0);
            (void)read_result; // Should return -1 for non-tiled images
            _TIFFfree(test_buffer);
        }
    }
    
    // Phase 6: Test tile size calculation functions to ensure integration
    // These complement the tile reading operations
    
    // Test TIFFTileSize (already called above, but call again for coverage)
    tmsize_t final_tile_size = TIFFTileSize(tif);
    (void)final_tile_size;
    
    // Test TIFFTileRowSize for row-based calculations
    tmsize_t tile_row_size = TIFFTileRowSize(tif);
    (void)tile_row_size;
    
    // Test TIFFVTileSize with variable row count
    uint32_t variable_rows = fdp.ConsumeIntegralInRange<uint32_t>(1, tile_length * 2);
    tmsize_t variable_tile_size = TIFFVTileSize(tif, variable_rows);
    (void)variable_tile_size;
    
    // Test TIFFDefaultTileSize function
    uint32_t default_width = 0;
    uint32_t default_height = 0;
    TIFFDefaultTileSize(tif, &default_width, &default_height);
    (void)default_width;
    (void)default_height;
    
    // Clean up
    TIFFClose(tif);
    
    return 0;
}

} // namespace harness_010
