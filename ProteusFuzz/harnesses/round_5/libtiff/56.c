// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        return 0;
    }
    if (data && size > 0) {
        size_t written = fwrite(data, 1, size, f);
        if (written != size) {
            fclose(f);
            return 0;
        }
    } else {
        // Write a minimal valid TIFF header if no data provided
        // Basic TIFF header: 4 bytes magic, 2 bytes offset to first IFD
        uint16_t magic = 0x4949; // Little endian
        uint16_t version = 42;
        uint32_t ifd_offset = 8;
        fwrite(&magic, 2, 1, f);
        fwrite(&version, 2, 1, f);
        fwrite(&ifd_offset, 4, 1, f);
        // Write a dummy IFD with 0 entries
        uint16_t entries = 0;
        uint32_t next_ifd = 0;
        fwrite(&entries, 2, 1, f);
        fwrite(&next_ifd, 4, 1, f);
    }
    fclose(f);
    return 1;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare the environment: create a dummy file
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }

    // Open the file for reading
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Ensure we are in a valid state to call the target functions
    // Read the directory to initialize internal structures
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Fuzz target 1: TIFFTileRowSize64
    uint64_t val1 = TIFFTileRowSize64(tif);
    (void)val1; // Use the value to avoid unused variable warnings

    // Fuzz target 2: TIFFVTileSize64
    // Use data from input to determine nrows, but keep it reasonable
    uint32_t nrows = 0;
    if (Size >= 4) {
        nrows = *(const uint32_t *)(Data);
        // Cap nrows to avoid excessive allocation or invalid states
        if (nrows > 10000) nrows = 10000;
    } else if (Size > 0) {
        nrows = Data[0] % 1000;
    }
    uint64_t val2 = TIFFVTileSize64(tif, nrows);
    (void)val2;

    // Fuzz target 3: TIFFScanlineSize64
    uint64_t val3 = TIFFScanlineSize64(tif);
    (void)val3;

    // Fuzz target 4: TIFFGetStrileByteCount
    // Use data to determine strile index
    uint32_t strile = 0;
    if (Size >= 8) {
        strile = *(const uint32_t *)(Data + 4);
        if (strile > 1000) strile = 1000;
    } else if (Size > 4) {
        strile = Data[4] % 100;
    }
    uint64_t val4 = TIFFGetStrileByteCount(tif, strile);
    (void)val4;

    // Fuzz target 5: TIFFTileSize64
    uint64_t val5 = TIFFTileSize64(tif);
    (void)val5;

    // Fuzz target 6: TIFFStripSize64
    uint64_t val6 = TIFFStripSize64(tif);
    (void)val6;

    // Cleanup
    TIFFClose(tif);

    return 0;
}