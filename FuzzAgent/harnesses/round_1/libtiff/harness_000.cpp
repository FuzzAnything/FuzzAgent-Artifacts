#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <string>
#include <vector>

namespace harness_000 {
/*
 * Fuzzing harness for libtiff library
 * Targets core TIFF APIs: TIFFOpen, TIFFReadDirectory, TIFFReadRGBAImage, etc.
 * Uses FuzzedDataProvider to split input and test various TIFF operations
 * Uses memory-based I/O via TIFFClientOpen to avoid file system dependencies
 */


// Error handler to suppress warnings/errors during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

// Memory I/O structure for TIFFClientOpen
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t offset;
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

// Write function (not used for read-only)
static tmsize_t mem_write(thandle_t handle, void* buf, tmsize_t size) {
    return 0;
}

// Seek function for memory buffer
static toff_t mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_buffer* buffer = (mem_buffer*)handle;
    
    switch (whence) {
        case SEEK_SET:
            buffer->offset = (offset < buffer->size) ? offset : buffer->size;
            break;
        case SEEK_CUR:
            buffer->offset = (buffer->offset + offset < buffer->size) ? 
                            buffer->offset + offset : buffer->size;
            break;
        case SEEK_END:
            buffer->offset = (buffer->size + offset < buffer->size) ? 
                            buffer->size + offset : buffer->size;
            break;
    }
    
    return (toff_t)buffer->offset;
}

// Close function (does nothing for memory buffer)
static int mem_close(thandle_t handle) {
    return 0;
}

// Size function for memory buffer
static toff_t mem_size(thandle_t handle) {
    mem_buffer* buffer = (mem_buffer*)handle;
    return (toff_t)buffer->size;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    if (size < 16) {
        return 0;
    }

    // Set error handlers to suppress warnings
    TIFFSetErrorHandler(tiff_error_handler);
    TIFFSetWarningHandler(tiff_error_handler);

    // Create memory buffer structure
    mem_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;

    // Try to open the TIFF from memory buffer
    TIFF* tif = TIFFClientOpen("MemTIFF", "r", (thandle_t)&buffer,
                               mem_read, mem_write, mem_seek,
                               mem_close, mem_size, NULL, NULL);
    if (!tif) {
        return 0;
    }

    // Test 1: Basic TIFF operations
    // Get image dimensions if available
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

    // Test 2: Read directory information
    // Try to read the first directory (already read by TIFFOpen)
    // Try to read next directory if exists
    int dir_read_result = TIFFReadDirectory(tif);
    
    // If we have valid dimensions, try to read RGBA image
    if (width > 0 && height > 0 && width < 10000 && height < 10000) {
        // Limit dimensions to prevent excessive memory usage
        uint32_t safe_width = width > 4096 ? 4096 : width;
        uint32_t safe_height = height > 4096 ? 4096 : height;
        
        // Allocate buffer for RGBA image
        uint32_t* raster = (uint32_t*)_TIFFmalloc(safe_width * safe_height * sizeof(uint32_t));
        if (raster != nullptr) {
            // Try to read RGBA image
            TIFFReadRGBAImage(tif, safe_width, safe_height, raster, 0);
            _TIFFfree(raster);
        }
    }

    // Test 3: Try to read scanlines if not tiled
    if (!TIFFIsTiled(tif)) {
        uint32_t rows_per_strip = 0;
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
        
        if (rows_per_strip > 0 && rows_per_strip < 10000) {
            tmsize_t scanline_size = TIFFScanlineSize(tif);
            if (scanline_size > 0 && scanline_size < 100000) {
                void* scanline_buffer = _TIFFmalloc(scanline_size);
                if (scanline_buffer != nullptr) {
                    // Try to read a few scanlines
                    for (uint32_t row = 0; row < 10 && row < height; row++) {
                        TIFFReadScanline(tif, scanline_buffer, row, 0);
                    }
                    _TIFFfree(scanline_buffer);
                }
            }
        }
    }

    // Test 4: Try to read strips if available
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    if (strip_count > 0 && strip_count < 1000) {
        uint64_t strip_size = TIFFStripSize64(tif);
        if (strip_size > 0 && strip_size < 1000000) {
            void* strip_buffer = _TIFFmalloc(strip_size);
            if (strip_buffer != nullptr) {
                // Try to read a few strips
                for (uint32_t strip = 0; strip < 5 && strip < strip_count; strip++) {
                    TIFFReadEncodedStrip(tif, strip, strip_buffer, strip_size);
                }
                _TIFFfree(strip_buffer);
            }
        }
    }

    // Test 5: Check if tiled and try to read tiles
    if (TIFFIsTiled(tif)) {
        uint32_t tile_width = 0, tile_length = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length);
        
        uint32_t tile_count = TIFFNumberOfTiles(tif);
        if (tile_count > 0 && tile_count < 1000) {
            uint64_t tile_size = TIFFTileSize64(tif);
            if (tile_size > 0 && tile_size < 1000000) {
                void* tile_buffer = _TIFFmalloc(tile_size);
                if (tile_buffer != nullptr) {
                    // Try to read a few tiles
                    for (uint32_t tile = 0; tile < 5 && tile < tile_count; tile++) {
                        TIFFReadEncodedTile(tif, tile, tile_buffer, tile_size);
                    }
                    _TIFFfree(tile_buffer);
                }
            }
        }
    }

    // Test 6: Try to read EXIF and GPS directories if present
    TIFFReadEXIFDirectory(tif, 0);
    TIFFReadGPSDirectory(tif, 0);

    // Test 7: Test various TIFF query functions
    TIFFIsByteSwapped(tif);
    TIFFIsBigEndian(tif);
    TIFFIsBigTIFF(tif);
    TIFFCurrentDirectory(tif);
    TIFFNumberOfDirectories(tif);
    TIFFFileno(tif);

    // Clean up
    TIFFClose(tif);

    return 0;
}

} // namespace harness_000
