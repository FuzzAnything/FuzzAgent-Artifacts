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

namespace harness_006 {

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

    if (Size < 128) {
        return 0;  // Need sufficient data for comprehensive write operations
    }

    FuzzedDataProvider fdp(Data, Size);

    // ============================================
    // 1. OPEN PHASE: Create TIFF handle for writing
    // ============================================
    std::ostringstream os;
    TIFF *tif = TIFFStreamOpen("MemTIFF", &os);
    if (!tif) {
        return 0;
    }

    // ============================================
    // 2. CONFIGURE PHASE: Set image parameters
    // ============================================
    // Consume configuration parameters from fuzzer input
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.PickValueInArray({1, 2, 4, 8, 16, 32});
    uint16_t planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    uint16_t photometric = fdp.PickValueInArray({
        PHOTOMETRIC_MINISWHITE, 
        PHOTOMETRIC_MINISBLACK,
        PHOTOMETRIC_RGB,
        PHOTOMETRIC_PALETTE,
        PHOTOMETRIC_MASK
    });
    
    // Set basic TIFF tags for writing
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    
    // Optional compression setting
    if (fdp.ConsumeBool()) {
        uint16_t compression = fdp.PickValueInArray({
            COMPRESSION_NONE,
            COMPRESSION_CCITTRLE,
            COMPRESSION_CCITTFAX3,
            COMPRESSION_CCITTFAX4,
            COMPRESSION_LZW,
            COMPRESSION_PACKBITS,
            COMPRESSION_DEFLATE
        });
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    }
    
    // ============================================
    // 3. WRITE CHECK PHASE: Validate write mode
    // ============================================
    // Call TIFFWriteCheck to enter write mode
    int write_check_result = TIFFWriteCheck(tif, 0, "harness_006");
    if (write_check_result != 1) {
        // Write check failed, but continue to test other paths
        // Some write operations might still work
    }
    
    // ============================================
    // 4. BUFFER SETUP PHASE: Setup write buffer
    // ============================================
    // Try TIFFWriteBufferSetup with different buffer sizes
    if (fdp.ConsumeBool()) {
        size_t bufsize = fdp.ConsumeIntegralInRange<size_t>(1024, 65536);
        int buffer_setup_result = TIFFWriteBufferSetup(tif, nullptr, bufsize);
        // Result can be 0 (failure) or 1 (success), both are valid test cases
    }
    
    // Calculate buffer sizes
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    tmsize_t strip_size = TIFFStripSize(tif);
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // Don't continue if sizes are too large
    if (scanline_size > MAX_SIZE || strip_size > MAX_SIZE || tile_size > MAX_SIZE ||
        scanline_size == 0) {
        TIFFClose(tif);
        return 0;
    }
    
    // Allocate buffers for writing
    std::vector<uint8_t> scanline_buffer(scanline_size);
    std::vector<uint8_t> strip_buffer(strip_size);
    std::vector<uint8_t> tile_buffer(tile_size);
    
    // Fill buffers with fuzzer data
    if (scanline_size > 0 && fdp.remaining_bytes() >= (size_t)scanline_size) {
        auto scanline_data = fdp.ConsumeBytes<uint8_t>(scanline_size);
        if (scanline_data.size() == (size_t)scanline_size) {
            memcpy(scanline_buffer.data(), scanline_data.data(), scanline_size);
        }
    }
    
    if (strip_size > 0 && fdp.remaining_bytes() >= (size_t)strip_size) {
        auto strip_data = fdp.ConsumeBytes<uint8_t>(strip_size);
        if (strip_data.size() == (size_t)strip_size) {
            memcpy(strip_buffer.data(), strip_data.data(), strip_size);
        }
    }
    
    if (tile_size > 0 && fdp.remaining_bytes() >= (size_t)tile_size) {
        auto tile_data = fdp.ConsumeBytes<uint8_t>(tile_size);
        if (tile_data.size() == (size_t)tile_size) {
            memcpy(tile_buffer.data(), tile_data.data(), tile_size);
        }
    }
    
    // ============================================
    // 5. WRITE OPERATIONS PHASE: Comprehensive write testing
    // ============================================
    // Determine writing mode (strip vs tile)
    bool use_tiled = fdp.ConsumeBool();
    
    if (use_tiled) {
        // Set tile dimensions for tiled writing
        uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
        uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
        
        if (TIFFIsTiled(tif)) {
            uint32_t tiles_across = (width + tile_width - 1) / tile_width;
            uint32_t tiles_down = (height + tile_height - 1) / tile_height;
            uint32_t total_tiles = tiles_across * tiles_down;
            
            // Test all tile write operations
            for (uint32_t tile_index = 0; tile_index < total_tiles && tile_index < 10 && fdp.remaining_bytes() > 0; tile_index++) {
                uint32_t tile_x = tile_index % tiles_across;
                uint32_t tile_y = tile_index / tiles_across;
                uint32_t x = tile_x * tile_width;
                uint32_t y = tile_y * tile_height;
                
                // Choose which tile write operation to test
                uint8_t tile_op = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                switch (tile_op) {
                    case 0:
                        // TIFFWriteTile - write tile by coordinates
                        TIFFWriteTile(tif, tile_buffer.data(), x, y, 0, 0);
                        break;
                    case 1:
                        // TIFFWriteEncodedTile - write encoded tile by index
                        TIFFWriteEncodedTile(tif, tile_index, tile_buffer.data(), tile_size);
                        break;
                    case 2:
                        // TIFFWriteRawTile - write raw tile by index
                        TIFFWriteRawTile(tif, tile_index, tile_buffer.data(), tile_size);
                        break;
                    case 3:
                        // Combination: try both encoded and raw
                        if (fdp.ConsumeBool()) {
                            TIFFWriteEncodedTile(tif, tile_index, tile_buffer.data(), tile_size);
                        } else {
                            TIFFWriteRawTile(tif, tile_index, tile_buffer.data(), tile_size);
                        }
                        break;
                }
            }
        }
    } else {
        // Strip/scanline based writing
        // Set rows per strip
        uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, height);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
        
        uint32_t strips_per_image = TIFFNumberOfStrips(tif);
        
        // Test all strip/scanline write operations
        for (uint32_t row = 0; row < height && row < 100 && fdp.remaining_bytes() > 0; row++) {
            uint8_t write_op = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            switch (write_op) {
                case 0:
                    // TIFFWriteScanline - write individual scanline
                    TIFFWriteScanline(tif, scanline_buffer.data(), row, 0);
                    break;
                case 1:
                    // TIFFWriteEncodedStrip - write encoded strip
                    if (row < strips_per_image * rows_per_strip) {
                        uint32_t strip = row / rows_per_strip;
                        TIFFWriteEncodedStrip(tif, strip, strip_buffer.data(), strip_size);
                    }
                    break;
                case 2:
                    // TIFFWriteRawStrip - write raw strip
                    if (row < strips_per_image * rows_per_strip) {
                        uint32_t strip = row / rows_per_strip;
                        TIFFWriteRawStrip(tif, strip, strip_buffer.data(), strip_size);
                    }
                    break;
                case 3:
                    // Test scanline with different sample parameter
                    TIFFWriteScanline(tif, scanline_buffer.data(), row, fdp.ConsumeIntegral<uint16_t>() % samples_per_pixel);
                    break;
            }
        }
    }
    
    // ============================================
    // 6. FLUSH PHASE: Ensure data is written
    // ============================================
    // Call flush operations
    if (fdp.ConsumeBool()) {
        TIFFFlush(tif);
    } else {
        TIFFFlushData(tif);
    }
    
    // ============================================
    // 7. DIRECTORY COMMIT PHASE: Write directory
    // ============================================
    // Write directory to commit changes
    TIFFWriteDirectory(tif);
    
    // ============================================
    // 8. CLOSE PHASE: Clean up resources
    // ============================================
    TIFFClose(tif);
    
    return 0;
}

} // namespace harness_006
