#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <sstream>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <tiffio.hxx>

namespace harness_001 {

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

    if (Size < 32) {
        return 0;  // Need minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(Data, Size);

    // Create a memory stream for writing (avoid file I/O dependencies)
    std::ostringstream os;
    TIFF *tif = TIFFStreamOpen("MemTIFF", &os);
    if (!tif) {
        return 0;
    }

    // Consume configuration parameters from fuzzer input
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.PickValueInArray({1, 2, 4, 8, 16, 32});
    uint16_t planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    // Use only basic photometric options to avoid compression dependencies
    uint16_t photometric = fdp.PickValueInArray({PHOTOMETRIC_MINISWHITE, PHOTOMETRIC_MINISBLACK,
                                                 PHOTOMETRIC_RGB});

    // Set basic TIFF tags for writing - avoid compression to prevent library dependencies
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    // Don't set compression - use default (COMPRESSION_NONE)
    // Don't set rows per strip - use default
    TIFFSetDirectory(tif, 0);

    // Calculate buffer sizes
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // Don't continue if sizes are too large
    if (scanline_size > MAX_SIZE || tile_size > MAX_SIZE || 
        scanline_size == 0 || tile_size == 0) {
        TIFFClose(tif);
        return 0;
    }

    // Allocate buffers for writing
    std::vector<uint8_t> scanline_buffer(scanline_size);
    std::vector<uint8_t> tile_buffer(tile_size);
    
    // Fill buffers with fuzzer data
    if (scanline_size > 0) {
        auto scanline_data = fdp.ConsumeBytes<uint8_t>(scanline_size);
        if (scanline_data.size() == (size_t)scanline_size) {
            memcpy(scanline_buffer.data(), scanline_data.data(), scanline_size);
        }
    }
    
    if (tile_size > 0 && fdp.remaining_bytes() >= (size_t)tile_size) {
        auto tile_data = fdp.ConsumeBytes<uint8_t>(tile_size);
        if (tile_data.size() == (size_t)tile_size) {
            memcpy(tile_buffer.data(), tile_data.data(), tile_size);
        }
    }

    // 3. Write operations based on configuration
    bool is_tiled = fdp.ConsumeBool();
    
    if (is_tiled) {
        // Set tile dimensions if not already set
        uint32_t tile_width = 256;
        uint32_t tile_height = 256;
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
        
        if (TIFFIsTiled(tif)) {
            uint32_t tiles_across = (width + tile_width - 1) / tile_width;
            uint32_t tiles_down = (height + tile_height - 1) / tile_height;
            
            // Try TIFFWriteTile
            for (uint32_t tile_y = 0; tile_y < tiles_down && fdp.remaining_bytes() > 0; tile_y++) {
                for (uint32_t tile_x = 0; tile_x < tiles_across && fdp.remaining_bytes() > 0; tile_x++) {
                    uint32_t x = tile_x * tile_width;
                    uint32_t y = tile_y * tile_height;
                    uint32_t tile = tile_y * tiles_across + tile_x;
                    
                    // Try both TIFFWriteTile and TIFFWriteEncodedTile
                    if (fdp.ConsumeBool()) {
                        TIFFWriteTile(tif, tile_buffer.data(), x, y, 0, 0);
                    } else {
                        TIFFWriteEncodedTile(tif, tile, tile_buffer.data(), tile_size);
                    }
                }
            }
        }
    } else {
        // Scanline-based writing operations
        for (uint32_t row = 0; row < height && fdp.remaining_bytes() > 0; row++) {
            // Alternate between different writing methods
            uint8_t method = fdp.ConsumeIntegral<uint8_t>() % 3;
            
            switch (method) {
                case 0:
                    // TIFFWriteScanline
                    TIFFWriteScanline(tif, scanline_buffer.data(), row, 0);
                    break;
                case 1:
                    // TIFFWriteEncodedStrip
                    if (row < height) {
                        uint32_t strip = row / TIFFDefaultStripSize(tif, (uint32_t)-1);
                        TIFFWriteEncodedStrip(tif, strip, scanline_buffer.data(), scanline_size);
                    }
                    break;
                case 2:
                    // Basic scanline write
                    TIFFWriteScanline(tif, scanline_buffer.data(), row, 0);
                    break;
            }
        }
    }

    // 4. TIFFWriteDirectory - Write directory structure
    TIFFWriteDirectory(tif);

    // 5. TIFFPrintDirectory - Optional directory inspection
    if (fdp.ConsumeBool()) {
        FILE* devnull = fopen("/dev/null", "w");
        if (devnull) {
            TIFFPrintDirectory(tif, devnull, 0);
            fclose(devnull);
        }
    }

    // 6. TIFFClose - Clean up and close file
    TIFFClose(tif);

    return 0;
}

} // namespace harness_001
