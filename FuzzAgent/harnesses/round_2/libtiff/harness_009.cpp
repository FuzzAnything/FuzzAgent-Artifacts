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

namespace harness_009 {

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

    // Need sufficient data for comprehensive tile writing operations
    if (Size < 128) {
        return 0;  // Minimum: configuration + tile data + operation choices
    }

    FuzzedDataProvider fdp(Data, Size);

    // ============================================
    // 1. CREATE PHASE: Generate TIFF in memory for writing
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
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(16, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(16, 1024);
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
    
    // Configure tiling - always create tiled image for tile writing operations
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    
    // Use TIFFDefaultTileSize to get default dimensions if needed
    if (fdp.ConsumeBool()) {
        uint32_t default_tile_width, default_tile_height;
        TIFFDefaultTileSize(tif, &default_tile_width, &default_tile_height);
        if (default_tile_width > 0 && default_tile_height > 0) {
            tile_width = default_tile_width;
            tile_height = default_tile_height;
        }
    }
    
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    
    // Optional compression setting - vary to hit different code paths
    if (fdp.ConsumeBool()) {
        uint16_t compression = fdp.PickValueInArray({
            COMPRESSION_NONE,
            COMPRESSION_CCITTRLE,
            COMPRESSION_CCITTFAX3,
            COMPRESSION_CCITTFAX4,
            COMPRESSION_LZW,
            COMPRESSION_PACKBITS,
            COMPRESSION_DEFLATE,
            COMPRESSION_ADOBE_DEFLATE,
            COMPRESSION_JPEG
        });
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    }
    
    // ============================================
    // 3. WRITE CHECK PHASE: Validate write mode
    // ============================================
    
    // Call TIFFWriteCheck to enter write mode (supports testing this API)
    int write_mode = fdp.ConsumeBool() ? 1 : 0;  // 0 for strips, 1 for tiles
    int write_check_result = TIFFWriteCheck(tif, write_mode, "harness_009");
    
    // Setup strips/tiles organization
    if (write_check_result == 0) {
        TIFFSetupStrips(tif);
    }
    
    // ============================================
    // 4. TILE CALCULATION PHASE: Compute tile parameters
    // ============================================
    
    // Calculate number of tiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    if (num_tiles == 0) {
        TIFFClose(tif);
        return 0;
    }
    
    // Calculate tile size
    tmsize_t tile_size = TIFFTileSize(tif);
    if (tile_size > MAX_SIZE || tile_size == 0) {
        TIFFClose(tif);
        return 0;
    }
    
    // ============================================
    // 5. TILE DATA PREPARATION: Generate test tile data
    // ============================================
    
    // Allocate tile buffer
    std::vector<uint8_t> tile_buffer(tile_size);
    
    // Fill buffer with fuzzer data
    if (fdp.remaining_bytes() >= (size_t)tile_size) {
        auto tile_data = fdp.ConsumeBytes<uint8_t>(tile_size);
        if (tile_data.size() == (size_t)tile_size) {
            memcpy(tile_buffer.data(), tile_data.data(), tile_size);
        }
    } else {
        // If not enough data, fill with pattern
        for (size_t i = 0; i < tile_buffer.size(); i++) {
            tile_buffer[i] = (uint8_t)(i % 256);
        }
    }
    
    // ============================================
    // 6. TILE WRITING OPERATIONS: Target APIs
    // ============================================
    
    // Determine which operations to perform
    uint8_t operation_sequence = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    for (uint32_t tile_idx = 0; tile_idx < num_tiles && fdp.remaining_bytes() > 0; tile_idx++) {
        // Calculate tile coordinates
        uint32_t tiles_across = (width + tile_width - 1) / tile_width;
        uint32_t tiles_down = (height + tile_height - 1) / tile_height;
        
        uint32_t tile_y = tile_idx / tiles_across;
        uint32_t tile_x = tile_idx % tiles_across;
        
        uint32_t x = tile_x * tile_width;
        uint32_t y = tile_y * tile_height;
        uint32_t z = 0;  // Default to 0 for 2D images
        uint16_t sample = 0;  // Default sample
        // Check if tile coordinates are valid
        int tile_check_result = TIFFCheckTile(tif, x, y, z, sample);
        
        // Compute tile index from coordinates
        uint32_t computed_tile = TIFFComputeTile(tif, x, y, z, sample);
        
        // Test different tile writing APIs based on operation sequence
        switch (operation_sequence) {
            case 0: {
                // TIFFWriteTile - writes tile at coordinates
                if (tile_check_result != 0) {
                    TIFFWriteTile(tif, tile_buffer.data(), x, y, z, sample);
                }
                break;
            }
                
            case 1: {
                // TIFFWriteEncodedTile - writes encoded tile data
                if (computed_tile < num_tiles) {
                    TIFFWriteEncodedTile(tif, computed_tile, tile_buffer.data(), tile_size);
                }
                break;
            }
                
            case 2: {
                // TIFFWriteRawTile - writes raw tile data
                if (computed_tile < num_tiles) {
                    TIFFWriteRawTile(tif, computed_tile, tile_buffer.data(), tile_size);
                }
                break;
            }
                
            case 3: {
                // Mixed: Try all three APIs
                if (tile_check_result != 0 && fdp.ConsumeBool()) {
                    TIFFWriteTile(tif, tile_buffer.data(), x, y, z, sample);
                }
                if (computed_tile < num_tiles && fdp.ConsumeBool()) {
                    TIFFWriteEncodedTile(tif, computed_tile, tile_buffer.data(), tile_size);
                }
                if (computed_tile < num_tiles && fdp.ConsumeBool()) {
                    TIFFWriteRawTile(tif, computed_tile, tile_buffer.data(), tile_size);
                }
                break;
            }
                
            case 4: {
                // Test with invalid tile indices (error paths)
                if (fdp.ConsumeBool()) {
                    // Invalid tile index (beyond range)
                    uint32_t invalid_tile = num_tiles + fdp.ConsumeIntegralInRange<uint32_t>(1, 10);
                    TIFFWriteEncodedTile(tif, invalid_tile, tile_buffer.data(), tile_size);
                }
                break;
            }
                
            case 5: {
                // Test with varying buffer sizes
                tmsize_t buffer_size = fdp.ConsumeIntegralInRange<tmsize_t>(1, tile_size * 2);
                if (buffer_size > 0) {
                    std::vector<uint8_t> variable_buffer(buffer_size);
                    if (fdp.remaining_bytes() >= buffer_size) {
                        auto var_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
                        memcpy(variable_buffer.data(), var_data.data(), std::min(var_data.size(), (size_t)buffer_size));
                    }
                    
                    if (computed_tile < num_tiles) {
                        TIFFWriteEncodedTile(tif, computed_tile, variable_buffer.data(), buffer_size);
                    }
                }
                break;
            }
                
            case 6: {
                // Test with partial tile data (buffer smaller than tile_size)
                tmsize_t partial_size = fdp.ConsumeIntegralInRange<tmsize_t>(1, tile_size);
                if (partial_size > 0) {
                    std::vector<uint8_t> partial_buffer(partial_size);
                    if (fdp.remaining_bytes() >= partial_size) {
                        auto partial_data = fdp.ConsumeBytes<uint8_t>(partial_size);
                        memcpy(partial_buffer.data(), partial_data.data(), 
                               std::min(partial_data.size(), (size_t)partial_size));
                    } else {
                        // Fill with pattern if not enough data
                        for (size_t i = 0; i < partial_buffer.size(); i++) {
                            partial_buffer[i] = (uint8_t)(i % 256);
                        }
                    }
                    
                    if (computed_tile < num_tiles) {
                        TIFFWriteEncodedTile(tif, computed_tile, partial_buffer.data(), partial_size);
                    }
                }
                break;
            }
                
            case 7: {
                // Test tile writing with different sample values
                for (uint16_t s = 0; s < samples_per_pixel && s < 4; s++) {
                    uint32_t sample_tile = TIFFComputeTile(tif, x, y, z, s);
                    if (sample_tile < num_tiles) {
                        TIFFWriteEncodedTile(tif, sample_tile, tile_buffer.data(), tile_size);
                    }
                }
                break;
            }
        }
        
        // Occasionally write directory to test multi-directory scenarios
        if (fdp.ConsumeProbability<double>() < 0.1) {
            TIFFWriteDirectory(tif);
        }
        
        // Break early if we've consumed enough tiles
        if (tile_idx > 10) {
            break;
        }
    }
    
    // ============================================
    // 7. CLEANUP PHASE: Proper resource management
    // ============================================
    
    // Write final directory
    TIFFWriteDirectory(tif);
    
    // Close and cleanup
    TIFFClose(tif);
    
    // Optional global cleanup (not typically needed but included for coverage)
    // Note: TIFFCleanup requires TIFF* parameter, not global cleanup
    
    return 0;
}

} // namespace harness_009
