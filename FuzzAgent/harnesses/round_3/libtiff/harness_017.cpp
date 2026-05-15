#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <tiffio.h>

namespace harness_017 {
/* Fuzzing harness for libtiff targeting RGBA tile reading functions with synthetic file creation
 * Focus: Create synthetic tiled RGB TIFF files and test RGBA tile/strip reading functions
 * Target APIs: TIFFReadRGBATile (172 undiscovered branches), TIFFReadRGBATileExt (172 undiscovered branches),
 *              TIFFReadRGBAStripExt (146 undiscovered branches), TIFFReadRGBAStrip (146 undiscovered branches),
 *              TIFFReadRGBAImage (144 undiscovered branches)
 * Supporting APIs: TIFFOpen (write mode), TIFFSetField (configuration), TIFFWriteEncodedTile,
 *                  TIFFClose, TIFFCleanup, TIFFIsTiled, TIFFGetField
 * Strategy: Create synthetic tiled RGB TIFF files from fuzzer input, configure proper tile parameters,
 *           then exercise RGBA reading functions to unlock 600+ undiscovered branches
 * Coverage Gap: These RGBA functions require specific tiled RGB file configurations that existing
 *               harnesses don't create, resulting in 0 hits despite being reached
 * Differentiation: Unlike harness_015/016 which read existing files, this harness creates synthetic
 *                  tiled RGB files specifically configured for RGBA tile reading APIs
 */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for configuration and pixel data
    if (size < 256) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume configuration parameters for tiled RGB TIFF file
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(64, 512);     // Reasonable image width
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(64, 512);    // Reasonable image height
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 128); // Must be power of 2 for tiles
    uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 128);
    
    // Ensure tile dimensions are multiples of each other and valid for image
    tile_width = ((tile_width + 15) / 16) * 16;  // Round to multiple of 16
    tile_height = ((tile_height + 15) / 16) * 16;
    
    // Ensure image dimensions are multiples of tile dimensions
    width = ((width + tile_width - 1) / tile_width) * tile_width;
    height = ((height + tile_height - 1) / tile_height) * tile_height;
    
    // Consume test mode: which RGBA functions to test
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 5;
    bool use_extended_apis = fdp.ConsumeBool();
    int stop_on_error = fdp.ConsumeIntegral<int>() % 2;
    
    // Consume tile coordinates (must be tile boundaries)
    uint32_t tile_col = fdp.ConsumeIntegralInRange<uint32_t>(0, (width / tile_width) - 1) * tile_width;
    uint32_t tile_row = fdp.ConsumeIntegralInRange<uint32_t>(0, (height / tile_height) - 1) * tile_height;
    
    // Consume some pixel data for the tiles
    size_t tile_pixel_count = tile_width * tile_height;
    size_t tile_byte_size = tile_pixel_count * 3;  // RGB, 3 bytes per pixel
    
    // Limit tile data size to prevent excessive memory usage
    if (tile_byte_size > 1048576) {  // 1MB max per tile
        return 0;
    }
    
    std::vector<uint8_t> tile_data;
    if (fdp.remaining_bytes() >= tile_byte_size) {
        tile_data = fdp.ConsumeBytes<uint8_t>(tile_byte_size);
    } else {
        // Not enough data for tile, use what we have
        tile_data = fdp.ConsumeRemainingBytes<uint8_t>();
        // Pad with zeros if needed
        tile_data.resize(tile_byte_size, 0);
    }
    
    // Create a temporary filename for TIFF operations
    char filename[] = "/tmp/tiff_fuzz_rgba_tile_XXXXXX.tif";
    int fd = mkstemps(filename, 4);
    if (fd < 0) {
        return 0;
    }
    close(fd); // We'll open via TIFFOpen, not file descriptor

    // PHASE 1: Create synthetic tiled RGB TIFF file
    TIFF* tif_write = TIFFOpen(filename, "w");
    if (!tif_write) {
        unlink(filename);
        return 0;
    }
    
    // Configure tiled RGB format as specified in coverage guidance
    TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif_write, TIFFTAG_SAMPLESPERPIXEL, 3);          // RGB = 3 samples
    TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, 8);           // 8 bits per sample
    TIFFSetField(tif_write, TIFFTAG_COMPRESSION, COMPRESSION_NONE); // No compression
    TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB); // RGB color space
    TIFFSetField(tif_write, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG); // Contiguous planar config
    TIFFSetField(tif_write, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(tif_write, TIFFTAG_TILELENGTH, tile_height);
    TIFFSetField(tif_write, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    
    // Write directory to establish the image structure
    TIFFWriteDirectory(tif_write);
    
    // Write some tile data to make the file valid
    // Calculate number of tiles
    uint32_t tiles_across = width / tile_width;
    uint32_t tiles_down = height / tile_height;
    
    // Write a few tiles with the pixel data
    for (uint32_t ty = 0; ty < tiles_down && ty < 2; ty++) {
        for (uint32_t tx = 0; tx < tiles_across && tx < 2; tx++) {
            // Write encoded tile (simplified - in real scenario would need proper encoding)
            tmsize_t write_size = TIFFWriteEncodedTile(tif_write, 
                ty * tiles_across + tx, 
                tile_data.data(), 
                tile_byte_size);
            // Ignore write errors for fuzzing purposes
        }
    }
    
    TIFFClose(tif_write);
    
    // PHASE 2: Reopen file for reading and test RGBA functions
    TIFF* tif_read = TIFFOpen(filename, "r");
    if (!tif_read) {
        unlink(filename);
        return 0;
    }
    
    // Read the directory
    int dir_read = TIFFReadDirectory(tif_read);
    if (!dir_read) {
        TIFFClose(tif_read);
        unlink(filename);
        return 0;
    }
    
    // Verify the file is tiled as expected
    int is_tiled = TIFFIsTiled(tif_read);
    if (!is_tiled) {
        TIFFClose(tif_read);
        unlink(filename);
        return 0;
    }
    
    // Get actual tile dimensions from file
    uint32_t actual_tile_width = 0, actual_tile_height = 0;
    TIFFGetFieldDefaulted(tif_read, TIFFTAG_TILEWIDTH, &actual_tile_width);
    TIFFGetFieldDefaulted(tif_read, TIFFTAG_TILELENGTH, &actual_tile_height);
    
    // Adjust coordinates to be valid tile boundaries
    if (actual_tile_width > 0 && actual_tile_height > 0) {
        tile_col = (tile_col / actual_tile_width) * actual_tile_width;
        tile_row = (tile_row / actual_tile_height) * actual_tile_height;
    }
    
    // Ensure coordinates are within image bounds
    uint32_t actual_width = 0, actual_height = 0;
    TIFFGetField(tif_read, TIFFTAG_IMAGEWIDTH, &actual_width);
    TIFFGetField(tif_read, TIFFTAG_IMAGELENGTH, &actual_height);
    
    if (tile_col >= actual_width) tile_col = 0;
    if (tile_row >= actual_height) tile_row = 0;
    
    // Test the targeted RGBA functions based on test mode
    switch (test_mode) {
        case 0: {
            // Test TIFFReadRGBATile and TIFFReadRGBATileExt
            size_t rgba_tile_size = actual_tile_width * actual_tile_height * 4; // RGBA = 4 bytes per pixel
            if (rgba_tile_size > 0 && rgba_tile_size < 16777216) { // Limit to 16MB
                uint32_t* rgba_buffer = (uint32_t*)_TIFFmalloc(rgba_tile_size);
                if (rgba_buffer) {
                    if (use_extended_apis) {
                        TIFFReadRGBATileExt(tif_read, tile_col, tile_row, rgba_buffer, stop_on_error);
                    } else {
                        TIFFReadRGBATile(tif_read, tile_col, tile_row, rgba_buffer);
                    }
                    _TIFFfree(rgba_buffer);
                }
            }
            break;
        }
        
        case 1: {
            // Test TIFFReadRGBAStrip and TIFFReadRGBAStripExt
            // First check if file has strips (some tiled files also have strip info)
            uint32_t rows_per_strip = 0;
            TIFFGetFieldDefaulted(tif_read, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
            
            if (rows_per_strip > 0) {
                // Calculate valid strip row (must be strip boundary)
                uint32_t strip_row = (tile_row / rows_per_strip) * rows_per_strip;
                
                size_t rgba_strip_size = actual_width * rows_per_strip * 4;
                if (rgba_strip_size > 0 && rgba_strip_size < 16777216) {
                    uint32_t* rgba_buffer = (uint32_t*)_TIFFmalloc(rgba_strip_size);
                    if (rgba_buffer) {
                        if (use_extended_apis) {
                            TIFFReadRGBAStripExt(tif_read, strip_row, rgba_buffer, stop_on_error);
                        } else {
                            TIFFReadRGBAStrip(tif_read, strip_row, rgba_buffer);
                        }
                        _TIFFfree(rgba_buffer);
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test TIFFReadRGBAImage
            size_t rgba_image_size = actual_width * actual_height * 4;
            if (rgba_image_size > 0 && rgba_image_size < 16777216) {
                uint32_t* rgba_buffer = (uint32_t*)_TIFFmalloc(rgba_image_size);
                if (rgba_buffer) {
                    TIFFReadRGBAImage(tif_read, actual_width, actual_height, rgba_buffer, 0);
                    _TIFFfree(rgba_buffer);
                }
            }
            break;
        }
        
        case 3: {
            // Test multiple RGBA functions in sequence
            size_t small_buffer_size = 4096; // Small buffer for error path testing
            uint32_t* small_buffer = (uint32_t*)_TIFFmalloc(small_buffer_size);
            if (small_buffer) {
                // Test with potentially invalid coordinates (error paths)
                TIFFReadRGBATile(tif_read, actual_width + 100, actual_height + 100, small_buffer);
                TIFFReadRGBAStrip(tif_read, actual_height + 100, small_buffer);
                
                // Test with valid coordinates but small buffer (partial reads)
                if (actual_tile_width > 0 && actual_tile_height > 0) {
                    TIFFReadRGBATile(tif_read, 0, 0, small_buffer);
                }
                _TIFFfree(small_buffer);
            }
            break;
        }
        
        case 4: {
            // Comprehensive test: try all RGBA functions
            size_t test_buffer_size = std::min(actual_tile_width * actual_tile_height * 4, 
                                             static_cast<uint32_t>(65536));
            if (test_buffer_size > 0) {
                uint32_t* test_buffer = (uint32_t*)_TIFFmalloc(test_buffer_size);
                if (test_buffer) {
                    // Try tile reading
                    TIFFReadRGBATile(tif_read, tile_col, tile_row, test_buffer);
                    TIFFReadRGBATileExt(tif_read, tile_col, tile_row, test_buffer, stop_on_error);
                    
                    // Try strip reading if applicable
                    uint32_t rows_per_strip = 0;
                    TIFFGetFieldDefaulted(tif_read, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
                    if (rows_per_strip > 0 && tile_row < actual_height) {
                        uint32_t strip_row = (tile_row / rows_per_strip) * rows_per_strip;
                        TIFFReadRGBAStrip(tif_read, strip_row, test_buffer);
                        TIFFReadRGBAStripExt(tif_read, strip_row, test_buffer, stop_on_error);
                    }
                    
                    _TIFFfree(test_buffer);
                }
            }
            break;
        }
    }
    // Cleanup
    TIFFClose(tif_read);
    unlink(filename);
    
    // Note: TIFFCleanup is not called here as it's a global cleanup function
    // that takes a TIFF* parameter and may interfere with other tests.
    // The coverage guidance mentioned TIFFCleanup but it requires a TIFF handle.
    
    return 0;
}

} // namespace harness_017
