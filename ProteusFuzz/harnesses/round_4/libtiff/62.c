// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
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
#include "tiffio.h"

static void write_dummy_tiff(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;

    // Write a minimal valid TIFF header (Little Endian)
    // Magic number: 0x002A (Little Endian)
    uint16_t magic = 0x002A;
    fwrite(&magic, sizeof(uint16_t), 1, f);

    // Offset to first IFD (8 bytes, after header)
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);

    // Write fuzzed data as the IFD and image data
    if (size > 0) {
        fwrite(data, 1, size, f);
    } else {
        // Write at least one dummy entry to avoid immediate crash on empty input
        uint16_t dummy_entry = 0;
        fwrite(&dummy_entry, sizeof(uint16_t), 1, f);
    }

    fclose(f);
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare the environment: Create a dummy file with fuzzed content
    write_dummy_tiff(Data, Size);

    // Open the file using libtiff
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Extract some values from the input to use as indices
    uint32_t strile_idx = 0;
    uint32_t strip_idx = 0;
    uint32_t nrows = 0;

    if (Size >= 4) {
        strile_idx = *(const uint32_t *)(Data);
        strip_idx = *(const uint32_t *)(Data);
        nrows = *(const uint32_t *)(Data);
    }

    // Introduce variation by using different indices from the input buffer
    uint32_t strile_variants[5] = {
        strile_idx,
        (Size > 4) ? *(const uint32_t *)(Data + 4) : 0,
        (Size > 8) ? *(const uint32_t *)(Data + 8) : 0,
        0xFFFFFFFF,
        0
    };

    int pbErr = 0;
    uint64_t result;

    // Fuzz TIFFGetStrileByteCount
    for (int i = 0; i < 5; i++) {
        result = TIFFGetStrileByteCount(tif, strile_variants[i]);
        (void)result;
    }

    // Fuzz TIFFGetStrileByteCountWithErr
    for (int i = 0; i < 5; i++) {
        pbErr = 0;
        result = TIFFGetStrileByteCountWithErr(tif, strile_variants[i], &pbErr);
        (void)result;
    }

    // Fuzz TIFFRawStripSize64
    for (int i = 0; i < 5; i++) {
        result = TIFFRawStripSize64(tif, strile_variants[i]);
        (void)result;
    }

    // Fuzz TIFFGetStrileOffsetWithErr
    for (int i = 0; i < 5; i++) {
        pbErr = 0;
        result = TIFFGetStrileOffsetWithErr(tif, strile_variants[i], &pbErr);
        (void)result;
    }

    // Fuzz TIFFGetStrileOffset
    for (int i = 0; i < 5; i++) {
        result = TIFFGetStrileOffset(tif, strile_variants[i]);
        (void)result;
    }

    // Fuzz TIFFVStripSize64 with various row counts
    uint32_t nrows_variants[5] = {
        nrows,
        (Size > 4) ? *(const uint32_t *)(Data + 4) : 0,
        0xFFFFFFFF,
        0,
        1
    };

    for (int i = 0; i < 5; i++) {
        result = TIFFVStripSize64(tif, nrows_variants[i]);
        (void)result;
    }

    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");

    return 0;
}