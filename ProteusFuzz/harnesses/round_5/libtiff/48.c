// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_error_handler);

    uint32_t width, height;
    if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) ||
        !TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        TIFFClose(tif);
        return 0;
    }

    if (width > 4096 || height > 4096) {
        TIFFClose(tif);
        return 0;
    }

    uint32_t *rgba_buf = NULL;
    size_t buf_size = width * height * sizeof(uint32_t);
    if (buf_size > 0 && buf_size < (100 * 1024 * 1024)) {
        rgba_buf = (uint32_t *)malloc(buf_size);
        if (!rgba_buf) {
            TIFFClose(tif);
            return 0;
        }
        memset(rgba_buf, 0, buf_size);
    }

    if (rgba_buf) {
        TIFFReadRGBAImage(tif, width, height, rgba_buf, 0);
        TIFFReadRGBAImage(tif, width, height, rgba_buf, 1);
    }

    if (rgba_buf) {
        TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, ORIENTATION_TOPLEFT, ORIENTATION_LEFTBOT);
        TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, ORIENTATION_BOTLEFT, ORIENTATION_LEFTBOT);
        TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, ORIENTATION_TOPLEFT, ORIENTATION_RIGHTBOT);
        TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, ORIENTATION_BOTLEFT, ORIENTATION_RIGHTBOT);
    }

    TIFFRGBAImage img;
    char errbuf[1024];
    if (TIFFRGBAImageOK(tif, &img)) {
        if (TIFFRGBAImageBegin(&img, tif, 0, errbuf)) {
            if (rgba_buf) {
                TIFFRGBAImageGet(&img, rgba_buf, width, height);
            }
            TIFFRGBAImageEnd(&img);
        }
    }

    uint32_t rowsperstrip;
    if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip) && rowsperstrip > 0) {
        if (rgba_buf) {
            TIFFReadRGBAStrip(tif, 0, rgba_buf);
            TIFFReadRGBAStripExt(tif, 0, rgba_buf, 0);
            TIFFReadRGBAStripExt(tif, 0, rgba_buf, 1);
            uint32_t numstrips = (height + rowsperstrip - 1) / rowsperstrip;
            for (uint32_t i = 1; i < numstrips && i < 10; i++) {
                TIFFReadRGBAStrip(tif, i, rgba_buf);
                TIFFReadRGBAStripExt(tif, i, rgba_buf, 0);
            }
        }
    }

    uint32_t tilewidth, tilelength;
    if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth) &&
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tilelength)) {
        if (tilewidth > 0 && tilelength > 0 && tilewidth <= width && tilelength <= height) {
            size_t tile_buf_size = tilewidth * tilelength * sizeof(uint32_t);
            if (tile_buf_size < (100 * 1024 * 1024)) {
                uint32_t *tile_buf = (uint32_t *)malloc(tile_buf_size);
                if (tile_buf) {
                    memset(tile_buf, 0, tile_buf_size);
                    TIFFReadRGBATileExt(tif, 0, 0, tile_buf, 0);
                    TIFFReadRGBATileExt(tif, 0, 0, tile_buf, 1);
                    uint32_t num_x = (width + tilewidth - 1) / tilewidth;
                    uint32_t num_y = (height + tilelength - 1) / tilelength;
                    for (uint32_t y = 0; y < num_y && y < 5; y++) {
                        for (uint32_t x = 0; x < num_x && x < 5; x++) {
                            TIFFReadRGBATileExt(tif, x, y, tile_buf, 0);
                            TIFFReadRGBATileExt(tif, x, y, tile_buf, 1);
                        }
                    }
                    free(tile_buf);
                }
            }
        }
    }

    if (rgba_buf) {
        free(rgba_buf);
    }
    TIFFClose(tif);
    return 0;
}