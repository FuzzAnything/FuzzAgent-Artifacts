// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

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

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    TIFFSetWarningHandlerExt((TIFFErrorHandlerExt)dummy_warn_handler);
    TIFFSetErrorHandlerExt((TIFFErrorHandlerExt)dummy_error_handler);

    uint32_t width, height;
    uint16_t bitspersample, samplesperpixel, photometric, orientation;
    uint16_t tilewidth, tilelength;

    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) &&
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample) &&
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel) &&
        TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric) &&
        TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation)) {

        uint32_t *rgba_buf = NULL;
        size_t buf_size = 0;

        if (width > 0 && height > 0) {
            buf_size = width * height * sizeof(uint32_t);
            if (buf_size > 1024 * 1024 * 1024) {
                buf_size = 1024 * 1024 * 1024;
            }
            rgba_buf = (uint32_t *)malloc(buf_size);
        }

        if (rgba_buf) {
            int stop = (Data[0] & 1);

            if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth) &&
                TIFFGetField(tif, TIFFTAG_TILELENGTH, &tilelength)) {

                uint32_t tile_buf[256 * 256];
                memset(tile_buf, 0, sizeof(tile_buf));

                uint32_t col = (Data[1] % (width / tilewidth + 1)) * tilewidth;
                uint32_t row = (Data[2] % (height / tilelength + 1)) * tilelength;

                TIFFReadRGBATile(tif, col, row, tile_buf);
                TIFFReadRGBATileExt(tif, col, row, tile_buf, stop);
            }

            TIFFReadRGBAStrip(tif, 0, rgba_buf);

            TIFFReadRGBAImage(tif, width, height, rgba_buf, stop);
            TIFFReadRGBAImageOriented(tif, width, height, rgba_buf,
                                      (Data[3] % 8) + 1,
                                      (Data[4] % 2) ? 1 : 0);

            TIFFRGBAImage img;
            memset(&img, 0, sizeof(img));
            img.tif = tif;
            img.stoponerr = stop;
            img.width = width;
            img.height = height;
            img.bitspersample = bitspersample;
            img.samplesperpixel = samplesperpixel;
            img.orientation = orientation;
            img.req_orientation = orientation;
            img.photometric = photometric;

            char emsg[1024];
            if (TIFFRGBAImageOK(tif, emsg)) {
                if (TIFFRGBAImageBegin(&img, tif, stop, emsg)) {
                    TIFFRGBAImageGet(&img, rgba_buf, width, height);
                    TIFFRGBAImageEnd(&img);
                }
            }

            free(rgba_buf);
        }
    }

    TIFFClose(tif);
    return 0;
}