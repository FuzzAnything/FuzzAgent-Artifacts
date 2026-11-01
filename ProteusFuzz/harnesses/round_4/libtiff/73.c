// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    /* Silent error handler to prevent crashes during fuzzing */
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    /* Silent warning handler to prevent crashes during fuzzing */
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Write fuzzing data to a temporary file */
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    /* Open the TIFF file */
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    /* Set custom error and warning handlers to avoid crashes */
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    /* Fuzz TIFFReadDirectory */
    (void)TIFFReadDirectory(tif);

    /* Fuzz TIFFIsUpSampled */
    (void)TIFFIsUpSampled(tif);

    /* Fuzz TIFFIsMSB2LSB */
    (void)TIFFIsMSB2LSB(tif);

    /* Prepare buffer for RGBA reading */
    uint32_t *buf = NULL;
    uint32_t width = 0;
    uint32_t height = 0;

    /* Attempt to get image dimensions safely */
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        /* Limit allocation to prevent OOM */
        if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
            size_t buf_size = (size_t)width * (size_t)height * sizeof(uint32_t);
            if (buf_size > 0 && buf_size <= (1024 * 1024 * 1024)) { /* 1GB limit */
                buf = (uint32_t *)malloc(buf_size);
                if (buf) {
                    /* Fuzz TIFFReadRGBAImage */
                    (void)TIFFReadRGBAImage(tif, width, height, buf, 1);

                    /* Fuzz TIFFReadRGBAImageOriented with various orientations */
                    int orientations[] = {ORIENTATION_BOTLEFT, ORIENTATION_TOPLEFT,
                                         ORIENTATION_BOTRIGHT, ORIENTATION_TOPRIGHT,
                                         ORIENTATION_LEFTBOT, ORIENTATION_LEFTTOP,
                                         ORIENTATION_RIGHTBOT, ORIENTATION_RIGHTTOP};
                    for (int i = 0; i < 8; i++) {
                        (void)TIFFReadRGBAImageOriented(tif, width, height, buf, 1, orientations[i]);
                    }

                    /* Fuzz TIFFReadRGBAStripExt */
                    /* Try with row 0 and stop_on_error flags */
                    (void)TIFFReadRGBAStripExt(tif, 0, buf, 1);
                    (void)TIFFReadRGBAStripExt(tif, 0, buf, 0);
                }
            }
        }
    }

    /* Cleanup */
    if (buf) {
        free(buf);
    }
    TIFFClose(tif);
    return 0;
}