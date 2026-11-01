// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
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
    if (f == NULL) {
        return;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, f);
    } else {
        // Write a minimal valid TIFF header if input is empty to avoid immediate open failure
        uint16_t header[8] = {42, 0, 0, 0, 8, 0, 0, 0};
        fwrite(header, 1, sizeof(header), f);
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    // Open the file for reading
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    // Ensure we have a valid directory to work with
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Check if the file is tiled, as many target functions are tile-specific
    int is_tiled = 0;
    uint32_t tile_width, tile_length;
    if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width) && 
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length)) {
        is_tiled = 1;
    }

    // Prepare buffer for reading/writing
    // Use a safe size based on input or a reasonable default
    size_t buf_size = Size > 0 ? Size : 1024;
    if (buf_size > 1024 * 1024) buf_size = 1024 * 1024; // Cap at 1MB
    uint8_t *buf = (uint8_t *)malloc(buf_size);
    if (buf == NULL) {
        TIFFClose(tif);
        return 0;
    }

    if (is_tiled) {
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
        tmsize_t tile_size = TIFFTileSize(tif);
        
        // Fuzz TIFFReadRawTile
        if (num_tiles > 0) {
            uint32_t tile_idx = (Data[0] % num_tiles);
            tmsize_t ret = TIFFReadRawTile(tif, tile_idx, buf, tile_size);
            (void)ret;
        }

        // Fuzz TIFFVTileSize
        uint32_t nrows = (Data[1] % 1000) + 1;
        tmsize_t vsize = TIFFVTileSize(tif, nrows);
        (void)vsize;

        // Fuzz TIFFReadEncodedTile
        if (num_tiles > 0) {
            uint32_t tile_idx2 = (Data[2] % num_tiles);
            tmsize_t ret2 = TIFFReadEncodedTile(tif, tile_idx2, buf, -1);
            (void)ret2;
        }

        // Fuzz TIFFReadTile
        // We need to generate valid x, y, z, s coordinates
        // x, y are derived from tile index or random modulo image dimensions
        uint32_t x = (Data[3] % 100);
        uint32_t y = (Data[4] % 100);
        uint32_t z = 0; // Assume single page
        uint16_t s = 0; // Assume single sample
        tmsize_t ret3 = TIFFReadTile(tif, buf, x, y, z, s);
        (void)ret3;

        // Fuzz TIFFTileRowSize
        tmsize_t row_size = TIFFTileRowSize(tif);
        (void)row_size;
    }

    // Cleanup
    free(buf);
    TIFFClose(tif);

    return 0;
}