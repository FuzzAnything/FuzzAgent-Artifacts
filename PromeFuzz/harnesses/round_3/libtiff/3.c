// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    }
    
    const char* filename = TIFFFileName(tif);
    (void)filename;
    
    uint32_t tag1 = *(const uint32_t*)Data;
    const TIFFField* field1 = TIFFFieldWithTag(tif, tag1);
    (void)field1;
    
    uint32_t tag2 = tag1 + 1;
    const TIFFField* field2 = TIFFFieldWithTag(tif, tag2);
    (void)field2;
    
    uint32_t tag3 = tag1 + 2;
    const TIFFField* field3 = TIFFFieldWithTag(tif, tag3);
    (void)field3;
    
    uint32_t tag4 = tag1 + 3;
    const TIFFField* field4 = TIFFFieldWithTag(tif, tag4);
    (void)field4;
    
    tmsize_t tile_size = TIFFTileSize(tif);
    
    uint32_t tag5 = tag1 + 4;
    const TIFFField* field5 = TIFFFieldWithTag(tif, tag5);
    (void)field5;
    
    void* buffer = _TIFFmalloc(tile_size > 0 ? tile_size : 1024);
    if (!buffer) {
        TIFFClose(tif);
        unlink("./dummy_file");
        return 0;
    }
    
    if (tile_size > 0 && Size >= tile_size) {
        memcpy(buffer, Data, tile_size > Size ? Size : tile_size);
    } else if (Size > 0) {
        memcpy(buffer, Data, Size > 1024 ? 1024 : Size);
    }
    
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    if (num_tiles > 0) {
        uint32_t tile_idx = tag1 % (num_tiles > 1 ? num_tiles : 1);
        tmsize_t write_size = tile_size > 0 ? tile_size : 1024;
        if (write_size > Size) write_size = Size;
        TIFFWriteEncodedTile(tif, tile_idx, buffer, write_size);
    }
    
    uint32_t row1 = tag1 % 100;
    uint16_t sample1 = tag1 % 10;
    TIFFWriteScanline(tif, buffer, row1, sample1);
    
    uint32_t row2 = (tag1 + 1) % 100;
    uint16_t sample2 = (tag1 + 1) % 10;
    TIFFWriteScanline(tif, buffer, row2, sample2);
    
    _TIFFfree(buffer);
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}