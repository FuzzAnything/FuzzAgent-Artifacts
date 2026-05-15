// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
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
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }

    uint32_t strile = 0;
    uint32_t nrows = 1;
    int err_flag = 0;
    uint64_t result = 0;
    
    if (Size >= 8) {
        strile = *(const uint32_t *)Data;
        nrows = *(const uint32_t *)(Data + 4);
    }
    
    result = TIFFGetStrileOffsetWithErr(tif, strile, &err_flag);
    
    result = TIFFGetStrileByteCount(tif, strile);
    
    result = TIFFGetStrileByteCountWithErr(tif, strile, &err_flag);
    
    result = TIFFVStripSize64(tif, nrows);
    
    result = TIFFRawStripSize64(tif, strile);
    
    result = TIFFGetStrileOffset(tif, strile);
    
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}