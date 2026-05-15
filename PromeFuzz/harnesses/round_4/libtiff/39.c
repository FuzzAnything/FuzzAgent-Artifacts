// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFRasterScanlineSize64 at tif_strip.c:357:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint32_t nrows = 1;
    if (Size >= 14) {
        nrows = *(const uint32_t *)(Data + 10);
        if (nrows == 0) nrows = 1;
    }
    
    uint64_t result;
    
    result = TIFFTileRowSize64(tif);
    (void)result;
    
    result = TIFFRasterScanlineSize64(tif);
    (void)result;
    
    result = TIFFVStripSize64(tif, nrows);
    (void)result;
    
    result = TIFFStripSize64(tif);
    (void)result;
    
    result = TIFFScanlineSize64(tif);
    (void)result;
    
    result = TIFFTileSize64(tif);
    (void)result;
    
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}