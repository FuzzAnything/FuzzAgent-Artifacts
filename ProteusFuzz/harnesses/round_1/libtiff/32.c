// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    /* Extract some values from input to drive fuzzing logic */
    uint32_t nrows = (Size > 4) ? *(const uint32_t *)(Data) : 0;
    uint32_t strile_idx = (Size > 8) ? *(const uint32_t *)(Data + 4) : 0;
    uint32_t strip_idx = (Size > 12) ? *(const uint32_t *)(Data + 8) : 0;

    /* Fuzz TIFFVStripSize64 */
    uint64_t vstrip_size = TIFFVStripSize64(tif, nrows);
    (void)vstrip_size;

    /* Fuzz TIFFGetStrileOffset */
    uint64_t strile_off = TIFFGetStrileOffset(tif, strile_idx);
    (void)strile_off;

    /* Fuzz TIFFGetStrileByteCount */
    uint64_t strile_bc = TIFFGetStrileByteCount(tif, strile_idx);
    (void)strile_bc;

    /* Fuzz TIFFGetStrileByteCountWithErr */
    int pbErr = 0;
    uint64_t strile_bc_err = TIFFGetStrileByteCountWithErr(tif, strile_idx, &pbErr);
    (void)strile_bc_err;
    (void)pbErr;

    /* Fuzz TIFFRawStripSize64 */
    uint64_t raw_strip_size = TIFFRawStripSize64(tif, strip_idx);
    (void)raw_strip_size;

    /* Fuzz TIFFGetStrileOffsetWithErr */
    int pbErr2 = 0;
    uint64_t strile_off_err = TIFFGetStrileOffsetWithErr(tif, strile_idx, &pbErr2);
    (void)strile_off_err;
    (void)pbErr2;

    TIFFClose(tif);
    return 0;
}