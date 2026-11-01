// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetErrorHandlerExtR at tif_open.c:121:6 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tiffio.h"

static int dummy_error_handler_ext(TIFF *tif, void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static int dummy_warn_handler_ext(TIFF *tif, void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to use for TIFFOpenExt
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Allocate options
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove(dummy_file);
        return 0;
    }

    // Set error and warning handlers
    // Note: The user_data passed here is just a pointer to the fuzz input for variety
    TIFFOpenOptionsSetErrorHandlerExtR(opts, dummy_error_handler_ext, (void *)Data);
    TIFFOpenOptionsSetWarningHandlerExtR(opts, dummy_warn_handler_ext, (void *)Data);

    // Set warn_about_unknown_tags based on first byte
    int warn_flag = (Size > 0) ? (Data[0] % 2) : 0;
    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_flag);

    // Try to open the file
    TIFF *tif = TIFFOpenExt(dummy_file, "r", opts);
    if (tif) {
        TIFFClose(tif);
    }

    // Free options
    TIFFOpenOptionsFree(opts);

    // Cleanup dummy file
    remove(dummy_file);

    return 0;
}