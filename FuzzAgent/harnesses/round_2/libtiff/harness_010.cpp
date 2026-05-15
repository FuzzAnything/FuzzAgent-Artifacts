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

namespace harness_010 {

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

    // Need sufficient data for comprehensive size calculation testing
    if (Size < 128) {
        return 0;  // Minimum: configuration + operation choices
    }

    FuzzedDataProvider fdp(Data, Size);

    // ============================================
    // 1. CREATE PHASE: Generate TIFF in memory
    // ============================================
    
    // Create a memory stream for writing
    std::ostringstream os;
    TIFF *tif = TIFFStreamOpen("MemTIFF", &os);
    if (!tif) {
        return 0;
    }
    
    // ============================================
    // 2. CONFIGURATION PHASE: Set up varied image configurations
    // ============================================
    
    // Consume configuration parameters from fuzzer input
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.PickValueInArray({1, 2, 4, 8, 16, 32});
    uint16_t planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    uint16_t photometric = fdp.PickValueInArray({
        PHOTOMETRIC_MINISWHITE, 
        PHOTOMETRIC_MINISBLACK,
        PHOTOMETRIC_RGB,
        PHOTOMETRIC_PALETTE,
        PHOTOMETRIC_YCBCR,
        PHOTOMETRIC_CIELAB,
        PHOTOMETRIC_LOGL,
        PHOTOMETRIC_LOGLUV
    });
    
    // Choose between tiled and striped organization
    bool use_tiled = fdp.ConsumeBool();
    
    // Set basic TIFF tags
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    
    // Set organization-specific parameters
    if (use_tiled) {
        // Configure tiling
        uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
        uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
        
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    } else {
        // Configure stripping
        uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, height);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    }
    
    // Set optional compression (affects raw strip size calculations)
    uint16_t compression = fdp.PickValueInArray({
        COMPRESSION_NONE,
        COMPRESSION_CCITTRLE,
        COMPRESSION_CCITTFAX3,
        COMPRESSION_CCITTFAX4,
        COMPRESSION_LZW,
        COMPRESSION_OJPEG,
        COMPRESSION_JPEG,
        COMPRESSION_ADOBE_DEFLATE,
        COMPRESSION_DEFLATE
    });
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    // Set YCbCr subsampling if using YCbCr photometric
    if (photometric == PHOTOMETRIC_YCBCR) {
        uint16_t ycbcr_subsampling[2];
        ycbcr_subsampling[0] = fdp.PickValueInArray({1, 2, 4});
        ycbcr_subsampling[1] = fdp.PickValueInArray({1, 2, 4});
        TIFFSetField(tif, TIFFTAG_YCBCRSUBSAMPLING, ycbcr_subsampling[0], ycbcr_subsampling[1]);
    }
    
    // ============================================
    // 3. SIZE CALCULATION PHASE: Exercise all size calculation functions
    // ============================================
    
    // Determine which functions to test based on configuration
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (test_mode) {
        case 0: {
            // Test tile size calculations
            if (use_tiled) {
                // Get tile dimensions
                uint32_t tile_width = 0, tile_height = 0;
                TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
                TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
                
                if (tile_width > 0 && tile_height > 0) {
                    // Test TIFFVTileSize/TIFFVTileSize64 with various row counts
                    for (uint32_t nrows = 1; nrows <= tile_height * 2; nrows *= 2) {
                        uint64_t vts64 = TIFFVTileSize64(tif, nrows);
                        tmsize_t vts = TIFFVTileSize(tif, nrows);
                        
                        // Also test with tile height
                        uint64_t ts64 = TIFFVTileSize64(tif, tile_height);
                        tmsize_t ts = TIFFVTileSize(tif, tile_height);
                        
                        // Test TIFFTileRowSize/TIFFTileRowSize64
                        uint64_t trs64 = TIFFTileRowSize64(tif);
                        tmsize_t trs = TIFFTileRowSize(tif);
                        
                        // Test TIFFTileSize/TIFFTileSize64
                        uint64_t tilesize64 = TIFFTileSize64(tif);
                        tmsize_t tilesize = TIFFTileSize(tif);
                        
                        // Prevent unused variable warnings
                        (void)vts64; (void)vts; (void)ts64; (void)ts;
                        (void)trs64; (void)trs; (void)tilesize64; (void)tilesize;
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test strip size calculations
            if (!use_tiled) {
                // Get strip information
                uint32_t rows_per_strip = 0;
                TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
                
                if (rows_per_strip > 0) {
                    uint32_t strip_count = TIFFNumberOfStrips(tif);
                    
                    // Test strip size calculations for various strips
                    for (uint32_t strip = 0; strip < strip_count && strip < 10; strip++) {
                        uint64_t rss64 = TIFFRawStripSize64(tif, strip);
                        tmsize_t rss = TIFFRawStripSize(tif, strip);
                        
                        // Test TIFFStripSize/TIFFStripSize64
                        uint64_t ss64 = TIFFStripSize64(tif);
                        tmsize_t ss = TIFFStripSize(tif);
                        
                        // Prevent unused variable warnings
                        (void)rss64; (void)rss; (void)ss64; (void)ss;
                    }
                    
                    // Test TIFFVStripSize/TIFFVStripSize64 with various row counts
                    for (uint32_t nrows = 1; nrows <= rows_per_strip * 2; nrows *= 2) {
                        uint64_t vss64 = TIFFVStripSize64(tif, nrows);
                        tmsize_t vss = TIFFVStripSize(tif, nrows);
                        
                        // Also test with rows_per_strip
                        uint64_t vss_full64 = TIFFVStripSize64(tif, rows_per_strip);
                        tmsize_t vss_full = TIFFVStripSize(tif, rows_per_strip);
                        
                        // Prevent unused variable warnings
                        (void)vss64; (void)vss; (void)vss_full64; (void)vss_full;
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test scanline size calculations
            uint64_t rss64 = TIFFRasterScanlineSize64(tif);
            tmsize_t rss = TIFFRasterScanlineSize(tif);
            
            uint64_t sss64 = TIFFScanlineSize64(tif);
            tmsize_t sss = TIFFScanlineSize(tif);
            
            // Prevent unused variable warnings
            (void)rss64; (void)rss; (void)sss64; (void)sss;
            
            // Test edge cases with various row counts
            for (uint32_t nrows = 0; nrows <= 3; nrows++) {
                uint64_t vts64 = TIFFVTileSize64(tif, nrows);
                tmsize_t vts = TIFFVTileSize(tif, nrows);
                
                uint64_t vss64 = TIFFVStripSize64(tif, nrows);
                tmsize_t vss = TIFFVStripSize(tif, nrows);
                
                // Prevent unused variable warnings
                (void)vts64; (void)vts; (void)vss64; (void)vss;
            }
            break;
        }
        
        case 3: {
            // Test mixed size calculations with directory navigation
            // Create additional directories
            if (TIFFWriteDirectory(tif)) {
                // Change configuration for second directory
                uint32_t new_width = fdp.ConsumeIntegralInRange<uint32_t>(1, 2048);
                uint32_t new_height = fdp.ConsumeIntegralInRange<uint32_t>(1, 2048);
                
                TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, new_width);
                TIFFSetField(tif, TIFFTAG_IMAGELENGTH, new_height);
                
                // Switch organization for second directory
                bool new_use_tiled = !use_tiled;
                if (new_use_tiled) {
                    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(32, 256);
                    uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(32, 256);
                    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
                    TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
                } else {
                    uint32_t rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, new_height);
                    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
                }
                
                // Write second directory
                TIFFWriteDirectory(tif);
                
                // Navigate between directories and test size calculations
                for (tdir_t dir = 0; dir < 2; dir++) {
                    TIFFSetDirectory(tif, dir);
                    
                    // Test a mix of size calculations
                    uint64_t rss64 = TIFFRasterScanlineSize64(tif);
                    tmsize_t rss = TIFFRasterScanlineSize(tif);
                    
                    if (TIFFIsTiled(tif)) {
                        uint64_t trs64 = TIFFTileRowSize64(tif);
                        tmsize_t trs = TIFFTileRowSize(tif);
                        
                        uint64_t tilesize64 = TIFFTileSize64(tif);
                        tmsize_t tilesize = TIFFTileSize(tif);
                        
                        // Prevent unused variable warnings
                        (void)trs64; (void)trs; (void)tilesize64; (void)tilesize;
                    } else {
                        uint64_t ss64 = TIFFStripSize64(tif);
                        tmsize_t ss = TIFFStripSize(tif);
                        
                        // Prevent unused variable warnings
                        (void)ss64; (void)ss;
                    }
                    
                    // Prevent unused variable warnings
                    (void)rss64; (void)rss;
                }
            }
            break;
        }
    }
    
    // ============================================
    // 4. CLEANUP PHASE
    // ============================================
    TIFFClose(tif);
    
    return 0;
}

} // namespace harness_010
