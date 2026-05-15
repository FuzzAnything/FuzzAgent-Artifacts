#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>  // For close(), unlink(), mkstemp()
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>

namespace harness_011 {
/*
 * libtiff fuzzing harness for advanced TIFF writing operations with semantic diversity
 * Targeting 1500+ undiscovered complexity points in tif_write.c (0% coverage)
 * 
 * Target functions with comprehensive coordinate-based testing:
 * - TIFFWriteTile: Coordinate-based tile writing (x,y,z,s coordinates) - Semantic diversity from index-based approaches
 * - TIFFWriteScanline: Scanline writing with planar configuration edge cases
 * - TIFFWriteEncodedStrip: Encoded strip writing with varied buffer sizes
 * - TIFFWriteEncodedTile: Encoded tile writing with compression testing
 * - TIFFWriteRawStrip: Raw strip writing for uncompressed data paths
 * - TIFFWriteRawTile: Raw tile writing for error condition testing
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Basic reading operations via TIFFStreamOpen
 * - harness_001: Basic writing operations (tiles/strips with some raw operations)
 * - harness_002: Multi-directory and compression
 * - harness_003: Directory writing lifecycle
 * - harness_004: File opening/client I/O
 * - harness_005: Advanced RGB reading
 * - harness_006: LogLuv color space
 * - harness_007: Advanced compression codecs
 * - harness_008: Directory and image writing
 * - harness_009: EXIF/GPS metadata reading
 * - harness_010: Complete writing subsystem lifecycle (crashed early)
 * - harness_011: COORDINATE-BASED writing operations with comprehensive error handling and semantic diversity
 * 
 * Key semantic diversity elements:
 * 1. Coordinate-based TIFFWriteTile API usage (vs index-based in other harnesses)
 * 2. Comprehensive planar configuration testing (PLANARCONFIG_SEPARATE edge cases)
 * 3. Multi-sample pixel handling with varied sample indices
 * 4. Deep error condition testing with invalid parameter combinations
 * 5. Proper setup/cleanup sequence with TIFFWriteCheck validation
 * 6. Mixed operation sequences with mode transitions
 */


// Maximum file size for safety
const size_t MAX_FILE_SIZE = 10 * 1024 * 1024; // 10MB

// Error handler that does nothing (silent)
void silent_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    return;
}

// Warning handler that does nothing (silent)
void silent_warning_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    return;
}

// Array of compression codecs to test
static const uint16_t compression_codecs[] = {
    COMPRESSION_NONE,
    COMPRESSION_LZW,
    COMPRESSION_JPEG,
    COMPRESSION_DEFLATE,
    COMPRESSION_ADOBE_DEFLATE,
    COMPRESSION_PACKBITS,
    COMPRESSION_CCITTRLE,
    COMPRESSION_CCITTFAX3,
    COMPRESSION_CCITTFAX4
};

// Array of photometric interpretations
static const uint16_t photometric_options[] = {
    PHOTOMETRIC_MINISWHITE,
    PHOTOMETRIC_MINISBLACK,
    PHOTOMETRIC_RGB,
    PHOTOMETRIC_PALETTE,
    PHOTOMETRIC_SEPARATED,
    PHOTOMETRIC_YCBCR,
    PHOTOMETRIC_CIELAB,
    PHOTOMETRIC_LOGL,
    PHOTOMETRIC_LOGLUV
};

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Set silent error handlers to avoid output noise
    TIFFSetErrorHandler(silent_error_handler);
    TIFFSetWarningHandler(silent_warning_handler);
    
    // Minimum input size check - need enough for comprehensive configuration
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a temporary file for TIFF operations
    std::string temp_filename = "/tmp/libtiff_fuzz_coordinate_writing_XXXXXX";
    int fd = mkstemp(&temp_filename[0]);
    if (fd < 0) {
        return 0;
    }
    close(fd);
    
    // Consume comprehensive test configuration parameters
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool test_coordinate_tiles = fdp.ConsumeBool();
    bool test_scanlines = fdp.ConsumeBool();
    bool test_encoded_strips = fdp.ConsumeBool();
    bool test_encoded_tiles = fdp.ConsumeBool();
    bool test_raw_operations = fdp.ConsumeBool();
    bool test_planar_config = fdp.ConsumeBool();
    bool test_error_conditions = fdp.ConsumeBool();
    
    // Image configuration parameters with semantic diversity
    uint32_t image_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 2048);
    uint32_t image_height = fdp.ConsumeIntegralInRange<uint32_t>(1, 2048);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 6); // Extended for planar testing
    uint16_t photometric_index = fdp.ConsumeIntegral<uint16_t>() % (sizeof(photometric_options)/sizeof(photometric_options[0]));
    uint16_t photometric = photometric_options[photometric_index];
    uint16_t compression_index = fdp.ConsumeIntegral<uint16_t>() % (sizeof(compression_codecs)/sizeof(compression_codecs[0]));
    uint16_t compression = compression_codecs[compression_index];
    
    // Planar configuration (key for semantic diversity)
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    if (test_planar_config) {
        planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    }
    
    // Tile configuration for coordinate-based testing
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
    uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
    
    // Strip configuration
    uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 64);
    
    // Consume buffer data for writing operations
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 65536);
    std::vector<uint8_t> write_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(temp_filename.c_str(), "w");
    if (!tif) {
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // Set comprehensive TIFF fields
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    // Calculate buffer sizes
    size_t bytes_per_sample = (bits_per_sample + 7) / 8;
    size_t scanline_size = image_width * samples_per_pixel * bytes_per_sample;
    size_t tile_size = tile_width * tile_height * samples_per_pixel * bytes_per_sample;
    size_t strip_size = rows_per_strip * scanline_size;
    
    // Ensure write buffer is large enough for operations
    size_t required_buffer_size = std::max({scanline_size, tile_size, strip_size, (size_t)1});
    if (write_buffer.size() < required_buffer_size) {
        write_buffer.resize(required_buffer_size);
    }
    
    // Validate writing configuration
    TIFFWriteCheck(tif, 0, "coordinate_writing_harness");
    
    // ============================================
    // TEST 1: COORDINATE-BASED TILE WRITING (Semantic Diversity)
    // ============================================
    if (test_coordinate_tiles) {
        // Configure as tiled image
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
        
        // Calculate tile grid dimensions
        uint32_t tiles_across = (image_width + tile_width - 1) / tile_width;
        uint32_t tiles_down = (image_height + tile_height - 1) / tile_height;
        
        if (tiles_across > 0 && tiles_down > 0 && write_buffer.size() > 0) {
            // Test coordinate-based TIFFWriteTile with various positions
            for (uint32_t tile_y = 0; tile_y < std::min(tiles_down, static_cast<uint32_t>(3)); tile_y++) {
                for (uint32_t tile_x = 0; tile_x < std::min(tiles_across, static_cast<uint32_t>(3)); tile_x++) {
                    // Calculate pixel coordinates
                    uint32_t x = tile_x * tile_width;
                    uint32_t y = tile_y * tile_height;
                    uint32_t z = 0; // Default z-plane
                    
                    // For planar separate, test different sample planes
                    for (uint16_t sample = 0; sample < std::min(samples_per_pixel, static_cast<uint16_t>(3)); sample++) {
                        TIFFWriteTile(tif, write_buffer.data(), x, y, z, sample);
                    }
                }
            }
            
            // Test edge cases for coordinate-based writing
            if (test_error_conditions) {
                // Test coordinates out of bounds
                TIFFWriteTile(tif, write_buffer.data(), image_width + 100, 0, 0, 0);
                TIFFWriteTile(tif, write_buffer.data(), 0, image_height + 100, 0, 0);
                
                // Test sample index out of bounds
                TIFFWriteTile(tif, write_buffer.data(), 0, 0, 0, samples_per_pixel + 1);
                
                // Test with NULL buffer
                TIFFWriteTile(tif, NULL, 0, 0, 0, 0);
            }
        }
    }
    
    // ============================================
    // TEST 2: SCANLINE WRITING WITH PLANAR CONFIG
    // ============================================
    if (test_scanlines && image_height > 0 && write_buffer.size() > 0) {
        // Configure strip organization for scanline compatibility
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
        
        // Test TIFFWriteScanline with various configurations
        for (uint32_t row = 0; row < std::min(image_height, static_cast<uint32_t>(4)); row++) {
            if (planar_config == PLANARCONFIG_SEPARATE && samples_per_pixel > 1) {
                // Test writing to separate sample planes
                for (uint16_t sample = 0; sample < std::min(samples_per_pixel, static_cast<uint16_t>(3)); sample++) {
                    TIFFWriteScanline(tif, write_buffer.data(), row, sample);
                }
            } else {
                // Contiguous configuration or single sample
                TIFFWriteScanline(tif, write_buffer.data(), row, 0);
            }
        }
    }
    
    // ============================================
    // TEST 3: ENCODED STRIP WRITING
    // ============================================
    if (test_encoded_strips) {
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
        
        // Calculate number of strips
        uint32_t strips_per_image = (image_height + rows_per_strip - 1) / rows_per_strip;
        
        if (strips_per_image > 0 && write_buffer.size() > 0) {
            // Test TIFFWriteEncodedStrip with varied byte counts
            for (uint32_t strip = 0; strip < std::min(strips_per_image, static_cast<uint32_t>(4)); strip++) {
                tmsize_t bytes_to_write = fdp.ConsumeIntegralInRange<tmsize_t>(1, write_buffer.size());
                TIFFWriteEncodedStrip(tif, strip, write_buffer.data(), bytes_to_write);
            }
        }
    }
    
    // ============================================
    // TEST 4: ENCODED TILE WRITING
    // ============================================
    if (test_encoded_tiles) {
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
        
        // Calculate number of tiles
        uint32_t tiles_across = (image_width + tile_width - 1) / tile_width;
        uint32_t tiles_down = (image_height + tile_height - 1) / tile_height;
        uint32_t total_tiles = tiles_across * tiles_down;
        
        if (total_tiles > 0 && write_buffer.size() > 0) {
            // Test TIFFWriteEncodedTile with varied byte counts
            for (uint32_t tile = 0; tile < std::min(total_tiles, static_cast<uint32_t>(4)); tile++) {
                tmsize_t bytes_to_write = fdp.ConsumeIntegralInRange<tmsize_t>(1, write_buffer.size());
                TIFFWriteEncodedTile(tif, tile, write_buffer.data(), bytes_to_write);
            }
        }
    }
    
    // ============================================
    // TEST 5: RAW OPERATIONS (Uncompressed Paths)
    // ============================================
    if (test_raw_operations && write_buffer.size() > 0) {
        // Temporarily set compression to NONE for raw operations
        uint16_t original_compression = compression;
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        
        if (test_coordinate_tiles) {
            // Configure as tiled for raw tile testing
            TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
            TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
            
            // Calculate tile grid
            uint32_t tiles_across = (image_width + tile_width - 1) / tile_width;
            uint32_t tiles_down = (image_height + tile_height - 1) / tile_height;
            uint32_t total_tiles = tiles_across * tiles_down;
            
            if (total_tiles > 0) {
                // Test TIFFWriteRawTile
                for (uint32_t tile = 0; tile < std::min(total_tiles, static_cast<uint32_t>(3)); tile++) {
                    tmsize_t raw_bytes = fdp.ConsumeIntegralInRange<tmsize_t>(1, write_buffer.size());
                    TIFFWriteRawTile(tif, tile, write_buffer.data(), raw_bytes);
                }
            }
        }
        
        if (test_encoded_strips) {
            // Configure as stripped for raw strip testing
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
            
            // Calculate strips
            uint32_t strips_per_image = (image_height + rows_per_strip - 1) / rows_per_strip;
            
            if (strips_per_image > 0) {
                // Test TIFFWriteRawStrip
                for (uint32_t strip = 0; strip < std::min(strips_per_image, static_cast<uint32_t>(3)); strip++) {
                    tmsize_t raw_bytes = fdp.ConsumeIntegralInRange<tmsize_t>(1, write_buffer.size());
                    TIFFWriteRawStrip(tif, strip, write_buffer.data(), raw_bytes);
                }
            }
        }
        
        // Restore original compression
        TIFFSetField(tif, TIFFTAG_COMPRESSION, original_compression);
    }
    
    // ============================================
    // TEST 6: COMPREHENSIVE ERROR HANDLING
    // ============================================
    if (test_error_conditions && write_buffer.size() > 0) {
        uint8_t error_test = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        switch (error_test) {
            case 0:
                // Test with invalid planar config for photometric
                TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);
                TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_PALETTE);
                TIFFWriteScanline(tif, write_buffer.data(), 0, 0);
                break;
                
            case 1:
                // Test writing before proper configuration
                TIFFWriteScanline(tif, write_buffer.data(), 0, 0);
                break;
                
            case 2:
                // Test with zero dimension image
                TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 0);
                TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 0);
                TIFFWriteEncodedStrip(tif, 0, write_buffer.data(), 1);
                break;
                
            case 3:
                // Test with incompatible compression
                TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_JPEG);
                TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_LOGLUV);
                TIFFWriteEncodedTile(tif, 0, write_buffer.data(), 1);
                break;
                
            case 4:
                // Test coordinate-based writing on non-tiled image
                TIFFWriteTile(tif, write_buffer.data(), 0, 0, 0, 0);
                break;
                
            case 5:
                // Test strip writing on tiled image
                TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
                TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
                TIFFWriteEncodedStrip(tif, 0, write_buffer.data(), 1);
                break;
                
            case 6:
                // Test tile writing on stripped image
                TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
                TIFFWriteEncodedTile(tif, 0, write_buffer.data(), 1);
                break;
                
            case 7:
                // Test with negative byte counts
                TIFFWriteEncodedStrip(tif, 0, write_buffer.data(), -1);
                TIFFWriteEncodedTile(tif, 0, write_buffer.data(), -1);
                break;
        }
    }
    
    // Final directory write to complete the image
    TIFFWriteDirectory(tif);
    
    // Cleanup
    TIFFClose(tif);
    unlink(temp_filename.c_str());
    
    return 0;
}

} // namespace harness_011
