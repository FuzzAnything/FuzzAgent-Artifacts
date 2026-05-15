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
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove_dummy_file();
        return 0;
    }
    
    tmsize_t mem_limit = (tmsize_t)(Data[0] * 1024);
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, mem_limit);
    
    TIFF *tif = TIFFOpenExt("./dummy_file", "w", opts);
    TIFFOpenOptionsFree(opts);
    
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint32_t width = *(uint32_t*)Data;
    uint32_t height = *(uint32_t*)(Data + 4);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    
    TIFFWriteDirectory(tif);
    TIFFClose(tif);
    
    remove_dummy_file();
    return 0;
}