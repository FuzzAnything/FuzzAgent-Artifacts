#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <tiffio.hxx>

namespace harness_011 {

/* stolen from tiffiop.h, which is a private header so we can't just include it */
/* safe multiply returns either the multiplied value or 0 if it overflowed */
#define __TIFFSafeMultiply(t, v, m)                                            \
    ((((t)(m) != (t)0) && (((t)(((v) * (m)) / (m))) == (t)(v)))                \
         ? (t)((v) * (m))                                                      \
         : (t)0)

const uint64_t MAX_SIZE = 500000000;

extern "C" void handle_error(const char *unused, const char *unused2,
                             va_list unused3)
{
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
#ifndef STANDALONE
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
#endif

#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
    // libjpeg-turbo has issues with MSAN and SIMD code
    // See https://bugs.chromium.org/p/oss-fuzz/issues/detail?id=7547
    // and https://github.com/libjpeg-turbo/libjpeg-turbo/pull/365
    setenv("JSIMD_FORCENONE", "1", 1);
#endif
#endif

    // Need sufficient data for comprehensive raw reading operations
    if (Size < 128) {
        return 0;  // Minimum: configuration + operation choices + buffer data
    }

    FuzzedDataProvider fdp(Data, Size);

    // ============================================
    // 1. CREATE PHASE: Generate TIFF in memory for raw reading testing
    // ============================================
    
    // Create a memory stream for writing (we'll create a TIFF then read it)
    std::ostringstream os;
    TIFF *tif_write = TIFFStreamOpen("MemTIFF", &os);
    if (!tif_write) {
        return 0;
    }
    
    // ============================================
    // 2. CONFIGURATION PHASE: Set up varied image configurations
    // ============================================
    
    // Consume configuration parameters from fuzzer input
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(16, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(16, 1024);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.PickValueInArray({1, 2, 4, 8, 16});
    uint16_t planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    uint16_t photometric = fdp.PickValueInArray({
        PHOTOMETRIC_MINISWHITE, 
        PHOTOMETRIC_MINISBLACK,
        PHOTOMETRIC_RGB
    });
    
    // Choose between tiled and striped organization
    bool use_tiled = fdp.ConsumeBool();
    
    // Set basic TIFF tags
    TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif_write, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif_write, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, photometric);
    
    // Set organization-specific parameters
    if (use_tiled) {
        // Configure tiling
        uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
        uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
        
        TIFFSetField(tif_write, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif_write, TIFFTAG_TILELENGTH, tile_height);
    } else {
        // Configure stripping
        uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 64);
        TIFFSetField(tif_write, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    }
    
    // Set compression (some raw reading operations require specific compression schemes)
    uint16_t compression = fdp.PickValueInArray({
        COMPRESSION_NONE,
        COMPRESSION_CCITTRLE,
        COMPRESSION_CCITTFAX3,
        COMPRESSION_CCITTFAX4,
        COMPRESSION_LZW,
        COMPRESSION_PACKBITS
    });
    TIFFSetField(tif_write, TIFFTAG_COMPRESSION, compression);
    
    // Write dummy image data
    tmsize_t scanline_size = TIFFScanlineSize(tif_write);
    tmsize_t tile_size = use_tiled ? TIFFTileSize(tif_write) : 0;
    
    // Don't continue if sizes are too large
    if (scanline_size > MAX_SIZE || tile_size > MAX_SIZE || 
        scanline_size == 0 || (use_tiled && tile_size == 0)) {
        TIFFClose(tif_write);
        return 0;
    }
    
    // Allocate and fill buffer with fuzzer data
    std::vector<uint8_t> image_data;
    if (use_tiled && tile_size > 0) {
        // For tiled images, write tile data
        uint32_t tiles_across = TIFFNumberOfTiles(tif_write);
        for (uint32_t tile = 0; tile < tiles_across; tile++) {
            if (fdp.remaining_bytes() < (size_t)tile_size) {
                break;
            }
            std::vector<uint8_t> tile_data = fdp.ConsumeBytes<uint8_t>(tile_size);
            TIFFWriteRawTile(tif_write, tile, tile_data.data(), tile_size);
        }
    } else if (scanline_size > 0) {
        // For stripped images, write scanline data
        for (uint32_t row = 0; row < height; row++) {
            if (fdp.remaining_bytes() < (size_t)scanline_size) {
                break;
            }
            std::vector<uint8_t> scanline_data = fdp.ConsumeBytes<uint8_t>(scanline_size);
            TIFFWriteScanline(tif_write, scanline_data.data(), row, 0);
        }
    }
    
    TIFFClose(tif_write);
    
    // ============================================
    // 3. RAW READING PHASE: Test raw data reading operations
    // ============================================
    
    // Get the written TIFF data
    std::string tiff_data = os.str();
    if (tiff_data.empty()) {
        return 0;
    }
    
    // Open the TIFF for reading
    std::istringstream is(tiff_data);
    TIFF *tif_read = TIFFStreamOpen("MemTIFF", &is);
    if (!tif_read) {
        return 0;
    }
    
    // Get image organization
    int is_tiled = TIFFIsTiled(tif_read);
    uint32_t strip_count = TIFFNumberOfStrips(tif_read);
    uint32_t tile_count = TIFFNumberOfTiles(tif_read);
    
    // Test different raw reading operations based on fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (operation) {
        case 0: {
            // Test TIFFReadRawStrip for stripped images
            if (!is_tiled && strip_count > 0) {
                uint32_t strip_to_read = fdp.ConsumeIntegralInRange<uint32_t>(0, strip_count - 1);
                tmsize_t strip_size = TIFFRawStripSize(tif_read, strip_to_read);
                
                if (strip_size > 0 && strip_size < MAX_SIZE) {
                    std::vector<uint8_t> strip_buffer(strip_size);
                    TIFFReadRawStrip(tif_read, strip_to_read, strip_buffer.data(), strip_size);
                }
            }
            break;
        }
        
        case 1: {
            // Test TIFFReadRawTile for tiled images
            if (is_tiled && tile_count > 0) {
                uint32_t tile_to_read = fdp.ConsumeIntegralInRange<uint32_t>(0, tile_count - 1);
                tmsize_t tile_size_read = TIFFTileSize(tif_read);
                
                if (tile_size_read > 0 && tile_size_read < MAX_SIZE) {
                    std::vector<uint8_t> tile_buffer(tile_size_read);
                    TIFFReadRawTile(tif_read, tile_to_read, tile_buffer.data(), tile_size_read);
                }
            }
            break;
        }
        
        case 2: {
            // Test TIFFReadFromUserBuffer
            uint32_t strile_to_read = 0;
            tmsize_t raw_size = 0;
            
            if (is_tiled && tile_count > 0) {
                strile_to_read = fdp.ConsumeIntegralInRange<uint32_t>(0, tile_count - 1);
                raw_size = TIFFTileSize(tif_read);
            } else if (!is_tiled && strip_count > 0) {
                strile_to_read = fdp.ConsumeIntegralInRange<uint32_t>(0, strip_count - 1);
                raw_size = TIFFRawStripSize(tif_read, strile_to_read);
            }
            
            if (raw_size > 0 && raw_size < MAX_SIZE) {
                // Create input and output buffers
                std::vector<uint8_t> input_buffer(raw_size);
                std::vector<uint8_t> output_buffer(raw_size * 2); // Extra space for decompression
                
                // Fill input buffer with fuzzer data
                if (fdp.remaining_bytes() >= (size_t)raw_size) {
                    auto input_data = fdp.ConsumeBytes<uint8_t>(raw_size);
                    memcpy(input_buffer.data(), input_data.data(), raw_size);
                }
                
                // Test TIFFReadFromUserBuffer
                TIFFReadFromUserBuffer(tif_read, strile_to_read, 
                                      input_buffer.data(), raw_size,
                                      output_buffer.data(), raw_size * 2);
            }
            break;
        }
        
        case 3: {
            // Test TIFFReadBufferSetup
            tmsize_t buffer_size = fdp.ConsumeIntegralInRange<tmsize_t>(1024, 65536);
            
            // Test with NULL buffer (let libtiff allocate)
            TIFFReadBufferSetup(tif_read, NULL, buffer_size);
            
            // Test with user-provided buffer
            if (buffer_size > 0 && buffer_size < MAX_SIZE) {
                std::vector<uint8_t> user_buffer(buffer_size);
                TIFFReadBufferSetup(tif_read, user_buffer.data(), buffer_size);
            }
            break;
        }
    }
    
    // Clean up
    TIFFClose(tif_read);
    
    return 0;
}

} // namespace harness_011
