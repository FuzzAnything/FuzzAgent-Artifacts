#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>

namespace harness_000 {
/*
 * First fuzzing harness for libtiff library
 * Targets basic TIFF file operations: TIFFOpen, TIFFReadDirectory, and basic reading functions
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
    // Use a fixed filename to avoid issues with file creation
    const char* filename = "/tmp/fuzzed_tiff.tif";
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
    // Use different modes based on fuzzed input
    uint8_t mode_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
    const char* mode = "r";
    switch (mode_selector) {
        case 0:
            mode = "r";  // Read mode
            break;
        case 1:
            mode = "r+"; // Read/write mode
            break;
        case 2:
            mode = "w";  // Write mode (though we won't actually write)
            break;
    }
    
    TIFF* tif = TIFFOpen(filename, mode);
    if (!tif) {
        // Clean up temp file
        remove(filename);
        return 0;
    }
    
    // Test basic TIFF properties retrieval
    uint32_t width = 0, height = 0;
    uint16_t samples_per_pixel = 0, bits_per_sample = 0;
    uint16_t photometric = 0, compression = 0;
    
    // Try to get various TIFF fields - these may or may not succeed depending on file content
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    
    // Test directory reading operations
    // Determine how many directories to try to read based on fuzzed input
    uint8_t max_dirs = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);
    int dirs_read = 0;
    
    // Read through multiple directories if they exist
    do {
        dirs_read++;
        
        // Test getting strip/tile information
        if (TIFFIsTiled(tif)) {
            uint32_t tile_width = 0, tile_length = 0;
            TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length);
            
            // Try to read a small tile if conditions are reasonable
            if (tile_width > 0 && tile_length > 0 && tile_width <= 1024 && tile_length <= 1024) {
                tmsize_t tile_size = TIFFTileSize(tif);
                if (tile_size > 0 && tile_size <= 65536) {
                    void* tile_buffer = _TIFFmalloc(tile_size);
                    if (tile_buffer) {
                        // Consume some fuzzed data to determine which tile to read
                        uint32_t tile_x = fdp.ConsumeIntegralInRange<uint32_t>(0, width > 0 ? width - 1 : 0);
                        uint32_t tile_y = fdp.ConsumeIntegralInRange<uint32_t>(0, height > 0 ? height - 1 : 0);
                        TIFFReadTile(tif, tile_buffer, tile_x, tile_y, 0, 0);
                        _TIFFfree(tile_buffer);
                    }
                }
            }
        } else {
            // Not tiled - try strip operations
            tstrile_t strip_count = TIFFNumberOfStrips(tif);
            if (strip_count > 0) {
                // Try to read a random strip
                tstrile_t strip_index = fdp.ConsumeIntegralInRange<tstrile_t>(0, strip_count - 1);
                tmsize_t strip_size = TIFFStripSize(tif);
                if (strip_size > 0 && strip_size <= 65536) {
                    void* strip_buffer = _TIFFmalloc(strip_size);
                    if (strip_buffer) {
                        TIFFReadEncodedStrip(tif, strip_index, strip_buffer, strip_size);
                        _TIFFfree(strip_buffer);
                    }
                }
            }
        }
        
        // Test scanline reading for non-tiled images
        if (!TIFFIsTiled(tif) && height > 0 && height <= 1000) {
            tmsize_t scanline_size = TIFFScanlineSize(tif);
            if (scanline_size > 0 && scanline_size <= 65536) {
                void* scanline_buffer = _TIFFmalloc(scanline_size);
                if (scanline_buffer) {
                    // Try to read a few random scanlines
                    uint8_t num_scanlines = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
                    for (uint8_t i = 0; i < num_scanlines && i < height; i++) {
                        uint32_t row = fdp.ConsumeIntegralInRange<uint32_t>(0, height - 1);
                        TIFFReadScanline(tif, scanline_buffer, row, 0);
                    }
                    _TIFFfree(scanline_buffer);
                }
            }
        }
        
    } while (dirs_read < max_dirs && TIFFReadDirectory(tif));
    
    // Test TIFF properties that don't require valid image data
    uint16_t planar_config = 0;
    uint16_t orientation = 0;
    float x_resolution = 0.0f, y_resolution = 0.0f;
    
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar_config);
    TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation);
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &x_resolution);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &y_resolution);
    
    // Test getting string fields if they exist
    char* document_name = nullptr;
    char* image_description = nullptr;
    char* software = nullptr;
    
    TIFFGetField(tif, TIFFTAG_DOCUMENTNAME, &document_name);
    TIFFGetField(tif, TIFFTAG_IMAGEDESCRIPTION, &image_description);
    TIFFGetField(tif, TIFFTAG_SOFTWARE, &software);
    
    // Test TIFFCurrentDirectory and TIFFLastDirectory
    tdir_t current_dir = TIFFCurrentDirectory(tif);
    int is_last = TIFFLastDirectory(tif);
    
    // Clean up
    TIFFClose(tif);
    
    // Remove temporary file
    remove(filename);
    
    return 0;
}

} // namespace harness_000
