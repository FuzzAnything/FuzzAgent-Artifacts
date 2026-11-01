// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFDataWidth at tif_dirinfo.c:637:5 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write input data to dummy file
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Open the file for reading
    TIFF *tif = TIFFOpen(filename, "r");
    if (tif == NULL) {
        return 0;
    }

    // Fuzz TIFFNumberOfStrips
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    (void)num_strips;

    // Fuzz TIFFDataWidth with various types derived from input
    for (size_t i = 0; i < Size && i < 20; i++) {
        TIFFDataType type = (TIFFDataType)(Data[i] % 16);
        int width = TIFFDataWidth(type);
        (void)width;
    }

    // Fuzz TIFFGetStrileByteCountWithErr
    int err = 0;
    for (size_t i = 0; i < Size && i < 100; i++) {
        uint32_t strile = (uint32_t)(Data[i] * (Size > 1 ? Data[i+1] : 1));
        uint64_t byte_count = TIFFGetStrileByteCountWithErr(tif, strile, &err);
        (void)byte_count;
        (void)err;
    }

    // Fuzz TIFFVStripSize64
    for (size_t i = 0; i < Size && i < 100; i++) {
        uint32_t nrows = (uint32_t)(Data[i] * (Size > 1 ? Data[i+1] : 1));
        uint64_t vstrip_size = TIFFVStripSize64(tif, nrows);
        (void)vstrip_size;
    }

    // Fuzz TIFFGetStrileByteCount
    for (size_t i = 0; i < Size && i < 100; i++) {
        uint32_t strile = (uint32_t)(Data[i] * (Size > 1 ? Data[i+1] : 1));
        uint64_t byte_count = TIFFGetStrileByteCount(tif, strile);
        (void)byte_count;
    }

    // Fuzz TIFFSetupStrips
    int setup_result = TIFFSetupStrips(tif);
    (void)setup_result;

    // Cleanup
    cleanup_tiff(tif);

    return 0;
}