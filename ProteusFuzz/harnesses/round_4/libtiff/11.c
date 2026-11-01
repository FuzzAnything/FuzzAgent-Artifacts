// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
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

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    if (Size > 0) {
        tmsize_t limit = (tmsize_t)Data[0];
        if (limit == 0) limit = 1;
        TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, limit);
    }

    TIFF *tif = TIFFOpenExt("./dummy_file", "w", opts);
    
    TIFFOpenOptionsFree(opts);

    if (tif == NULL) {
        return 0;
    }

    if (Size >= 2) {
        uint32_t tag1 = (uint32_t)Data[1] | ((uint32_t)Data[2] << 8);
        uint32_t tag2 = (Size > 3) ? (uint32_t)Data[3] | ((uint32_t)Data[4] << 8) : 0;
        
        TIFFSetField(tif, tag1, 1);
        TIFFSetField(tif, tag2, 2);
    }

    TIFFWriteDirectory(tif);

    TIFFClose(tif);

    return 0;
}