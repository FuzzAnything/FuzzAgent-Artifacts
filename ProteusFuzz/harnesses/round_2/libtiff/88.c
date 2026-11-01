// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void cleanup_and_close(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a temporary file for fuzzing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }

    // Write the fuzz input to the file
    size_t written = fwrite(Data, 1, Size, fp);
    fclose(fp);
    if (written != Size) {
        return 0;
    }

    // Open the TIFF file for reading
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        // File might not be a valid TIFF, which is expected in fuzzing
        return 0;
    }

    // Allocate buffers for testing
    uint8_t *buf = (uint8_t *)malloc(Size);
    uint32_t *long_buf = (uint32_t *)malloc(Size);
    uint8_t *triple_buf = (uint8_t *)malloc(Size);

    if (!buf || !long_buf || !triple_buf) {
        cleanup_and_close(tif);
        free(buf);
        free(long_buf);
        free(triple_buf);
        return 0;
    }

    // Initialize buffers with input data
    memset(buf, 0, Size);
    memset(long_buf, 0, Size);
    memset(triple_buf, 0, Size);
    memcpy(buf, Data, Size);
    memcpy(long_buf, Data, Size);
    memcpy(triple_buf, Data, Size);

    // Fuzz TIFFReadEncodedStrip
    // Try reading different strips (0 to 10) with varying buffer sizes
    for (uint32_t strip = 0; strip < 10; strip++) {
        tmsize_t read_size = Size > 0 ? Size : 1;
        TIFFReadEncodedStrip(tif, strip, buf, read_size);
    }

    // Fuzz TIFFReverseBits
    // Apply bit reversal on the buffer
    TIFFReverseBits(buf, Size);

    // Fuzz TIFFSwabArrayOfTriples
    // Ensure size is a multiple of 3 for triples
    tmsize_t triple_count = Size / 3;
    if (triple_count > 0) {
        TIFFSwabArrayOfTriples(triple_buf, triple_count);
    }

    // Fuzz TIFFSwabArrayOfLong
    // Ensure size is a multiple of 4 for longs
    tmsize_t long_count = Size / 4;
    if (long_count > 0) {
        TIFFSwabArrayOfLong(long_buf, long_count);
    }

    // Fuzz TIFFGetBitRevTable
    // Get both reversed and non-reversed tables
    const unsigned char *rev_table = TIFFGetBitRevTable(1);
    const unsigned char *non_rev_table = TIFFGetBitRevTable(0);
    if (rev_table && non_rev_table && Size > 0) {
        // Use the tables to avoid unused variable warnings
        volatile unsigned char dummy = rev_table[0] ^ non_rev_table[0];
        (void)dummy;
    }

    // Fuzz TIFFSwabLong
    // Swap individual longs if we have enough data
    if (Size >= sizeof(uint32_t)) {
        uint32_t val = 0x12345678;
        TIFFSwabLong(&val);
        (void)val; // Avoid unused variable warning
    }

    // Cleanup
    free(buf);
    free(long_buf);
    free(triple_buf);
    cleanup_and_close(tif);

    return 0;
}