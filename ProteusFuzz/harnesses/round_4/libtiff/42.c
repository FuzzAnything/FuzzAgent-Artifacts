// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFDefaultTileSize at tif_tile.c:267:6 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid TIFF header if input is empty
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Attempt to read directory to initialize state
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // 1. Fuzz TIFFCurrentDirectory
    tdir_t cur_dir = TIFFCurrentDirectory(tif);
    (void)cur_dir; // Use variable to avoid unused warning

    // 2. Fuzz TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;

    // 3. Fuzz TIFFDefaultTileSize
    uint32_t tile_w = 0, tile_h = 0;
    TIFFDefaultTileSize(tif, &tile_w, &tile_h);

    // 4. Fuzz TIFFDefaultStripSize
    uint32_t request_size = (Size > 0) ? (Data[0] % 10000) + 1 : 100;
    uint32_t strip_size = TIFFDefaultStripSize(tif, request_size);
    (void)strip_size;

    // 5. Fuzz TIFFVTileSize64
    uint32_t nrows = (Size > 1) ? ((Data[0] << 8) | Data[1]) % 10000 : 100;
    uint64_t vtile_size = TIFFVTileSize64(tif, nrows);
    
    // 6. Fuzz TIFFReadTile
    if (vtile_size > 0 && vtile_size < (1024 * 1024)) {
        uint8_t *buf = (uint8_t *)malloc(vtile_size);
        if (buf) {
            // Generate diverse coordinates based on input data
            uint32_t x = (Size > 2) ? Data[2] : 0;
            uint32_t y = (Size > 3) ? Data[3] : 0;
            uint32_t z = (Size > 4) ? Data[4] : 0;
            uint16_t s = (Size > 5) ? Data[5] : 0;

            tmsize_t res = TIFFReadTile(tif, buf, x, y, z, s);
            (void)res;

            free(buf);
        }
    }

    TIFFClose(tif);
    return 0;
}