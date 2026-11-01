// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tiffio.h"

/* Custom error handler for testing */
static void customErrorHandler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom warning handler for testing */
static void customWarningHandler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom extended error handler for testing */
static void customErrorHandlerExt(void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom extended warning handler for testing */
static void customWarningHandlerExt(void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom tag extender for testing. 
   Based on the compiler error, TIFFExtendProc is defined as void (*)(TIFF *).
   We must match this signature exactly.
*/
static void customTagExtender(TIFF *tif) {
    (void)tif;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Save original handlers to restore later */
    TIFFErrorHandler origWarningHandler = TIFFSetWarningHandler(NULL);
    TIFFExtendProc origTagExtender = TIFFSetTagExtender(NULL);
    TIFFErrorHandlerExt origErrorHandlerExt = TIFFSetErrorHandlerExt(NULL);
    TIFFErrorHandlerExt origWarningHandlerExt = TIFFSetWarningHandlerExt(NULL);
    TIFFErrorHandler origErrorHandler = TIFFSetErrorHandler(NULL);

    /* Use input data to decide which functions to call and with what arguments */
    uint8_t choice = Data[0] % 7;

    switch (choice) {
        case 0:
            /* Test TIFFSetWarningHandler */
            TIFFSetWarningHandler(customWarningHandler);
            break;
        case 1:
            /* Test TIFFSetTagExtender */
            TIFFSetTagExtender(customTagExtender);
            break;
        case 2:
            /* Test TIFFSetErrorHandlerExt */
            TIFFSetErrorHandlerExt(customErrorHandlerExt);
            break;
        case 3:
            /* Test TIFFError - ensure Size >= 2 before accessing Data[1] */
            if (Size >= 2) {
                TIFFError("fuzz_test", "Test error message: %d", Data[1]);
            }
            break;
        case 4:
            /* Test TIFFSetWarningHandlerExt */
            TIFFSetWarningHandlerExt(customWarningHandlerExt);
            break;
        case 5:
            /* Test TIFFSetErrorHandler */
            TIFFSetErrorHandler(customErrorHandler);
            break;
        case 6:
            /* Test multiple handlers in sequence */
            TIFFSetWarningHandler(customWarningHandler);
            TIFFSetErrorHandler(customErrorHandler);
            TIFFSetWarningHandlerExt(customWarningHandlerExt);
            TIFFSetErrorHandlerExt(customErrorHandlerExt);
            TIFFSetTagExtender(customTagExtender);
            if (Size >= 2) {
                TIFFError("fuzz_test", "Multi-handler test: %d", Data[1]);
            }
            break;
    }

    /* Restore original handlers */
    TIFFSetWarningHandler(origWarningHandler);
    TIFFSetTagExtender(origTagExtender);
    TIFFSetErrorHandlerExt(origErrorHandlerExt);
    TIFFSetWarningHandlerExt(origWarningHandlerExt);
    TIFFSetErrorHandler(origErrorHandler);

    return 0;
}