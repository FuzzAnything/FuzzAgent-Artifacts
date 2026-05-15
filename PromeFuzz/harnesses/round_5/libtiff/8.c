// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    tdir_t dir_num = 0;
    if (TIFFSetDirectory(tif, dir_num) != 1) {
        TIFFClose(tif);
        remove_dummy_file();
        return 0;
    }
    
    uint64_t dir_offset = TIFFCurrentDirOffset(tif);
    
    uint32_t tile_count = TIFFNumberOfTiles(tif);
    
    uint64_t tile_size = TIFFTileSize64(tif);
    
    if (tile_count > 0 && tile_size > 0 && tile_size <= 1024*1024*10) {
        void *tile_buf = malloc(tile_size);
        if (tile_buf) {
            uint32_t tile_to_read = 0;
            if (tile_to_read < tile_count) {
                tmsize_t read_bytes = TIFFReadEncodedTile(tif, tile_to_read, tile_buf, tile_size);
                (void)read_bytes;
            }
            free(tile_buf);
        }
    }
    
    uint64_t scanline_size = TIFFScanlineSize64(tif);
    
    if (scanline_size > 0 && scanline_size <= 1024*1024*10) {
        void *scanline_buf = malloc(scanline_size);
        if (scanline_buf) {
            uint32_t row = 0;
            uint16_t sample = 0;
            int read_result = TIFFReadScanline(tif, scanline_buf, row, sample);
            (void)read_result;
            free(scanline_buf);
        }
    }
    
    TIFFClose(tif);
    
    TIFF *tif2 = TIFFOpen("./dummy_file", "r");
    if (tif2) {
        TIFFClose(tif2);
    }
    
    remove_dummy_file();
    return 0;
}