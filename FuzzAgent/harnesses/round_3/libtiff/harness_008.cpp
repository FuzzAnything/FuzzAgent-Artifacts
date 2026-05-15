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

namespace harness_008 {
/* Fuzzing harness for libtiff targeting systematic tile/strip RGBA image reading operations
 * Focus: Comprehensive RGBA image lifecycle with 333+ undiscovered branches in tif_getimage.c
 * Strategy: Test systematic RGBA image reading paths for both tiled and striped TIFF images
 * with varied configurations (orientation, stop_on_error, tile/strip coordinates, buffer sizes)
 */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for TIFF file and configuration
    if (size < 128) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume configuration parameters for systematic testing
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0: tile, 1: strip, 2: full image, 3: oriented
    bool use_extended = fdp.ConsumeBool();
    int stop_on_error = fdp.ConsumeIntegral<int>() % 2;
    uint16_t orientation = fdp.ConsumeIntegral<uint16_t>() % 8;
    
    // Consume tile/strip coordinates
    uint32_t tile_x = fdp.ConsumeIntegral<uint32_t>();
    uint32_t tile_y = fdp.ConsumeIntegral<uint32_t>();
    uint32_t strip_num = fdp.ConsumeIntegral<uint32_t>();
    
    // Consume image dimensions for full image reading
    uint32_t req_width = fdp.ConsumeIntegral<uint32_t>();
    uint32_t req_height = fdp.ConsumeIntegral<uint32_t>();
    
    // Limit dimensions to prevent excessive memory usage
    if (req_width > 4096) req_width = 4096;
    if (req_height > 4096) req_height = 4096;
    
    // Use remaining data as TIFF file content
    std::string tiff_data = fdp.ConsumeRemainingBytesAsString();
    
    // Create a temporary file for TIFF operations
    char filename[] = "/tmp/tiff_fuzz_rgba_systematic_XXXXXX.tif";
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
    
    // Read the first directory if available
    int dir_read = TIFFReadDirectory(tif);
    
    if (!dir_read) {
        TIFFClose(tif);
        unlink(filename);
        return 0;
    }
    
    // Test 1: TIFFRGBAImageOK - Validate image can be read as RGBA
    char emsg[1024];
    int ok_result = TIFFRGBAImageOK(tif, emsg);
    
    // If image is not suitable for RGBA reading, clean up and return
    if (!ok_result) {
        TIFFClose(tif);
        unlink(filename);
        return 0;
    }
    
    // Test 2: Initialize RGBA image context with TIFFRGBAImageBegin
    TIFFRGBAImage img;
    int begin_result = TIFFRGBAImageBegin(&img, tif, stop_on_error, emsg);
    
    if (!begin_result) {
        TIFFClose(tif);
        unlink(filename);
        return 0;
    }
    
    // Get actual image dimensions from TIFF
    uint32_t actual_width = 0, actual_height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &actual_width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &actual_height);
    
    // Test different RGBA reading operations based on test_mode
    switch (test_mode) {
        case 0: {  // Tile-based RGBA reading
            if (TIFFIsTiled(tif)) {
                uint32_t tile_width = 0, tile_length = 0;
                TIFFGetFieldDefaulted(tif, TIFFTAG_TILEWIDTH, &tile_width);
                TIFFGetFieldDefaulted(tif, TIFFTAG_TILELENGTH, &tile_length);
                
                if (tile_width > 0 && tile_length > 0) {
                    // Calculate valid tile coordinates
                    uint32_t valid_tile_x = (tile_x % (actual_width / tile_width + 1)) * tile_width;
                    uint32_t valid_tile_y = (tile_y % (actual_height / tile_length + 1)) * tile_length;
                    
                    // Allocate buffer for tile data
                    size_t tile_buffer_size = tile_width * tile_length * 4;
                    if (tile_buffer_size > 0 && tile_buffer_size < 16777216) {  // Limit to 16MB
                        uint32_t* tile_buffer = (uint32_t*)_TIFFmalloc(tile_buffer_size);
                        if (tile_buffer) {
                            if (use_extended) {
                                TIFFReadRGBATileExt(tif, valid_tile_x, valid_tile_y, tile_buffer, stop_on_error);
                            } else {
                                TIFFReadRGBATile(tif, valid_tile_x, valid_tile_y, tile_buffer);
                            }
                            _TIFFfree(tile_buffer);
                        }
                    }
                }
            }
            break;
        }
        
        case 1: {  // Strip-based RGBA reading
            uint32_t strips_per_image = TIFFNumberOfStrips(tif);
            if (strips_per_image > 0) {
                uint32_t rows_per_strip = 0;
                TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
                
                if (rows_per_strip > 0) {
                    uint32_t valid_strip = strip_num % strips_per_image;
                    uint32_t strip_height = rows_per_strip;
                    
                    // Adjust for last strip which might be shorter
                    if (valid_strip == strips_per_image - 1 && actual_height % rows_per_strip != 0) {
                        strip_height = actual_height % rows_per_strip;
                    }
                    
                    // Allocate buffer for strip data
                    size_t strip_buffer_size = actual_width * strip_height * 4;
                    if (strip_buffer_size > 0 && strip_buffer_size < 16777216) {  // Limit to 16MB
                        uint32_t* strip_buffer = (uint32_t*)_TIFFmalloc(strip_buffer_size);
                        if (strip_buffer) {
                            if (use_extended) {
                                TIFFReadRGBAStripExt(tif, valid_strip, strip_buffer, stop_on_error);
                            } else {
                                TIFFReadRGBAStrip(tif, valid_strip, strip_buffer);
                            }
                            _TIFFfree(strip_buffer);
                        }
                    }
                }
            }
            break;
        }
        
        case 2: {  // Full RGBA image reading
            // Use actual image dimensions or requested dimensions (whichever is smaller)
            uint32_t read_width = std::min(actual_width, req_width);
            uint32_t read_height = std::min(actual_height, req_height);
            
            if (read_width > 0 && read_height > 0) {
                size_t image_buffer_size = read_width * read_height * 4;
                if (image_buffer_size > 0 && image_buffer_size < 16777216) {  // Limit to 16MB
                    uint32_t* image_buffer = (uint32_t*)_TIFFmalloc(image_buffer_size);
                    if (image_buffer) {
                        TIFFReadRGBAImage(tif, read_width, read_height, image_buffer, stop_on_error);
                        _TIFFfree(image_buffer);
                    }
                }
            }
            break;
        }
        
        case 3: {  // Oriented RGBA image reading
            // Use actual image dimensions or requested dimensions (whichever is smaller)
            uint32_t read_width = std::min(actual_width, req_width);
            uint32_t read_height = std::min(actual_height, req_height);
            
            if (read_width > 0 && read_height > 0) {
                size_t image_buffer_size = read_width * read_height * 4;
                if (image_buffer_size > 0 && image_buffer_size < 16777216) {  // Limit to 16MB
                    uint32_t* image_buffer = (uint32_t*)_TIFFmalloc(image_buffer_size);
                    if (image_buffer) {
                        TIFFReadRGBAImageOriented(tif, read_width, read_height, image_buffer, 
                                                  orientation, stop_on_error);
                        _TIFFfree(image_buffer);
                    }
                }
            }
            break;
        }
    }
    
    // Test 3: TIFFRGBAImageGet - Get RGBA image data using the initialized context
    if (actual_width > 0 && actual_height > 0) {
        // Use reasonable dimensions to prevent excessive memory usage
        uint32_t get_width = std::min(actual_width, (uint32_t)256);
        uint32_t get_height = std::min(actual_height, (uint32_t)256);
        
        size_t get_buffer_size = get_width * get_height * 4;
        if (get_buffer_size > 0 && get_buffer_size < 16777216) {  // Limit to 16MB
            uint32_t* get_buffer = (uint32_t*)_TIFFmalloc(get_buffer_size);
            if (get_buffer) {
                TIFFRGBAImageGet(&img, get_buffer, get_width, get_height);
                _TIFFfree(get_buffer);
            }
        }
    }
    
    // Test 4: Clean up RGBA image context with TIFFRGBAImageEnd
    TIFFRGBAImageEnd(&img);
    
    // Try reading additional directories if available (multi-page TIFFs)
    while (TIFFReadDirectory(tif)) {
        // Read a few more directories to exercise multi-page TIFF handling
        static int max_additional_dirs = 2;
        if (--max_additional_dirs <= 0) break;
    }
    
    // Final cleanup
    TIFFClose(tif);
    unlink(filename);
    
    return 0;
}

} // namespace harness_008
