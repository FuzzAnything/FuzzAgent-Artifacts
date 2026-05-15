// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff error/warning messages during fuzzing
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Write fuzzer input to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    write(fd, Data, Size);
    close(fd);

    // Open the dummy file as TIFF
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }

    // Read the first directory to ensure TIFF structure is initialized
    TIFFReadDirectory(tif);

    // Extract parameters from fuzzer input if available
    uint32_t nrows = 0;
    uint32_t strip_num = 0;
    
    if (Size >= sizeof(uint32_t)) {
        memcpy(&nrows, Data, sizeof(uint32_t));
    }
    if (Size >= 2 * sizeof(uint32_t)) {
        memcpy(&strip_num, Data + sizeof(uint32_t), sizeof(uint32_t));
    }

    // Call target API functions
    tmsize_t vtsize = TIFFVTileSize(tif, nrows);
    tmsize_t ssize = TIFFStripSize(tif);
    tmsize_t tsize = TIFFTileSize(tif);
    tmsize_t trsize = TIFFTileRowSize(tif);
    tmsize_t slsize = TIFFScanlineSize(tif);

    // Try TIFFReadEncodedStrip if we have valid strip number
    if (ssize > 0) {
        void* buf = _TIFFmalloc(ssize);
        if (buf) {
            TIFFReadEncodedStrip(tif, strip_num, buf, ssize);
            _TIFFfree(buf);
        }
    }

    // Try to read a tile if tiled image
    if (TIFFIsTiled(tif) && tsize > 0) {
        void* tile_buf = _TIFFmalloc(tsize);
        if (tile_buf) {
            TIFFReadTile(tif, tile_buf, 0, 0, 0, 0);
            _TIFFfree(tile_buf);
        }
    }

    // Try to read a scanline
    if (slsize > 0) {
        void* scanline_buf = _TIFFmalloc(slsize);
        if (scanline_buf) {
            TIFFReadScanline(tif, scanline_buf, 0, 0);
            _TIFFfree(scanline_buf);
        }
    }

    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");

    return 0;
}