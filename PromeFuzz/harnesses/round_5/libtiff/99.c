// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
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
    // Suppress libtiff error/warning messages during fuzzing
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Create dummy file with fuzzer data
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open TIFF file
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    // Extract parameters from fuzzer input
    uint32_t x = 0, y = 0, z = 0, tile = 0, row = 0;
    uint16_t s = 0;
    
    if (Size >= 4) memcpy(&x, Data, 4);
    if (Size >= 8) memcpy(&y, Data + 4, 4);
    if (Size >= 12) memcpy(&z, Data + 8, 4);
    if (Size >= 14) memcpy(&s, Data + 12, 2);
    if (Size >= 18) memcpy(&tile, Data + 14, 4);
    if (Size >= 22) memcpy(&row, Data + 18, 4);

    // Call target functions
    TIFFIsTiled(tif);
    TIFFIsUpSampled(tif);
    TIFFCheckTile(tif, x, y, z, s);

    // Try to read encoded tile
    unsigned char buf[1024];
    if (Size > 0) {
        tmsize_t buf_size = Size % sizeof(buf);
        if (buf_size == 0) buf_size = sizeof(buf);
        TIFFReadEncodedTile(tif, tile, buf, buf_size);
    }

    // Try to read RGBA tile
    uint32_t rgba_tile_buf[256]; // 16x16 tile
    TIFFReadRGBATile(tif, x, y, rgba_tile_buf);

    // Try to read RGBA strip
    uint32_t rgba_strip_buf[1024]; // 256 pixels
    TIFFReadRGBAStrip(tif, row, rgba_strip_buf);

    // Cleanup
    TIFFClose(tif);
    unlink(filename);
    
    return 0;
}