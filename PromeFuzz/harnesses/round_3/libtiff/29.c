// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tiffio.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint32_t strile_index = 0;
    uint32_t strip_index = 0;
    uint32_t nrows = 0;
    
    if (Size >= 4) {
        strile_index = *(uint32_t*)Data;
        strip_index = strile_index;
    }
    
    if (Size >= 8) {
        nrows = *(uint32_t*)(Data + 4);
    }
    
    uint64_t offset = TIFFGetStrileOffset(tif, strile_index);
    (void)offset;
    
    uint64_t raw_size = TIFFRawStripSize64(tif, strip_index);
    (void)raw_size;
    
    uint64_t byte_count = TIFFGetStrileByteCount(tif, strile_index);
    (void)byte_count;
    
    uint64_t tile_size = TIFFTileSize64(tif);
    (void)tile_size;
    
    uint64_t vstrip_size = TIFFVStripSize64(tif, nrows);
    (void)vstrip_size;
    
    uint64_t strip_size = TIFFStripSize64(tif);
    (void)strip_size;
    
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}