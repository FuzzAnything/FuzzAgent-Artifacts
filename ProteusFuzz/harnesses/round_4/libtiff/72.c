// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

/*
 * Fuzz driver for libtiff reading functions.
 * Target APIs:
 *  - TIFFReadRGBAImage
 *  - TIFFIsUpSampled
 *  - TIFFReadRGBAImageOriented
 *  - TIFFReadDirectory
 *  - TIFFReadRGBAStripExt
 *  - TIFFIsMSB2LSB
 */

/* 
 * Define missing orientation constants if not present in the specific libtiff version
 * to ensure compilation across different versions.
 */
#ifndef ORIENTATION_LEFTTOP
#define ORIENTATION_LEFTTOP 5
#endif
#ifndef ORIENTATION_RIGHTTOP
#define ORIENTATION_RIGHTTOP 6
#endif
#ifndef ORIENTATION_LEFTBOTTOM
#define ORIENTATION_LEFTBOTTOM 7
#endif
#ifndef ORIENTATION_RIGHTBOTTOM
#define ORIENTATION_RIGHTBOTTOM 8
#endif

/*
 * Error and warning handlers matching the TIFFErrorHandlerExt signature:
 * void (*TIFFErrorHandlerExt)(void *clientdata, const char *module, const char *fmt, va_list ap)
 */
static void dummy_error_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Write input data to a dummy file */
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    /* Open TIFF file */
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    /* Set custom error handlers to avoid crashes on malformed input */
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    /* Test 1: TIFFIsMSB2LSB */
    int is_msb2lsb = TIFFIsMSB2LSB(tif);
    (void)is_msb2lsb;

    /* Test 2: TIFFReadDirectory */
    int dir_result = TIFFReadDirectory(tif);
    (void)dir_result;

    /* Test 3: TIFFIsUpSampled */
    int is_upsampled = TIFFIsUpSampled(tif);
    (void)is_upsampled;

    /* Get image dimensions for RGBA reading */
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        /* Allocate buffer for RGBA image */
        uint32_t *rgba_buf = NULL;
        size_t buf_size = (size_t)width * (size_t)height * sizeof(uint32_t);
        if (buf_size > 0 && buf_size < (1ULL << 30)) { /* Prevent huge allocations */
            rgba_buf = (uint32_t *)malloc(buf_size);
            if (rgba_buf) {
                memset(rgba_buf, 0, buf_size);

                /* Test 4: TIFFReadRGBAImage */
                int ret1 = TIFFReadRGBAImage(tif, width, height, rgba_buf, 0);
                (void)ret1;

                /* Test 5: TIFFReadRGBAImageOriented with various orientations */
                int orientations[] = {
                    ORIENTATION_TOPLEFT, ORIENTATION_TOPRIGHT,
                    ORIENTATION_BOTLEFT, ORIENTATION_BOTRIGHT,
                    ORIENTATION_LEFTTOP, ORIENTATION_RIGHTTOP,
                    ORIENTATION_LEFTBOTTOM, ORIENTATION_RIGHTBOTTOM
                };

                for (int i = 0; i < 8; i++) {
                    int ret2 = TIFFReadRGBAImageOriented(tif, width, height, rgba_buf,
                                                         orientations[i], 0);
                    (void)ret2;
                }

                /* Test 6: TIFFReadRGBAStripExt */
                uint32_t rowsperstrip = 0;
                if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip) && rowsperstrip > 0) {
                    /* Try reading a few strips */
                    for (uint32_t strip = 0; strip < 3; strip++) {
                        int ret3 = TIFFReadRGBAStripExt(tif, strip, rgba_buf, 0);
                        (void)ret3;
                    }
                }

                free(rgba_buf);
            }
        }
    }

    /* Cleanup */
    TIFFClose(tif);

    return 0;
}