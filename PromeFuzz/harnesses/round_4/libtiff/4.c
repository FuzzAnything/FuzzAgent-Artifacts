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
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    const char *filename = TIFFFileName(tif);
    
    uint32_t tag1 = *(uint32_t *)Data;
    uint32_t tag2 = *(uint32_t *)(Data + 4);
    uint32_t tag3 = *(uint32_t *)(Data + 8);
    uint32_t tag4 = *(uint32_t *)(Data + 12);
    uint32_t tag5 = *(uint32_t *)(Data + 16);
    
    const TIFFField *field1 = TIFFFieldWithTag(tif, tag1);
    const TIFFField *field2 = TIFFFieldWithTag(tif, tag2);
    const TIFFField *field3 = TIFFFieldWithTag(tif, tag3);
    const TIFFField *field4 = TIFFFieldWithTag(tif, tag4);
    
    tmsize_t tile_size = TIFFTileSize(tif);
    
    const TIFFField *field5 = TIFFFieldWithTag(tif, tag5);
    
    tmsize_t alloc_size = tile_size > 0 ? tile_size : 1024;
    void *buffer = _TIFFmalloc(alloc_size);
    
    if (buffer && Size > 24) {
        size_t copy_size = Size - 24;
        if (copy_size > alloc_size) copy_size = alloc_size;
        memcpy(buffer, Data + 24, copy_size);
    }
    
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    if (buffer && tile_size > 0 && num_tiles > 0) {
        uint32_t tile_idx = *(uint32_t *)(Data + 20) % num_tiles;
        tmsize_t write_size = tile_size;
        if (write_size > alloc_size) write_size = alloc_size;
        TIFFWriteEncodedTile(tif, tile_idx, buffer, write_size);
    }
    
    if (buffer) {
        uint32_t width = 0, height = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        
        if (width > 0 && height > 0) {
            tmsize_t scanline_size = TIFFScanlineSize(tif);
            if (scanline_size > 0 && scanline_size <= alloc_size) {
                uint32_t row1 = *(uint32_t *)(Data + 20) % height;
                uint32_t row2 = *(uint32_t *)(Data + 24) % height;
                uint16_t sample1 = *(uint16_t *)(Data + 22) % 4;
                uint16_t sample2 = *(uint16_t *)(Data + 26) % 4;
                
                TIFFWriteScanline(tif, buffer, row1, sample1);
                TIFFWriteScanline(tif, buffer, row2, sample2);
            }
        }
    }
    
    if (buffer) {
        _TIFFfree(buffer);
    }
    
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}