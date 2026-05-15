// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_handle_error(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_handle_warning(const char* module, const char* fmt, va_list ap) {
    // Suppress warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handle_error);
    TIFFSetWarningHandler(dummy_handle_warning);
    
    // Write fuzzer input to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    // Open the TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Extract some parameters from the fuzzer input for API calls
    // Use modulo operations to keep values in reasonable ranges
    uint32_t param1 = 0, param2 = 0, param3 = 0;
    uint16_t param4 = 0;
    uint32_t tile_num = 0;
    
    if (Size >= sizeof(uint32_t) * 3 + sizeof(uint16_t)) {
        memcpy(&param1, Data, sizeof(uint32_t));
        memcpy(&param2, Data + sizeof(uint32_t), sizeof(uint32_t));
        memcpy(&param3, Data + sizeof(uint32_t) * 2, sizeof(uint32_t));
        memcpy(&param4, Data + sizeof(uint32_t) * 3, sizeof(uint16_t));
        memcpy(&tile_num, Data + sizeof(uint32_t) * 3 + sizeof(uint16_t), sizeof(uint32_t));
    } else if (Size > 0) {
        // Use available bytes to initialize parameters
        size_t offset = 0;
        if (Size >= sizeof(uint32_t)) {
            memcpy(&param1, Data, sizeof(uint32_t));
            offset += sizeof(uint32_t);
        }
        if (Size - offset >= sizeof(uint32_t)) {
            memcpy(&param2, Data + offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);
        }
        if (Size - offset >= sizeof(uint32_t)) {
            memcpy(&param3, Data + offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);
        }
        if (Size - offset >= sizeof(uint16_t)) {
            memcpy(&param4, Data + offset, sizeof(uint16_t));
        }
    }
    
    // Call TIFFIsTiled - check if file uses tiles
    int is_tiled = TIFFIsTiled(tif);
    
    // Call TIFFIsUpSampled - check if image data is upsampled
    int is_upsampled = TIFFIsUpSampled(tif);
    
    // Call TIFFCheckTile if the file is tiled
    if (is_tiled) {
        TIFFCheckTile(tif, param1, param2, param3, param4);
    }
    
    // Call TIFFReadEncodedTile if the file is tiled
    if (is_tiled) {
        // Get tile size
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0) {
            void* buf = malloc(tile_size);
            if (buf) {
                TIFFReadEncodedTile(tif, tile_num, buf, tile_size);
                free(buf);
            }
        }
    }
    
    // Call TIFFReadRGBATile if the file is tiled
    if (is_tiled) {
        uint32_t* raster = malloc(sizeof(uint32_t) * 256 * 256); // Reasonable size for tile
        if (raster) {
            TIFFReadRGBATile(tif, param1, param2, raster);
            free(raster);
        }
    }
    
    // Call TIFFReadRGBAStrip (works for both tiled and stripped images)
    {
        uint32_t* raster = malloc(sizeof(uint32_t) * 1024); // Reasonable size for strip
        if (raster) {
            TIFFReadRGBAStrip(tif, param1, raster);
            free(raster);
        }
    }
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}