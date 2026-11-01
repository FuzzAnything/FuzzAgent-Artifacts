// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

/* Custom error handler for TIFFError and TIFFErrorExt */
static void
custom_error_handler(const char *module, const char *fmt, va_list ap)
{
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom extended error handler for TIFFErrorExt */
static void
custom_error_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap)
{
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom warning handler for TIFFWarning and TIFFWarningExt */
static void
custom_warning_handler(const char *module, const char *fmt, va_list ap)
{
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom extended warning handler for TIFFWarningExt */
static void
custom_warning_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap)
{
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Extended error handler for TIFFErrorExtR that returns a bool (int) */
static int
custom_error_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap)
{
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0; /* Continue propagation */
}

/* Extended warning handler for TIFFWarningExtR that returns a bool (int) */
static int
custom_warning_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap)
{
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0; /* Continue propagation */
}

int
LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size)
{
    if (Size == 0) {
        return 0;
    }

    /* Setup custom handlers */
    TIFFSetErrorHandler(custom_error_handler);
    TIFFSetErrorHandlerExt(custom_error_handler_ext);
    TIFFSetWarningHandler(custom_warning_handler);
    TIFFSetWarningHandlerExt(custom_warning_handler_ext);

    /* Test TIFFWarning */
    TIFFWarning("fuzz_module", "Warning message: %s", (char *)Data);

    /* Test TIFFError */
    TIFFError("fuzz_module", "Error message: %s", (char *)Data);

    /* Create a dummy file for file-based tests */
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    /* Open TIFF file for testing extended functions */
    TIFF *tif = NULL;
    thandle_t handle = NULL;
    int handle_owned = 0; /* Flag to track if we opened the file handle manually */

    /* Try to open the file directly first */
    tif = TIFFOpen(dummy_file, "r");
    if (!tif) {
        /* Write a minimal valid TIFF header to ensure we can open it */
        fp = fopen(dummy_file, "wb");
        if (fp) {
            uint8_t header[8] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, fp);
            fclose(fp);
            tif = TIFFOpen(dummy_file, "r");
        }
    }

    /* Get a file handle for TIFFWarningExt/TIFFErrorExt if needed */
    /* Note: TIFFGetClientdata is not available in older libtiff versions or might be internal.
       We will manually open the file to get a handle for the Ext functions if needed.
       The Ext functions take a thandle_t which is typically a FILE* or void* passed to the IO callbacks.
       For testing the API, we can just pass a valid FILE* opened in read mode.
    */
    if (tif) {
        /* We have a valid TIFF handle. We can use the internal file descriptor or just open a new one.
           To be safe and simple, we open a new FILE* for the Ext tests.
           The Ext functions don't strictly require the handle to be the one used by TIFF,
           as they just pass it to the handler.
        */
        fp = fopen(dummy_file, "rb");
        if (fp) {
            handle = (thandle_t)fp;
            handle_owned = 1;
        }
    } else {
        /* Fallback: just open the file to get a handle for Ext functions */
        fp = fopen(dummy_file, "rb");
        if (fp) {
            handle = (thandle_t)fp;
            handle_owned = 1;
        }
    }

    /* Test TIFFWarningExt */
    if (handle) {
        TIFFWarningExt(handle, "fuzz_module", "Warning message ext: %s", (char *)Data);
    }

    /* Test TIFFErrorExt */
    if (handle) {
        TIFFErrorExt(handle, "fuzz_module", "Error message ext: %s", (char *)Data);
    }

    /* Test TIFFWarningExtR and TIFFErrorExtR if TIFF handle is valid */
    if (tif) {
        TIFFWarningExtR(tif, "fuzz_module", "Warning message extR: %s", (char *)Data);
        TIFFErrorExtR(tif, "fuzz_module", "Error message extR: %s", (char *)Data);

        TIFFClose(tif);
    }

    /* Cleanup */
    if (handle_owned && handle) {
        fclose((FILE *)handle);
    }
    
    remove(dummy_file);

    return 0;
}