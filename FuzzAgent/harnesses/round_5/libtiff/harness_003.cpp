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

namespace harness_003 {
/*
 * libtiff fuzzing harness for directory writing operations
 * Targets key TIFF directory writing APIs: TIFFWriteDirectory, TIFFRewriteDirectory,
 * TIFFCheckpointDirectory, TIFFWriteCustomDirectory, TIFFPrintDirectory
 * Semantic differentiation from existing harnesses:
 * - harness_000: reading operations
 * - harness_001: basic writing operations (tiles/strips)
 * - harness_002: multi-directory and compression
 * - harness_003: directory manipulation lifecycle (create → configure → write → modify → rewrite → close)
 * Focuses on exercising the completely uncovered tif_dirwrite.c module (0% coverage)
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
    
    // Minimum input size check - need enough for configuration parameters
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a temporary file for TIFF operations
    std::string temp_filename = "/tmp/libtiff_fuzz_dir_write_XXXXXX";
    int fd = mkstemp(&temp_filename[0]);
    if (fd < 0) {
        return 0;
    }
    close(fd);
    
    // Consume operation type to determine testing strategy
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool test_custom_dirs = fdp.ConsumeBool();
    bool test_checkpoint = fdp.ConsumeBool();
    bool test_rewrite = fdp.ConsumeBool();
    bool test_print = fdp.ConsumeBool();
    
    // Consume basic image parameters
    uint16_t image_width = fdp.ConsumeIntegralInRange<uint16_t>(1, 1024);
    uint16_t image_height = fdp.ConsumeIntegralInRange<uint16_t>(1, 1024);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    
    // Select photometric interpretation
    uint16_t photometric_idx = fdp.ConsumeIntegralInRange<uint16_t>(0, num_photometric_options - 1);
    uint16_t photometric = photometric_options[photometric_idx];
    
    // Select compression codec
    uint16_t comp_idx = fdp.ConsumeIntegralInRange<uint16_t>(0, num_compression_codecs - 1);
    uint16_t compression = compression_codecs[comp_idx];
    
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(temp_filename.c_str(), "w");
    if (!tif) {
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // PHASE 1: Initial directory creation and configuration
    // Set basic TIFF fields for initial directory
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    // Set rows per strip
    uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 64);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    
    // Set additional optional fields based on fuzzer input
    if (fdp.ConsumeBool()) {
        uint16_t orientation = fdp.ConsumeIntegralInRange<uint16_t>(1, 8);
        TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
    }
    
    if (fdp.ConsumeBool()) {
        float x_resolution = fdp.ConsumeFloatingPoint<float>();
        float y_resolution = fdp.ConsumeFloatingPoint<float>();
        TIFFSetField(tif, TIFFTAG_XRESOLUTION, x_resolution);
        TIFFSetField(tif, TIFFTAG_YRESOLUTION, y_resolution);
    }
    
    // PHASE 2: Write initial directory using TIFFWriteDirectory
    if (!TIFFWriteDirectory(tif)) {
        TIFFClose(tif);
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // PHASE 3: Test directory rewriting with TIFFRewriteDirectory
    if (test_rewrite) {
        // Create a new directory
        if (TIFFCreateDirectory(tif)) {
            // Set some different fields for the new directory
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width / 2 + 1);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_height / 2 + 1);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
            
            // Write the directory
            if (!TIFFWriteDirectory(tif)) {
                TIFFClose(tif);
                unlink(temp_filename.c_str());
                return 0;
            }
            
            // Now go back and modify the directory
            if (TIFFSetDirectory(tif, 0)) {
                // Modify some fields
                TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width + 10);
                
                // Try to rewrite the directory
                if (!TIFFRewriteDirectory(tif)) {
                    // Note: This might fail for valid reasons, continue anyway
                }
            }
        }
    }
    
    // PHASE 4: Test checkpoint directory functionality
    if (test_checkpoint) {
        // Create another directory for checkpoint testing
        if (TIFFCreateDirectory(tif)) {
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 64);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 64);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            
            // Write some dummy scanline data if possible
            size_t scanline_size = 64 * 1; // width * samples_per_pixel * bytes_per_sample (8 bits = 1 byte)
            if (fdp.remaining_bytes() >= scanline_size) {
                std::vector<uint8_t> scanline = fdp.ConsumeBytes<uint8_t>(scanline_size);
                for (uint32_t row = 0; row < 64; row++) {
                    TIFFWriteScanline(tif, scanline.data(), row, 0);
                }
            }
            
            // Write directory first
            if (!TIFFWriteDirectory(tif)) {
                TIFFClose(tif);
                unlink(temp_filename.c_str());
                return 0;
            }
            
            // Try checkpointing
            if (!TIFFCheckpointDirectory(tif)) {
                // Note: This might fail for valid reasons, continue anyway
            }
        }
    }
    
    // PHASE 5: Test custom directory creation and writing
    if (test_custom_dirs) {
        // Try creating EXIF directory (specialized custom directory)
        if (TIFFCreateEXIFDirectory(tif)) {
            // EXIF directories have specific tags, try setting some
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 32);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 32);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 2);
            
            // Try writing custom directory
            uint64_t custom_dir_offset = 0;
            if (!TIFFWriteCustomDirectory(tif, &custom_dir_offset)) {
                // Note: Might fail, continue anyway
            }
        }
        
        // Try creating another EXIF directory with different settings
        if (TIFFCreateEXIFDirectory(tif)) {
            // EXIF directories have specific tags, try setting some
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 16);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 16);
            
            if (!TIFFWriteDirectory(tif)) {
                // Note: Might fail, continue anyway
            }
        }
        
        // Try creating GPS directory
        if (TIFFCreateGPSDirectory(tif)) {
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 8);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 8);
            
            if (!TIFFWriteDirectory(tif)) {
                // Note: Might fail, continue anyway
            }
        }
    }

    // PHASE 6: Test directory navigation and printing
    if (test_print) {
        // Close and reopen file for reading to test TIFFPrintDirectory
        TIFFClose(tif);
        tif = TIFFOpen(temp_filename.c_str(), "r");
        
        if (tif) {
            // Count directories
            uint16_t num_dirs = TIFFNumberOfDirectories(tif);
            
            // Try to print directory information for each directory
            for (tdir_t dir_idx = 0; dir_idx < num_dirs; dir_idx++) {
                if (TIFFSetDirectory(tif, dir_idx)) {
                    // Create a temporary file for output
                    FILE* temp_output = tmpfile();
                    if (temp_output) {
                        // Try printing directory with different flags
                        uint32_t print_flags = 0;
                        if (fdp.ConsumeBool()) print_flags |= TIFFPRINT_STRIPS;
                        if (fdp.ConsumeBool()) print_flags |= TIFFPRINT_CURVES;
                        if (fdp.ConsumeBool()) print_flags |= TIFFPRINT_COLORMAP;
                        
                        TIFFPrintDirectory(tif, temp_output, print_flags);
                        fclose(temp_output);
                    }
                }
            }
            
            // Reopen for writing if we need to continue
            TIFFClose(tif);
            tif = TIFFOpen(temp_filename.c_str(), "a");
        } else {
            // If we can't reopen, clean up and exit
            unlink(temp_filename.c_str());
            return 0;
        }
    }
    
    // PHASE 7: Test directory switching and multiple operations
    if (fdp.remaining_bytes() > 0) {
        // Create several more directories with varying configurations
        uint8_t num_extra_dirs = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
        
        for (uint8_t i = 0; i < num_extra_dirs; i++) {
            if (TIFFCreateDirectory(tif)) {
                // Vary parameters for each directory
                uint16_t dir_width = fdp.ConsumeIntegralInRange<uint16_t>(1, 256);
                uint16_t dir_height = fdp.ConsumeIntegralInRange<uint16_t>(1, 256);
                uint16_t dir_bps = fdp.ConsumeIntegralInRange<uint16_t>(1, 16);
                uint16_t dir_spp = fdp.ConsumeIntegralInRange<uint16_t>(1, 3);
                
                TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, dir_width);
                TIFFSetField(tif, TIFFTAG_IMAGELENGTH, dir_height);
                TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, dir_bps);
                TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, dir_spp);
                
                // Choose random photometric
                uint16_t dir_photometric_idx = fdp.ConsumeIntegralInRange<uint16_t>(0, 2);
                uint16_t dir_photometric;
                switch (dir_photometric_idx) {
                    case 0: dir_photometric = PHOTOMETRIC_MINISBLACK; break;
                    case 1: dir_photometric = PHOTOMETRIC_MINISWHITE; break;
                    default: dir_photometric = PHOTOMETRIC_RGB; break;
                }
                TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, dir_photometric);
                
                // Write the directory
                if (!TIFFWriteDirectory(tif)) {
                    break; // Stop if we can't write
                }
            }
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink(temp_filename.c_str());
    
    return 0;
}

} // namespace harness_003
