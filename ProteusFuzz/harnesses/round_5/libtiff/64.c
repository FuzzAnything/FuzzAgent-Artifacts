// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a temporary file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    size_t written = fwrite(Data, 1, Size, fp);
    fclose(fp);
    if (written != Size) {
        return 0;
    }

    // Open the TIFF file
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // Extract fuzzing parameters from the input data if available
    uint32_t strile_index = 0;
    uint32_t nrows = 0;
    uint32_t strip_index = 0;

    if (Size >= 4) {
        strile_index = *(const uint32_t *)(Data);
    }
    if (Size >= 8) {
        nrows = *(const uint32_t *)(Data + 4);
    }
    if (Size >= 12) {
        strip_index = *(const uint32_t *)(Data + 8);
    }

    // Fuzz TIFFGetStrileOffsetWithErr
    int err = 0;
    uint64_t offset_with_err = TIFFGetStrileOffsetWithErr(tif, strile_index, &err);
    (void)offset_with_err;
    (void)err;

    // Fuzz TIFFGetStrileByteCountWithErr
    uint64_t bytecount_with_err = TIFFGetStrileByteCountWithErr(tif, strile_index, &err);
    (void)bytecount_with_err;

    // Fuzz TIFFVStripSize64
    uint64_t vstrip_size = TIFFVStripSize64(tif, nrows);
    (void)vstrip_size;

    // Fuzz TIFFRawStripSize64
    uint64_t raw_strip_size = TIFFRawStripSize64(tif, strip_index);
    (void)raw_strip_size;

    // Fuzz TIFFGetStrileOffset
    uint64_t offset = TIFFGetStrileOffset(tif, strile_index);
    (void)offset;

    // Fuzz TIFFGetStrileByteCount
    uint64_t bytecount = TIFFGetStrileByteCount(tif, strile_index);
    (void)bytecount;

    // Cleanup
    TIFFClose(tif);

    // Remove the temporary file
    remove(filename);

    return 0;
}