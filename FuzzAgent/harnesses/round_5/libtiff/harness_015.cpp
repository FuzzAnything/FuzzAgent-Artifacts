#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>  // For close(), unlink(), mkstemp()
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>

namespace harness_015 {
/*
 * libtiff fuzzing harness for TIFF writing operations with comprehensive compression codec integration
 * Targeting critical 0% coverage modules: tif_write.c, tif_dirwrite.c, tif_flush.c
 * 
 * Target APIs from tif_write.c:
 * - TIFFWriteScanline: Scanline writing with compression integration
 * - TIFFWriteEncodedStrip: Strip writing with compression
 * - TIFFWriteRawStrip: Raw strip writing operations
 * - TIFFWriteTile: Tile writing operations
 * - TIFFWriteEncodedTile: Encoded tile writing with compression
 * - TIFFWriteRawTile: Raw tile writing
 * - TIFFWriteCheck: Write configuration validation
 * - TIFFWriteBufferSetup: Buffer setup for writing
 * - TIFFGrowStrips: Dynamic strip management (internal via write operations)
 * - TIFFAppendToStrip: Strip appending operations (internal via write operations)
 * 
 * Target APIs from tif_dirwrite.c:
 * - TIFFWriteDirectory: Directory writing with compression settings
 * - TIFFWriteCustomDirectory: Custom directory writing
 * - Various TIFFWriteDirectoryTag* functions tested through field setting
 * 
 * Target APIs from tif_flush.c:
 * - TIFFFlush: Complete flush operations
 * - TIFFFlushData: Data flushing with compression buffers
 * - TIFFForceStrileArrayWriting: Force strile array writing
 * - TIFFFlushData1: Internal data flushing (called by TIFFFlushData)
 * 
 * Compression testing includes comprehensive integration:
 * - COMPRESSION_NONE, COMPRESSION_LZW, COMPRESSION_DEFLATE
 * - COMPRESSION_ADOBE_DEFLATE, COMPRESSION_PACKBITS
 * - COMPRESSION_CCITTRLE, COMPRESSION_CCITTFAX3, COMPRESSION_CCITTFAX4
 * - COMPRESSION_ZSTD, COMPRESSION_LZMA, COMPRESSION_WEBP, COMPRESSION_JPEG
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_001: Basic writing operations (tiles/strips with some raw operations)
 * - harness_007: Advanced compression codecs (deep compression module focus)
 * - harness_010: Complete writing subsystem lifecycle (high-level writing)
 * - harness_013: CODEC management (registration, querying, unregistration)
 * - harness_014: Low-level writing operations with compression (targeting tif_write.c/tif_dirwrite.c)
 * - harness_015: COMPREHENSIVE WRITING WITH COMPRESSION CODEC INTEGRATION AND FLUSH OPERATIONS
 *                Specifically targeting tif_flush.c (0% coverage) with flush operations
 *                integrated with compression codecs and directory tag writing
 * 
 * This harness specifically targets the 500+ undiscovered branches in tif_write.c,
 * tif_dirwrite.c, and tif_flush.c by testing comprehensive write operations with
 * various compression settings, explicit flush operations, and directory manipulation.
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

// Array of compression codecs to test with comprehensive integration
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

// Array of photometric interpretations for compression testing
static const uint16_t photometric_options[] = {
    PHOTOMETRIC_MINISWHITE,
    PHOTOMETRIC_MINISBLACK,
    PHOTOMETRIC_RGB,
    PHOTOMETRIC_PALETTE,
    PHOTOMETRIC_SEPARATED,
    PHOTOMETRIC_YCBCR,    // Important for JPEG compression
    PHOTOMETRIC_CIELAB,
    PHOTOMETRIC_LOGL,
    PHOTOMETRIC_LOGLUV
};
static const size_t num_photometric_options = sizeof(photometric_options) / sizeof(photometric_options[0]);

// Array of planar configurations
static const uint16_t planar_configs[] = {
    PLANARCONFIG_CONTIG,
    PLANARCONFIG_SEPARATE
};
static const size_t num_planar_configs = sizeof(planar_configs) / sizeof(planar_configs[0]);

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
    std::string temp_filename = "/tmp/libtiff_fuzz_write_comp_flush_XXXXXX";
    int fd = mkstemp(&temp_filename[0]);
    if (fd < 0) {
        return 0;
    }
    close(fd);
    
    // Consume comprehensive configuration parameters
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint16_t compression_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_compression_codecs - 1);
    uint16_t compression = compression_codecs[compression_idx];
    
    // Image dimensions
    uint32_t image_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 2048);
    uint32_t image_height = fdp.ConsumeIntegralInRange<uint32_t>(1, 2048);
    
    // Bits per sample - vary based on compression requirements
    uint16_t bits_per_sample;
    if (compression == COMPRESSION_JPEG) {
        bits_per_sample = 8; // JPEG typically requires 8 bits per sample
    } else if (compression == COMPRESSION_CCITTRLE || compression == COMPRESSION_CCITTFAX3 || 
               compression == COMPRESSION_CCITTFAX4) {
        bits_per_sample = 1; // CCITT requires 1 bit per sample
    } else {
        bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    }
    
    // Samples per pixel
    uint16_t samples_per_pixel;
    if (compression == COMPRESSION_JPEG) {
        samples_per_pixel = fdp.ConsumeBool() ? 1 : 3; // JPEG supports 1 (grayscale) or 3 (RGB/YCbCr)
    } else {
        samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    }
    
    // Photometric interpretation
    uint16_t photometric_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_photometric_options - 1);
    uint16_t photometric = photometric_options[photometric_idx];
    
    // Adjust photometric for compatibility with compression
    if (compression == COMPRESSION_JPEG && photometric != PHOTOMETRIC_YCBCR) {
        // JPEG works best with YCbCr for color images
        if (samples_per_pixel == 3) {
            photometric = PHOTOMETRIC_YCBCR;
        } else {
            photometric = PHOTOMETRIC_MINISBLACK;
        }
    }
    
    // Planar configuration
    uint16_t planar_config_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_planar_configs - 1);
    uint16_t planar_config = planar_configs[planar_config_idx];
    
    // Sample format
    uint16_t sample_format_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_sample_formats - 1);
    uint16_t sample_format = sample_formats[sample_format_idx];
    
    // Tile/strip configuration
    bool use_tiles = fdp.ConsumeBool();
    uint32_t tile_width = 0, tile_height = 0, rows_per_strip = 0;
    
    if (use_tiles) {
        tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
        tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
        // Ensure tile dimensions don't exceed image dimensions
        tile_width = std::min(tile_width, image_width);
        tile_height = std::min(tile_height, image_height);
    } else {
        rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 64);
        rows_per_strip = std::min(rows_per_strip, image_height);
    }
    
    // Compression-specific parameters
    uint16_t predictor = 0;
    uint16_t jpeg_quality = 0;
    uint16_t webp_quality = 0;
    uint16_t zstd_level = 0;
    
    if (compression == COMPRESSION_LZW || compression == COMPRESSION_DEFLATE || 
        compression == COMPRESSION_ADOBE_DEFLATE) {
        predictor = fdp.ConsumeBool() ? PREDICTOR_HORIZONTAL : PREDICTOR_NONE;
    }
    
    if (compression == COMPRESSION_JPEG) {
        jpeg_quality = fdp.ConsumeIntegralInRange<uint16_t>(1, 100);
    }
    
    if (compression == COMPRESSION_WEBP) {
        webp_quality = fdp.ConsumeIntegralInRange<uint16_t>(1, 100);
    }
    
    if (compression == COMPRESSION_ZSTD) {
        zstd_level = fdp.ConsumeIntegralInRange<uint16_t>(1, 22);
    }
    
    // Consume image data for writing
    size_t max_data_size = image_width * image_height * samples_per_pixel * ((bits_per_sample + 7) / 8);
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(max_data_size, (size_t)65536));
    std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(temp_filename.c_str(), "w");
    if (!tif) {
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // Set basic TIFF fields - testing TIFFWriteDirectoryTag* functions indirectly
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, sample_format);
    
    // Set compression-specific fields
    if (predictor != 0) {
        TIFFSetField(tif, TIFFTAG_PREDICTOR, predictor);
    }
    
    if (jpeg_quality != 0) {
        TIFFSetField(tif, TIFFTAG_JPEGQUALITY, jpeg_quality);
        // JPEG requires additional fields
        if (samples_per_pixel == 3) {
            TIFFSetField(tif, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);
        }
    }
    
    if (webp_quality != 0) {
        TIFFSetField(tif, TIFFTAG_WEBP_LEVEL, webp_quality);
        TIFFSetField(tif, TIFFTAG_WEBP_LOSSLESS, fdp.ConsumeBool());
    }
    
    if (zstd_level != 0) {
        TIFFSetField(tif, TIFFTAG_ZSTD_LEVEL, zstd_level);
    }
    
    // Set tile or strip organization
    if (use_tiles) {
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    } else {
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    }
    
    // Call TIFFWriteCheck to validate writing configuration (tif_write.c)
    TIFFWriteCheck(tif, use_tiles ? 1 : 0, "fuzzer_write_comp_flush_test");
    
    // Ensure buffer is setup (TIFFWriteBufferSetup in tif_write.c)
    // This happens automatically when writing, but we can trigger it
    
    // Calculate bytes per sample
    size_t bytes_per_sample = (bits_per_sample + 7) / 8;
    
    // Perform writing operations based on test mode
    int result = 0;
    
    switch (test_mode) {
        case 0: {
            // Test 0: Scanline writing with compression and flush operations
            if (!use_tiles) {
                size_t scanline_size = image_width * samples_per_pixel * bytes_per_sample;
                if (scanline_size > 0 && image_data.size() >= scanline_size) {
                    for (uint32_t row = 0; row < image_height && row < 10; row++) { // Limit to 10 rows for performance
                        size_t offset = (row * scanline_size) % image_data.size();
                        if (offset + scanline_size <= image_data.size()) {
                            // TIFFWriteScanline from tif_write.c
                            result = TIFFWriteScanline(tif, &image_data[offset], row, 0);
                            
                            // Periodically flush data (TIFFFlushData from tif_flush.c)
                            if (row % 3 == 0) {
                                TIFFFlushData(tif);
                            }
                        }
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test 1: Strip writing with compression
            if (!use_tiles) {
                // Calculate strip size
                uint32_t strip_size = rows_per_strip * image_width * samples_per_pixel * bytes_per_sample;
                uint32_t num_strips = (image_height + rows_per_strip - 1) / rows_per_strip;
                
                if (strip_size > 0 && image_data.size() >= strip_size) {
                    for (uint32_t strip = 0; strip < num_strips && strip < 5; strip++) { // Limit to 5 strips
                        size_t offset = (strip * strip_size) % image_data.size();
                        if (offset + strip_size <= image_data.size()) {
                            // TIFFWriteEncodedStrip from tif_write.c
                            tmsize_t written = TIFFWriteEncodedStrip(tif, strip, &image_data[offset], strip_size);
                            
                            // Test TIFFWriteRawStrip
                            if (written > 0 && strip % 2 == 0) {
                                TIFFWriteRawStrip(tif, strip, &image_data[offset], std::min((tmsize_t)strip_size, written));
                            }
                        }
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test 2: Tile writing with compression
            if (use_tiles) {
                // Calculate tile size
                uint32_t tile_size = tile_width * tile_height * samples_per_pixel * bytes_per_sample;
                uint32_t tiles_across = (image_width + tile_width - 1) / tile_width;
                uint32_t tiles_down = (image_height + tile_height - 1) / tile_height;
                uint32_t num_tiles = tiles_across * tiles_down;
                
                if (tile_size > 0 && image_data.size() >= tile_size) {
                    for (uint32_t tile = 0; tile < num_tiles && tile < 4; tile++) { // Limit to 4 tiles
                        size_t offset = (tile * tile_size) % image_data.size();
                        if (offset + tile_size <= image_data.size()) {
                            // TIFFWriteEncodedTile from tif_write.c
                            tmsize_t written = TIFFWriteEncodedTile(tif, tile, &image_data[offset], tile_size);
                            
                            // Test TIFFWriteRawTile
                            if (written > 0 && tile % 2 == 0) {
                                TIFFWriteRawTile(tif, tile, &image_data[offset], std::min((tmsize_t)tile_size, written));
                            }
                            
                            // Test TIFFWriteTile (computes tile from coordinates)
                            if (tile < tiles_across) {
                                uint32_t x = (tile % tiles_across) * tile_width;
                                uint32_t y = (tile / tiles_across) * tile_height;
                                if (x < image_width && y < image_height) {
                                    TIFFWriteTile(tif, &image_data[offset], x, y, 0, 0);
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test 3: Comprehensive operations with explicit flush testing
            // Write a few scanlines or tiles
            if (!use_tiles && image_height > 0) {
                size_t scanline_size = image_width * samples_per_pixel * bytes_per_sample;
                if (scanline_size > 0 && image_data.size() >= scanline_size && image_height > 0) {
                    // Write first scanline
                    TIFFWriteScanline(tif, &image_data[0], 0, 0);
                }
            } else if (use_tiles && tile_width > 0 && tile_height > 0) {
                size_t tile_size = tile_width * tile_height * samples_per_pixel * bytes_per_sample;
                if (tile_size > 0 && image_data.size() >= tile_size) {
                    // Write first tile
                    TIFFWriteEncodedTile(tif, 0, &image_data[0], tile_size);
                }
            }
            
            // Test comprehensive flush operations (tif_flush.c)
            // 1. TIFFFlushData - flush compressed data
            TIFFFlushData(tif);
            
            // 2. TIFFForceStrileArrayWriting - force strile array writing
            // This requires specific conditions to work (DIRTYSTRIP set, no DIRTYDIRECT)
            // We'll attempt it but it may not succeed depending on state
            TIFFForceStrileArrayWriting(tif);
            
            // 3. Complete TIFFFlush operation
            TIFFFlush(tif);
            
            // Write directory to test TIFFWriteDirectory (tif_dirwrite.c)
            TIFFWriteDirectory(tif);
            
            // Create a new directory for custom directory testing
            TIFFCreateDirectory(tif);
            
            // Set some fields in new directory
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, image_width / 2);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, image_height / 2);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            
            // Write custom directory
            TIFFWriteCustomDirectory(tif, NULL);
            
            break;
        }
    }
    
    // Final flush operations
    TIFFFlushData(tif);
    TIFFFlush(tif);
    
    // Clean up
    TIFFClose(tif);
    unlink(temp_filename.c_str());
    
    return 0;
}

} // namespace harness_015
