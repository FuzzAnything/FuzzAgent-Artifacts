// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
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

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);

    // Write fuzzer input to a file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    write(fd, Data, Size);
    close(fd);

    // Open the TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }

    // Call target API functions
    tmsize_t raster_scanline_size = TIFFRasterScanlineSize(tif);
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    tmsize_t tile_size = TIFFTileSize(tif);
    tmsize_t strip_size = TIFFStripSize(tif);
    uint32_t num_strips = TIFFNumberOfStrips(tif);

    // Try to read a strip if there are any
    if (num_strips > 0) {
        // Use first strip for reading
        uint32_t strip_to_read = 0;
        
        // Allocate buffer for strip reading
        tmsize_t buf_size = strip_size > 0 ? strip_size : 4096;
        void* buf = malloc(buf_size);
        if (buf) {
            // Try reading with different sizes to explore more states
            tmsize_t read_size = TIFFReadEncodedStrip(tif, strip_to_read, buf, buf_size);
            
            // Try reading with -1 (read full strip)
            if (read_size >= 0) {
                TIFFReadEncodedStrip(tif, strip_to_read, buf, -1);
            }
            
            free(buf);
        }
    }

    // Try to get tile size for tiled images
    if (TIFFIsTiled(tif)) {
        tmsize_t tile_size_again = TIFFTileSize(tif);
        (void)tile_size_again; // Use variable to avoid compiler warning
    }

    // Try to get scanline sizes with different configurations
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &(uint32_t){0});
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &(uint16_t){0});
    
    tmsize_t scanline_size_again = TIFFScanlineSize(tif);
    tmsize_t raster_scanline_size_again = TIFFRasterScanlineSize(tif);
    (void)scanline_size_again;
    (void)raster_scanline_size_again;

    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");

    return 0;
}