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
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Use first byte to determine directory index
    tdir_t dir_index = (tdir_t)(Data[0] & 0x0F);
    
    // 1. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index);
    
    // 2. TIFFCurrentDirOffset
    uint64_t dir_offset = TIFFCurrentDirOffset(tif);
    (void)dir_offset; // Suppress unused warning
    
    // 3. TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // 4. TIFFTileSize64
    uint64_t tile_size = TIFFTileSize64(tif);
    
    // 5. TIFFReadEncodedTile (if there are tiles)
    if (num_tiles > 0 && tile_size > 0 && tile_size <= 1024*1024) {
        void *tile_buf = malloc((size_t)tile_size);
        if (tile_buf) {
            // Use second byte for tile index
            uint32_t tile_idx = (uint32_t)(Data[1] % num_tiles);
            TIFFReadEncodedTile(tif, tile_idx, tile_buf, (tmsize_t)tile_size);
            free(tile_buf);
        }
    }
    
    // 6. TIFFScanlineSize64
    uint64_t scanline_size = TIFFScanlineSize64(tif);
    
    // 7. TIFFReadScanline (if scanline size is reasonable)
    if (scanline_size > 0 && scanline_size <= 1024*1024) {
        void *scanline_buf = malloc((size_t)scanline_size);
        if (scanline_buf) {
            // Use third byte for row number
            uint32_t row = (uint32_t)(Data[2]);
            // Use fourth byte for sample
            uint16_t sample = (uint16_t)(Data[3] & 0x0F);
            TIFFReadScanline(tif, scanline_buf, row, sample);
            free(scanline_buf);
        }
    }
    
    // 8. TIFFClose
    TIFFClose(tif);
    
    // 9. TIFFClose (second call as specified)
    // Note: tif is already closed, but we'll call on NULL to match specification
    TIFFClose(NULL);
    
    cleanup_dummy_file();
    return 0;
}