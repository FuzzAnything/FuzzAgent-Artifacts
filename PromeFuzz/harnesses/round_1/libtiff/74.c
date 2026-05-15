// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFDefaultTileSize at tif_tile.c:267:6 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
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

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }

    uint32_t width = 0, height = 0;
    TIFFDefaultTileSize(tif, &width, &height);

    uint32_t x = *(const uint32_t *)Data;
    uint32_t y = *(const uint32_t *)(Data + 4) % 1000;
    uint32_t z = *(const uint32_t *)(Data + 8) % 1000;
    uint16_t s = *(const uint16_t *)(Data + 12) % 10;
    
    int check_result = TIFFCheckTile(tif, x, y, z, s);
    
    uint32_t tile_num = TIFFComputeTile(tif, x, y, z, s);
    
    uint32_t total_tiles = TIFFNumberOfTiles(tif);
    
    uint32_t current_tile = TIFFCurrentTile(tif);
    
    uint32_t nrows = *(const uint32_t *)(Data + 14) % 1000;
    uint64_t tile_size = TIFFVTileSize64(tif, nrows);

    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}