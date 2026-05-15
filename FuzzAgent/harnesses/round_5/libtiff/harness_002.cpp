#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>  // For close(), unlink()
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>

namespace harness_002 {
/*
 * libtiff fuzzing harness for multi-directory operations and specialized features
 * Targets key TIFF APIs: TIFFCreateDirectory, TIFFSetDirectory, TIFFReadDirectory,
 * TIFFReadRGBAImage, and compression codec configuration
 * Semantic differentiation from harness_000 (reading) and harness_001 (writing):
 * - Focuses on multi-directory TIFF file manipulation
 * - Tests directory switching and creation
 * - Exercises TIFFReadRGBAImage for color transformation
 * - Tests various compression codecs in different directories
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

// Array of compression codecs to test
static const uint16_t compression_codecs[] = {
    COMPRESSION_NONE,
    COMPRESSION_LZW,
    COMPRESSION_JPEG,
    COMPRESSION_DEFLATE,
    COMPRESSION_ADOBE_DEFLATE,
    COMPRESSION_PACKBITS,
    COMPRESSION_CCITTRLE,
    COMPRESSION_CCITTFAX3,
    COMPRESSION_CCITTFAX4,
    COMPRESSION_ZSTD,
    COMPRESSION_LZMA,
    COMPRESSION_WEBP
};
static const size_t num_compression_codecs = sizeof(compression_codecs) / sizeof(compression_codecs[0]);

// Array of photometric interpretations
static const uint16_t photometric_options[] = {
    PHOTOMETRIC_MINISWHITE,
    PHOTOMETRIC_MINISBLACK,
    PHOTOMETRIC_RGB,
    PHOTOMETRIC_PALETTE,
    PHOTOMETRIC_MASK,
    PHOTOMETRIC_SEPARATED,
    PHOTOMETRIC_YCBCR,
    PHOTOMETRIC_CIELAB,
    PHOTOMETRIC_ICCLAB,
    PHOTOMETRIC_ITULAB,
    PHOTOMETRIC_LOGL,
    PHOTOMETRIC_LOGLUV
};
static const size_t num_photometric_options = sizeof(photometric_options) / sizeof(photometric_options[0]);

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Set silent error handlers to avoid output noise
    TIFFSetErrorHandler(silent_error_handler);
    TIFFSetWarningHandler(silent_error_handler);
    
    // Minimum input size check
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a temporary file for TIFF operations
    std::string temp_filename = "/tmp/libtiff_fuzz_multi_dir_XXXXXX";
    int fd = mkstemp(&temp_filename[0]);
    if (fd < 0) {
        return 0;
    }
    close(fd);
    
    // Consume basic parameters
    uint8_t num_directories = fdp.ConsumeIntegralInRange<uint8_t>(1, 4);
    bool test_read_rgba = fdp.ConsumeBool();
    bool test_compression_variation = fdp.ConsumeBool();
    
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(temp_filename.c_str(), "w");
    if (!tif) {
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // Create multiple directories with different configurations
    for (uint8_t dir_idx = 0; dir_idx < num_directories; dir_idx++) {
        if (dir_idx > 0) {
            // Create a new directory after the first one
            int create_result = TIFFCreateDirectory(tif);
            if (create_result != 1) {
                // If creation fails, continue with existing directory
                TIFFClose(tif);
                unlink(temp_filename.c_str());
                return 0;
            }
        }
        
        // Consume directory-specific parameters
        uint16_t image_width = fdp.ConsumeIntegralInRange<uint16_t>(1, 512);
        uint16_t image_height = fdp.ConsumeIntegralInRange<uint16_t>(1, 512);
        uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
        uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
        
        // Select photometric interpretation
        uint16_t photometric_idx = fdp.ConsumeIntegralInRange<uint16_t>(0, num_photometric_options - 1);
        uint16_t photometric = photometric_options[photometric_idx];
        
        // Select compression codec if testing variation
        uint16_t compression = COMPRESSION_NONE;
        if (test_compression_variation) {
            uint16_t comp_idx = fdp.ConsumeIntegralInRange<uint16_t>(0, num_compression_codecs - 1);
            compression = compression_codecs[comp_idx];
        }
        
        // Set basic TIFF fields for this directory
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_height);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
        
        // Set rows per strip for stripped images
        uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 64);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
        
        // Write some dummy data to the directory
        if (image_width > 0 && image_height > 0 && samples_per_pixel > 0 && bits_per_sample > 0) {
            // Calculate scanline size
            size_t bytes_per_sample = (bits_per_sample + 7) / 8;
            size_t scanline_size = image_width * samples_per_pixel * bytes_per_sample;
            
            // Consume buffer data for writing
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(scanline_size, scanline_size * 2);
            if (buffer_size > 0 && fdp.remaining_bytes() >= buffer_size) {
                std::vector<uint8_t> write_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
                
                if (write_buffer.size() >= scanline_size) {
                    // Write a few scanlines
                    uint32_t num_scanlines_to_write = fdp.ConsumeIntegralInRange<uint32_t>(1, 
                        std::min(image_height, static_cast<uint16_t>(4)));
                    
                    for (uint32_t row = 0; row < num_scanlines_to_write && row < image_height; row++) {
                        TIFFWriteScanline(tif, write_buffer.data(), row, 0);
                    }
                }
            }
        }
        
        // Write directory
        TIFFWriteDirectory(tif);
    }
    
    // Close and reopen for reading to test directory manipulation
    TIFFClose(tif);
    tif = TIFFOpen(temp_filename.c_str(), "r");
    if (!tif) {
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // Test directory switching and reading
    uint16_t actual_directories = TIFFNumberOfDirectories(tif);
    if (actual_directories > 0) {
        // Switch through directories
        for (tdir_t dir_num = 0; dir_num < actual_directories; dir_num++) {
            int set_result = TIFFSetDirectory(tif, dir_num);
            if (set_result != 1) {
                continue;
            }
            
            // Read directory information
            int read_result = TIFFReadDirectory(tif);
            if (read_result != 1) {
                continue;
            }
            
            // Get current directory info
            tdir_t current_dir = TIFFCurrentDirectory(tif);
            (void)current_dir; // Suppress unused warning
            
            // Read image parameters
            uint32_t width = 0, height = 0;
            TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
            
            // Test TIFFReadRGBAImage if requested and parameters are suitable
            if (test_read_rgba && width > 0 && height > 0 && width <= 256 && height <= 256) {
                uint32_t* raster = (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
                if (raster != nullptr) {
                    // Try to read RGBA image
                    int rgba_result = TIFFReadRGBAImage(tif, width, height, raster, 0);
                    (void)rgba_result; // Suppress unused warning
                    
                    _TIFFfree(raster);
                }
            }
            
            // Test reading a scanline
            if (width > 0 && height > 0) {
                size_t scanline_size = TIFFScanlineSize(tif);
                if (scanline_size > 0 && scanline_size <= 65536) {
                    std::vector<uint8_t> scanline_buffer(scanline_size);
                    if (scanline_buffer.size() >= scanline_size) {
                        uint32_t row_to_read = fdp.ConsumeIntegralInRange<uint32_t>(0, height - 1);
                        TIFFReadScanline(tif, scanline_buffer.data(), row_to_read, 0);
                    }
                }
            }
        }
        
        // Test random directory access
        if (actual_directories > 1) {
            uint8_t random_dir = fdp.ConsumeIntegralInRange<uint8_t>(0, actual_directories - 1);
            TIFFSetDirectory(tif, random_dir);
            TIFFReadDirectory(tif);
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink(temp_filename.c_str());
    
    return 0;
}

} // namespace harness_002
