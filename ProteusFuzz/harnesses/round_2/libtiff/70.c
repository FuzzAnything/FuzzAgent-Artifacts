// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
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
    if (f == NULL) {
        return;
    }
    if (size > 0) {
        fwrite(data, 1, size, f);
    } else {
        // Write a minimal valid TIFF header if input is empty to ensure open succeeds
        uint16_t header[8] = {
            0x002A, // Little endian
            0x002A, // Version
            0x0008, // First IFD offset
            0x0000, 0x0000, 0x0000, 0x0000
        };
        fwrite(header, sizeof(header), 1, f);
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare the environment: write data to a dummy file
    write_dummy_file(Data, Size);

    // Open the file in read-only mode
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    // Extract data for fuzzing parameters
    uint32_t strile = 0;
    uint32_t nrows = 0;
    int pbErr = 0;

    if (Size >= 4) {
        strile = *(const uint32_t *)(Data);
    }
    if (Size >= 8) {
        nrows = *(const uint32_t *)(Data + 4);
    }

    // Fuzz TIFFGetStrileByteCount
    uint64_t count1 = TIFFGetStrileByteCount(tif, strile);
    (void)count1;

    // Fuzz TIFFVStripSize64
    uint64_t count2 = TIFFVStripSize64(tif, nrows);
    (void)count2;

    // Fuzz TIFFGetStrileByteCountWithErr
    pbErr = 0;
    uint64_t count3 = TIFFGetStrileByteCountWithErr(tif, strile, &pbErr);
    (void)count3;
    (void)pbErr;

    // Fuzz TIFFRawStripSize64
    uint64_t count4 = TIFFRawStripSize64(tif, strile);
    (void)count4;

    // Fuzz TIFFGetStrileOffsetWithErr
    pbErr = 0;
    uint64_t offset1 = TIFFGetStrileOffsetWithErr(tif, strile, &pbErr);
    (void)offset1;
    (void)pbErr;

    // Fuzz TIFFGetStrileOffset
    uint64_t offset2 = TIFFGetStrileOffset(tif, strile);
    (void)offset2;

    // Cleanup
    TIFFClose(tif);

    return 0;
}