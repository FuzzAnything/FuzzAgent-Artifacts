// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "libtiff/tiffio.h"

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Write fuzzer input to a temporary file
    const char* filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    write(fd, Data, Size);
    close(fd);

    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warning_handler);

    // Open the TIFF file
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    // Extract parameters from fuzzer input
    uint32_t params[4];
    size_t param_size = sizeof(params);
    if (Size >= param_size) {
        memcpy(params, Data, param_size);
    } else {
        // Use partial data if available
        memset(params, 0, param_size);
        memcpy(params, Data, Size);
    }

    // Call target API functions
    uint32_t row = params[0];
    uint32_t x = params[1];
    uint32_t y = params[2];
    uint32_t z = params[3];
    uint16_t sample = (uint16_t)(params[0] & 0xFFFF);
    int stop_on_error = (params[1] & 0x1);

    // 1. TIFFReadRGBAStrip
    uint32_t* raster_strip = NULL;
    if (TIFFIsTiled(tif) == 0) {  // Only call for strip-based images
        uint32_t strip_size = TIFFStripSize(tif);
        if (strip_size > 0 && strip_size < 1024 * 1024 * 10) {  // Reasonable size limit
            raster_strip = (uint32_t*)malloc(strip_size);
            if (raster_strip) {
                TIFFReadRGBAStrip(tif, row, raster_strip);
                free(raster_strip);
            }
        }
    }

    // 2. TIFFCheckTile
    TIFFCheckTile(tif, x, y, z, sample);

    // 3. TIFFReadRGBATileExt
    uint32_t* raster_tile_ext = NULL;
    if (TIFFIsTiled(tif)) {
        uint32_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 1024 * 1024 * 10) {  // Reasonable size limit
            raster_tile_ext = (uint32_t*)malloc(tile_size);
            if (raster_tile_ext) {
                TIFFReadRGBATileExt(tif, x, y, raster_tile_ext, stop_on_error);
                free(raster_tile_ext);
            }
        }
    }

    // 4. TIFFReadRGBATile
    uint32_t* raster_tile = NULL;
    if (TIFFIsTiled(tif)) {
        uint32_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 1024 * 1024 * 10) {  // Reasonable size limit
            raster_tile = (uint32_t*)malloc(tile_size);
            if (raster_tile) {
                TIFFReadRGBATile(tif, x, y, raster_tile);
                free(raster_tile);
            }
        }
    }

    // 5. TIFFIsTiled
    TIFFIsTiled(tif);

    // 6. TIFFIsCODECConfigured
    uint16_t codec = (uint16_t)(params[0] & 0xFFFF);
    TIFFIsCODECConfigured(codec);

    // Cleanup
    TIFFClose(tif);
    unlink(filename);

    return 0;
}