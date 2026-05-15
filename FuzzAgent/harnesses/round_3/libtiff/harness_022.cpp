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

namespace harness_022 {
/* Fuzzing harness for libtiff targeting comprehensive RGBA image reading operations
 * Focus: Implement exact invocation sequence from coverage guidance for all RGBA reading functions
 * Critical Coverage Gap: 800+ undiscovered branches in tif_getimage.c (30% branch coverage)
 * Target APIs (exact sequence from guidance):
 *   1. TIFFOpen/TIFFOpenExt/TIFFFdOpen - Open TIFF file from fuzzer input
 *   2. TIFFRGBAImageBegin - Initialize RGBA image processing context
 *   3. TIFFRGBAImageOK - Verify image can be processed
 *   4. TIFFRGBAImageGet - Retrieve image data
 *   5. Exercise RGBA reading operations based on image characteristics:
 *      - If tiled: call TIFFReadRGBATile and TIFFReadRGBATileExt
 *      - If stripped: call TIFFReadRGBAStrip and TIFFReadRGBAStripExt
 *      - Call TIFFReadRGBAImage and TIFFReadRGBAImageOriented with different orientations
 *   6. TIFFRGBAImageEnd - Free RGBA resources
 *   7. TIFFClose - Close TIFF file
 * Supporting APIs: TIFFRGBAImageBegin, TIFFRGBAImageOK, TIFFRGBAImageGet, TIFFRGBAImageEnd
 * Strategy: Follow exact guidance sequence, test all RGBA reading functions based on image type,
 *           implement complete RGBA image processing lifecycle with comprehensive error handling
 * Differentiation: Unlike harness_020 which focuses only on tile operations, this harness implements
 *                  the complete RGBA lifecycle sequence covering both tiled and stripped images,
 *                  plus orientation variants as specified in guidance
 * Target coverage: TIFFReadRGBATile (172 branches), TIFFReadRGBATileExt (172), TIFFReadRGBAImage (144),
 *                  TIFFReadRGBAImageOriented (144), TIFFReadRGBAStrip (146), TIFFReadRGBAStripExt (146)
 *                  Total: 800+ branches
 */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for TIFF file and configuration
    if (size < 512) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume test configuration parameters from fuzzer input
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0-3 different test scenarios
    uint8_t open_method = fdp.ConsumeIntegral<uint8_t>() % 3;    // 0: TIFFOpen, 1: TIFFOpenExt, 2: TIFFFdOpen
    int stop_on_error = fdp.ConsumeBool() ? 1 : 0;               // stop on error flag for RGBA APIs
    bool test_tiled_apis = fdp.ConsumeBool();                    // Test tiled APIs if image is tiled
    bool test_stripped_apis = fdp.ConsumeBool();                 // Test stripped APIs if image is stripped
    bool test_full_image_apis = fdp.ConsumeBool();               // Test full image APIs
    
    // Consume orientation for TIFFReadRGBAImageOriented
    int orientation = fdp.ConsumeIntegralInRange<int>(0, 7);     // ORIENTATION_TOPLEFT through ORIENTATION_BOTRIGHT
    
    // Consume remaining data as TIFF file content
    std::string tiff_data = fdp.ConsumeRemainingBytesAsString();
    
    // Create a temporary file for TIFF operations
    char filename[] = "/tmp/tiff_fuzz_rgba_comprehensive_XXXXXX.tif";
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
    
    // Step 1: Open TIFF file using the specified method
    TIFF* tif = nullptr;
    switch (open_method) {
        case 0:  // TIFFOpen
            tif = TIFFOpen(filename, "r");
            break;
        case 1:  // TIFFOpenExt
            {
                TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
                if (opts) {
                    // Set some options for testing
                    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, fdp.ConsumeBool());
                    tif = TIFFOpenExt(filename, "r", opts);
                    TIFFOpenOptionsFree(opts);
                } else {
                    tif = TIFFOpenExt(filename, "r", nullptr);
                }
            }
            break;
        case 2:  // TIFFFdOpen
            {
                // Re-open the file to get a file descriptor for TIFFFdOpen
                int fd2 = open(filename, O_RDONLY);
                if (fd2 >= 0) {
                    tif = TIFFFdOpen(fd2, filename, "r");
                    // Note: TIFFFdOpen takes ownership of the file descriptor
                } else {
                    tif = nullptr;
                }
            }
            break;
    }
    
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
    
    // Get image parameters to determine organization (tiled vs stripped)
    uint32_t image_width = 0, image_height = 0;
    uint32_t tile_width = 0, tile_length = 0;
    uint32_t rows_per_strip = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &image_width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &image_height);
    
    int is_tiled = TIFFIsTiled(tif);
    if (is_tiled) {
        TIFFGetFieldDefaulted(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetFieldDefaulted(tif, TIFFTAG_TILELENGTH, &tile_length);
    } else {
        TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
        if (rows_per_strip == 0 || rows_per_strip > image_height) {
            rows_per_strip = image_height;
        }
    }
    
    // Step 2: Initialize RGBA image processing context using TIFFRGBAImageBegin
    TIFFRGBAImage img;
    char begin_emsg[1024] = "";
    int rgba_begin_result = TIFFRGBAImageBegin(&img, tif, stop_on_error, begin_emsg);
    
    // Step 3: Verify image can be processed using TIFFRGBAImageOK
    char ok_emsg[1024] = "";
    int image_ok = TIFFRGBAImageOK(tif, ok_emsg);
    
    if (rgba_begin_result && image_ok) {
        // Step 4: Retrieve image data using TIFFRGBAImageGet
        // Allocate buffer for TIFFRGBAImageGet
        uint32_t* rgba_get_buffer = nullptr;
        size_t rgba_get_buffer_size = image_width * image_height;
        if (rgba_get_buffer_size > 0 && rgba_get_buffer_size < 16777216) {  // Limit to 16M pixels (~64MB for RGBA)
            rgba_get_buffer = (uint32_t*)_TIFFmalloc(rgba_get_buffer_size * sizeof(uint32_t));
            if (rgba_get_buffer) {
                // Call TIFFRGBAImageGet to retrieve image data
                int rgba_get_result = TIFFRGBAImageGet(&img, rgba_get_buffer, image_width, image_height);
                _TIFFfree(rgba_get_buffer);
            }
        }
        
        // Allocate buffer for full image APIs
        uint32_t* full_image_buffer = nullptr;
        size_t full_image_size = image_width * image_height;
        if (full_image_size > 0 && full_image_size < 16777216) {  // Limit to 16M pixels (~64MB for RGBA)
            full_image_buffer = (uint32_t*)_TIFFmalloc(full_image_size * sizeof(uint32_t));
        }
        
        // Step 5: Exercise RGBA reading operations based on image characteristics
        
        // Test full image APIs if enabled and buffer allocated
        if (test_full_image_apis && full_image_buffer) {
            // Test TIFFReadRGBAImage
            int rgba_image_result = TIFFReadRGBAImage(tif, image_width, image_height, full_image_buffer, 0);
            
            // Test TIFFReadRGBAImageOriented with different orientations
            int oriented_result = TIFFReadRGBAImageOriented(tif, image_width, image_height, full_image_buffer, orientation, 0);
        }
        
        // Test tiled APIs if image is tiled and enabled
        if (is_tiled && test_tiled_apis && tile_width > 0 && tile_length > 0) {
            // Calculate tile bounds
            uint32_t tiles_across = (image_width + tile_width - 1) / tile_width;
            uint32_t tiles_down = (image_height + tile_length - 1) / tile_length;
            
            if (tiles_across > 0 && tiles_down > 0) {
                // Test a few tiles (limit to prevent excessive memory/time)
                uint32_t max_tiles_to_test = std::min(static_cast<uint32_t>(4), tiles_across * tiles_down);
                
                // Allocate buffer for tile
                size_t tile_buffer_size = tile_width * tile_length;
                if (tile_buffer_size > 0 && tile_buffer_size < 4194304) {  // Limit to 4M pixels (~16MB)
                    uint32_t* tile_buffer = (uint32_t*)_TIFFmalloc(tile_buffer_size * sizeof(uint32_t));
                    
                    if (tile_buffer) {
                        for (uint32_t i = 0; i < max_tiles_to_test; i++) {
                            // Calculate tile coordinates
                            uint32_t tile_x = (i % tiles_across) * tile_width;
                            uint32_t tile_y = (i / tiles_across) * tile_length;
                            
                            // Test TIFFReadRGBATile
                            int tile_result = TIFFReadRGBATile(tif, tile_x, tile_y, tile_buffer);
                            
                            // Test TIFFReadRGBATileExt with stop_on_error flag
                            int tile_ext_result = TIFFReadRGBATileExt(tif, tile_x, tile_y, tile_buffer, stop_on_error);
                        }
                        
                        _TIFFfree(tile_buffer);
                    }
                }
            }
        }
        
        // Test stripped APIs if image is stripped and enabled
        if (!is_tiled && test_stripped_apis && rows_per_strip > 0) {
            // Calculate strip bounds
            uint32_t strips_per_image = (image_height + rows_per_strip - 1) / rows_per_strip;
            
            if (strips_per_image > 0) {
                // Test a few strips (limit to prevent excessive memory/time)
                uint32_t max_strips_to_test = std::min(static_cast<uint32_t>(4), strips_per_image);
                
                // Allocate buffer for strip
                size_t strip_buffer_size = image_width * rows_per_strip;
                if (strip_buffer_size > 0 && strip_buffer_size < 4194304) {  // Limit to 4M pixels (~16MB)
                    uint32_t* strip_buffer = (uint32_t*)_TIFFmalloc(strip_buffer_size * sizeof(uint32_t));
                    
                    if (strip_buffer) {
                        for (uint32_t i = 0; i < max_strips_to_test; i++) {
                            // Calculate strip row
                            uint32_t row = i * rows_per_strip;
                            
                            // Test TIFFReadRGBAStrip
                            int strip_result = TIFFReadRGBAStrip(tif, row, strip_buffer);
                            
                            // Test TIFFReadRGBAStripExt with stop_on_error flag
                            int strip_ext_result = TIFFReadRGBAStripExt(tif, row, strip_buffer, stop_on_error);
                        }
                        
                        _TIFFfree(strip_buffer);
                    }
                }
            }
        }
        
        // Free full image buffer if allocated
        if (full_image_buffer) {
            _TIFFfree(full_image_buffer);
        }
        
        // Step 6: Free RGBA resources using TIFFRGBAImageEnd
        TIFFRGBAImageEnd(&img);
    }
    
    // Step 7: Close TIFF file
    TIFFClose(tif);
    
    // Clean up temporary file
    unlink(filename);
    
    return 0;
}

} // namespace harness_022
