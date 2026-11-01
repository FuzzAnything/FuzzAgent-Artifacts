// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "tiffio.h"

/*
 * Fuzz driver for libtiff tile and strip computation functions.
 * Target APIs: TIFFComputeTile, TIFFCheckTile, TIFFComputeStrip,
 * TIFFNumberOfTiles, TIFFCurrentRow, TIFFCurrentTile.
 */

static void write_dummy_tiff(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return;
    }
    
    /* Write at least a minimal TIFF header to allow TIFFOpen to succeed */
    /* If input is too small, pad with zeros to create a valid minimal header */
    uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00}; /* Little-endian, 8-byte offset */
    
    if (size >= 8) {
        /* Use first 8 bytes of input as header if available, ensuring magic */
        if (data[0] == 0x49 && data[1] == 0x49) {
            memcpy(header, data, 8);
        }
    }
    
    ssize_t w = write(fd, header, sizeof(header));
    if (w < 0) {
        close(fd);
        return;
    }
    
    /* Write the rest of the input data */
    if (size > 8) {
        write(fd, data + 8, size - 8);
    } else {
        /* Pad with zeros to make a minimal file if input was too short */
        uint8_t zeros[100] = {0};
        write(fd, zeros, 100);
    }
    
    close(fd);
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Prepare the dummy file */
    write_dummy_tiff(Data, Size);

    /* Open the file for reading */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    /* Extract some values from input to drive fuzzing logic */
    uint32_t x = (Size > 0) ? Data[0] : 0;
    uint32_t y = (Size > 1) ? Data[1] : 0;
    uint32_t z = (Size > 2) ? Data[2] : 0;
    uint16_t s = (Size > 3) ? Data[3] : 0;
    uint32_t row = (Size > 4) ? Data[4] : 0;
    
    /* Ensure s is within uint16_t range if derived from single byte */
    s = s & 0xFFFF;

    /* 1. Test TIFFCheckTile */
    /* Validates tile coordinates. Returns 0 on error. */
    (void)TIFFCheckTile(tif, x, y, z, s);

    /* 2. Test TIFFComputeTile */
    /* Calculates tile index. */
    uint32_t tile_idx = TIFFComputeTile(tif, x, y, z, s);
    (void)tile_idx;

    /* 3. Test TIFFComputeStrip */
    /* Calculates strip index. */
    uint32_t strip_idx = TIFFComputeStrip(tif, row, s);
    (void)strip_idx;

    /* 4. Test TIFFNumberOfTiles */
    /* Calculates total number of tiles. */
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;

    /* 5. Test TIFFCurrentRow */
    /* Retrieves current scanline. */
    uint32_t curr_row = TIFFCurrentRow(tif);
    (void)curr_row;

    /* 6. Test TIFFCurrentTile */
    /* Retrieves current tile index. */
    uint32_t curr_tile = TIFFCurrentTile(tif);
    (void)curr_tile;

    /* Cleanup */
    TIFFClose(tif);

    return 0;
}