// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        return 0;
    }
    
    const char *filename = TIFFFileName(tif);
    (void)filename;
    
    uint32_t tag1 = *(const uint32_t*)Data;
    const TIFFField *field1 = TIFFFieldWithTag(tif, tag1);
    (void)field1;
    
    uint32_t tag2 = tag1 + 1;
    const TIFFField *field2 = TIFFFieldWithTag(tif, tag2);
    (void)field2;
    
    uint32_t tag3 = tag1 + 2;
    const TIFFField *field3 = TIFFFieldWithTag(tif, tag3);
    (void)field3;
    
    uint32_t tag4 = tag1 + 3;
    const TIFFField *field4 = TIFFFieldWithTag(tif, tag4);
    (void)field4;
    
    tmsize_t tile_size = TIFFTileSize(tif);
    (void)tile_size;
    
    uint32_t tag5 = tag1 + 4;
    const TIFFField *field5 = TIFFFieldWithTag(tif, tag5);
    (void)field5;
    
    void *mem = _TIFFmalloc(tile_size > 0 ? tile_size : 1024);
    
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;
    
    if (mem && tile_size > 0 && num_tiles > 0) {
        uint32_t tile_idx = tag1 % (num_tiles > 1 ? num_tiles : 1);
        tmsize_t written = TIFFWriteEncodedTile(tif, tile_idx, mem, tile_size);
        (void)written;
    }
    
    uint32_t row1 = tag1 % 100;
    uint16_t sample1 = tag1 % 10;
    int scanline1 = TIFFWriteScanline(tif, mem, row1, sample1);
    (void)scanline1;
    
    uint32_t row2 = (tag1 + 1) % 100;
    uint16_t sample2 = (tag1 + 1) % 10;
    int scanline2 = TIFFWriteScanline(tif, mem, row2, sample2);
    (void)scanline2;
    
    if (mem) {
        _TIFFfree(mem);
    }
    
    TIFFClose(tif);
    return 0;
}