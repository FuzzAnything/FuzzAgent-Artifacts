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
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Try to set directory 0
    TIFFSetDirectory(tif, 0);
    
    // Get current directory offset
    uint64_t offset = TIFFCurrentDirOffset(tif);
    (void)offset;
    
    // Get number of tiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // Get tile size
    uint64_t tile_size = TIFFTileSize64(tif);
    
    // Try to read first tile if tiles exist
    if (num_tiles > 0 && tile_size > 0 && tile_size < 1024*1024) {
        void* tile_buf = malloc(tile_size);
        if (tile_buf) {
            TIFFReadEncodedTile(tif, 0, tile_buf, tile_size);
            free(tile_buf);
        }
    }
    
    // Get scanline size
    uint64_t scanline_size = TIFFScanlineSize64(tif);
    
    // Try to read first scanline
    if (scanline_size > 0 && scanline_size < 1024*1024) {
        void* scanline_buf = malloc(scanline_size);
        if (scanline_buf) {
            TIFFReadScanline(tif, scanline_buf, 0, 0);
            free(scanline_buf);
        }
    }
    
    // Close the TIFF file
    TIFFClose(tif);
    
    // Try to close again (as specified in the order)
    TIFFClose(tif);
    
    unlink("./dummy_file");
    return 0;
}