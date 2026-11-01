// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void dummy_error_handler(const char *module, const char *fmt) {
    (void)module;
    (void)fmt;
}

static void dummy_warn_handler(const char *module, const char *fmt) {
    (void)module;
    (void)fmt;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    if (Size > 0) {
        tmsize_t limit = (tmsize_t)Data[0];
        if (limit == 0) limit = 1;
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, limit);
    }

    TIFF *tif = TIFFOpenExt(filename, "w", opts);
    if (tif == NULL) {
        TIFFOpenOptionsFree(opts);
        return 0;
    }

    TIFFOpenOptionsFree(opts);

    if (Size >= 2) {
        uint32_t tag1 = Data[0] | ((uint32_t)Data[1] << 8);
        uint16_t val1 = (uint16_t)Data[0];
        TIFFSetField(tif, tag1, val1);
    }

    if (Size >= 4) {
        uint32_t tag2 = Data[2] | ((uint32_t)Data[3] << 8);
        uint32_t val2 = (uint32_t)Data[2] | ((uint32_t)Data[3] << 8);
        TIFFSetField(tif, tag2, val2);
    }

    TIFFWriteDirectory(tif);

    TIFFClose(tif);

    return 0;
}