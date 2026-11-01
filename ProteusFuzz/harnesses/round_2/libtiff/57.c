// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty
            // Little-endian, TIFF version 42, first IFD at offset 8
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    uint32_t x = 0, y = 0, z = 0;
    uint16_t s = 0;
    uint32_t row = 0;

    // Ensure we have enough data to extract parameters safely
    if (Size < 18) {
        return 0;
    }

    // Extract parameters from input data to diversify fuzzing
    // Use memcpy to avoid potential alignment issues and ensure bounds safety
    memcpy(&x, Data, sizeof(x));
    memcpy(&y, Data + 4, sizeof(y));
    memcpy(&z, Data + 8, sizeof(z));
    memcpy(&s, Data + 12, sizeof(s));
    memcpy(&row, Data + 14, sizeof(row));

    // Create a dummy file to open
    setup_dummy_file(Data, Size);

    // Attempt to open the file in read mode
    tif = TIFFOpen("./dummy_file", "r");

    if (tif != NULL) {
        // Fuzz TIFFComputeTile
        (void)TIFFComputeTile(tif, x, y, z, s);

        // Fuzz TIFFComputeStrip
        (void)TIFFComputeStrip(tif, row, s);

        // Fuzz TIFFNumberOfTiles
        (void)TIFFNumberOfTiles(tif);

        // Fuzz TIFFCurrentRow
        (void)TIFFCurrentRow(tif);

        // Fuzz TIFFCurrentTile
        (void)TIFFCurrentTile(tif);

        // Fuzz TIFFCheckTile
        (void)TIFFCheckTile(tif, x, y, z, s);

        // Cleanup
        TIFFClose(tif);
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}