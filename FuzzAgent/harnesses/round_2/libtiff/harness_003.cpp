#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <tiffio.hxx>

namespace harness_003 {
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

    // Create a memory stream from the input data
    std::istringstream s(std::string(Data, Data + Size));
    TIFF *tif = TIFFStreamOpen("MemTIFF", &s);
    if (!tif) {
        return 0;
    }

    // Get basic image information
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

    // Check if sizes are reasonable
    if (width == 0 || height == 0 || width > 65535 || height > 65535) {
        TIFFClose(tif);
        return 0;
    }

    // Check if image is tiled or stripped
    int is_tiled = TIFFIsTiled(tif);
    
    // Get current directory for potential multi-directory testing
    tdir_t current_dir = TIFFCurrentDirectory(tif);
    
    // Optionally set directory based on fuzzer input
    if (fdp.ConsumeBool()) {
        tdir_t dir_count = TIFFNumberOfDirectories(tif);
        if (dir_count > 1) {
            uint32_t dir_index = fdp.ConsumeIntegralInRange<uint32_t>(0, dir_count - 1);
            TIFFSetDirectory(tif, dir_index);
        }
    }

    // Allocate buffer for RGBA data
    uint32_t buffer_size = 0;
    uint32_t *raster_buffer = nullptr;
    
    if (is_tiled) {
        // Tile-based reading operations
        uint32_t tile_width = 0, tile_height = 0;
        TIFFGetFieldDefaulted(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetFieldDefaulted(tif, TIFFTAG_TILELENGTH, &tile_height);
        
        if (tile_width == 0 || tile_height == 0) {
            TIFFClose(tif);
            return 0;
        }
        
        // Calculate number of tiles
        uint32_t tiles_across = (width + tile_width - 1) / tile_width;
        uint32_t tiles_down = (height + tile_height - 1) / tile_height;
        uint32_t total_tiles = tiles_across * tiles_down;
        
        // Check tile count
        uint32_t tile_count = TIFFNumberOfTiles(tif);
        if (tile_count != total_tiles && tile_count > 0) {
            // Use actual tile count if available and different
            total_tiles = tile_count;
            tiles_across = (tile_count + tiles_down - 1) / tiles_down;
        }
        
        // Allocate buffer for one tile
        buffer_size = tile_width * tile_height;
        if (buffer_size > MAX_SIZE || buffer_size == 0) {
            TIFFClose(tif);
            return 0;
        }
        
        raster_buffer = (uint32_t *)_TIFFmalloc(buffer_size * sizeof(uint32_t));
        if (!raster_buffer) {
            TIFFClose(tif);
            return 0;
        }
        
        // Iterate through some tiles based on fuzzer input
        uint32_t max_tiles_to_test = fdp.ConsumeIntegralInRange<uint32_t>(1, total_tiles);
        max_tiles_to_test = (max_tiles_to_test > 16) ? 16 : max_tiles_to_test; // Limit to 16 tiles
        
        for (uint32_t tile_idx = 0; tile_idx < max_tiles_to_test && tile_idx < total_tiles; tile_idx++) {
            // Calculate tile coordinates
            uint32_t tile_y = tile_idx / tiles_across;
            uint32_t tile_x = tile_idx % tiles_across;
            uint32_t col = tile_x * tile_width;
            uint32_t row = tile_y * tile_height;
            
            // Clear buffer
            memset(raster_buffer, 0, buffer_size * sizeof(uint32_t));
            
            // Choose between TIFFReadRGBATile and TIFFReadRGBATileExt
            if (fdp.ConsumeBool()) {
                // Use TIFFReadRGBATileExt with stop_on_error flag from fuzzer
                int stop_on_error = fdp.ConsumeBool();
                TIFFReadRGBATileExt(tif, col, row, raster_buffer, stop_on_error);
            } else {
                // Use TIFFReadRGBATile
                TIFFReadRGBATile(tif, col, row, raster_buffer);
            }
        }
        
    } else {
        // Strip-based reading operations
        uint32_t rows_per_strip = 0;
        TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
        
        if (rows_per_strip == 0) {
            rows_per_strip = TIFFDefaultStripSize(tif, (uint32_t)-1);
        }
        
        if (rows_per_strip == 0) {
            TIFFClose(tif);
            return 0;
        }
        
        // Calculate number of strips
        uint32_t strip_count = TIFFNumberOfStrips(tif);
        uint32_t calculated_strips = (height + rows_per_strip - 1) / rows_per_strip;
        
        if (strip_count == 0) {
            strip_count = calculated_strips;
        }
        
        // Allocate buffer for one strip
        buffer_size = width * rows_per_strip;
        if (buffer_size > MAX_SIZE || buffer_size == 0) {
            TIFFClose(tif);
            return 0;
        }
        
        raster_buffer = (uint32_t *)_TIFFmalloc(buffer_size * sizeof(uint32_t));
        if (!raster_buffer) {
            TIFFClose(tif);
            return 0;
        }
        
        // Iterate through some strips based on fuzzer input
        uint32_t max_strips_to_test = fdp.ConsumeIntegralInRange<uint32_t>(1, strip_count);
        max_strips_to_test = (max_strips_to_test > 16) ? 16 : max_strips_to_test; // Limit to 16 strips
        
        for (uint32_t strip_idx = 0; strip_idx < max_strips_to_test && strip_idx < strip_count; strip_idx++) {
            // Calculate row for this strip
            uint32_t row = strip_idx * rows_per_strip;
            
            // Clear buffer
            memset(raster_buffer, 0, buffer_size * sizeof(uint32_t));
            
            // Choose between TIFFReadRGBAStrip and TIFFReadRGBAStripExt
            if (fdp.ConsumeBool()) {
                // Use TIFFReadRGBAStripExt with stop_on_error flag from fuzzer
                int stop_on_error = fdp.ConsumeBool();
                TIFFReadRGBAStripExt(tif, row, raster_buffer, stop_on_error);
            } else {
                // Use TIFFReadRGBAStrip
                TIFFReadRGBAStrip(tif, row, raster_buffer);
            }
        }
    }
    
    // Clean up
    if (raster_buffer) {
        _TIFFfree(raster_buffer);
    }
    
    // Test file opening variations if we have enough input remaining
    if (fdp.remaining_bytes() > 1024) {
        // Try different file opening methods
        uint8_t open_method = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        // Create a temporary file for file-based operations
        char temp_filename[] = "/tmp/temp_tiff_XXXXXX.tiff";
        int fd = mkstemps(temp_filename, 6);
        if (fd >= 0) {
            close(fd);  // Close it so TIFFOpen can open it
            
            // Write minimal TIFF data
            FILE* fp = fopen(temp_filename, "wb");
            if (fp) {
                uint8_t header[] = {
                    0x49, 0x49, 0x2A, 0x00,  // Little-endian, TIFF magic
                    0x08, 0x00, 0x00, 0x00,  // IFD offset
                    0x00, 0x00,              // Number of directory entries
                    0x00, 0x00, 0x00, 0x00   // Next IFD offset
                };
                fwrite(header, 1, sizeof(header), fp);
                
                // Write some fuzzer data
                auto temp_data = fdp.ConsumeBytes<uint8_t>(512);
                if (!temp_data.empty()) {
                    fwrite(temp_data.data(), 1, temp_data.size(), fp);
                }
                fclose(fp);
                
                TIFF* test_tif = nullptr;
                switch (open_method) {
                    case 0:
                        // TIFFOpen
                        test_tif = TIFFOpen(temp_filename, "r");
                        break;
                    case 1:
                        // TIFFFdOpen
                        {
                            int tiff_fd = open(temp_filename, O_RDONLY);
                            if (tiff_fd >= 0) {
                                test_tif = TIFFFdOpen(tiff_fd, temp_filename, "r");
                            }
                        }
                        break;
                    case 2:
                        // Try to open with write mode
                        test_tif = TIFFOpen(temp_filename, "w");
                        break;
                }
                
                if (test_tif) {
                    // Quick test of the opened file
                    uint32_t test_width = 0, test_height = 0;
                    TIFFGetField(test_tif, TIFFTAG_IMAGEWIDTH, &test_width);
                    TIFFGetField(test_tif, TIFFTAG_IMAGELENGTH, &test_height);
                    
                    TIFFClose(test_tif);
                }
                
                unlink(temp_filename);
            }
        }
    }
    
    // Final cleanup
    TIFFClose(tif);
    
    return 0;
}

} // namespace harness_003
