// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFDataWidth at tif_dirinfo.c:637:5 in tiffio.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f == NULL) {
        return;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, f);
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    int err = 0;
    uint32_t num_strips = TIFFNumberOfStrips(tif);

    if (num_strips > 0) {
        uint32_t test_strip = 0;
        if (Size > 0) {
            test_strip = Data[0] % num_strips;
        }

        uint64_t byte_count = TIFFGetStrileByteCount(tif, test_strip);
        (void)byte_count;

        err = 0;
        uint64_t byte_count_err = TIFFGetStrileByteCountWithErr(tif, test_strip, &err);
        (void)byte_count_err;

        uint32_t rows = 0;
        if (Size > 1) {
            rows = Data[1];
        }
        uint64_t vstrip_size = TIFFVStripSize64(tif, rows);
        (void)vstrip_size;

        int setup_result = TIFFSetupStrips(tif);
        (void)setup_result;
    }

    if (Size > 2) {
        TIFFDataType type = (TIFFDataType)Data[2];
        int width = TIFFDataWidth(type);
        (void)width;
    }

    TIFFClose(tif);
    return 0;
}