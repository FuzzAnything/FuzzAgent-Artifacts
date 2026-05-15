// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
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
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);

    // Write fuzzer data to a dummy file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    write(fd, Data, Size);
    close(fd);

    // Try to open the file in different modes
    const char* modes[] = {"r", "w", "a", "r+", "w+"};
    for (size_t m = 0; m < sizeof(modes)/sizeof(modes[0]); m++) {
        TIFF* tif = TIFFOpen("./dummy_file", modes[m]);
        if (!tif) continue;

        // Query functions that work on any valid TIFF handle
        (void)TIFFCurrentStrip(tif);
        (void)TIFFNumberOfStrips(tif);
        (void)TIFFCurrentRow(tif);
        (void)TIFFCurrentTile(tif);

        // Try TIFFWriteScanline if opened for writing
        if (strchr(modes[m], 'w') || strchr(modes[m], 'a') || strchr(modes[m], '+')) {
            // Set up minimal required fields for writing
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            
            uint8_t scanline[1] = {0};
            if (Size > 0) scanline[0] = Data[0] % 256;
            (void)TIFFWriteScanline(tif, scanline, 0, 0);
        }

        // Try TIFFReadEncodedStrip if opened for reading
        if (strchr(modes[m], 'r') || strchr(modes[m], '+')) {
            uint32_t strip = 0;
            if (Size > 0) strip = Data[0] % 256;
            
            // Try to get strip size
            tmsize_t strip_size = TIFFStripSize(tif);
            if (strip_size > 0) {
                uint8_t* buf = (uint8_t*)malloc(strip_size);
                if (buf) {
                    (void)TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                    free(buf);
                }
            } else {
                // Try with small buffer
                uint8_t small_buf[256];
                (void)TIFFReadEncodedStrip(tif, strip, small_buf, sizeof(small_buf));
            }
        }

        TIFFClose(tif);
    }

    // Clean up
    unlink("./dummy_file");
    return 0;
}