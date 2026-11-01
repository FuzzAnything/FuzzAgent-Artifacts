// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *filename = "./dummy_file";
    const char *mode = "w";

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    tmsize_t max_mem = (Size > 0) ? (tmsize_t)Data[0] : 0;
    if (max_mem == 0) max_mem = 1024 * 1024;
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_mem);

    TIFF *tif = TIFFOpenExt(filename, mode, opts);
    if (tif == NULL) {
        TIFFOpenOptionsFree(opts);
        return 0;
    }

    TIFFOpenOptionsFree(opts);

    uint32_t width = (Size > 1) ? ((Data[0] & 0x7F) + 1) : 1;
    uint32_t height = (Size > 2) ? ((Data[1] & 0x7F) + 1) : 1;
    uint16_t bits = (Size > 3) ? ((Data[2] & 0x0F) + 1) : 8;
    uint16_t photometric = (Size > 4) ? ((Data[3] & 0x03) + 1) : 1;

    if (TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width) == 0) {
        TIFFClose(tif);
        return 0;
    }
    if (TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height) == 0) {
        TIFFClose(tif);
        return 0;
    }
    if (TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits) == 0) {
        TIFFClose(tif);
        return 0;
    }
    if (TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric) == 0) {
        TIFFClose(tif);
        return 0;
    }

    if (TIFFWriteDirectory(tif) == 0) {
        TIFFClose(tif);
        return 0;
    }

    TIFFClose(tif);
    return 0;
}