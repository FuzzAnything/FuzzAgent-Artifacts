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

namespace harness_011 {
/* Fuzzing harness for libtiff targeting comprehensive TIFF tile and strip writing operations
 * Target: High-complexity writing APIs in tif_write.c with over 1,235 undiscovered branches
 * Strategy: Implement full lifecycle for both strip-based and tile-based writing with various configurations
 * APIs to target: TIFFWriteEncodedStrip, TIFFWriteRawStrip, TIFFWriteEncodedTile, TIFFWriteRawTile,
 *                 TIFFWriteScanline, TIFFWriteTile (6 primary targets)
 * Supporting APIs: TIFFOpen/TIFFCreateDirectory -> TIFFSetField/TIFFSetupStrips -> 
 *                  TIFFWriteBufferSetup -> Core writing -> TIFFFlush/TIFFWriteDirectory -> TIFFClose
 */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for configuration and pixel data
    if (size < 128) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume configuration parameters for TIFF file
    // Fixed-size parameters first
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 10; // Various compression schemes
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6; // Photometric interpretations
    uint16_t planar_config = fdp.ConsumeIntegral<uint16_t>() % 2; // PLANARCONFIG_CONTIG or SEPARATE
    uint16_t fillorder = fdp.ConsumeIntegral<uint16_t>() % 2; // FILLORDER_MSB2LSB or LSB2MSB
    uint16_t orientation = fdp.ConsumeIntegral<uint16_t>() % 8; // ORIENTATION_ values
    
    // Image organization parameters
    bool use_tiles = fdp.ConsumeBool();
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 256);
    
    // Writing mode selection
    uint8_t writing_mode = fdp.ConsumeIntegral<uint8_t>() % 6; // 0: encoded strip, 1: raw strip, 2: encoded tile, 3: raw tile, 4: scanline, 5: tile
    
    // Consume remaining data for pixel buffer
    std::vector<uint8_t> pixel_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (pixel_data.empty()) {
        return 0;
    }

    // Create a temporary filename for writing
    char filename[] = "/tmp/tiff_fuzz_write_XXXXXX.tif";
    int fd = mkstemps(filename, 4);
    if (fd < 0) {
        return 0;
    }
    close(fd); // We'll open via TIFFOpen, not file descriptor

    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    // Create a new directory if needed (optional)
    int create_dir = fdp.ConsumeBool();
    if (create_dir) {
        TIFFCreateDirectory(tif);
    }

    // Set basic image parameters using TIFFSetField
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_FILLORDER, fillorder);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);

    // Set organization-specific parameters
    if (use_tiles) {
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    } else {
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    }

    // Setup strips for strip-based organization
    if (!use_tiles) {
        TIFFSetupStrips(tif);
    }

    // Setup write buffer (optional but important for some code paths)
    bool setup_buffer = fdp.ConsumeBool();
    if (setup_buffer) {
        size_t buffer_size = std::min(pixel_data.size(), static_cast<size_t>(65536));
        TIFFWriteBufferSetup(tif, pixel_data.data(), buffer_size);
    }

    // Calculate pixel size and buffer requirements
    size_t bytes_per_sample = (bits_per_sample + 7) / 8;
    size_t bytes_per_pixel = bytes_per_sample * samples_per_pixel;
    size_t scanline_size = width * bytes_per_pixel;
    
    // Adjust scanline_size to be at least 1 to avoid division by zero
    if (scanline_size == 0) scanline_size = 1;
    
    // Calculate number of scanlines we can write with available pixel data
    size_t max_scanlines = std::min(pixel_data.size() / scanline_size, static_cast<size_t>(10));
    if (max_scanlines == 0) max_scanlines = 1;

    // Calculate strip/tile parameters
    uint32_t strips_per_image = 0;
    uint32_t tiles_per_image = 0;
    
    if (use_tiles) {
        uint32_t tiles_across = (width + tile_width - 1) / tile_width;
        uint32_t tiles_down = (height + tile_height - 1) / tile_height;
        tiles_per_image = tiles_across * tiles_down;
    } else {
        strips_per_image = TIFFNumberOfStrips(tif);
    }

    // Perform core writing operations based on selected mode
    tmsize_t bytes_written = 0;
    
    switch (writing_mode) {
        case 0: { // TIFFWriteEncodedStrip
            if (!use_tiles && strips_per_image > 0) {
                uint32_t strip_to_write = 0;
                if (strips_per_image > 1) {
                    strip_to_write = fdp.ConsumeIntegralInRange<uint32_t>(0, strips_per_image - 1);
                }
                
                // Calculate strip size
                tmsize_t strip_size = TIFFStripSize(tif);
                if (strip_size > 0) {
                    size_t data_to_use = std::min(pixel_data.size(), static_cast<size_t>(strip_size));
                    bytes_written = TIFFWriteEncodedStrip(tif, strip_to_write, pixel_data.data(), data_to_use);
                }
            }
            break;
        }
        
        case 1: { // TIFFWriteRawStrip
            if (!use_tiles && strips_per_image > 0) {
                uint32_t strip_to_write = 0;
                if (strips_per_image > 1) {
                    strip_to_write = fdp.ConsumeIntegralInRange<uint32_t>(0, strips_per_image - 1);
                }
                
                // Calculate raw strip size
                tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip_to_write);
                if (raw_strip_size > 0) {
                    size_t data_to_use = std::min(pixel_data.size(), static_cast<size_t>(raw_strip_size));
                    bytes_written = TIFFWriteRawStrip(tif, strip_to_write, pixel_data.data(), data_to_use);
                }
            }
            break;
        }
        
        case 2: { // TIFFWriteEncodedTile
            if (use_tiles && tiles_per_image > 0) {
                uint32_t tile_to_write = 0;
                if (tiles_per_image > 1) {
                    tile_to_write = fdp.ConsumeIntegralInRange<uint32_t>(0, tiles_per_image - 1);
                }
                
                // Calculate tile size
                tmsize_t tile_size = TIFFTileSize(tif);
                if (tile_size > 0) {
                    size_t data_to_use = std::min(pixel_data.size(), static_cast<size_t>(tile_size));
                    bytes_written = TIFFWriteEncodedTile(tif, tile_to_write, pixel_data.data(), data_to_use);
                }
            }
            break;
        }
        
        case 3: { // TIFFWriteRawTile
            if (use_tiles && tiles_per_image > 0) {
                uint32_t tile_to_write = 0;
                if (tiles_per_image > 1) {
                    tile_to_write = fdp.ConsumeIntegralInRange<uint32_t>(0, tiles_per_image - 1);
                }
                
                // Calculate tile size using TIFFTileSize (same as for encoded tile)
                tmsize_t tile_size = TIFFTileSize(tif);
                if (tile_size > 0) {
                    size_t data_to_use = std::min(pixel_data.size(), static_cast<size_t>(tile_size));
                    bytes_written = TIFFWriteRawTile(tif, tile_to_write, pixel_data.data(), data_to_use);
                }
            }
            break;
        }
        
        case 4: { // TIFFWriteScanline
            for (size_t i = 0; i < std::min(max_scanlines, static_cast<size_t>(3)); i++) {
                uint32_t row = i % height;
                size_t offset = i * scanline_size;
                if (offset + scanline_size <= pixel_data.size()) {
                    int result = TIFFWriteScanline(tif, pixel_data.data() + offset, row, 0);
                    if (result < 0) {
                        break;
                    }
                }
            }
            bytes_written = 1; // Mark as attempted
            break;
        }
        
        case 5: { // TIFFWriteTile
            if (use_tiles && tiles_per_image > 0) {
                // Write to a specific tile position
                uint32_t tile_x = 0;
                uint32_t tile_y = 0;
                if (width > tile_width) {
                    tile_x = fdp.ConsumeIntegralInRange<uint32_t>(0, (width - 1) / tile_width);
                }
                if (height > tile_height) {
                    tile_y = fdp.ConsumeIntegralInRange<uint32_t>(0, (height - 1) / tile_height);
                }
                
                tmsize_t tile_size = TIFFTileSize(tif);
                if (tile_size > 0 && tile_size <= static_cast<tmsize_t>(pixel_data.size())) {
                    bytes_written = TIFFWriteTile(tif, pixel_data.data(), tile_x * tile_width, tile_y * tile_height, 0, 0);
                }
            }
            break;
        }
    }

    // Test TIFFUnsetField for field manipulation (additional uncovered API)
    bool test_unset_field = fdp.ConsumeBool();
    if (test_unset_field && bytes_written > 0) {
        TIFFUnsetField(tif, TIFFTAG_ORIENTATION);
    }

    // Test TIFFRasterScanlineSize64 for size calculations (additional uncovered API)
    if (bytes_written > 0) {
        uint64_t raster_size = TIFFRasterScanlineSize64(tif);
        (void)raster_size; // Use variable to avoid compiler warning
    }

    // Flush data to ensure it's written to disk
    if (bytes_written > 0) {
        TIFFFlush(tif);
        TIFFFlushData(tif);
    }

    // Write directory to file
    if (bytes_written > 0) {
        TIFFWriteDirectory(tif);
    }

    // Close TIFF file
    TIFFClose(tif);

    // Clean up temporary file
    unlink(filename);

    return 0;
}

} // namespace harness_011
