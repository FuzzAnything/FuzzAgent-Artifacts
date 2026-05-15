#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <tiffio.h>

namespace harness_002 {
/* Fuzzing harness for libtiff targeting TIFF file writing operations */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for configuration and some pixel data
    if (size < 64) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume operation type to determine which writing path to test
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 4;

    // Consume configuration parameters for TIFF file
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 10; // Various compression schemes
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6; // Photometric interpretations
    uint16_t planar_config = fdp.ConsumeIntegral<uint16_t>() % 2; // PLANARCONFIG_CONTIG or SEPARATE
    uint16_t orientation = fdp.ConsumeIntegral<uint16_t>() % 8; // ORIENTATION_ values
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 256);

    // Create a temporary filename for writing
    char filename[] = "/tmp/tiff_fuzz_XXXXXX.tif";
    int fd = mkstemps(filename, 4);
    if (fd < 0) {
        return 0;
    }
    close(fd); // We'll open via TIFFOpen, not file descriptor

    // Allocate and configure TIFFOpenOptions
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (opts) {
        // Configure some options from fuzzer input
        tmsize_t max_mem = fdp.ConsumeIntegralInRange<tmsize_t>(1024, 1048576);
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_mem);
        TIFFOpenOptionsSetWarnAboutUnknownTags(opts, fdp.ConsumeBool());
    }

    // Open TIFF file for writing using TIFFOpenExt
    TIFF* tif = TIFFOpenExt(filename, "w", opts);
    if (!tif) {
        if (opts) TIFFOpenOptionsFree(opts);
        unlink(filename);
        return 0;
    }

    // Set basic image parameters using TIFFSetField
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);

    // Determine if we should use tiled or strip-based organization
    bool use_tiles = fdp.ConsumeBool();
    if (use_tiles) {
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    } else {
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    }

    // Check write permissions using TIFFWriteCheck
    int write_ok = TIFFWriteCheck(tif, use_tiles ? 1 : 0, "fuzzer");
    if (!write_ok) {
        TIFFClose(tif);
        if (opts) TIFFOpenOptionsFree(opts);
        unlink(filename);
        return 0;
    }

    // Calculate buffer size needed for pixel data
    size_t pixel_size = (bits_per_sample + 7) / 8;
    size_t scanline_size = width * samples_per_pixel * pixel_size;
    size_t tile_size = tile_width * tile_height * samples_per_pixel * pixel_size;
    size_t strip_size = scanline_size * rows_per_strip;

    // Consume remaining bytes for pixel data
    std::vector<uint8_t> pixel_buffer;
    size_t needed_size = 0;
    
    switch (operation_type) {
        case 0: // Tile writing operations
            needed_size = tile_size * 4; // Enough for a few tiles
            break;
        case 1: // Scanline writing operations  
            needed_size = scanline_size * std::min(height, (uint32_t)10);
            break;
        case 2: // Strip writing operations
            needed_size = strip_size * 2;
            break;
        case 3: // Raw writing operations
            needed_size = std::max(tile_size, strip_size) * 2;
            break;
    }

    if (needed_size > 0 && needed_size < 1048576) { // Limit to 1MB
        pixel_buffer = fdp.ConsumeBytes<uint8_t>(needed_size);
    } else {
        pixel_buffer = fdp.ConsumeRemainingBytes<uint8_t>();
    }

    if (pixel_buffer.empty()) {
        pixel_buffer.resize(std::min(scanline_size, (size_t)1024));
        memset(pixel_buffer.data(), 0, pixel_buffer.size());
    }

    // Perform writing operations based on operation_type
    switch (operation_type) {
        case 0: { // Tile writing
            if (use_tiles && pixel_buffer.size() >= tile_size) {
                // Write a few tiles using different tile writing functions
                uint32_t tiles_across = (width + tile_width - 1) / tile_width;
                uint32_t tiles_down = (height + tile_height - 1) / tile_height;
                
                for (uint32_t ty = 0; ty < std::min(tiles_down, (uint32_t)2); ty++) {
                    for (uint32_t tx = 0; tx < std::min(tiles_across, (uint32_t)2); tx++) {
                        uint32_t x = tx * tile_width;
                        uint32_t y = ty * tile_height;
                        uint32_t tile = TIFFComputeTile(tif, x, y, 0, 0);
                        
                        // Alternate between TIFFWriteTile and TIFFWriteEncodedTile
                        if ((tx + ty) % 2 == 0) {
                            TIFFWriteTile(tif, pixel_buffer.data(), x, y, 0, 0);
                        } else {
                            TIFFWriteEncodedTile(tif, tile, pixel_buffer.data(), tile_size);
                        }
                        
                        // Also test TIFFWriteRawTile with raw data
                        if (pixel_buffer.size() >= tile_size * 2) {
                            TIFFWriteRawTile(tif, tile, pixel_buffer.data() + tile_size, tile_size);
                        }
                    }
                }
            }
            break;
        }
        
        case 1: { // Scanline writing
            if (!use_tiles && pixel_buffer.size() >= scanline_size) {
                // Write a few scanlines
                for (uint32_t row = 0; row < std::min(height, (uint32_t)5); row++) {
                    TIFFWriteScanline(tif, pixel_buffer.data(), row, 0);
                }
            }
            break;
        }
        
        case 2: { // Strip writing
            if (!use_tiles && pixel_buffer.size() >= strip_size) {
                uint32_t strips = TIFFNumberOfStrips(tif);
                for (uint32_t strip = 0; strip < std::min(strips, (uint32_t)3); strip++) {
                    // Alternate between encoded and raw strip writing
                    if (strip % 2 == 0) {
                        TIFFWriteEncodedStrip(tif, strip, pixel_buffer.data(), strip_size);
                    } else {
                        TIFFWriteRawStrip(tif, strip, pixel_buffer.data(), strip_size);
                    }
                }
            }
            break;
        }
        
        case 3: { // Mixed raw writing operations
            // Test various raw writing functions
            if (use_tiles && pixel_buffer.size() >= tile_size) {
                uint32_t tile = TIFFComputeTile(tif, 0, 0, 0, 0);
                TIFFWriteRawTile(tif, tile, pixel_buffer.data(), std::min(tile_size, pixel_buffer.size()));
            } else if (!use_tiles && pixel_buffer.size() >= strip_size) {
                TIFFWriteRawStrip(tif, 0, pixel_buffer.data(), std::min(strip_size, pixel_buffer.size()));
            }
            break;
        }
    }

    // Write directory and close
    TIFFWriteDirectory(tif);
    TIFFClose(tif);
    
    // Clean up
    if (opts) TIFFOpenOptionsFree(opts);
    unlink(filename);
    
    return 0;
}

} // namespace harness_002
