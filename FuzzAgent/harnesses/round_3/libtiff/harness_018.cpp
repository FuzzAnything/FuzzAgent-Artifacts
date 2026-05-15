#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <tiffio.h>

namespace harness_018 {
/* Fuzzing harness for libtiff targeting LUV color conversion pipeline
 * Focus: Exercise the complete LUV-to-RGB color conversion path with undiscovered branches
 * Target APIs: 
 *   - XYZtoRGB24 (12 undiscovered branches, completely uncovered 0% hit)
 *   - LogLuv24toXYZ (Required for 24-bit LUV to XYZ conversion)
 *   - LogLuv32toXYZ (Required for 32-bit LUV to XYZ conversion)
 *   - uv_decode (Required for UV coordinate decoding)
 *   - TIFFOpenExt (File opening with options)
 *   - TIFFSetField (Configure photometric interpretation as LUV)
 *   - TIFFReadScanline/TIFFReadEncodedStrip (Read LUV-encoded data)
 *   - TIFFClose (Cleanup)
 * Strategy: Create synthetic LUV-encoded TIFF files from fuzzer input, then exercise
 *           the full LUV→XYZ→RGB24 conversion pipeline to unlock 12+ undiscovered branches
 * Coverage Gap: XYZtoRGB24 has 12 completely uncovered branches in tif_luv.c
 *               The tif_luv.c module has only 15.06% branch coverage overall
 * Differentiation: Unlike harness_001 which only mentions PHOTOMETRIC_LOGLUV, this harness
 *                  implements the complete conversion pipeline. Unlike harness_006 which
 *                  partially covers LUV encoding, this focuses on the critical XYZtoRGB24
 *                  conversion path that was completely missed.
 */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

/* Forward declarations for LUV conversion functions from tif_luv.c */
#ifdef __cplusplus
extern "C" {
#endif
/* Note: These functions are declared in tif_luv.c but may not be in public headers.
 * We declare them here to call them directly for targeted fuzzing.
 */
void XYZtoRGB24(float *xyz, uint8_t *rgb);
void LogLuv24toXYZ(uint32_t p, float *XYZ);
void LogLuv32toXYZ(uint32_t p, float *XYZ);
int uv_decode(double *up, double *vp, int c);
#ifdef __cplusplus
}
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for LUV configuration and pixel data
    if (size < 128) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume configuration parameters for LUV TIFF file
    uint8_t luv_format = fdp.ConsumeIntegral<uint8_t>() % 2; // 0 = LogLuv24, 1 = LogLuv32
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
    
    // Consume operation type: which path to exercise
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume encode method for LUV encoding
    int encode_method = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Consume pixel count and allocate LUV data
    size_t pixel_count = width * height;
    size_t luv_data_size;
    
    if (luv_format == 0) {
        luv_data_size = pixel_count * 4; // 32-bit per pixel for LogLuv24 (actually 24-bit packed in 32)
    } else {
        luv_data_size = pixel_count * 4; // 32-bit per pixel for LogLuv32
    }
    
    // Limit data size to prevent excessive memory usage
    if (luv_data_size > 1048576) {
        return 0;
    }
    
    std::vector<uint8_t> luv_data;
    if (fdp.remaining_bytes() >= luv_data_size) {
        luv_data = fdp.ConsumeBytes<uint8_t>(luv_data_size);
    } else {
        // Not enough data, use what we have and pad
        luv_data = fdp.ConsumeRemainingBytes<uint8_t>();
        luv_data.resize(luv_data_size, 0);
    }
    
    // Create a temporary filename for TIFF operations
    char filename[] = "/tmp/tiff_fuzz_luv_XXXXXX.tif";
    int fd = mkstemps(filename, 4);
    if (fd < 0) {
        return 0;
    }
    close(fd); // We'll open via TIFFOpenExt, not file descriptor

    // PHASE 1: Create synthetic LUV-encoded TIFF file
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        unlink(filename);
        return 0;
    }
    
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 256 * 1024 * 1024); // 256MB limit
    
    TIFF* tif_write = TIFFOpenExt(filename, "w", opts);
    TIFFOpenOptionsFree(opts);
    
    if (!tif_write) {
        unlink(filename);
        return 0;
    }
    
    // Configure as LUV image
    TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif_write, TIFFTAG_SAMPLESPERPIXEL, 3); // RGB after conversion
    TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_LOGLUV);
    TIFFSetField(tif_write, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif_write, TIFFTAG_COMPRESSION, luv_format == 0 ? COMPRESSION_SGILOG24 : COMPRESSION_SGILOG);
    
    // Write LUV data
    if (luv_format == 0) {
        // Write as tile for LogLuv24
        TIFFSetField(tif_write, TIFFTAG_TILEWIDTH, 16);
        TIFFSetField(tif_write, TIFFTAG_TILELENGTH, 16);
        
        uint32_t tile_width = 16;
        uint32_t tile_height = 16;
        uint32_t tiles_across = (width + tile_width - 1) / tile_width;
        uint32_t tiles_down = (height + tile_height - 1) / tile_height;
        
        for (uint32_t tile_row = 0; tile_row < tiles_down; tile_row++) {
            for (uint32_t tile_col = 0; tile_col < tiles_across; tile_col++) {
                if (tile_row * tiles_across + tile_col >= 3) break; // Limit to 3 tiles
                TIFFWriteEncodedTile(tif_write, 
                    tile_row * tiles_across + tile_col,
                    luv_data.data() + ((tile_row * tiles_across + tile_col) * tile_width * tile_height * 4),
                    tile_width * tile_height * 4);
            }
        }
    } else {
        // Write as scanlines for LogLuv32
        tmsize_t scanline_size = TIFFScanlineSize(tif_write);
        for (uint32_t row = 0; row < height && row < 10; row++) { // Limit to 10 scanlines
            TIFFWriteScanline(tif_write, 
                luv_data.data() + (row * scanline_size), 
                row, 0);
        }
    }
    
    TIFFClose(tif_write);
    
    // PHASE 2: Read and process LUV data with different conversion paths
    TIFF* tif_read = TIFFOpen(filename, "r");
    if (!tif_read) {
        unlink(filename);
        return 0;
    }
    
    // Read LUV data from the file
    std::vector<uint8_t> read_luv_data(luv_data_size);
    
    if (luv_format == 0 && TIFFIsTiled(tif_read)) {
        // Read tiles for LogLuv24
        uint32_t tile_width = 0, tile_height = 0;
        TIFFGetField(tif_read, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif_read, TIFFTAG_TILELENGTH, &tile_height);
        
        if (tile_width > 0 && tile_height > 0) {
            tmsize_t tile_size = TIFFTileSize(tif_read);
            if (tile_size > 0 && tile_size < 65536) {
                std::vector<uint8_t> tile_buffer(tile_size);
                uint32_t tiles_across = TIFFNumberOfTiles(tif_read);
                for (uint32_t tile = 0; tile < std::min(tiles_across, (uint32_t)3); tile++) {
                    TIFFReadEncodedTile(tif_read, tile, tile_buffer.data(), tile_size);
                }
            }
        }
    } else {
        // Read scanlines for LogLuv32
        tmsize_t scanline_size = TIFFScanlineSize(tif_read);
        if (scanline_size > 0 && scanline_size < 65536) {
            std::vector<uint8_t> scanline_buffer(scanline_size);
            for (uint32_t row = 0; row < height && row < 10; row++) {
                TIFFReadScanline(tif_read, scanline_buffer.data(), row, 0);
            }
        }
    }
    
    // PHASE 3: Exercise LUV color conversion functions directly
    // This is the core of the harness - targeting the undiscovered XYZtoRGB24 branches
    
    // Test different conversion paths based on operation_type
    switch (operation_type) {
        case 0: {
            // Test direct LUV→XYZ→RGB conversion using LogLuv24toXYZ
            if (luv_data_size >= 4) {
                uint32_t luv_pixel = 0;
                memcpy(&luv_pixel, luv_data.data(), 4);
                
                float xyz[3];
                uint8_t rgb[3];
                
                // Call LogLuv24toXYZ directly
                LogLuv24toXYZ(luv_pixel, xyz);
                
                // Call XYZtoRGB24 directly - TARGETING 12 UNDISCOVERED BRANCHES
                XYZtoRGB24(xyz, rgb);
                
                // Also test uv_decode independently
                double u, v;
                int ce = luv_pixel & 0x3fff;
                uv_decode(&u, &v, ce);
            }
            break;
        }
        
        case 1: {
            // Test direct LUV→XYZ→RGB conversion using LogLuv32toXYZ
            if (luv_data_size >= 4) {
                uint32_t luv_pixel = 0;
                memcpy(&luv_pixel, luv_data.data(), 4);
                
                float xyz[3];
                uint8_t rgb[3];
                
                // Call LogLuv32toXYZ directly
                LogLuv32toXYZ(luv_pixel, xyz);
                
                // Call XYZtoRGB24 directly - TARGETING 12 UNDISCOVERED BRANCHES
                XYZtoRGB24(xyz, rgb);
            }
            break;
        }
        
        case 2: {
            // Test batch conversion with multiple pixels
            size_t num_pixels = std::min(pixel_count, (size_t)10);
            for (size_t i = 0; i < num_pixels && (i * 4 + 4) <= luv_data_size; i++) {
                uint32_t luv_pixel = 0;
                memcpy(&luv_pixel, luv_data.data() + (i * 4), 4);
                
                float xyz[3];
                uint8_t rgb[3];
                
                if (luv_format == 0) {
                    LogLuv24toXYZ(luv_pixel, xyz);
                } else {
                    LogLuv32toXYZ(luv_pixel, xyz);
                }
                
                // Call XYZtoRGB24 for each pixel
                XYZtoRGB24(xyz, rgb);
                
                // Test uv_decode for LogLuv24 pixels
                if (luv_format == 0) {
                    double u, v;
                    int ce = luv_pixel & 0x3fff;
                    uv_decode(&u, &v, ce);
                }
            }
            break;
        }
    }
    
    TIFFClose(tif_read);
    unlink(filename);
    
    return 0;
}

} // namespace harness_018
