#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>  // For close(), unlink(), mkstemp()
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>

namespace harness_014 {
/*
 * libtiff fuzzing harness for TIFF file creation and writing with compression
 * Targeting critical uncovered APIs in tif_write.c (0% coverage) and tif_dirwrite.c (0% coverage)
 * 
 * Target APIs from tif_write.c:
 * - TIFFWriteScanline: Scanline writing with various configurations
 * - TIFFSetupStrips: Strip setup for writing operations
 * - TIFFWriteCheck: Write configuration validation
 * - TIFFWriteBufferSetup: Buffer setup for writing
 * - TIFFFlushData1: Data flushing operations
 * - TIFFGrowStrips: Dynamic strip management (internal)
 * - TIFFAppendToStrip: Strip appending operations (internal)
 * 
 * Target APIs from tif_dirwrite.c:
 * - TIFFWriteDirectory: Directory writing with various tag types
 * - TIFFWriteCustomDirectory: Custom directory writing
 * - Various TIFFWriteDirectoryTag* functions for different data types
 * 
 * Compression testing includes:
 * - COMPRESSION_NONE, COMPRESSION_LZW, COMPRESSION_DEFLATE
 * - COMPRESSION_ADOBE_DEFLATE, COMPRESSION_PACKBITS
 * - COMPRESSION_CCITTRLE, COMPRESSION_CCITTFAX3, COMPRESSION_CCITTFAX4
 * - COMPRESSION_ZSTD, COMPRESSION_LZMA, COMPRESSION_WEBP, COMPRESSION_JPEG
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_001: Basic writing operations (tiles/strips with some raw operations)
 * - harness_002: Multi-directory and compression (general multi-dir)
 * - harness_010: Complete writing subsystem lifecycle (high-level writing)
 * - harness_014: LOW-LEVEL WRITING OPERATIONS with compression (targeting 0% coverage in tif_write.c/tif_dirwrite.c)
 * 
 * This harness specifically targets the 500+ undiscovered branches in tif_write.c
 * and tif_dirwrite.c by testing comprehensive write operations with various
 * compression settings, edge cases, and directory manipulation.
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

// Warning handler that does nothing (silent)
void silent_warning_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    return;
}

// Array of compression codecs to test (focusing on write-compatible codecs)
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

// Array of photometric interpretations for writing
static const uint16_t photometric_options[] = {
    PHOTOMETRIC_MINISWHITE,
    PHOTOMETRIC_MINISBLACK,
    PHOTOMETRIC_RGB,
    PHOTOMETRIC_PALETTE,
    PHOTOMETRIC_SEPARATED,
    PHOTOMETRIC_YCBCR,
    PHOTOMETRIC_CIELAB,
    PHOTOMETRIC_LOGL,
    PHOTOMETRIC_LOGLUV
};
static const size_t num_photometric_options = sizeof(photometric_options) / sizeof(photometric_options[0]);

// Array of sample formats for comprehensive testing
static const uint16_t sample_formats[] = {
    SAMPLEFORMAT_UINT,
    SAMPLEFORMAT_INT,
    SAMPLEFORMAT_IEEEFP,
    SAMPLEFORMAT_VOID,
    SAMPLEFORMAT_COMPLEXINT,
    SAMPLEFORMAT_COMPLEXIEEEFP
};
static const size_t num_sample_formats = sizeof(sample_formats) / sizeof(sample_formats[0]);

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Set silent error handlers to avoid output noise
    TIFFSetErrorHandler(silent_error_handler);
    TIFFSetWarningHandler(silent_warning_handler);
    
    // Minimum input size check - need enough for comprehensive configuration
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a temporary file for TIFF operations
    std::string temp_filename = "/tmp/libtiff_fuzz_write_comp_XXXXXX";
    int fd = mkstemp(&temp_filename[0]);
    if (fd < 0) {
        return 0;
    }
    close(fd);
    
    // Consume parameters for comprehensive testing
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 6;
    bool use_tiles = fdp.ConsumeBool();
    bool use_strips = fdp.ConsumeBool();
    bool test_multiple_directories = fdp.ConsumeBool();
    bool test_custom_directory = fdp.ConsumeBool();
    bool test_flush_operations = fdp.ConsumeBool();
    bool test_buffer_setup = fdp.ConsumeBool();
    
    // Image configuration parameters
    uint32_t image_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t image_height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    
    // Select compression and photometric from arrays
    uint16_t compression = compression_codecs[fdp.ConsumeIntegralInRange<size_t>(0, num_compression_codecs - 1)];
    uint16_t photometric = photometric_options[fdp.ConsumeIntegralInRange<size_t>(0, num_photometric_options - 1)];
    uint16_t sample_format = sample_formats[fdp.ConsumeIntegralInRange<size_t>(0, num_sample_formats - 1)];
    
    // Tile/strip configuration
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
    uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
    uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 64);
    
    // Consume buffer data for writing
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 65536);
    std::vector<uint8_t> write_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(temp_filename.c_str(), "w");
    if (!tif) {
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // Set basic TIFF fields
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, sample_format);
    
    // Set organization based on test mode
    if (use_tiles) {
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    } else if (use_strips) {
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    }
    
    // Call TIFFWriteCheck to validate writing configuration
    // This is a key function in tif_write.c with 0% coverage
    TIFFWriteCheck(tif, use_tiles ? 1 : 0, "fuzzer_test");
    
    // Test TIFFSetupStrips - another key function in tif_write.c
    if (use_strips) {
        TIFFSetupStrips(tif);
    }
    
    // Test buffer setup operations if requested
    if (test_buffer_setup) {
        // Test with various buffer sizes
        tmsize_t buffer_size_test = fdp.ConsumeIntegralInRange<tmsize_t>(1024, 8192);
        void* custom_buffer = nullptr;
        if (fdp.ConsumeBool()) {
            custom_buffer = malloc(buffer_size_test);
        }
        TIFFWriteBufferSetup(tif, custom_buffer, buffer_size_test);
        if (custom_buffer) {
            free(custom_buffer);
        }
    }
    
    // Calculate required buffer sizes
    size_t bytes_per_sample = (bits_per_sample + 7) / 8;
    size_t scanline_size = image_width * samples_per_pixel * bytes_per_sample;
    size_t tile_size = tile_width * tile_height * samples_per_pixel * bytes_per_sample;
    size_t strip_size = rows_per_strip * scanline_size;
    
    // Ensure write buffer is large enough
    size_t required_buffer_size = std::max({scanline_size, tile_size, strip_size, (size_t)1024});
    if (write_buffer.size() < required_buffer_size) {
        write_buffer.resize(required_buffer_size);
        // If we have remaining fuzzer data, use it to fill the resized buffer
        // Otherwise leave as zero-initialized (valid for testing)
        if (fdp.remaining_bytes() > 0 && required_buffer_size > buffer_size) {
            size_t additional_bytes = std::min(required_buffer_size - buffer_size, fdp.remaining_bytes());
            std::vector<uint8_t> additional_data = fdp.ConsumeBytes<uint8_t>(additional_bytes);
            std::copy(additional_data.begin(), additional_data.end(), write_buffer.begin() + buffer_size);
        }
    }
    
    // Perform writing operations based on test mode
    switch (test_mode) {
        case 0: {
            // Scanline writing - test TIFFWriteScanline
            for (uint32_t row = 0; row < image_height; row++) {
                if (row >= 100) break; // Limit for performance
                for (uint16_t sample = 0; sample < samples_per_pixel; sample++) {
                    if (planar_config == PLANARCONFIG_SEPARATE) {
                        TIFFWriteScanline(tif, write_buffer.data(), row, sample);
                    }
                }
                if (planar_config == PLANARCONFIG_CONTIG) {
                    TIFFWriteScanline(tif, write_buffer.data(), row, 0);
                }
            }
            break;
        }
        
        case 1: {
            // Strip writing if configured
            if (use_strips) {
                uint32_t strip_count = TIFFNumberOfStrips(tif);
                for (uint32_t strip = 0; strip < strip_count; strip++) {
                    if (strip >= 20) break; // Limit for performance
                    tmsize_t strip_size_actual = TIFFRawStripSize(tif, strip);
                    if (strip_size_actual > 0) {
                        TIFFWriteEncodedStrip(tif, strip, write_buffer.data(), 
                                            std::min((tmsize_t)write_buffer.size(), strip_size_actual));
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Tile writing if configured
            if (use_tiles) {
                uint32_t tile_count = TIFFNumberOfTiles(tif);
                for (uint32_t tile = 0; tile < tile_count; tile++) {
                    if (tile >= 20) break; // Limit for performance
                    tmsize_t tile_size_actual = TIFFTileSize(tif);
                    if (tile_size_actual > 0) {
                        TIFFWriteEncodedTile(tif, tile, write_buffer.data(),
                                           std::min((tmsize_t)write_buffer.size(), tile_size_actual));
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Mixed operations
            if (use_strips) {
                // Write first few strips
                uint32_t strip_count = TIFFNumberOfStrips(tif);
                for (uint32_t strip = 0; strip < std::min(strip_count, 5U); strip++) {
                    tmsize_t strip_size_actual = TIFFRawStripSize(tif, strip);
                    if (strip_size_actual > 0) {
                        TIFFWriteEncodedStrip(tif, strip, write_buffer.data(),
                                            std::min((tmsize_t)write_buffer.size(), strip_size_actual));
                    }
                }
            }
            
            // Also write some scanlines
            for (uint32_t row = 0; row < std::min(image_height, 10U); row++) {
                TIFFWriteScanline(tif, write_buffer.data(), row, 0);
            }
            break;
        }
        
        case 4: {
            // Test directory operations
            // Write initial directory
            TIFFWriteDirectory(tif);
            
            // Create and write additional directories if requested
            if (test_multiple_directories) {
                for (int i = 0; i < 3; i++) {
                    TIFFCreateDirectory(tif);
                    
                    // Set different parameters for each directory
                    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width / (i + 1));
                    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_height / (i + 1));
                    TIFFSetField(tif, TIFFTAG_COMPRESSION, 
                               compression_codecs[(i + 1) % num_compression_codecs]);
                    
                    TIFFWriteDirectory(tif);
                }
            }
            break;
        }
        
        case 5: {
            // Test custom directory and flush operations
            if (test_custom_directory) {
                uint64_t diroff = 0;
                TIFFWriteCustomDirectory(tif, &diroff);
            }
            
            if (test_flush_operations) {
                // Test TIFFFlushData1 via TIFFFlush
                TIFFFlush(tif);
                
                // Also test TIFFFlushData directly if accessible
                TIFFFlushData(tif);
            }
            break;
        }
    }
    
    // Test various directory tag writing functions indirectly through TIFFSetField
    // These exercise the TIFFWriteDirectoryTag* functions in tif_dirwrite.c
    
    // Write ASCII tag
    std::string ascii_data = fdp.ConsumeRandomLengthString(100);
    TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, ascii_data.c_str());
    
    // Write numeric tags
    uint32_t uint_value = fdp.ConsumeIntegral<uint32_t>();
    TIFFSetField(tif, TIFFTAG_XRESOLUTION, static_cast<float>(uint_value) / 100.0f);
    TIFFSetField(tif, TIFFTAG_YRESOLUTION, static_cast<float>(uint_value) / 100.0f);
    
    // Write rational tag
    float rational_value = fdp.ConsumeFloatingPoint<float>();
    TIFFSetField(tif, TIFFTAG_XPOSITION, rational_value);
    
    // Write array tags if we have enough data
    if (fdp.remaining_bytes() > 100) {
        std::vector<uint16_t> short_array(10);
        for (size_t i = 0; i < short_array.size(); i++) {
            short_array[i] = fdp.ConsumeIntegral<uint16_t>();
        }
        TIFFSetField(tif, TIFFTAG_REFERENCEBLACKWHITE, short_array.data());
    }
    
    // Final directory write to ensure all tags are written
    TIFFWriteDirectory(tif);
    
    // Cleanup
    TIFFClose(tif);
    unlink(temp_filename.c_str());
    
    return 0;
}

} // namespace harness_014
