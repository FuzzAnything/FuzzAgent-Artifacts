// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>
#include "tiffio.h"

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

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    const char *filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        return 0;
    }
    lseek(fd, 0, SEEK_SET);

    TIFF *tif = TIFFFdOpen(fd, filename, "r");
    if (tif == NULL) {
        close(fd);
        return 0;
    }

    TIFFSetWarningHandler(dummy_warn_handler);
    TIFFSetErrorHandler(dummy_error_handler);

    char emsg[1024];
    emsg[0] = '\0';

    /* Test TIFFRGBAImageOK */
    (void)TIFFRGBAImageOK(tif, emsg);

    /* Test TIFFReadRGBAImage */
    uint32_t width, height;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        /* Limit allocation size to prevent OOM */
        if (width > 4096 || height > 4096) {
            width = 4096;
            height = 4096;
        }
        uint32_t *rgba_buf = (uint32_t *)malloc(width * height * sizeof(uint32_t));
        if (rgba_buf) {
            (void)TIFFReadRGBAImage(tif, width, height, rgba_buf, 0);
            free(rgba_buf);
        }
    }

    /* Test TIFFReadRGBAImageOriented */
    uint32_t *rgba_buf_oriented = NULL;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        if (width > 4096 || height > 4096) {
            width = 4096;
            height = 4096;
        }
        rgba_buf_oriented = (uint32_t *)malloc(width * height * sizeof(uint32_t));
        if (rgba_buf_oriented) {
            (void)TIFFReadRGBAImageOriented(tif, width, height, rgba_buf_oriented, ORIENTATION_BOTLEFT, 0);
            free(rgba_buf_oriented);
        }
    }

    /* Test TIFFRGBAImageBegin and related */
    TIFFRGBAImage img;
    memset(&img, 0, sizeof(img));
    if (TIFFRGBAImageBegin(&img, tif, 0, emsg)) {
        /* Attempt to get a row if dimensions are valid */
        if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
            if (width > 4096 || height > 4096) {
                width = 4096;
                height = 4096;
            }
            uint32_t *row_buf = (uint32_t *)malloc(width * sizeof(uint32_t));
            if (row_buf) {
                (void)TIFFRGBAImageGet(&img, row_buf, 0, 0);
                free(row_buf);
            }
        }
        (void)TIFFRGBAImageEnd(&img);
    }

    /* Test TIFFReadRGBATile */
    uint32_t *tile_buf = NULL;
    uint32_t tile_width, tile_height;
    if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width) &&
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height)) {
        if (tile_width > 4096 || tile_height > 4096) {
            tile_width = 4096;
            tile_height = 4096;
        }
        tile_buf = (uint32_t *)malloc(tile_width * tile_height * sizeof(uint32_t));
        if (tile_buf) {
            (void)TIFFReadRGBATile(tif, 0, 0, tile_buf);
            free(tile_buf);
        }
    }

    TIFFClose(tif);
    close(fd);
    unlink(filename);

    return 0;
}