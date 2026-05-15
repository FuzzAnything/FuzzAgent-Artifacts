#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <string>
#include <vector>

namespace harness_002 {
/*
 * Fuzzing harness for libtiff library - Tile Writing Operations
 * Targets TIFF tile writing APIs: TIFFWriteTile, TIFFWriteEncodedTile, TIFFWriteRawTile
 * Uses TIFFOpenExt for initialization as specified in coverage guidance
 * Semantic differentiation from harness_001: Focuses exclusively on tile-based writing operations
 * and uses TIFFOpenExt with TIFFOpenOptions instead of TIFFClientOpen
 */


// Error handler to suppress warnings/errors during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

// Memory I/O structure for memory-based file simulation
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
    }
    return 0;
}

// Size function for memory buffer
static toff_t mem_size(thandle_t handle) {
    mem_buffer* buffer = (mem_buffer*)handle;
    return (toff_t)buffer->size;
}

// Map and unmap functions (not used)
static int mem_map(thandle_t handle, void** base, toff_t* size) {
    return 0;
}
static void mem_unmap(thandle_t handle, void* base, toff_t size) {
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic tile operations
    if (size < 64) {
        return 0;
    }

    // Set error handlers to suppress warnings
    TIFFSetErrorHandler(tiff_error_handler);
    TIFFSetWarningHandler(tiff_error_handler);

    FuzzedDataProvider fdp(data, size);

    // Create memory buffer structure
    mem_buffer buffer;
    buffer.data = (uint8_t*)malloc(4096);
    if (!buffer.data) {
        return 0;
    }
    buffer.size = 0;
    buffer.offset = 0;
    buffer.capacity = 4096;

    // Create TIFFOpenOptions for TIFFOpenExt as specified in guidance
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        free(buffer.data);
        return 0;
    }

    // Try to open the TIFF in write mode using TIFFClientOpenExt (since we need memory I/O)
    // Note: TIFFOpenExt requires a filename, so we use TIFFClientOpenExt with options
    TIFF* tif = TIFFClientOpenExt("MemTIFF", "w", (thandle_t)&buffer,
                                 mem_read, mem_write, mem_seek,
                                 mem_close, mem_size, mem_map, mem_unmap,
                                 opts);
    
    // Free options after use
    TIFFOpenOptionsFree(opts);
    
    if (!tif) {
        free(buffer.data);
        return 0;
    }

    // Consume fixed-size parameters from fuzzer input for tile configuration
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t tile_width = fdp.ConsumeIntegralInRange<uint32_t>(1, width);
    uint32_t tile_height = fdp.ConsumeIntegralInRange<uint32_t>(1, height);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6; // 0-5 valid values
    uint16_t planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 8; // Several compression types
    uint16_t predictor = fdp.ConsumeIntegral<uint16_t>() % 4; // Prediction scheme

    // Set up image parameters with tile configuration
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    // Set tile-specific fields to ensure tiled mode
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    
    // Set predictor if compression supports it
    if (compression == COMPRESSION_LZW || compression == COMPRESSION_DEFLATE) {
        TIFFSetField(tif, TIFFTAG_PREDICTOR, predictor);
    }

    // Calculate tile size
    tmsize_t tile_size = TIFFTileSize(tif);
    uint32_t tile_count = TIFFNumberOfTiles(tif);
    
    if (tile_size <= 0 || tile_count == 0) {
        TIFFClose(tif);
        return 0;
    }

    // Allocate buffer for tile data using _TIFFmalloc as specified in guidance
    void* tile_buffer = _TIFFmalloc(tile_size);
    if (!tile_buffer) {
        TIFFClose(tif);
        return 0;
    }
    // Generate tile data from remaining fuzzer input
    std::vector<uint8_t> tile_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Fill tile buffer with fuzzed data (repeat if necessary)
    size_t data_to_copy = tile_data.size() < (size_t)tile_size ? tile_data.size() : (size_t)tile_size;
    if (data_to_copy > 0) {
        memcpy(tile_buffer, tile_data.data(), data_to_copy);
        // Fill remaining with pattern if needed
        if (data_to_copy < (size_t)tile_size) {
            memset((uint8_t*)tile_buffer + data_to_copy, 0, (size_t)tile_size - data_to_copy);
        }
    } else {
        // No data left, fill with zeros
        memset(tile_buffer, 0, (size_t)tile_size);
    }

    // Test TIFFWriteTile - write tile at specific coordinates
    // Limit to first few tiles to avoid excessive iterations
    uint32_t tiles_to_test = tile_count > 5 ? 5 : tile_count;
    for (uint32_t tile_idx = 0; tile_idx < tiles_to_test; tile_idx++) {
        // Calculate tile coordinates
        uint32_t tile_col = tile_idx % ((width + tile_width - 1) / tile_width);
        uint32_t tile_row = tile_idx / ((width + tile_width - 1) / tile_width);
        uint32_t x = tile_col * tile_width;
        uint32_t y = tile_row * tile_height;
        
        if (x < width && y < height) {
            // Target API 1: TIFFWriteTile
            TIFFWriteTile(tif, tile_buffer, x, y, 0, 0);
        }
    }

    // Test TIFFWriteEncodedTile - write encoded tile data
    for (uint32_t tile_idx = 0; tile_idx < tiles_to_test; tile_idx++) {
        // Target API 2: TIFFWriteEncodedTile
        TIFFWriteEncodedTile(tif, tile_idx, tile_buffer, tile_size);
    }

    // Test TIFFWriteRawTile - write raw tile data
    for (uint32_t tile_idx = 0; tile_idx < tiles_to_test; tile_idx++) {
        // Target API 3: TIFFWriteRawTile
        TIFFWriteRawTile(tif, tile_idx, tile_buffer, tile_size);
    }

    // Clean up as specified in guidance
    _TIFFfree(tile_buffer);  // Release allocated buffer
    TIFFClose(tif);          // Close TIFF handle (calls mem_close which frees buffer.data)
    
    return 0;
}

} // namespace harness_002
