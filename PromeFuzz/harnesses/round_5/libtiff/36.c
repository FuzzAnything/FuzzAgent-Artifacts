// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    uint32_t tile_count = TIFFNumberOfTiles(tif);
    
    for (uint32_t i = 0; i < 10; i++) {
        uint32_t strile = i % (strip_count > 0 ? strip_count : 1);
        uint32_t strip = i % (strip_count > 0 ? strip_count : 1);
        uint32_t nrows = (i % 100) + 1;
        
        int err_flag = 0;
        
        uint64_t byte_count_with_err = TIFFGetStrileByteCountWithErr(tif, strile, &err_flag);
        
        uint64_t raw_strip_size = TIFFRawStripSize64(tif, strip);
        
        uint64_t byte_count = TIFFGetStrileByteCount(tif, strile);
        
        uint64_t offset_with_err = TIFFGetStrileOffsetWithErr(tif, strile, &err_flag);
        
        uint64_t offset = TIFFGetStrileOffset(tif, strile);
        
        uint64_t vstrip_size = TIFFVStripSize64(tif, nrows);
        
        if (TIFFIsTiled(tif)) {
            uint32_t tile_idx = i % (tile_count > 0 ? tile_count : 1);
            TIFFGetStrileByteCountWithErr(tif, tile_idx, &err_flag);
            TIFFGetStrileByteCount(tif, tile_idx);
            TIFFGetStrileOffsetWithErr(tif, tile_idx, &err_flag);
            TIFFGetStrileOffset(tif, tile_idx);
        }
    }
    
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}