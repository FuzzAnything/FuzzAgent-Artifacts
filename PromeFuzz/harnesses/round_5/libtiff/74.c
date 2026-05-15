// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    write(fd, Data, Size);
    close(fd);

    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warning_handler);

    // Open the dummy file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }

    // Call target API functions
    tmsize_t tile_row_size = TIFFTileRowSize(tif);
    tmsize_t tile_size = TIFFTileSize(tif);
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    
    // Use first 4 bytes of data for nrows parameter, default to 1 if not enough data
    uint32_t nrows = 1;
    if (Size >= 4) {
        nrows = *(const uint32_t*)Data;
    }
    tmsize_t vtile_size = TIFFVTileSize(tif, nrows);
    
    tmsize_t strip_size = TIFFStripSize(tif);
    
    // Try to read an encoded strip if we have enough data for parameters
    if (strip_size > 0 && strip_size < 1024*1024) { // Limit to 1MB to avoid OOM
        void* buf = malloc(strip_size);
        if (buf) {
            uint32_t strip = 0;
            if (Size >= 8) {
                strip = *(const uint32_t*)(Data + 4);
            }
            tmsize_t read_size = strip_size;
            if (Size >= 12) {
                read_size = *(const tmsize_t*)(Data + 8);
                if (read_size > strip_size) {
                    read_size = strip_size;
                }
            }
            TIFFReadEncodedStrip(tif, strip, buf, read_size);
            free(buf);
        }
    }

    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}