// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFRasterScanlineSize64 at tif_strip.c:357:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure minimum size to avoid immediate crashes on very small inputs
    if (Size < 4) {
        return 0;
    }

    // Write input data to a dummy file
    write_dummy_file(Data, Size);

    // Attempt to open the file as a TIFF
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    // Try to read the directory to ensure the TIFF handle is in a valid state
    // for the target functions. If the file is not a valid TIFF, this will fail,
    // but the handle might still be usable for some checks or will return 0.
    if (!TIFFReadDirectory(tif)) {
        // Even if directory read fails, we can still test the functions
        // as they handle error cases internally (returning 0).
    }

    // Fuzz TIFFTileRowSize64
    (void)TIFFTileRowSize64(tif);

    // Fuzz TIFFGetStrileByteCount with various indices
    // Use data from input to determine index, ensuring it's within reasonable bounds
    uint32_t strile_index = 0;
    if (Size >= 4) {
        strile_index = *(uint32_t *)Data;
    } else if (Size >= 2) {
        strile_index = *(uint16_t *)Data;
    } else {
        strile_index = Data[0];
    }
    // Cap index to prevent excessive iteration if the library tries to loop
    if (strile_index > 100000) {
        strile_index = 100000;
    }
    (void)TIFFGetStrileByteCount(tif, strile_index);

    // Fuzz TIFFRasterScanlineSize64
    (void)TIFFRasterScanlineSize64(tif);

    // Fuzz TIFFVTileSize64 with various row counts
    uint32_t nrows = 0;
    if (Size >= 4) {
        nrows = *(uint32_t *)(Data + (Size % 4));
    } else {
        nrows = Data[0];
    }
    if (nrows > 100000) {
        nrows = 100000;
    }
    (void)TIFFVTileSize64(tif, nrows);

    // Fuzz TIFFScanlineSize64
    (void)TIFFScanlineSize64(tif);

    // Fuzz TIFFStripSize64
    (void)TIFFStripSize64(tif);

    // Cleanup
    TIFFClose(tif);

    return 0;
}