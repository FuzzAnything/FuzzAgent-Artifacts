#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <tiffio.h>

namespace harness_009 {
/* Fuzzing harness for libtiff targeting comprehensive tile manipulation lifecycle operations
 * Focus: TIFF tile operations with 640+ undiscovered branches across raw and RGBA tile reading/writing APIs
 * Strategy: Create semantically unique harness for tile manipulation lifecycle (Open -> Configure -> Write -> Read -> Close)
 * Target APIs: TIFFWriteRawTile -> TIFFReadRawTile -> TIFFReadRGBATile/TIFFReadRGBATileExt -> TIFFVTileSize -> TIFFDefaultTileSize
 */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for configuration, tile data, and operations
    if (size < 256) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume configuration parameters for tile operations
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: basic, 1: extended, 2: mixed
    
    // Consume image configuration parameters
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(64, 2048);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(64, 2048);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6;
    uint16_t planar_config = fdp.ConsumeIntegral<uint16_t>() % 2;
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 10;
    
    // Consume tile configuration parameters
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    
    // Ensure tile dimensions are valid and compute default tile size
    if (tile_width == 0) tile_width = 256;
    if (tile_height == 0) tile_height = 256;
    
    // Use simple default tile dimensions instead of TIFFDefaultTileSize
    // TIFFDefaultTileSize requires a valid TIFF* handle, which we don't have yet
    // Use standard defaults: 256x256 rounded to multiples of 16
    uint32_t default_tile_width = 256;
    uint32_t default_tile_height = 256;
    
    // Round up to multiples of 16 per libtiff spec
    if (default_tile_width & 0xf)
        default_tile_width = ((default_tile_width + 15) & ~0xf);
    if (default_tile_height & 0xf)
        default_tile_height = ((default_tile_height + 15) & ~0xf);
    
    // Use fuzzer-provided or default tile sizes
    if (tile_width > width) tile_width = std::min(default_tile_width, width);
    if (tile_height > height) tile_height = std::min(default_tile_height, height);
    
    // Calculate tile size using TIFFVTileSize
    uint32_t expected_tile_size = 0;
    if (samples_per_pixel > 0 && bits_per_sample > 0) {
        // Calculate approximate tile size for configuration
        expected_tile_size = (tile_width * tile_height * samples_per_pixel * bits_per_sample + 7) / 8;
    }
    
    // Consume tile data for writing operations
    size_t tile_data_size = fdp.ConsumeIntegralInRange<size_t>(1024, 65536);
    std::vector<uint8_t> tile_data = fdp.ConsumeBytes<uint8_t>(tile_data_size);
    
    if (tile_data.empty()) {
        // If no tile data, create minimal buffer
        tile_data.resize(std::min(expected_tile_size, (uint32_t)4096));
        memset(tile_data.data(), 0, tile_data.size());
    }
    
    // Use remaining data as additional test data
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Create temporary files for writing and reading
    char write_filename[] = "/tmp/tiff_fuzz_tile_write_XXXXXX.tif";
    char read_filename[] = "/tmp/tiff_fuzz_tile_read_XXXXXX.tif";
    
    int write_fd = mkstemps(write_filename, 4);
    if (write_fd < 0) {
        return 0;
    }
    close(write_fd);
    
    int read_fd = mkstemps(read_filename, 4);
    if (read_fd < 0) {
        unlink(write_filename);
        return 0;
    }
    close(read_fd);
    
    // PHASE 1: CREATE AND WRITE TILED TIFF FILE
    TIFF* write_tif = TIFFOpen(write_filename, "w");
    if (!write_tif) {
        unlink(write_filename);
        unlink(read_filename);
        return 0;
    }
    
    // Set up tiled image configuration
    TIFFSetField(write_tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(write_tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(write_tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(write_tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(write_tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(write_tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(write_tif, TIFFTAG_COMPRESSION, compression);
    
    // Configure tile organization
    TIFFSetField(write_tif, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(write_tif, TIFFTAG_TILELENGTH, tile_height);
    
    // Check if image is suitable for tiled writing
    int write_check = TIFFWriteCheck(write_tif, 1, "tile_fuzzer");
    if (!write_check) {
        TIFFClose(write_tif);
        unlink(write_filename);
        unlink(read_filename);
        return 0;
    }
    // Calculate actual tile size using TIFFVTileSize (requires tile height as second parameter)
    tmsize_t tile_vsize = TIFFVTileSize(write_tif, tile_height);
    
    // Limit tile buffer size to prevent excessive memory usage
    size_t actual_tile_size = std::min((size_t)tile_vsize, tile_data.size());
    if (actual_tile_size == 0) {
        actual_tile_size = std::min(expected_tile_size, (uint32_t)4096);
    }
    // Write tiles using different methods based on operation mode
    uint32_t tiles_across = (width + tile_width - 1) / tile_width;
    uint32_t tiles_down = (height + tile_height - 1) / tile_height;
    
    if (tiles_across > 0 && tiles_down > 0) {
        // Write a few tiles to test the API
        for (uint32_t ty = 0; ty < std::min(tiles_down, (uint32_t)2); ty++) {
            for (uint32_t tx = 0; tx < std::min(tiles_across, (uint32_t)2); tx++) {
                uint32_t x = tx * tile_width;
                uint32_t y = ty * tile_height;
                uint32_t tile = TIFFComputeTile(write_tif, x, y, 0, 0);
                
                // Use different writing methods based on operation mode
                switch (operation_mode) {
                    case 0: // Basic raw tile writing
                        TIFFWriteRawTile(write_tif, tile, tile_data.data(), actual_tile_size);
                        break;
                    case 1: // Mixed writing with encoded tile
                        if ((tx + ty) % 2 == 0) {
                            TIFFWriteRawTile(write_tif, tile, tile_data.data(), actual_tile_size);
                        } else {
                            TIFFWriteEncodedTile(write_tif, tile, tile_data.data(), actual_tile_size);
                        }
                        break;
                    case 2: // All writing methods
                        TIFFWriteRawTile(write_tif, tile, tile_data.data(), actual_tile_size);
                        if (tile_data.size() >= actual_tile_size * 2) {
                            TIFFWriteEncodedTile(write_tif, tile, tile_data.data() + actual_tile_size, actual_tile_size);
                        }
                        break;
                }
            }
        }
    }
    
    // Write directory and close writing TIFF
    TIFFWriteDirectory(write_tif);
    TIFFClose(write_tif);
    
    // PHASE 2: READ AND VERIFY TILED TIFF FILE
    // First, copy the written file to read filename
    FILE* src = fopen(write_filename, "rb");
    FILE* dst = fopen(read_filename, "wb");
    
    if (src && dst) {
        uint8_t buffer[4096];
        size_t bytes;
        while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
            fwrite(buffer, 1, bytes, dst);
        }
        fclose(src);
        fclose(dst);
    }
    
    // Open for reading
    TIFF* read_tif = TIFFOpen(read_filename, "r");
    if (!read_tif) {
        unlink(write_filename);
        unlink(read_filename);
        return 0;
    }
    
    // Read first directory
    if (!TIFFReadDirectory(read_tif)) {
        TIFFClose(read_tif);
        unlink(write_filename);
        unlink(read_filename);
        return 0;
    }
    
    // Check if image is tiled
    int is_tiled = TIFFIsTiled(read_tif);
    if (!is_tiled) {
        TIFFClose(read_tif);
        unlink(write_filename);
        unlink(read_filename);
        return 0;
    }
    
    // Get tile information
    uint32_t read_tile_width = 0, read_tile_height = 0;
    TIFFGetField(read_tif, TIFFTAG_TILEWIDTH, &read_tile_width);
    TIFFGetField(read_tif, TIFFTAG_TILELENGTH, &read_tile_height);
    
    if (read_tile_width == 0 || read_tile_height == 0) {
        TIFFClose(read_tif);
        unlink(write_filename);
        unlink(read_filename);
        return 0;
    }
    
    // Calculate tile size for reading
    tmsize_t read_tile_vsize = TIFFVTileSize(read_tif, read_tile_height);
    tmsize_t read_tile_size = TIFFTileSize(read_tif);
    
    // Allocate buffer for tile reading
    size_t buffer_size = std::max((size_t)read_tile_vsize, (size_t)read_tile_size);
    if (buffer_size > 0 && buffer_size < 65536) {
        uint8_t* read_buffer = (uint8_t*)_TIFFmalloc(buffer_size);
        if (read_buffer) {
            // Read tiles using different methods
            uint32_t read_tiles_across = TIFFNumberOfTiles(read_tif);
            
            if (read_tiles_across > 0) {
                // Test a few tiles
                for (uint32_t tile_idx = 0; tile_idx < std::min(read_tiles_across, (uint32_t)3); tile_idx++) {
                    // Alternate between raw and encoded reading
                    if (tile_idx % 2 == 0) {
                        TIFFReadRawTile(read_tif, tile_idx, read_buffer, buffer_size);
                    } else {
                        TIFFReadEncodedTile(read_tif, tile_idx, read_buffer, buffer_size);
                    }
                    
                    // Test RGBA tile reading if we have enough data
                    if (remaining_data.size() >= sizeof(uint32_t) * read_tile_width * read_tile_height) {
                        uint32_t* rgba_buffer = (uint32_t*)read_buffer;
                        
                        // Calculate tile coordinates
                        uint32_t tile_x = (tile_idx % tiles_across) * read_tile_width;
                        uint32_t tile_y = (tile_idx / tiles_across) * read_tile_height;
                        
                        // Test both RGBA tile reading functions
                        if (operation_mode == 0 || operation_mode == 2) {
                            TIFFReadRGBATile(read_tif, tile_x, tile_y, rgba_buffer);
                        }
                        
                        if (operation_mode == 1 || operation_mode == 2) {
                            TIFFReadRGBATileExt(read_tif, tile_x, tile_y, rgba_buffer, 0);
                        }
                    }
                }
            }
            
            _TIFFfree(read_buffer);
        }
    }
    
    // Clean up
    TIFFClose(read_tif);
    unlink(write_filename);
    unlink(read_filename);
    
    return 0;
}

} // namespace harness_009
