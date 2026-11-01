// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    // Open the file for reading
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // Set custom error handlers to prevent crashes from logging
    TIFFSetWarningHandler(dummy_warn_handler);
    TIFFSetErrorHandler(dummy_error_handler);

    // Allocate a buffer for tile operations.
    // Size is clamped to prevent allocation of huge buffers.
    size_t buf_size = Size;
    if (buf_size > 1024 * 1024) {
        buf_size = 1024 * 1024;
    }
    if (buf_size == 0) {
        buf_size = 1;
    }
    uint8_t *buf = (uint8_t *)malloc(buf_size);
    if (!buf) {
        TIFFClose(tif);
        return 0;
    }
    memset(buf, 0, buf_size);

    // Get tile count and size
    uint32_t n_tiles = TIFFNumberOfTiles(tif);
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // Fuzz TIFFTileRowSize
    TIFFTileRowSize(tif);

    // Fuzz TIFFVTileSize with varying nrows derived from input
    if (n_tiles > 0) {
        uint32_t nrows = (Size > 0) ? (Data[0] % 100) + 1 : 1;
        TIFFVTileSize(tif, nrows);
    }

    // Fuzz TIFFReadTile
    // Coordinates are derived from input data modulo reasonable limits
    uint32_t x = (Size > 0) ? Data[0] % 1000 : 0;
    uint32_t y = (Size > 1) ? Data[1] % 1000 : 0;
    uint32_t z = (Size > 2) ? Data[2] % 10 : 0;
    uint16_t s = (Size > 3) ? Data[3] % 4 : 0;
    
    TIFFReadTile(tif, buf, x, y, z, s);

    // Fuzz TIFFReadEncodedTile
    for (uint32_t i = 0; i < n_tiles && i < 10; i++) {
        // Use -1 for size to let libtiff determine it, or a small size from input
        tmsize_t read_size = -1;
        if (Size > 4) {
            read_size = (tmsize_t)(Data[4] % 100);
        }
        TIFFReadEncodedTile(tif, i, buf, read_size);
    }

    // Fuzz TIFFReadRawTile
    for (uint32_t i = 0; i < n_tiles && i < 10; i++) {
        tmsize_t read_size = -1;
        if (Size > 5) {
            read_size = (tmsize_t)(Data[5] % 100);
        }
        TIFFReadRawTile(tif, i, buf, read_size);
    }

    // Fuzz TIFFWriteTile (may fail if file is read-only, which is expected)
    // We reuse the coordinates derived earlier.
    TIFFWriteTile(tif, buf, x, y, z, s);

    // Cleanup
    free(buf);
    TIFFClose(tif);
    return 0;
}