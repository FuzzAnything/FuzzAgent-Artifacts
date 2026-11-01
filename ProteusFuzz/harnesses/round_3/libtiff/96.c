// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

/*
 * Fuzz driver for libtiff targeting:
 * TIFFCreateCustomDirectory, TIFFPrintDirectory, TIFFFreeDirectory,
 * TIFFSetDirectory, TIFFCreateEXIFDirectory, TIFFWriteDirectory
 */

static void dummy_error_handler(void *client_data, const char *module, const char *fmt, va_list ap) {
    /* Suppress error output during fuzzing */
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *client_data, const char *module, const char *fmt, va_list ap) {
    /* Suppress warning output during fuzzing */
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a dummy file for the TIFF handle */
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "w+b");
    if (!fp) {
        return 0;
    }

    /* Write input data to the file to simulate a TIFF-like structure */
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fflush(fp);
    rewind(fp);

    /* Open TIFF file for reading and writing */
    TIFF *tif = TIFFOpen(filename, "w+");
    if (!tif) {
        fclose(fp);
        return 0;
    }

    /* Set custom error and warning handlers to avoid stderr pollution */
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    /*
     * We cannot easily construct a valid TIFFFieldArray from fuzz input
     * due to its complex internal structure. Instead, we rely on
     * TIFFCreateEXIFDirectory which internally uses a predefined array.
     * For TIFFCreateCustomDirectory, we skip it to avoid crashes from
     * invalid pointers, as per instructions: "Do not pass NULL".
     */

    /* Test TIFFCreateEXIFDirectory */
    (void)TIFFCreateEXIFDirectory(tif);

    /* Set some basic tags to make directory non-empty */
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 1);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

    /* Test TIFFWriteDirectory */
    (void)TIFFWriteDirectory(tif);

    /* Test TIFFSetDirectory with index 0 */
    (void)TIFFSetDirectory(tif, 0);

    /* Test TIFFPrintDirectory to stdout (redirected in fuzzing env) */
    /* Use a dummy flags value derived from input if available */
    long flags = 0;
    if (Size > 0) {
        flags = (long)Data[0];
    }
    TIFFPrintDirectory(tif, stdout, flags);

    /* Test TIFFFreeDirectory */
    TIFFFreeDirectory(tif);

    /* Close and cleanup */
    TIFFClose(tif);
    fclose(fp);
    remove(filename);

    return 0;
}