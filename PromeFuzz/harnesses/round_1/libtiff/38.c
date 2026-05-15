// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
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
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

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

    // Get image dimensions if available
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

    // Allocate buffer for RGBA data
    uint32_t* raster = NULL;
    if (width > 0 && height > 0 && width < 65536 && height < 65536) {
        size_t raster_size = width * height;
        if (raster_size < 1000000) { // Limit memory usage
            raster = (uint32_t*)malloc(raster_size * sizeof(uint32_t));
        }
    }

    // 1. Test TIFFReadRGBAStrip
    if (raster && TIFFIsTiled(tif) == 0) {
        uint32_t rows_per_strip = 0;
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
        if (rows_per_strip > 0 && height > 0) {
            for (uint32_t row = 0; row < height; row += rows_per_strip) {
                TIFFReadRGBAStrip(tif, row, raster);
            }
        }
    }

    // 2. Test TIFFRGBAImageOK
    char emsg[1024];
    TIFFRGBAImageOK(tif, emsg);

    // 3. Test TIFFReadRGBATileExt
    if (TIFFIsTiled(tif)) {
        uint32_t tile_width = 0, tile_height = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
        
        if (tile_width > 0 && tile_height > 0 && raster) {
            for (uint32_t row = 0; row < height; row += tile_height) {
                for (uint32_t col = 0; col < width; col += tile_width) {
                    TIFFReadRGBATileExt(tif, col, row, raster, 0);
                }
            }
        }
    }

    // 4. Test TIFFReadRGBAImageOriented
    if (raster && width > 0 && height > 0) {
        for (int orientation = 0; orientation <= 7; orientation++) {
            TIFFReadRGBAImageOriented(tif, width, height, raster, orientation, 0);
        }
    }

    // 5. Test TIFFReadRGBAStripExt
    if (raster && TIFFIsTiled(tif) == 0) {
        uint32_t rows_per_strip = 0;
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
        if (rows_per_strip > 0 && height > 0) {
            for (uint32_t row = 0; row < height; row += rows_per_strip) {
                TIFFReadRGBAStripExt(tif, row, raster, 0);
            }
        }
    }

    // 6. Test TIFFReadRGBAImage
    if (raster && width > 0 && height > 0) {
        TIFFReadRGBAImage(tif, width, height, raster, 0);
    }

    // Cleanup
    free(raster);
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}