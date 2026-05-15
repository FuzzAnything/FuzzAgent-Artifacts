#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>  // For close(), unlink(), mkstemp()
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>

namespace harness_005 {
/*
 * libtiff fuzzing harness for advanced RGB image reading operations
 * Targets critical RGBA tile/strip reading APIs with 0% coverage:
 * - TIFFReadRGBATile (159 branches) - tile-based RGBA reading
 * - TIFFReadRGBAStrip (143 branches) - strip-based RGBA reading
 * - TIFFReadRGBAStripExt (143 branches) - extended strip reading
 * - TIFFReadRGBATileExt (159 branches) - extended tile reading
 * Required helper APIs: TIFFOpen/TIFFClientOpen, TIFFRGBAImageBegin, 
 * TIFFRGBAImageEnd, TIFFClose
 * Semantic differentiation from existing harnesses:
 * - harness_000: general reading operations via TIFFStreamOpen
 * - harness_001: writing operations
 * - harness_002: multi-directory and compression
 * - harness_003: directory writing lifecycle
 * - harness_004: file opening/client I/O
 * - harness_005: Tile-based and strip-based RGBA image reading operations
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

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Set silent error handlers to avoid output noise
    TIFFSetErrorHandler(silent_error_handler);
    TIFFSetWarningHandler(silent_error_handler);
    
    // Minimum input size check - need enough for TIFF header + parameters
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a temporary file for TIFF data
    std::string temp_filename = "/tmp/libtiff_fuzz_rgba_XXXXXX";
    int fd = mkstemp(&temp_filename[0]);
    if (fd < 0) {
        return 0;
    }
    
    // Consume part of input for TIFF file content
    size_t tiff_data_size = fdp.ConsumeIntegralInRange<size_t>(64, std::min(size, MAX_FILE_SIZE));
    std::vector<uint8_t> tiff_data = fdp.ConsumeBytes<uint8_t>(tiff_data_size);
    
    // Write TIFF data to temporary file
    ssize_t written = write(fd, tiff_data.data(), tiff_data.size());
    close(fd);
    
    if (written != (ssize_t)tiff_data.size()) {
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // Consume parameters for RGBA operations
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint8_t stop_on_error = fdp.ConsumeBool() ? 1 : 0;
    
    // Open TIFF file
    TIFF* tif = TIFFOpen(temp_filename.c_str(), "r");
    if (!tif) {
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // Get image dimensions
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    // Check if file is tiled or striped
    int is_tiled = TIFFIsTiled(tif);
    uint32_t tile_width = 0, tile_height = 0;
    uint32_t rows_per_strip = 0;
    
    if (is_tiled) {
        TIFFGetFieldDefaulted(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetFieldDefaulted(tif, TIFFTAG_TILELENGTH, &tile_height);
    } else {
        TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    }
    
    // Allocate raster buffer for RGBA data
    // Maximum buffer size for safety: 4 bytes per pixel * max dimension
    size_t max_pixels = 0;
    if (is_tiled && tile_width > 0 && tile_height > 0) {
        max_pixels = tile_width * tile_height;
    } else if (rows_per_strip > 0 && width > 0) {
        max_pixels = width * rows_per_strip;
    } else if (width > 0 && height > 0) {
        max_pixels = width * height;
    } else {
        max_pixels = 1024 * 1024; // 1MP default
    }
    
    // Safety limit
    if (max_pixels > 4096 * 4096) {
        max_pixels = 4096 * 4096;
    }
    
    std::vector<uint32_t> raster(max_pixels, 0);
    
    // Perform RGBA operations based on operation_type
    int result = 0;
    
    switch (operation_type) {
        case 0: // TIFFReadRGBATile (if tiled)
            if (is_tiled && tile_width > 0 && tile_height > 0 && width > 0 && height > 0) {
                // Calculate valid tile position (must be on tile boundary)
                uint32_t max_col = (width / tile_width) * tile_width;
                uint32_t max_row = (height / tile_height) * tile_height;
                
                if (max_col > 0 && max_row > 0) {
                    uint32_t col = fdp.ConsumeIntegralInRange<uint32_t>(0, max_col - 1);
                    uint32_t row = fdp.ConsumeIntegralInRange<uint32_t>(0, max_row - 1);
                    
                    // Adjust to tile boundary
                    col = (col / tile_width) * tile_width;
                    row = (row / tile_height) * tile_height;
                    
                    result = TIFFReadRGBATile(tif, col, row, raster.data());
                }
            }
            break;
            
        case 1: // TIFFReadRGBATileExt with stop_on_error
            if (is_tiled && tile_width > 0 && tile_height > 0 && width > 0 && height > 0) {
                uint32_t max_col = (width / tile_width) * tile_width;
                uint32_t max_row = (height / tile_height) * tile_height;
                
                if (max_col > 0 && max_row > 0) {
                    uint32_t col = fdp.ConsumeIntegralInRange<uint32_t>(0, max_col - 1);
                    uint32_t row = fdp.ConsumeIntegralInRange<uint32_t>(0, max_row - 1);
                    
                    col = (col / tile_width) * tile_width;
                    row = (row / tile_height) * tile_height;
                    
                    result = TIFFReadRGBATileExt(tif, col, row, raster.data(), stop_on_error);
                }
            }
            break;
            
        case 2: // TIFFReadRGBAStrip (if striped)
            if (!is_tiled && rows_per_strip > 0 && height > 0) {
                // Calculate valid strip position (must be on strip boundary)
                uint32_t max_row = (height / rows_per_strip) * rows_per_strip;
                
                if (max_row > 0) {
                    uint32_t row = fdp.ConsumeIntegralInRange<uint32_t>(0, max_row - 1);
                    row = (row / rows_per_strip) * rows_per_strip;
                    
                    result = TIFFReadRGBAStrip(tif, row, raster.data());
                }
            }
            break;
            
        case 3: // TIFFReadRGBAStripExt with stop_on_error
            if (!is_tiled && rows_per_strip > 0 && height > 0) {
                uint32_t max_row = (height / rows_per_strip) * rows_per_strip;
                
                if (max_row > 0) {
                    uint32_t row = fdp.ConsumeIntegralInRange<uint32_t>(0, max_row - 1);
                    row = (row / rows_per_strip) * rows_per_strip;
                    
                    result = TIFFReadRGBAStripExt(tif, row, raster.data(), stop_on_error);
                }
            }
            break;
            
        case 4: // Test TIFFRGBAImageBegin/TIFFRGBAImageEnd directly
            {
                TIFFRGBAImage img;
                char emsg[1024];
                
                if (TIFFRGBAImageBegin(&img, tif, stop_on_error, emsg)) {
                    // Successfully began RGBA image
                    TIFFRGBAImageEnd(&img);
                    result = 1;
                }
            }
            break;
            
        case 5: // Test multiple operations in sequence
            if (is_tiled && tile_width > 0 && tile_height > 0 && width > 0 && height > 0) {
                uint32_t max_col = (width / tile_width) * tile_width;
                uint32_t max_row = (height / tile_height) * tile_height;
                
                if (max_col > 0 && max_row > 0) {
                    uint32_t col = fdp.ConsumeIntegralInRange<uint32_t>(0, max_col - 1);
                    uint32_t row = fdp.ConsumeIntegralInRange<uint32_t>(0, max_row - 1);
                    
                    col = (col / tile_width) * tile_width;
                    row = (row / tile_height) * tile_height;
                    
                    // Test both tile functions
                    int result1 = TIFFReadRGBATile(tif, col, row, raster.data());
                    int result2 = TIFFReadRGBATileExt(tif, col, row, raster.data(), stop_on_error);
                    result = result1 || result2;
                }
            } else if (!is_tiled && rows_per_strip > 0 && height > 0) {
                uint32_t max_row = (height / rows_per_strip) * rows_per_strip;
                
                if (max_row > 0) {
                    uint32_t row = fdp.ConsumeIntegralInRange<uint32_t>(0, max_row - 1);
                    row = (row / rows_per_strip) * rows_per_strip;
                    
                    // Test both strip functions
                    int result1 = TIFFReadRGBAStrip(tif, row, raster.data());
                    int result2 = TIFFReadRGBAStripExt(tif, row, raster.data(), stop_on_error);
                    result = result1 || result2;
                }
            }
            break;
            
        case 6: // Test error paths with invalid coordinates
            if (is_tiled) {
                // Pass intentionally invalid coordinates to test error handling
                result = TIFFReadRGBATile(tif, width + 100, height + 100, raster.data());
            } else {
                result = TIFFReadRGBAStrip(tif, height + 100, raster.data());
            }
            // We expect 0 (failure) for invalid coordinates
            break;
            
        case 7: // Test with various stop_on_error values
            if (is_tiled && tile_width > 0 && tile_height > 0 && width > 0 && height > 0) {
                uint32_t max_col = (width / tile_width) * tile_width;
                uint32_t max_row = (height / tile_height) * tile_height;
                
                if (max_col > 0 && max_row > 0) {
                    uint32_t col = fdp.ConsumeIntegralInRange<uint32_t>(0, max_col - 1);
                    uint32_t row = fdp.ConsumeIntegralInRange<uint32_t>(0, max_row - 1);
                    
                    col = (col / tile_width) * tile_width;
                    row = (row / tile_height) * tile_height;
                    
                    // Test with alternating stop_on_error values
                    int stop1 = fdp.ConsumeBool() ? 1 : 0;
                    result = TIFFReadRGBATileExt(tif, col, row, raster.data(), stop1);
                    
                    if (fdp.remaining_bytes() > 0) {
                        int stop2 = fdp.ConsumeBool() ? 1 : 0;
                        // Use different coordinates if available
                        if (col + tile_width < max_col) {
                            result |= TIFFReadRGBATileExt(tif, col + tile_width, row, raster.data(), stop2);
                        }
                    }
                }
            } else if (!is_tiled && rows_per_strip > 0 && height > 0) {
                uint32_t max_row = (height / rows_per_strip) * rows_per_strip;
                
                if (max_row > 0) {
                    uint32_t row = fdp.ConsumeIntegralInRange<uint32_t>(0, max_row - 1);
                    row = (row / rows_per_strip) * rows_per_strip;
                    
                    int stop1 = fdp.ConsumeBool() ? 1 : 0;
                    result = TIFFReadRGBAStripExt(tif, row, raster.data(), stop1);
                    
                    if (fdp.remaining_bytes() > 0 && row + rows_per_strip < max_row) {
                        int stop2 = fdp.ConsumeBool() ? 1 : 0;
                        result |= TIFFReadRGBAStripExt(tif, row + rows_per_strip, raster.data(), stop2);
                    }
                }
            }
            break;
    }
    
    // Clean up
    TIFFClose(tif);
    unlink(temp_filename.c_str());
    
    // Use result to avoid unused variable warning
    (void)result;
    
    return 0;
}

} // namespace harness_005
