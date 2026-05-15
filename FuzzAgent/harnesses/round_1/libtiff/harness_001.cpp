#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <string>
#include <vector>

namespace harness_001 {
/*
 * Fuzzing harness for libtiff library - Write Operations
 * Targets TIFF writing APIs: TIFFWriteTile, TIFFWriteEncodedTile, TIFFWriteScanline, 
 * TIFFWriteEncodedStrip, TIFFPrintDirectory, TIFFSetDirectory, TIFFSetSubDirectory,
 * TIFFWriteDirectory, TIFFRewriteDirectory, and TIFFFlush.
 * Uses memory-based I/O via TIFFClientOpen with write support
 * Semantic differentiation from harness_000 which focuses on read operations
 */


// Error handler to suppress warnings/errors during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

// Memory I/O structure for TIFFClientOpen with write support
typedef struct {
    uint8_t* data;
    size_t size;
    size_t offset;
    size_t capacity;
} mem_buffer;

// Read function for memory buffer
static tmsize_t mem_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_buffer* buffer = (mem_buffer*)handle;
    size_t remaining = buffer->size - buffer->offset;
    size_t to_read = (size_t)size;
    
    if (to_read > remaining) {
        to_read = remaining;
    }
    
    if (to_read > 0) {
        memcpy(buf, buffer->data + buffer->offset, to_read);
        buffer->offset += to_read;
    }
    
    return (tmsize_t)to_read;
}

// Write function for memory buffer with dynamic allocation
static tmsize_t mem_write(thandle_t handle, void* buf, tmsize_t size) {
    mem_buffer* buffer = (mem_buffer*)handle;
    size_t needed = buffer->offset + (size_t)size;
    
    // Allocate more space if needed
    if (needed > buffer->capacity) {
        size_t new_capacity = needed * 2;
        uint8_t* new_data = (uint8_t*)realloc(buffer->data, new_capacity);
        if (!new_data) {
            return (tmsize_t)-1;
        }
        buffer->data = new_data;
        buffer->capacity = new_capacity;
    }
    
    memcpy(buffer->data + buffer->offset, buf, (size_t)size);
    buffer->offset += (size_t)size;
    if (buffer->offset > buffer->size) {
        buffer->size = buffer->offset;
    }
    
    return (tmsize_t)size;
}

// Seek function for memory buffer
static toff_t mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_buffer* buffer = (mem_buffer*)handle;
    
    switch (whence) {
        case SEEK_SET:
            buffer->offset = (offset < buffer->capacity) ? offset : buffer->capacity;
            break;
        case SEEK_CUR:
            buffer->offset = (buffer->offset + offset < buffer->capacity) ? 
                            buffer->offset + offset : buffer->capacity;
            break;
        case SEEK_END:
            buffer->offset = (buffer->size + offset < buffer->capacity) ? 
                            buffer->size + offset : buffer->capacity;
            break;
    }
    
    return (toff_t)buffer->offset;
}

// Close function for memory buffer
static int mem_close(thandle_t handle) {
    mem_buffer* buffer = (mem_buffer*)handle;
    if (buffer->data) {
        free(buffer->data);
        buffer->data = nullptr;
    }
    buffer->size = 0;
    buffer->offset = 0;
    buffer->capacity = 0;
    return 0;
}

// Size function for memory buffer
static toff_t mem_size(thandle_t handle) {
    mem_buffer* buffer = (mem_buffer*)handle;
    return (toff_t)buffer->size;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    if (size < 64) {
        return 0;
    }

    // Set error handlers to suppress warnings
    TIFFSetErrorHandler(tiff_error_handler);
    TIFFSetWarningHandler(tiff_error_handler);

    FuzzedDataProvider fdp(data, size);
    
    // Create memory buffer structure with initial capacity
    mem_buffer buffer;
    buffer.capacity = 65536; // 64KB initial capacity
    buffer.data = (uint8_t*)malloc(buffer.capacity);
    if (!buffer.data) {
        return 0;
    }
    buffer.size = 0;
    buffer.offset = 0;
    
    // Try to open the TIFF in write mode from memory buffer
    TIFF* tif = TIFFClientOpen("MemTIFF", "w", (thandle_t)&buffer,
                               mem_read, mem_write, mem_seek,
                               mem_close, mem_size, NULL, NULL);
    if (!tif) {
        free(buffer.data);
        return 0;
    }

    // Consume fixed-size parameters from fuzzer input
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6; // 0-5 valid values
    uint16_t planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 8; // Several compression types
    
    // Set up image parameters
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    // Setup strips/tiles organization
    TIFFSetupStrips(tif);
    
    // Write initial directory
    TIFFWriteDirectory(tif);
    
    // Test directory navigation functions
    TIFFSetDirectory(tif, 0);
    TIFFSetSubDirectory(tif, 0);
    
    // Calculate bytes per sample
    size_t bytes_per_sample = (bits_per_sample + 7) / 8;
    size_t bytes_per_pixel = bytes_per_sample * samples_per_pixel;
    
    // Generate synthetic pixel data from remaining fuzzer input
    std::vector<uint8_t> pixel_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Test TIFFWriteScanline with synthetic data
    if (pixel_data.size() >= width * bytes_per_pixel) {
        for (uint32_t row = 0; row < height && row < 10; row++) {
            size_t offset = (row % (pixel_data.size() / (width * bytes_per_pixel))) * width * bytes_per_pixel;
            if (offset + width * bytes_per_pixel <= pixel_data.size()) {
                TIFFWriteScanline(tif, pixel_data.data() + offset, row, 0);
            }
        }
    }
    
    // Test TIFFWriteEncodedStrip
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    if (strip_count > 0 && pixel_data.size() > 0) {
        for (uint32_t strip = 0; strip < strip_count && strip < 5; strip++) {
            tmsize_t strip_size = TIFFStripSize(tif);
            if (strip_size > 0 && pixel_data.size() >= (size_t)strip_size) {
                size_t offset = (strip % (pixel_data.size() / strip_size)) * strip_size;
                if (offset + strip_size <= pixel_data.size()) {
                    TIFFWriteEncodedStrip(tif, strip, pixel_data.data() + offset, strip_size);
                }
            }
        }
    }
    
    // Check if tiled and test tile writing functions
    if (TIFFIsTiled(tif)) {
        uint32_t tile_width = 0, tile_length = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length);
        
        if (tile_width > 0 && tile_length > 0) {
            uint32_t tile_count = TIFFNumberOfTiles(tif);
            tmsize_t tile_size = TIFFTileSize(tif);
            
            // Test TIFFWriteTile
            if (tile_count > 0 && tile_size > 0 && pixel_data.size() >= (size_t)tile_size) {
                for (uint32_t tile = 0; tile < tile_count && tile < 5; tile++) {
                    uint32_t x = (tile % (width / tile_width)) * tile_width;
                    uint32_t y = (tile / (width / tile_width)) * tile_length;
                    if (x < width && y < height) {
                        size_t offset = (tile % (pixel_data.size() / tile_size)) * tile_size;
                        if (offset + tile_size <= pixel_data.size()) {
                            TIFFWriteTile(tif, pixel_data.data() + offset, x, y, 0, 0);
                        }
                    }
                }
            }
            
            // Test TIFFWriteEncodedTile
            if (tile_count > 0 && tile_size > 0 && pixel_data.size() >= (size_t)tile_size) {
                for (uint32_t tile = 0; tile < tile_count && tile < 5; tile++) {
                    size_t offset = (tile % (pixel_data.size() / tile_size)) * tile_size;
                    if (offset + tile_size <= pixel_data.size()) {
                        TIFFWriteEncodedTile(tif, tile, pixel_data.data() + offset, tile_size);
                    }
                }
            }
        }
    }
    
    // Print directory contents
    TIFFPrintDirectory(tif, stdout, 0);
    
    // Flush data
    TIFFFlush(tif);
    TIFFFlushData(tif);
    
    // Rewrite directory with updated metadata
    TIFFRewriteDirectory(tif);
    
    // Clean up
    TIFFClose(tif);
    
    // Note: mem_close will free buffer.data
    return 0;
}

} // namespace harness_001
