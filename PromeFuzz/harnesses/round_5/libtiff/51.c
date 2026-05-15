// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFDefaultTileSize at tif_tile.c:267:6 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
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

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }

    uint32_t width = 0, height = 0;
    TIFFDefaultTileSize(tif, &width, &height);

    uint32_t nrows = 1;
    if (Size > 4) {
        nrows = *(const uint32_t *)Data % 1024;
        if (nrows == 0) nrows = 1;
    }
    tmsize_t vtsize = TIFFVTileSize(tif, nrows);

    uint32_t num_tiles = TIFFNumberOfTiles(tif);

    uint32_t current_tile = TIFFCurrentTile(tif);

    uint32_t request = 8192;
    if (Size > 8) {
        request = *(const uint32_t *)(Data + 4) % 65536;
    }
    uint32_t strip_size = TIFFDefaultStripSize(tif, request);

    uint64_t vtsize64 = TIFFVTileSize64(tif, nrows);

    TIFFClose(tif);
    unlink("./dummy_file");

    return 0;
}