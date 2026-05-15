// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetErrorHandlerExtR at tif_open.c:121:6 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "libtiff/tiffio.h"

static int dummy_error_handler(TIFF* tif, void* user_data, const char* module, const char* fmt, va_list ap) {
    (void)tif;
    (void)user_data;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static int dummy_warning_handler(TIFF* tif, void* user_data, const char* module, const char* fmt, va_list ap) {
    (void)tif;
    (void)user_data;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    TIFFOpenOptionsSetErrorHandlerExtR(opts, dummy_error_handler, NULL);
    TIFFOpenOptionsSetWarningHandlerExtR(opts, dummy_warning_handler, NULL);

    const char* filename = "./dummy_file";
    FILE* f = fopen(filename, "wb");
    if (f == NULL) {
        TIFFOpenOptionsFree(opts);
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    const char* mode = "r";
    TIFF* tif = TIFFOpenExt(filename, mode, opts);
    TIFFOpenOptionsFree(opts);

    if (tif != NULL) {
        TIFFErrorExtR(tif, "test_module", "Test error message");
        TIFFWarningExtR(tif, "test_module", "Test warning message");
        TIFFClose(tif);
    }

    remove(filename);
    return 0;
}