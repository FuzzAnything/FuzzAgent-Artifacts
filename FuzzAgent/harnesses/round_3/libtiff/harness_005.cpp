#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <tiffio.h>

namespace harness_005 {
/* Fuzzing harness for libtiff targeting TIFF strip and tile reading operations */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for configuration and TIFF file
    if (size < 128) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume configuration parameters for testing
    bool test_strip_reading = fdp.ConsumeBool();
    bool test_user_buffer = fdp.ConsumeBool();
    bool test_tile_reading = fdp.ConsumeBool();
    
    // Consume parameters for strip/tile operations
    uint32_t strip_num = fdp.ConsumeIntegral<uint32_t>();
    uint32_t tile_num = fdp.ConsumeIntegral<uint32_t>();
    
    // Consume buffer sizes for reading operations
    uint32_t strip_buffer_size = fdp.ConsumeIntegralInRange<uint32_t>(1, 65536);
    uint32_t tile_buffer_size = fdp.ConsumeIntegralInRange<uint32_t>(1, 65536);
    uint32_t user_buffer_in_size = fdp.ConsumeIntegralInRange<uint32_t>(1, 65536);
    uint32_t user_buffer_out_size = fdp.ConsumeIntegralInRange<uint32_t>(1, 65536);
    
    // Consume user buffer content BEFORE the TIFF data
    std::vector<uint8_t> user_buffer_content;
    if (test_user_buffer && user_buffer_in_size > 0) {
        user_buffer_content = fdp.ConsumeBytes<uint8_t>(user_buffer_in_size);
        // If we didn't get enough bytes for the buffer, disable this test
        if (user_buffer_content.size() < user_buffer_in_size) {
            test_user_buffer = false;
        }
    }
    
    // Use remaining data as TIFF file content
    std::string tiff_data = fdp.ConsumeRemainingBytesAsString();
    
    // Create a temporary file for TIFF operations
    char filename[] = "/tmp/tiff_fuzz_strip_tile_XXXXXX.tif";
    int fd = mkstemps(filename, 4);
    if (fd < 0) {
        return 0;
    }
    
    // Write TIFF data to the temporary file
    if (write(fd, tiff_data.c_str(), tiff_data.size()) != (ssize_t)tiff_data.size()) {
        close(fd);
        unlink(filename);
        return 0;
    }
    
    close(fd);
    
    // Open TIFF file for reading
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        unlink(filename);
        return 0;
    }
    
    // Determine if the TIFF is tiled or striped
    bool is_tiled = TIFFIsTiled(tif) != 0;
    uint32_t strips_per_image = TIFFNumberOfStrips(tif);
    uint32_t tiles_per_image = TIFFNumberOfTiles(tif);
    
    // Test TIFFReadEncodedStrip if enabled and applicable
    if (test_strip_reading && strips_per_image > 0 && !is_tiled) {
        // Ensure strip number is within bounds
        uint32_t actual_strip = strip_num % strips_per_image;
        
        // Allocate buffer for strip reading
        tmsize_t strip_size = TIFFRawStripSize(tif, actual_strip);
        if (strip_size > 0 && strip_size < strip_buffer_size) {
            uint8_t* strip_buffer = (uint8_t*)_TIFFmalloc(strip_size);
            if (strip_buffer) {
                // Read encoded strip
                TIFFReadEncodedStrip(tif, actual_strip, strip_buffer, strip_size);
                _TIFFfree(strip_buffer);
            }
        }
    }
    
    // Test TIFFReadFromUserBuffer if enabled
    if (test_user_buffer && !user_buffer_content.empty()) {
        // Allocate input and output buffers
        uint8_t* user_in_buffer = (uint8_t*)_TIFFmalloc(user_buffer_in_size);
        uint8_t* user_out_buffer = (uint8_t*)_TIFFmalloc(user_buffer_out_size);
        
        if (user_in_buffer && user_out_buffer) {
            // Copy the pre-consumed buffer content
            memcpy(user_in_buffer, user_buffer_content.data(), user_buffer_in_size);
            
            // Determine which strile to read
            uint32_t strile_num = 0;
            if (is_tiled && tiles_per_image > 0) {
                strile_num = tile_num % tiles_per_image;
            } else if (!is_tiled && strips_per_image > 0) {
                strile_num = strip_num % strips_per_image;
            }
            
            // Try to read from user buffer if we have a valid strile
            if (strile_num > 0) {
                TIFFReadFromUserBuffer(tif, strile_num, user_in_buffer, 
                                      user_buffer_in_size, user_out_buffer, 
                                      user_buffer_out_size);
            }
            
            _TIFFfree(user_in_buffer);
            _TIFFfree(user_out_buffer);
        } else {
            if (user_in_buffer) _TIFFfree(user_in_buffer);
            if (user_out_buffer) _TIFFfree(user_out_buffer);
        }
    }
    
    // Test TIFFReadRawTile if enabled and applicable
    if (test_tile_reading && is_tiled && tiles_per_image > 0) {
        // Ensure tile number is within bounds
        uint32_t actual_tile = tile_num % tiles_per_image;
        
        // Allocate buffer for tile reading
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < tile_buffer_size) {
            uint8_t* tile_buffer = (uint8_t*)_TIFFmalloc(tile_size);
            if (tile_buffer) {
                // Read raw tile
                TIFFReadRawTile(tif, actual_tile, tile_buffer, tile_size);
                _TIFFfree(tile_buffer);
            }
        }
    }
    
    // Clean up
    TIFFClose(tif);
    unlink(filename);
    
    return 0;
}

} // namespace harness_005
