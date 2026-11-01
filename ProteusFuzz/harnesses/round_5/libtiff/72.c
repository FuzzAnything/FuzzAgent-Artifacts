// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

static void write_dummy_tiff(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;

    // Write a minimal valid TIFF header (Little Endian)
    // Byte 0-1: Magic number (0x002A for Little Endian)
    uint16_t magic = 0x002A;
    fwrite(&magic, sizeof(uint16_t), 1, f);
    
    // Byte 2-3: Offset to first IFD (8 bytes)
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);

    // Pad to offset 8
    for (int i = 0; i < 4; i++) fputc(0, f);

    // Write a minimal IFD with 0 entries (empty directory)
    // Number of directory entries (0)
    uint16_t num_entries = 0;
    fwrite(&num_entries, sizeof(uint16_t), 1, f);
    
    // Offset to next IFD (0 = end of chain)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, f);

    // Append fuzzing data as raw image data (though not referenced by IFD)
    if (Size > 0) {
        fwrite(Data, 1, Size, f);
    }

    fclose(f);
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    // Step 1: Prepare environment
    // Create a dummy file with minimal TIFF structure
    write_dummy_tiff(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Ensure we have a directory loaded
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Step 2: Fuzz target functions
    // We use the fuzzing data to generate coordinates and sample indices
    uint32_t x, y, z;
    uint16_t s;

    if (Size >= 8) {
        x = (uint32_t)Data[0] | ((uint32_t)Data[1] << 8) | ((uint32_t)Data[2] << 16) | ((uint32_t)Data[3] << 24);
        y = (uint32_t)Data[4] | ((uint32_t)Data[5] << 8) | ((uint32_t)Data[6] << 16) | ((uint32_t)Data[7] << 24);
    } else {
        x = 0;
        y = 0;
    }

    if (Size >= 10) {
        z = (uint32_t)Data[8] | ((uint32_t)Data[9] << 8);
    } else {
        z = 0;
    }

    if (Size >= 12) {
        s = (uint16_t)Data[10] | ((uint16_t)Data[11] << 8);
    } else {
        s = 0;
    }

    // Fuzz TIFFCurrentTile
    // This function reads tif_curtile. It is safe to call even if no tile has been read.
    (void)TIFFCurrentTile(tif);

    // Fuzz TIFFCurrentRow
    // This function reads tif_row.
    (void)TIFFCurrentRow(tif);

    // Fuzz TIFFComputeTile
    // Calculates tile index from coordinates.
    (void)TIFFComputeTile(tif, x, y, z, s);

    // Fuzz TIFFCheckTile
    // Validates tile coordinates.
    (void)TIFFCheckTile(tif, x, y, z, s);

    // Fuzz TIFFComputeStrip
    // Calculates strip index.
    (void)TIFFComputeStrip(tif, y, s);

    // Fuzz TIFFNumberOfTiles
    // Calculates total number of tiles.
    (void)TIFFNumberOfTiles(tif);

    // Step 3: Cleanup
    TIFFClose(tif);

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}