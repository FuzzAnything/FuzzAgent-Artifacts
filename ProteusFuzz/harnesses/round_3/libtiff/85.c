// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetErrorHandlerExtR at tif_open.c:121:6 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "tiffio.h"

static int dummy_error_handler(struct tiff *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static int dummy_warn_handler(struct tiff *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write fuzz input to a dummy file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Allocate options
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove(filename);
        return 0;
    }

    // Set custom handlers with correct signature
    TIFFOpenOptionsSetErrorHandlerExtR(opts, dummy_error_handler, (void *)Data);
    TIFFOpenOptionsSetWarningHandlerExtR(opts, dummy_warn_handler, (void *)Data);

    // Set warn about unknown tags based on input data
    int warn_flag = (Size > 0) ? (Data[0] % 2) : 0;
    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_flag);

    // Try to open the file in read mode
    TIFF *tif = TIFFOpenExt(filename, "r", opts);
    if (tif) {
        uint32_t row;
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &row);
        TIFFClose(tif);
    }

    // Try to open the file in write mode
    TIFF *tif_w = TIFFOpenExt(filename, "w", opts);
    if (tif_w) {
        TIFFClose(tif_w);
    }

    // Cleanup options
    TIFFOpenOptionsFree(opts);

    // Remove the dummy file
    remove(filename);

    return 0;
}