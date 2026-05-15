// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
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

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    tdir_t dir_num = 0;
    int set_dir_result = TIFFSetDirectory(tif, dir_num);
    
    uint64_t dir_offset = TIFFCurrentDirOffset(tif);
    
    uint32_t tile_count = TIFFNumberOfTiles(tif);
    
    uint64_t tile_size = TIFFTileSize64(tif);
    
    if (tile_count > 0 && tile_size > 0 && tile_size < 1024*1024*10) {
        void *tile_buf = malloc(tile_size);
        if (tile_buf) {
            uint32_t tile_to_read = 0;
            if (tile_count > 1 && Size > 4) {
                tile_to_read = *(uint32_t*)Data % tile_count;
            }
            tmsize_t bytes_read = TIFFReadEncodedTile(tif, tile_to_read, tile_buf, tile_size);
            free(tile_buf);
        }
    }
    
    uint64_t scanline_size = TIFFScanlineSize64(tif);
    
    if (scanline_size > 0 && scanline_size < 1024*1024*10) {
        void *scanline_buf = malloc(scanline_size);
        if (scanline_buf) {
            uint32_t row = 0;
            uint16_t sample = 0;
            if (Size > 8) {
                row = *(uint32_t*)(Data + 4) % 1000;
                sample = *(uint16_t*)(Data + 8) % 10;
            }
            int scanline_result = TIFFReadScanline(tif, scanline_buf, row, sample);
            free(scanline_buf);
        }
    }
    
    TIFFClose(tif);
    
    unlink("./dummy_file");
    
    return 0;
}