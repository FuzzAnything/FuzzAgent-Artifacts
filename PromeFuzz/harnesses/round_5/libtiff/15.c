// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
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
#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove(filename);
        return 0;
    }

    uint32_t max_alloc;
    memcpy(&max_alloc, Data, sizeof(max_alloc));
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, (tmsize_t)max_alloc);

    TIFF *tif = TIFFOpenExt(filename, "w", opts);
    TIFFOpenOptionsFree(opts);

    if (tif) {
        uint32_t width, height;
        memcpy(&width, Data, sizeof(width));
        memcpy(&height, Data + 4, sizeof(height));

        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);

        TIFFWriteDirectory(tif);
        TIFFClose(tif);
    }

    remove(filename);
    return 0;
}