// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetErrorHandlerExtR at tif_open.c:121:6 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static int
custom_error_handler_ext(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap)
{
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static int
custom_warn_handler_ext(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap)
{
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

int
LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size)
{
    if (Size == 0) {
        return 0;
    }

    /* Write fuzz data to a dummy file */
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }

    TIFFOpenOptionsSetErrorHandlerExtR(opts, custom_error_handler_ext, NULL);
    TIFFOpenOptionsSetWarningHandlerExtR(opts, custom_warn_handler_ext, NULL);

    TIFF *tif = TIFFOpenExt(filename, "r", opts);

    if (tif) {
        /* Invoke error and warning handlers on the opened TIFF */
        TIFFErrorExtR(tif, "Fuzzer", "Test error: %d", 1);
        TIFFWarningExtR(tif, "Fuzzer", "Test warning: %d", 2);
        TIFFClose(tif);
    }

    /* Also invoke handlers with NULL TIFF to test global handler paths */
    TIFFErrorExtR(NULL, "Fuzzer", "Test error with NULL TIFF: %d", 3);
    TIFFWarningExtR(NULL, "Fuzzer", "Test warning with NULL TIFF: %d", 4);

    TIFFOpenOptionsFree(opts);

    return 0;
}