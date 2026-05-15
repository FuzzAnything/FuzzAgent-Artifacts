// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
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
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    // First sequence
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t width = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 1024*1024) {
            void* buf = malloc(tile_size);
            if (buf) {
                TIFFReadEncodedTile(tif, 0, buf, tile_size);
                free(buf);
            }
        }
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)(Data[0] * 256 + Data[1] % 256));
        
        tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 1024*1024) {
            void* buf = malloc(tile_size);
            if (buf) {
                TIFFReadEncodedTile(tif, 1 % (Size > 2 ? Data[2] + 1 : 1), buf, tile_size);
                free(buf);
            }
        }
        
        TIFFClose(tif);
    }
    
    // Second sequence
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t rgba_buf[4096];
        uint32_t x = Size > 3 ? Data[3] : 0;
        uint32_t y = Size > 4 ? Data[4] : 0;
        TIFFReadRGBATile(tif, x, y, rgba_buf);
        TIFFClose(tif);
    }
    
    unlink("./dummy_file");
    return 0;
}