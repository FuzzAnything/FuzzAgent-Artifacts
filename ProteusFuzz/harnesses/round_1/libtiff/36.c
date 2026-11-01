// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    /* Do nothing */
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Write input data to a temporary file */
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = TIFFOpen(filename, "rb");
    if (!tif) {
        return 0;
    }

    /* Set custom error handlers to prevent crashes from printing */
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_error_handler);

    uint32_t width, height;
    if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) ||
        !TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        TIFFClose(tif);
        return 0;
    }

    /* Prevent allocation of excessively large buffers */
    if (width > 4096 || height > 4096) {
        TIFFClose(tif);
        return 0;
    }

    uint32_t *rgba_buf = NULL;
    size_t buf_size = (size_t)width * height * sizeof(uint32_t);
    if (buf_size > 0 && buf_size / sizeof(uint32_t) == width * height) {
        rgba_buf = (uint32_t *)malloc(buf_size);
    }

    if (rgba_buf) {
        /* Test 1: TIFFReadRGBAImage */
        TIFFReadRGBAImage(tif, width, height, rgba_buf, 0);

        /* Test 2: TIFFReadRGBAImageOriented */
        TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, ORIENTATION_TOPLEFT, 0);
        TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, ORIENTATION_BOTLEFT, 0);
        TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, ORIENTATION_TOPRIGHT, 0);
        TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, ORIENTATION_BOTRIGHT, 0);
        TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, ORIENTATION_LEFTTOP, 0);
        TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, ORIENTATION_RIGHTTOP, 0);

        /* Test 3: TIFFReadRGBATileExt */
        uint32_t tile_width, tile_length;
        if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width) &&
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length)) {
            if (tile_width > 0 && tile_length > 0 &&
                tile_width <= 4096 && tile_length <= 4096) {
                size_t tile_buf_size = (size_t)tile_width * tile_length * sizeof(uint32_t);
                if (tile_buf_size > 0 && tile_buf_size / sizeof(uint32_t) == tile_width * tile_length) {
                    uint32_t *tile_buf = (uint32_t *)malloc(tile_buf_size);
                    if (tile_buf) {
                        TIFFReadRGBATileExt(tif, 0, 0, tile_buf, 0);
                        TIFFReadRGBATileExt(tif, 0, 0, tile_buf, 1);
                        free(tile_buf);
                    }
                }
            }
        }

        /* Test 4: TIFFReadRGBAStrip */
        uint32_t rows_per_strip;
        if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip)) {
            if (rows_per_strip > 0 && rows_per_strip <= height) {
                /* Ensure buffer is large enough for a strip */
                if (rgba_buf) {
                    TIFFReadRGBAStrip(tif, 0, rgba_buf);
                }
            }
        }

        /* Test 5: TIFFReadRGBAStripExt */
        if (rgba_buf) {
            TIFFReadRGBAStripExt(tif, 0, rgba_buf, 0);
            TIFFReadRGBAStripExt(tif, 0, rgba_buf, 1);
        }

        /* Test 6: TIFFRGBAImageGet (requires TIFFRGBAImage struct setup) */
        TIFFRGBAImage img;
        memset(&img, 0, sizeof(img));
        char emsg[1024];
        if (TIFFRGBAImageOK(tif, &img)) {
            if (TIFFRGBAImageBegin(&img, tif, 0, emsg)) {
                TIFFRGBAImageGet(&img, rgba_buf, width, height);
            }
        }
    }

    if (rgba_buf) {
        free(rgba_buf);
    }

    TIFFClose(tif);
    return 0;
}