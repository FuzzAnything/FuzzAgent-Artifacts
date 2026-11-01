// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
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
#include "tiffio.h"

static void dummy_error_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    /* Silent error handler for fuzzing */
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    /* Silent warning handler for fuzzing */
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Write fuzz data to a dummy file */
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return 0;
    }
    if (fwrite(Data, 1, Size, f) != Size) {
        fclose(f);
        return 0;
    }
    fclose(f);

    /* Open the TIFF file */
    TIFF *tif = TIFFOpen(filename, "rb");
    if (!tif) {
        return 0;
    }

    /* Set custom error and warning handlers to prevent crashes from printing */
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    /* Try to read directory to initialize state */
    TIFFReadDirectory(tif);

    /* Get image dimensions for buffer allocation */
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

    /* Allocate buffer for RGBA data if dimensions are valid and reasonable */
    uint32_t *rgba_buffer = NULL;
    if (width > 0 && height > 0 && width < 10000 && height < 10000) {
        size_t buf_size = width * height * sizeof(uint32_t);
        if (buf_size > 0 && buf_size < (1024 * 1024 * 1024)) { /* Limit to 1GB */
            rgba_buffer = (uint32_t *)malloc(buf_size);
            if (rgba_buffer) {
                memset(rgba_buffer, 0, buf_size);
            }
        }
    }

    /* Fuzz TIFFIsMSB2LSB */
    (void)TIFFIsMSB2LSB(tif);

    /* Fuzz TIFFIsUpSampled */
    (void)TIFFIsUpSampled(tif);

    /* Fuzz TIFFReadRGBAStrip and TIFFReadRGBAStripExt */
    /* Only attempt if we have a buffer and the file is likely striped */
    if (rgba_buffer && width > 0 && height > 0) {
        uint32_t rows_per_strip = 0;
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
        
        /* Avoid division by zero or invalid rows */
        if (rows_per_strip == 0) {
            rows_per_strip = height;
        }

        /* Try reading a few strips */
        for (uint32_t row = 0; row < height && row < 10; row += rows_per_strip) {
            /* Fuzz TIFFReadRGBAStrip */
            TIFFReadRGBAStrip(tif, row, rgba_buffer);

            /* Fuzz TIFFReadRGBAStripExt with different stop_on_error values */
            TIFFReadRGBAStripExt(tif, row, rgba_buffer, 0);
            TIFFReadRGBAStripExt(tif, row, rgba_buffer, 1);
        }
    }

    /* Fuzz TIFFReadRGBAImageOriented */
    if (rgba_buffer && width > 0 && height > 0) {
        /* Try different orientations using integer constants defined in tiff.h */
        /* ORIENTATION_TOPLEFT=1, TOPRIGHT=2, BOTTOMRIGHT=3, BOTTOMLEFT=4 */
        int orientations[] = {1, 2, 3, 4};
        for (int i = 0; i < 4; i++) {
            TIFFReadRGBAImageOriented(tif, width, height, rgba_buffer, 1, orientations[i]);
        }
    }

    /* Cleanup */
    if (rgba_buffer) {
        free(rgba_buffer);
    }
    TIFFClose(tif);

    return 0;
}