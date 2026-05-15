// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Write fuzzing data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }

    // Call target API functions
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    tmsize_t tile_row_size = TIFFTileRowSize(tif);
    tmsize_t tile_size = TIFFTileSize(tif);
    tmsize_t strip_size = TIFFStripSize(tif);
    
    // For TIFFVTileSize, use a reasonable nrows value
    uint32_t nrows = 1;
    if (TIFFIsTiled(tif)) {
        uint32_t tile_height = 0;
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
        if (tile_height > 0) {
            nrows = tile_height;
        }
    }
    tmsize_t vtile_size = TIFFVTileSize(tif, nrows);
    
    // For TIFFReadEncodedStrip, try to read first strip if exists
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    if (strip_count > 0) {
        // Allocate buffer for strip reading
        tmsize_t buf_size = TIFFStripSize(tif);
        if (buf_size > 0) {
            void *buf = malloc(buf_size);
            if (buf) {
                // Try reading first strip
                tmsize_t read_bytes = TIFFReadEncodedStrip(tif, 0, buf, buf_size);
                (void)read_bytes; // Ignore return value
                free(buf);
            }
        }
    }

    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}