#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>

namespace harness_002 {
/*
 * Fuzzing harness for libtiff library focusing on RGBA image reading functions
 * Targets completely uncovered RGBA APIs: TIFFReadRGBAImage, TIFFReadRGBATileExt, 
 * TIFFReadRGBAStripExt, TIFFReadRGBATile, TIFFReadRGBAStrip and related RGBA reading functions
 * Tests various image configurations, tile/strip arrangements, and color spaces
 * Uses FuzzedDataProvider to process input dynamically
 */


// Error handler to suppress library error messages during fuzzing
extern "C" void handle_error(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need at least some data for meaningful testing
    if (size < 100) {
        return 0;
    }
    
    // Initialize FuzzedDataProvider for structured input processing
    FuzzedDataProvider fdp(data, size);
    
    // Set error handlers to suppress library messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
    
    // Create a temporary file with the fuzzed data
    const char* filename = "/tmp/fuzzed_rgba_tiff.tif";
    FILE* tmpfile = fopen(filename, "wb");
    if (!tmpfile) {
        return 0;
    }
    
    // Write fuzzed data to temporary file
    // Use a reasonable max size to prevent OOM
    size_t write_size = fdp.ConsumeIntegralInRange<size_t>(100, std::min(size, (size_t)1048576)); // Up to 1MB
    auto file_data = fdp.ConsumeBytes<uint8_t>(write_size);
    fwrite(file_data.data(), 1, file_data.size(), tmpfile);
    fclose(tmpfile);
    
    // Open the TIFF file for reading
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        // Clean up temp file
        remove(filename);
        return 0;
    }
    
    // Get basic image dimensions
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    // Test TIFFRGBAImageOK first
    char emsg[1024];
    int rgba_ok = TIFFRGBAImageOK(tif, emsg);
    
    // Only proceed with RGBA operations if the image is suitable
    if (rgba_ok && width > 0 && height > 0 && width <= 4096 && height <= 4096) {
        // Calculate buffer size for RGBA image (4 bytes per pixel)
        uint64_t rgba_buffer_size = (uint64_t)width * (uint64_t)height * 4;
        
        // Limit buffer size to prevent OOM
        if (rgba_buffer_size > 0 && rgba_buffer_size <= 16777216) { // 16MB max
            uint32_t* raster = (uint32_t*)_TIFFmalloc(rgba_buffer_size);
            if (raster) {
                // Test different RGBA reading strategies based on fuzzed input
                uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                switch (strategy) {
                    case 0: {
                        // Test TIFFReadRGBAImage with different orientations
                        uint8_t orientation = fdp.ConsumeIntegral<uint8_t>() % 8;
                        TIFFReadRGBAImage(tif, width, height, raster, orientation);
                        break;
                    }
                    case 1: {
                        // Test TIFFReadRGBAImageOriented
                        uint8_t req_orientation = fdp.ConsumeIntegral<uint8_t>() % 8;
                        uint8_t stop_on_error = fdp.ConsumeBool();
                        TIFFReadRGBAImageOriented(tif, width, height, raster, req_orientation, stop_on_error);
                        break;
                    }
                    case 2: {
                        // Test TIFFRGBAImage API directly
                        TIFFRGBAImage img;
                        char begin_emsg[1024];
                        if (TIFFRGBAImageBegin(&img, tif, 0, begin_emsg)) {
                            // Try to get different portions of the image
                            uint32_t rows_to_get = fdp.ConsumeIntegralInRange<uint32_t>(1, height);
                            rows_to_get = rows_to_get > height ? height : rows_to_get;
                            TIFFRGBAImageGet(&img, raster, width, rows_to_get);
                            TIFFRGBAImageEnd(&img);
                        }
                        break;
                    }
                    case 3: {
                        // Test tile/strip specific RGBA functions
                        if (TIFFIsTiled(tif)) {
                            // Test tile-based RGBA reading
                            uint32_t tile_width = 0, tile_length = 0;
                            TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
                            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length);
                            
                            if (tile_width > 0 && tile_length > 0) {
                                // Allocate buffer sized for a tile (not the whole image)
                                uint64_t tile_buffer_size = (uint64_t)tile_width * (uint64_t)tile_length * 4;
                                
                                // Limit buffer size to prevent OOM
                                if (tile_buffer_size > 0 && tile_buffer_size <= 16777216) { // 16MB max
                                    uint32_t* tile_raster = (uint32_t*)_TIFFmalloc(tile_buffer_size);
                                    if (tile_raster) {
                                        // Try to read a specific tile
                                        uint32_t max_col_tiles = (width + tile_width - 1) / tile_width;
                                        uint32_t max_row_tiles = (height + tile_length - 1) / tile_length;
                                        
                                        if (max_col_tiles > 0 && max_row_tiles > 0) {
                                            uint32_t col_tile = fdp.ConsumeIntegralInRange<uint32_t>(0, max_col_tiles - 1);
                                            uint32_t row_tile = fdp.ConsumeIntegralInRange<uint32_t>(0, max_row_tiles - 1);
                                            uint32_t col = col_tile * tile_width;
                                            uint32_t row = row_tile * tile_length;
                                            
                                            // Test both regular and Ext versions
                                            uint8_t use_ext = fdp.ConsumeBool();
                                            if (use_ext) {
                                                uint8_t stop_on_error = fdp.ConsumeBool();
                                                TIFFReadRGBATileExt(tif, col, row, tile_raster, stop_on_error);
                                            } else {
                                                TIFFReadRGBATile(tif, col, row, tile_raster);
                                            }
                                        }
                                        _TIFFfree(tile_raster);
                                    }
                                }
                            }
                        } else {
                            // Test strip-based RGBA reading
                            tstrile_t strip_count = TIFFNumberOfStrips(tif);
                            if (strip_count > 0) {
                                // Get strip size (rows per strip)
                                uint32_t strip_size = TIFFDefaultStripSize(tif, 0);
                                if (strip_size > 0 && strip_size <= 4096) { // Reasonable limit
                                    // Allocate buffer sized for a strip (not the whole image)
                                    uint64_t strip_buffer_size = (uint64_t)width * (uint64_t)strip_size * 4;
                                    
                                    // Limit buffer size to prevent OOM
                                    if (strip_buffer_size > 0 && strip_buffer_size <= 16777216) { // 16MB max
                                        uint32_t* strip_raster = (uint32_t*)_TIFFmalloc(strip_buffer_size);
                                        if (strip_raster) {
                                            tstrile_t strip_index = fdp.ConsumeIntegralInRange<tstrile_t>(0, strip_count - 1);
                                            uint32_t row = strip_index * strip_size;
                                            
                                            // Test both regular and Ext versions
                                            uint8_t use_ext = fdp.ConsumeBool();
                                            if (use_ext) {
                                                uint8_t stop_on_error = fdp.ConsumeBool();
                                                TIFFReadRGBAStripExt(tif, row, strip_raster, stop_on_error);
                                            } else {
                                                TIFFReadRGBAStrip(tif, row, strip_raster);
                                            }
                                            _TIFFfree(strip_raster);
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                } // End of switch statement
                
                // Process the raster data to explore more code paths
                if (raster) {
                    // Extract color components from some pixels
                    uint32_t num_pixels_to_check = fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
                    num_pixels_to_check = num_pixels_to_check > width * height ? width * height : num_pixels_to_check;
                    
                    for (uint32_t i = 0; i < num_pixels_to_check; i++) {
                        uint32_t pixel_index = fdp.ConsumeIntegralInRange<uint32_t>(0, width * height - 1);
                        uint32_t pixel = raster[pixel_index];
                        
                        // Use the TIFFGet macros to extract components
                        uint8_t r = TIFFGetR(pixel);
                        uint8_t g = TIFFGetG(pixel);
                        uint8_t b = TIFFGetB(pixel);
                        uint8_t a = TIFFGetA(pixel);
                        
                        // Do something with the components to ensure they're used
                        (void)r; (void)g; (void)b; (void)a;
                    }
                }
                
                _TIFFfree(raster);
            }
        }
    }
    
    // Test additional RGBA-related functions even if image isn't fully suitable
    // Try TIFFReadRGBAImage with reduced dimensions for problematic images
    if (width > 0 && height > 0) {
        uint32_t test_width = width > 100 ? 100 : width;
        uint32_t test_height = height > 100 ? 100 : height;
        uint32_t test_buffer_size = test_width * test_height * 4;
        
        if (test_buffer_size > 0 && test_buffer_size <= 409600) { // 400KB max
            uint32_t* test_raster = (uint32_t*)_TIFFmalloc(test_buffer_size);
            if (test_raster) {
                // Try with stop_on_error flag set
                uint8_t stop_on_error = fdp.ConsumeBool();
                TIFFReadRGBAImageOriented(tif, test_width, test_height, test_raster, 
                                         ORIENTATION_TOPLEFT, stop_on_error);
                _TIFFfree(test_raster);
            }
        }
    }
    
    // Test getting photometric interpretation which affects RGBA conversion
    uint16_t photometric = 0;
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    
    // Test getting samples per pixel and bits per sample
    uint16_t samples_per_pixel = 0, bits_per_sample = 0;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    
    // Test planar configuration (affects RGBA reading for separate planes)
    uint16_t planar_config = 0;
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar_config);
    
    // Clean up
    TIFFClose(tif);
    
    // Remove temporary file
    remove(filename);
    
    return 0;
}

} // namespace harness_002
